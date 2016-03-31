/*
 * MappedFileReader.cpp
 *
 *  Created on: Jun 9, 2015
 *      Author: Patrick
 */

#include "MappedFileReader.h"

#include <boost/filesystem.hpp>

PREPARE_LOGGING(MappedFileReader)

/*
 * Initialize the variables
 */
MappedFileReader::MappedFileReader() :
    isPlaying(false),
    loopingEnabled(false),
    packetSize(1000),
    readIndex(0)
{
    LOG_TRACE(MappedFileReader, __PRETTY_FUNCTION__);
}

/*
 * Close the file
 */
MappedFileReader::~MappedFileReader()
{
    LOG_TRACE(MappedFileReader, __PRETTY_FUNCTION__);

    if (this->file.is_open()) {
        this->file.close();
    }
}

/*
 * Return a FilePacket and update the readIndex based on the packet size
 */
FilePacket MappedFileReader::getNextPacket(size_t bytes)
{
    LOG_TRACE(MappedFileReader, __PRETTY_FUNCTION__);

    FilePacket packet;
    packet.data = NULL;
    packet.dataSize = 0;
    packet.lastPacket = false;

    // Make sure the file is open
    if (not this->file.is_open()) {
        LOG_WARN(MappedFileReader, "Attempted to read without setting " \
                "file path");
        return packet;
    }

    // Adjust the packet size if necessary
    if (bytes > this->file.size()) {
        LOG_WARN(MappedFileReader, "Attempted to read length greater than " \
                "file size, setting to file size");
        bytes = this->file.size();
    }

    size_t bytesLeft = this->file.size() - this->readIndex;

    packet.data = this->file.data() + this->readIndex;

    if (bytes < bytesLeft) {
        packet.dataSize = bytes;
        this->readIndex += bytes;
    } else {
        packet.dataSize = bytesLeft;

        if (this->loopingEnabled) {
            this->readIndex = 0;
        } else {
            packet.lastPacket = true;
        }
    }

    return packet;
}

/*
 * If the new file path exists, close the old file and open the new one,
 * resetting the read index
 */
bool MappedFileReader::setFilePath(const std::string &newFilePath)
{
    LOG_TRACE(MappedFileReader, __PRETTY_FUNCTION__);

    if (this->filePath == newFilePath) {
        return true;
    }

    if (not boost::filesystem::exists(newFilePath)) {
        LOG_WARN(MappedFileReader, "Invalid file path: " << newFilePath);
        return false;
    }

    // Close the old file
    if (this->file.is_open()) {
        this->file.close();
    }

    this->filePath = newFilePath;

    this->file.open(this->filePath, boost::iostreams::mapped_file::readwrite);
    this->readIndex = 0;

    return true;
}

/*
 * Set the looping flag
 */
void MappedFileReader::setLoopingEnabled(const bool &enable)
{
    LOG_TRACE(MappedFileReader, __PRETTY_FUNCTION__);

    this->loopingEnabled = enable;
}

/*
 * Set the new packet size
 */
void MappedFileReader::setPacketSize(const size_t &newPacketSize)
{
    LOG_TRACE(MappedFileReader, __PRETTY_FUNCTION__);

    if (this->packetSize == newPacketSize) {
        return;
    }

    this->packetSize = newPacketSize;
}
