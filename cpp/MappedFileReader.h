/*
 * MappedFileReader.h
 *
 *  Created on: Jun 9, 2015
 *      Author: Patrick
 */

#ifndef THREADEDFILEREADER_H_
#define THREADEDFILEREADER_H_

#include <boost/iostreams/device/mapped_file.hpp>
#include <ossie/debug.h>

struct FilePacket {
    char *data;
    size_t dataSize;
    bool lastPacket;
};

class MappedFileReader
{
    ENABLE_LOGGING
    public:
        MappedFileReader();
        virtual ~MappedFileReader();

    public:
        // Request the next packet
        FilePacket getNextPacket(size_t bytes);

    public:
        // Interface functions
        inline const std::string& getFilePath() const
            { return this->filePath; }

        virtual bool setFilePath(const std::string &newFilePath);

        inline const size_t& getPacketSize() const { return this->packetSize; }
        void setPacketSize(const size_t &newPacketSize);

    private:
        boost::iostreams::mapped_file file;
        std::string filePath;
        bool isPlaying;
        size_t packetSize;
        size_t readIndex;
};

#endif /* THREADEDFILEREADER_H_ */
