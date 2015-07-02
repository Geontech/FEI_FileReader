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

class FormattedFileReader: public ThreadedFileReader
{
    ENABLE_LOGGING
    public:
        FormattedFileReader();
        virtual ~FormattedFileReader();

    public:
        // Interface functions
        const double& getBandwidth() const;

        const double& getCenterFrequency() const;

        const bool& getComplex() const;

        const double& getSampleRate() const;

        const std::string& getType() const;

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
        std::string type;
};

#endif /* FORMATTEDFILEREADER_H_ */
