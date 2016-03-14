/*
 * ThreadedFileReader.h
 *
 *  Created on: Jun 9, 2015
 *      Author: Patrick
 */

#ifndef THREADEDFILEREADER_H_
#define THREADEDFILEREADER_H_

#include <boost/thread.hpp>
#include <deque>
#include <ossie/debug.h>

struct FilePacket {
    char *data;
    size_t dataSize;
    bool lastPacket;
};

class ThreadedFileReader
{
    ENABLE_LOGGING
    public:
        ThreadedFileReader();
        virtual ~ThreadedFileReader();

    public:
        // Public functions for controlling the
        // reader
        void start();
        void stop();

        // Request the next packet
        const FilePacket *getNextPacket();

        // Replace the packet
        void replacePacket(const FilePacket *replacement);

    public:
        // Interface functions
        const std::string& getFilePath() const;
        virtual bool setFilePath(const std::string &newFilePath);

        const bool& getLoopingEnabled() const;
        void setLoopingEnabled(const bool &enable);

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
        std::string filePath;
        bool isPlaying;
        bool loopingEnabled;
        size_t packetSize;
        size_t queueSize;
        boost::thread *threadHandle;
};

#endif /* THREADEDFILEREADER_H_ */
