/*
 * FileReader.cpp
 *
 *  Created on: Jun 9, 2015
 *      Author: Patrick
 */

#include "FileReader.h"

#include <boost/filesystem.hpp>
#include <iostream>

PREPARE_LOGGING(FileReader)

FileReader::FileReader() :
    isPlaying(false),
    packetSize(1000),
    queueSize(10),
    threadHandle(NULL)
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    initializeQueues();
}

FileReader::~FileReader()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    stop();
    clearQueues();
}

void FileReader::pause()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);
}

void FileReader::restart()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    stop();
    initializeQueues();
    start();
}

void FileReader::start()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    if (this->threadHandle == NULL) {
        this->threadHandle = new boost::thread(&FileReader::fileReaderWorkFunction, this);
    }
}

void FileReader::stop()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    if (this->threadHandle != NULL) {
        this->threadHandle->interrupt();
        this->allocatedPacketAvailable.notify_all();
        this->freePacketAvailable.notify_all();
        this->threadHandle->join();
        delete this->threadHandle;
        this->threadHandle = NULL;
    }
}

bool FileReader::isReady() const
{
    return this->isPlaying;
}

FilePacket* const FileReader::getNextPacket()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    boost::mutex::scoped_lock lock(this->allocatedQueueLock);

    if (this->allocatedFilePackets.size() == 0) {
        boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(500);
        this->allocatedPacketAvailable.timed_wait(lock, timeout);

        if (this->allocatedFilePackets.size() == 0) {
            return NULL;
        }
    }

    FilePacket * const packet = this->allocatedFilePackets.front();

    this->allocatedFilePackets.pop_front();

    return packet;
}

void FileReader::replacePacket(FilePacket * const packet)
{
    boost::mutex::scoped_lock lock(this->freeQueueLock);

    this->freeFilePackets.push_back(packet);

    this->freePacketAvailable.notify_one();
}

const std::string& FileReader::getFilepath() const
{
    return this->filepath;
}

bool FileReader::setFilepath(const std::string &newFilepath)
{
    if (not boost::filesystem::exists(newFilepath)) {
        LOG_WARN(FileReader, "Invalid file path");
        return false;
    }

    this->filepath = newFilepath;

    if (this->threadHandle != NULL) {
        restart();
    }

    return true;
}

const size_t& FileReader::getPacketSize() const
{
    return this->packetSize;
}

void FileReader::setPacketSize(const size_t &newPacketSize)
{
    if (this->packetSize == newPacketSize) {
        return;
    }

    this->packetSize = newPacketSize;

    if (this->threadHandle != NULL) {
        restart();
    } else {
        initializeQueues();
    }
}

const size_t& FileReader::getQueueSize() const
{
    return this->queueSize;
}

void FileReader::setQueueSize(const size_t &newQueueSize)
{
    if (this->queueSize == newQueueSize) {
        return;
    }

    this->queueSize = newQueueSize;

    if (this->threadHandle != NULL) {
        restart();
    } else {
        initializeQueues();
    }
}

void FileReader::fileReaderWorkFunction()
{
    this->isPlaying = true;

    std::vector<std::string> filesToRead;

    // If the filepath points to a directory, add all regular
    // files beneath it to the list
    if (boost::filesystem::is_regular_file(this->filepath)) {
        filesToRead.push_back(this->filepath);
    } else if (boost::filesystem::is_directory(this->filepath)) {
        for (boost::filesystem::directory_iterator i(this->filepath); i != boost::filesystem::directory_iterator(); i++) {
            boost::filesystem::directory_entry e(*i);

            if (boost::filesystem::is_regular_file(e)) {
                filesToRead.push_back(e.path().file_string());
            }
        }
    } else {
        LOG_WARN(FileReader, "Unrecognized file type");
    }

    // Iterate over all files and read them, placing the data
    // into the packets that are free
    for (std::vector<std::string>::iterator i = filesToRead.begin(); i != filesToRead.end(); i++) {
        LOG_INFO(FileReader, "Opening file: " << *i);
        std::ifstream in(i->c_str(), std::ios::in | std::ios::binary);

        // An error occurred in opening the file for reading,
        // try the next one
        if (not in) {
            LOG_WARN(FileReader, "Unable to open file: " << *i);
            continue;
        }

        // Get the size of the file and prepare the firstPacket
        // flag
        size_t remainingBytes = boost::filesystem::file_size(*i);
        bool firstPacket = true;

        LOG_INFO(FileReader, "File is of size: " << remainingBytes << " bytes");

        do {
            boost::mutex::scoped_lock freeLock(this->freeQueueLock);

            LOG_INFO(FileReader, "Got free lock");

            // Check for a free packet
            if (this->freeFilePackets.size() == 0) {
                LOG_INFO(FileReader, "Waiting for free packet to be available");
                this->freePacketAvailable.wait(freeLock);
                LOG_INFO(FileReader, "Free packet available?");

                if (this->freeFilePackets.size() == 0) {
                    LOG_INFO(FileReader, "Nope");
                    break;
                }

                LOG_INFO(FileReader, "Yup");
            }

            // If the thread is asked to stop, break out of the
            // loop and clean up the file
            try {
                boost::this_thread::interruption_point();
            } catch (boost::thread_interrupted &e) {
                LOG_INFO(FileReader, "Main thread requested interruption");
                break;
            }

            // Get and remove the first packet form the queue
            FilePacket *packet = this->freeFilePackets.front();

            this->freeFilePackets.pop_front();

            LOG_INFO(FileReader, "Got free packet");

            freeLock.unlock();

            // Set the firstPacket flag on this packet
            packet->firstPacket = firstPacket;

            if (firstPacket) {
                firstPacket = false;
            }

            // Set the filename string to this file
            packet->filename = *i;

            LOG_INFO(FileReader, "Packet crafted for " << *i);

            // Read either the rest of the data, or the size of
            // a packet, whichever is smaller
            size_t bytesToRead = std::min(remainingBytes, this->packetSize);
            size_t bytesRead = 0;

            do {
                in.read(&packet->data[bytesRead], bytesToRead - bytesRead);

                bytesRead += in.gcount();
            } while (not in);

            LOG_INFO(FileReader, "Read " << bytesRead << " bytes");

            // Indicate the size of the data in the packet
            packet->dataSize = bytesRead;

            boost::mutex::scoped_lock allocatedLock(this->allocatedQueueLock);

            LOG_INFO(FileReader, "Got allocated lock");

            // Insert the packet at the end of the allocated
            // queue
            this->allocatedFilePackets.push_back(packet);

            this->allocatedPacketAvailable.notify_one();

            // Adjust the number of bytes remaining
            remainingBytes -= bytesRead;
        } while (remainingBytes > 0);

        LOG_INFO(FileReader, "Closing file");

        // Close the file
        in.close();

        // If the thread is asked to stop, break out of the
        // loop and clean up the file
        try {
            boost::this_thread::interruption_point();
        } catch (boost::thread_interrupted &e) {
            LOG_INFO(FileReader, "Main thread requested interruption");
            break;
        }
    }

    LOG_INFO(FileReader, "Exiting work function");
    this->isPlaying = false;
}

void FileReader::clearQueue(std::deque<FilePacket *> &queue)
{
    for (std::deque<FilePacket *>::iterator i = queue.begin(); i != queue.end(); i++) {
        if ((*i)->data != NULL) {
            delete[] (*i)->data;
        }

        if ((*i) != NULL) {
            delete *i;
        }
    }

    queue.clear();
}

void FileReader::clearQueues()
{
    clearQueue(this->allocatedFilePackets);
    clearQueue(this->freeFilePackets);
}

void FileReader::initializeQueues()
{
    LOG_INFO(FileReader, __PRETTY_FUNCTION__);
    // Clear out any existing packets before initializing
    if (this->allocatedFilePackets.size() != 0 || this->freeFilePackets.size() != 0) {
        clearQueues();
    }

    FilePacket *packet = NULL;

    for (size_t i = 0; i < this->queueSize; ++i) {
        LOG_INFO(FileReader, "New Packet");
        packet = new FilePacket;

        packet->data = new char[this->packetSize];

        this->freeFilePackets.push_back(packet);
    }

    LOG_INFO(FileReader, "Done with initialize");
}

void FileReader::resetQueues()
{
    for (size_t i = this->allocatedFilePackets.size(); i > 0; --i) {
        this->freeFilePackets.push_back(this->allocatedFilePackets.front());
        this->allocatedFilePackets.pop_front();
    }
}
