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

/*
 * Initialize the variables and fill the queues
 */
FileReader::FileReader() :
    isPlaying(false),
    loopingEnabled(false),
    packetSize(1000),
    queueSize(10),
    threadHandle(NULL)
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    initializeQueues();
}

/*
 * Stop the thread and clear the queues
 */
FileReader::~FileReader()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    stop();
    clearQueues();
}

/*
 * Reset the queues to an initial state and start the thread, if necessary
 */
void FileReader::start()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    if (this->threadHandle == NULL) {
        resetQueues();
        this->threadHandle = new boost::thread(&FileReader::fileReaderWorkFunction, this);
    }
}

/*
 * Shutdown the thread, if necessary
 */
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

/*
 * Indicate whether the thread is able to produce packets
 */
bool FileReader::isReady() const
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    return this->isPlaying;
}

/*
 * Return a pointer to the next allocated packet, if one is available.  If not,
 * wait for one or timeout
 */
const FilePacket *FileReader::getNextPacket()
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

    FilePacket *packet = this->allocatedFilePackets.front();

    this->allocatedFilePackets.pop_front();

    return packet;
}

/*
 * Replace a packet acquired through getNextPacket.  This is necessary to
 * prevent memory leaks.  TODO: Think of a better way to do this.  Maybe a
 * callback?
 */
void FileReader::replacePacket(const FilePacket *packet)
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    boost::mutex::scoped_lock lock(this->freeQueueLock);

    // The const keyword is imposed on users outside of this class
    FilePacket *internalPacket = const_cast<FilePacket *>(packet);

    this->freeFilePackets.push_back(internalPacket);

    this->freePacketAvailable.notify_one();
}

/*
 * Return the current file path being used
 */
const std::string& FileReader::getFilePath() const
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    return this->filePath;
}

/*
 * If the new file path exists, set the current file path and restart the
 * thread.  Returns a boolean to indicate success or failure
 */
bool FileReader::setFilePath(const std::string &newFilePath)
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    if (this->filePath == newFilePath) {
        return true;
    }

    if (not boost::filesystem::exists(newFilePath)) {
        LOG_WARN(FileReader, "Invalid file path: " << newFilePath);
        return false;
    }

    if (this->threadHandle != NULL) {
        stop();
        this->filePath = newFilePath;
        start();
    } else {
        this->filePath = newFilePath;
    }

    return true;
}

/*
 * Get the current looping enabled flag being used
 */
const bool& FileReader::getLoopingEnabled() const
{
    return this->loopingEnabled;
}

/*
 * Set the looping flag
 */
void FileReader::setLoopingEnabled(const bool &enable)
{
    this->loopingEnabled = enable;
}

/*
 * Get the current packet size being used
 */
const size_t& FileReader::getPacketSize() const
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    return this->packetSize;
}

/*
 * Set the new packet size, stop the thread if necessary, always initialize the
 * queues to reflect the new packet size, and start the thread if necessary
 */
void FileReader::setPacketSize(const size_t &newPacketSize)
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    if (this->packetSize == newPacketSize) {
        return;
    }

    this->packetSize = newPacketSize;

    if (this->threadHandle != NULL) {
        stop();
        initializeQueues();
        start();
    } else {
        initializeQueues();
    }
}

/*
 * Get the current queue size being used
 */
const size_t& FileReader::getQueueSize() const
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    return this->queueSize;
}

/*
 * Set the new queue size, stop the thread if necessary, always initialize the
 * queues to reflect the new queue size, and start the thread if necessary
 */
void FileReader::setQueueSize(const size_t &newQueueSize)
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    if (this->queueSize == newQueueSize) {
        return;
    }

    this->queueSize = newQueueSize;

    if (this->threadHandle != NULL) {
        stop();
        initializeQueues();
        start();
    } else {
        initializeQueues();
    }
}

/*
 * The read ahead function run by the thread to allow caching of the file data
 * TODO: Break this up into functions
 */
void FileReader::fileReaderWorkFunction()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    // Indicate that the file reader is ready to produce packets
    this->isPlaying = true;

    LOG_DEBUG(FileReader, "Opening file: " << this->filePath);
    std::ifstream in(this->filePath.c_str(), std::ios::in | std::ios::binary);

    // An error occurred in opening the file for reading
    if (not in) {
        LOG_WARN(FileReader, "Unable to open file: " << this->filePath);
        return;
    }

    do {
        // Get the size of the file and prepare the firstPacket flag
        size_t remainingBytes = boost::filesystem::file_size(this->filePath);

        LOG_DEBUG(FileReader, "File is of size: " << remainingBytes << " bytes");

        in.seekg(0);

        do {
            boost::mutex::scoped_lock freeLock(this->freeQueueLock);

            // Check for a free packet
            if (this->freeFilePackets.size() == 0) {
                // Wait for a free packet signal
                this->freePacketAvailable.wait(freeLock);

                // If the signal has been received but no free packet is
                // available, the thread has most likely been stopped.  If that
                // isn't the case, this prevents errors at the cost of not
                // finishing the file
                if (this->freeFilePackets.size() == 0) {
                    break;
                }
            }

            // If the thread is asked to stop, break out of the loop and clean
            // up the file
            try {
                boost::this_thread::interruption_point();
            } catch (boost::thread_interrupted &e) {
                LOG_DEBUG(FileReader, "Main thread requested interruption from file loop");
                break;
            }

            // Get and remove the first packet form the queue
            FilePacket *packet = this->freeFilePackets.front();

            this->freeFilePackets.pop_front();

            freeLock.unlock();

            // Read either the rest of the data, or the size of a packet,
            // whichever is smaller
            size_t bytesToRead = std::min(remainingBytes, this->packetSize);
            size_t bytesRead = 0;

            // Do this in a loop to cover partial reads
            do {
                in.read(&packet->data[bytesRead], bytesToRead - bytesRead);

                bytesRead += in.gcount();
            } while (not in || bytesRead < bytesToRead);

            // Indicate the size of the data in the packet
            packet->dataSize = bytesRead;

            if (remainingBytes == 0) {
                packet->lastPacket = true;
            } else {
                packet->lastPacket = false;
            }

            boost::mutex::scoped_lock allocatedLock(this->allocatedQueueLock);

            // Insert the packet at the end of the allocated
            // queue
            this->allocatedFilePackets.push_back(packet);

            this->allocatedPacketAvailable.notify_one();

            // Adjust the number of bytes remaining
            remainingBytes -= bytesRead;
        } while (remainingBytes > 0);

        // If the thread is asked to stop, break out of the loop and clean
        // up the file
        try {
            boost::this_thread::interruption_point();
        } catch (boost::thread_interrupted &e) {
            LOG_DEBUG(FileReader, "Main thread requested interruption from file loop");
            break;
        }

    } while (this->loopingEnabled);

    LOG_DEBUG(FileReader, "Closing file: " << this->filePath);

    // Close the file
    in.close();

    LOG_DEBUG(FileReader, "Exiting work function");
    this->isPlaying = false;
}

/*
 * Given a queue of FilePacket pointers, delete the packet payload, delete the
 * pointer, and then empty the queue of defunct pointers
 */
void FileReader::clearQueue(std::deque<FilePacket *> &queue)
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    for (std::deque<FilePacket *>::iterator i = queue.begin(); i != queue.end(); i++) {
        if((*i)->data != NULL) {
            delete[] (*i)->data;
        }

        if ((*i) != NULL) {
            delete *i;
        }
    }

    queue.clear();
}

/*
 * A convenience function to clear both the allocated and free file packet
 * queues
 */
void FileReader::clearQueues()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    clearQueue(this->allocatedFilePackets);
    clearQueue(this->freeFilePackets);
}

/*
 * Empty both queues if necessary, then fill the free queue with packets that
 * have a valid payload array
 */
void FileReader::initializeQueues()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    // Clear out any existing packets before initializing
    if (this->allocatedFilePackets.size() != 0 || this->freeFilePackets.size() != 0) {
        clearQueues();
    }

    FilePacket *packet = NULL;

    for (size_t i = 0; i < this->queueSize; ++i) {
        packet = new FilePacket;
        packet->data = new char[this->packetSize];
        packet->dataSize = 0;

        this->freeFilePackets.push_back(packet);
    }
}

/*
 * Take any packets that are in the allocated file packet queue and place them
 * into the free queue to prevent the overhead associated with reallocation
 */
void FileReader::resetQueues()
{
    LOG_TRACE(FileReader, __PRETTY_FUNCTION__);

    for (size_t i = this->allocatedFilePackets.size(); i > 0; --i) {
        this->freeFilePackets.push_back(this->allocatedFilePackets.front());
        this->allocatedFilePackets.pop_front();
    }
}
