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
    boost::filesystem::path pathObject(newFilePath);

    LOG_INFO(RedHawkFileReader, pathObject.filename().string());

    std::vector<std::string> strings;

    // The format is filename_CF_SR<_BW>.TYPE<R>
    // filename: Any filename used to identify the data
    // CF: The center frequency of the data
    // SR: The sample rate of the data
    // BW: The optional bandwidth.  If not included, use optional complex
    //      specifier to determine bandwidth
    // TYPE: The data type of the data (f, h, d)
    // R: When present, the data is treated as real, otherwise, complex
    boost::regex e("([^_]+)_([^_]+)_([^_]+)");
    boost::smatch what;

    if (boost::regex_search(pathObject.filename().string(), what, e)) {
        LOG_INFO(RedHawkFileReader, what.size());

        for (size_t i = 0; i < what.size(); ++i) {
            LOG_INFO(RedHawkFileReader, what[i].str());
        }
    }

    /*boost::split(strings, pathObject.filename().string(), boost::is_any_of("_"));

    for (std::vector<std::string>::iterator i = strings.begin(); i != strings.end(); ++i) {
        LOG_INFO(RedHawkFileReader, *i);
    }*/

    return true;
}

RedHawkFileReader::~RedHawkFileReader()
{
}
