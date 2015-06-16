/*
 * RedHawkFileReader.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: patrick
 */

#include "RedHawkFileReader.h"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

PREPARE_LOGGING(RedHawkFileReader)

RedHawkFileReader::RedHawkFileReader() :
    bandwidth(0),
    centerFrequency(0),
    sampleRate(0)
{
}

bool RedHawkFileReader::setFilePath(const std::string &newFilePath)
{
    if (not FileReader::setFilePath(newFilePath)) {
        LOG_WARN(RedHawkFileReader, "Ignoring file name as configuration");
        return false;
    }

    // Attempt to parse the file name as input for configuring the stream data
    // The format is fileHandle_CF_SR<_BW>.TYPE<R>
    // fileHandle: Any handle used to identify the data
    // CF: The center frequency of the data
    // SR: The sample rate of the data
    // BW: The optional bandwidth.  If not included, use optional complex
    //      specifier to determine bandwidth
    // TYPE: The data type of the data (f, h, d)
    // R: When present, the data is treated as real, otherwise, complex
    boost::filesystem::path pathObject(newFilePath);
    boost::regex re("([^_]+)_([^_]+)_([^_]+)");
    const std::string fileName = pathObject.filename().string();
    std::string::const_iterator begin = fileName.begin();
    boost::smatch result;

    while (boost::regex_search(begin, fileName.end(), result, re)) {
        size_t i;

        for (i = 0; i < result.size(); ++i) {
            std::string value(result[i].first, result[i].second);
            std::istringstream buffer;

            if (i == 2) {
                LOG_INFO(RedHawkFileReader, "CF: " << value);
                /*buffer << value;
                buffer >> this->centerFrequency;*/
            } else if (i == 3 && result.size() == 4) {
                std::vector<std::string> strings;

                boost::split(strings, value, boost::is_any_of("."));

                LOG_INFO(RedHawkFileReader, "SR: " << strings[0]);
                LOG_INFO(RedHawkFileReader, "TYPE: " << strings[1]);

                /*buffer << value;
                buffer >> this->sampleRate;*/
            }
        }

        begin = result[i-1].second;
    }

    return true;
}

RedHawkFileReader::~RedHawkFileReader()
{
}
