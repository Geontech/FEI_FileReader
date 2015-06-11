/*
 * FileReader.h
 *
 *  Created on: Jun 9, 2015
 *      Author: Patrick
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <boost/thread.hpp>
#include <deque>
#include <ossie/debug.h>

struct FilePacket {
    char *data;
    size_t dataSize;
    std::string filename;
    bool firstPacket;
};

class FileReader
{
    ENABLE_LOGGING
    public:
        FileReader();
        virtual ~FileReader();

    public:
        // Public functions for controlling the
        // reader
        void pause();
        void restart();
        void start();
        void stop();

        // Request the status of the file reader
        bool isReady() const;

        // Request the next packet
        FilePacket* const getNextPacket();

        // Replace the packet
        void replacePacket(FilePacket* const);

    public:
        // Interface functions
        const std::string& getFilepath() const;
        bool setFilepath(const std::string &newFilepath);

        const size_t& getPacketSize() const;
        void setPacketSize(const size_t &newPacketSize);

        const size_t& getQueueSize() const;
        void setQueueSize(const size_t &newQueueSize);

    private:
        // Threading function
        void fileReaderWorkFunction();

        // Data functions
        void clearQueue(std::deque<FilePacket *> &queue);
        void clearQueues();
        void initializeQueues();
        void resetQueues();

    private:
        std::deque<FilePacket *> allocatedFilePackets;
        boost::condition_variable allocatedPacketAvailable;
        boost::mutex allocatedQueueLock;
        std::deque<FilePacket *> freeFilePackets;
        boost::condition_variable freePacketAvailable;
        boost::mutex freeQueueLock;
        std::string filepath;
        bool isPlaying;
        size_t packetSize;
        size_t queueSize;
        boost::thread *threadHandle;
};

#endif /* FILEREADER_H_ */
