/*
 * RedHawkFileReader.h
 *
 *  Created on: Jun 15, 2015
 *      Author: patrick
 */

#ifndef REDHAWKFILEREADER_H_
#define REDHAWKFILEREADER_H_

#include "FileReader.h"
#include <ossie/debug.h>

class RedHawkFileReader: public FileReader
{
    ENABLE_LOGGING
    public:
        RedHawkFileReader();
        virtual ~RedHawkFileReader();

    public:
        virtual bool setFilePath(const std::string &newFilePath);

    private:
        double bandwidth;
        double centerFrequency;
        double sampleRate;
};

#endif /* REDHAWKFILEREADER_H_ */
