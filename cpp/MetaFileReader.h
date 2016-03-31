/*
 * MetaFileReader.h
 *
 *  Created on: Mar 17, 2016
 *      Author: pwolfram
 */

#ifndef METAFILEREADER_H_
#define METAFILEREADER_H_

#include "MappedFileReader.h"

#include <ossie/debug.h>

/*
 * The enumeration of types
 */
enum MetaFileType {
    CHAR = 0,
    OCTET,
    SHORT,
    USHORT,
    LONG,
    ULONG,
    FLOAT,
    LONGLONG,
    ULONGLONG,
    DOUBLE,
    FORMAT_UNKNOWN
};

class MetaFileReader : public MappedFileReader
{
    ENABLE_LOGGING
    public:
        MetaFileReader();
        virtual ~MetaFileReader();

    public:
        // Interface functions
        inline const double& getBandwidth() const { return this->bandwidth; }

        inline const double& getCenterFrequency() const
            { return this->centerFrequency; }

        inline const bool& getComplex() const { return this->complex; }

        inline const double& getSampleRate() const { return this->sampleRate; }

        inline const MetaFileType& getType() const { return this->type; }

        inline const std::string& getFilePath() const
            { return this->filePath; }

        // Must be implemented by subclass(es)
        virtual bool setFilePath(const std::string &newFilePath) = 0;

    protected:
        double bandwidth;
        bool complex;
        double centerFrequency;
        std::string filePath;
        double sampleRate;
        MetaFileType type;
};

#endif /* METAFILEREADER_H_ */
