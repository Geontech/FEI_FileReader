/*
 * FormattedFileReader.h
 *
 *  Created on: Jun 15, 2015
 *      Author: Patrick
 */

#ifndef FORMATTEDFILEREADER_H_
#define FORMATTEDFILEREADER_H_

#include "MetaFileReader.h"
#include <ossie/debug.h>

class FormattedFileReader: public MetaFileReader
{
    ENABLE_LOGGING
    public:
        FormattedFileReader();
        virtual ~FormattedFileReader();

    public:
        static bool isValid(const std::string &filePath);

    public:
        // Interface functions
        virtual bool setFilePath(const std::string &newFilePath);

    private:
        // Helper functions
        static bool fromFileName(const std::string &fileName,
                std::string &fileHandle,std::string &cf, std::string &sr,
                std::string &bw, std::string &type, std::string &cx);
        static double multiplierFromUnit(const std::string &unit);
        static bool splitNumberAndUnit(const std::string &numberAndUnit,
                                std::string &number,
                                std::string &unit);
};

#endif /* FORMATTEDFILEREADER_H_ */
