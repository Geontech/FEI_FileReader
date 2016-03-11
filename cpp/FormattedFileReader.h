/*
 * FormattedFileReader.h
 *
 *  Created on: Jun 15, 2015
 *      Author: Patrick
 */

#ifndef FORMATTEDFILEREADER_H_
#define FORMATTEDFILEREADER_H_

#include "ThreadedFileReader.h"
#include <ossie/debug.h>

/*
 * The enumeration of types
 */
enum FormattedFileType {
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

class FormattedFileReader: public ThreadedFileReader
{
    ENABLE_LOGGING
    public:
        FormattedFileReader();
        virtual ~FormattedFileReader();

    public:
        // Interface functions
        inline const double& getBandwidth() const { return this->bandwidth; }

        inline const double& getCenterFrequency() const
            { return this->centerFrequency; }

        inline const bool& getComplex() const { return this->complex; }

        inline const double& getSampleRate() const { return this->sampleRate; }

        inline const FormattedFileType& getType() const { return this->type; }

        virtual bool setFilePath(const std::string &newFilePath);

    private:
        // Helper functions
        bool fromFileName(const std::string &fileName, std::string &fileHandle,
                            std::string &cf, std::string &sr, std::string &bw,
                            std::string &type, std::string &cx);
        double multiplierFromUnit(const std::string &unit);
        bool splitNumberAndUnit(const std::string &numberAndUnit,
                                std::string &number,
                                std::string &unit);

    private:
        double bandwidth;
        bool complex;
        double centerFrequency;
        double sampleRate;
        FormattedFileType type;
};

#endif /* FORMATTEDFILEREADER_H_ */
