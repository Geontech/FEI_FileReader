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
    bandwidth(-1),
    complex(true),
    centerFrequency(-1),
    sampleRate(-1),
    type("")
{
}

RedHawkFileReader::~RedHawkFileReader()
{
}

const double& RedHawkFileReader::getBandwidth() const
{
    return this->bandwidth;
}

const double& RedHawkFileReader::getCenterFrequency() const
{
    return this->centerFrequency;
}

const double& RedHawkFileReader::getSampleRate() const
{
    return this->sampleRate;
}

const std::string& RedHawkFileReader::getType() const
{
    return this->type;
}

bool RedHawkFileReader::setFilePath(const std::string &newFilePath)
{
    if (not FileReader::setFilePath(newFilePath)) {
        LOG_WARN(RedHawkFileReader, "Ignoring file name as configuration");
        return false;
    }

    // Get the name of the file from the path
    boost::filesystem::path pathObject(newFilePath);
    const std::string fileName = pathObject.filename();

    // Attempt to extract metadata from the file name
    std::string fileHandle, cf, sr, bw, type, cx;

    if (not fromFileName(fileName, fileHandle, cf, sr, bw, type, cx)) {
        LOG_WARN(RedHawkFileReader, "Unable to extract metadata from file "
                    << fileName);
        return true;
    }

    // Take the string values and turn them into doubles
    std::string number, unit;
    std::stringstream buffer;

    if (cf != "" && splitNumberAndUnit(cf, number, unit)) {
        buffer << number;
        buffer >> this->centerFrequency;
        this->centerFrequency *= multiplierFromUnit(unit);
    } else {
        this->centerFrequency = -1;
    }

    buffer.clear();

    if (sr != "" && splitNumberAndUnit(sr, number, unit)) {
        buffer << number;
        buffer >> this->sampleRate;
        this->sampleRate *= multiplierFromUnit(unit);
    } else {
        this->sampleRate = -1;
    }

    buffer.clear();

    if (bw != "" && splitNumberAndUnit(bw, number, unit)) {
        buffer << number;
        buffer >> this->bandwidth;
        this->bandwidth *= multiplierFromUnit(unit);
    } else {
        this->bandwidth = -1;
    }

    this->complex = (cx == "complex");

    std::cout << "CF: " << cf << std::endl;
    std::cout << "CF2: " << this->centerFrequency << std::endl;
    std::cout << "SR: " << sr << std::endl;
    std::cout << "SR2: " << this->sampleRate << std::endl;
    std::cout << "BW: " << bw << std::endl;
    std::cout << "BW2: " << this->bandwidth << std::endl;
    std::cout << "Type: " << type << std::endl;
    std::cout << "Complex: " << cx << std::endl;
    std::cout << "Complex2: " << this->complex << std::endl;

    return true;
}

/* Attempt to parse the file name as input for configuring the stream data
 * The format is fileHandle_CF_SR<_BW>.TYPE<R>
 * fileHandle: Any handle used to identify the data
 * CF: The center frequency of the data
 * SR: The sample rate of the data
 * BW: The optional bandwidth.  If not included, use optional complex
 *      specifier to determine bandwidth
 * TYPE: The data type of the data (32f, 16s, 8u)
 * R: When present, the data is treated as real, otherwise, complex
 */
bool RedHawkFileReader::fromFileName(const std::string &fileName,
                                    std::string &fileHandle,
                                    std::string &cf, std::string &sr,
                                    std::string &bw, std::string &type,
                                    std::string &cx)
{
    boost::regex splitterRE("([^_]+)_([^_]+)_([^_]+)_?(.*)$");
    boost::smatch results;

    LOG_INFO(RedHawkFileReader, "File name: " << fileName);

    if (not boost::regex_search(fileName.begin(), fileName.end(),
            results, splitterRE)) {
        LOG_WARN(RedHawkFileReader, "Invalid format for " << fileName <<
                    ". Should be fileHandle_CF_SR<_BW>.TYPE<R>");
        return false;
    }

    fileHandle = std::string(results[1].first, results[1].second);
    cf = std::string(results[2].first, results[2].second);

    boost::regex finalSplitterRE("([^\\.]+)\\.(.*)");
    boost::smatch innerResults;
    std::vector<std::string> strings;
    const std::string thirdValue(results[3].first, results[3].second);
    const std::string fourthValue(results[4].first, results[4].second);

    // In this case, the bandwidth hasn't been specified
    if (fourthValue == "") {
        if (not boost::regex_search(thirdValue.begin(), thirdValue.end(),
                                        innerResults, finalSplitterRE)) {
            LOG_WARN(RedHawkFileReader, "Invalid format");
            return false;
        }

        sr = std::string(innerResults[1].first, innerResults[1].second);
        bw = "";
        type = std::string(innerResults[2].first, innerResults[2].second);
    } else {
        sr = thirdValue;

        if (not boost::regex_search(fourthValue.begin(), fourthValue.end(),
                innerResults, finalSplitterRE)) {
            LOG_WARN(RedHawkFileReader, "Invalid format");
            return false;
        }

        bw = std::string(innerResults[1].first, innerResults[1].second);
        type = std::string(innerResults[2].first, innerResults[2].second);
    }

    if (type.substr(type.size() - 1) == "c" ||
            type.substr(type.size() - 1) == "C") {
        cx = "complex";

        type = type.substr(0, type.size() - 1);
    } else if (type.substr(type.size() - 1) == "r" ||
            type.substr(type.size() - 1) == "R") {
        cx = "real";

        type = type.substr(0, type.size() - 1);
    } else {
        cx = "complex";
    }

    return true;
}

double RedHawkFileReader::multiplierFromUnit(const std::string &unit)
{
    switch (unit[0]) {
        case 'k':
        case 'K':
            return 1e3;

        case 'M':
            return 1e6;

        case 'G':
            return 1e9;
    }

    return 1;
}

bool RedHawkFileReader::splitNumberAndUnit(const std::string &numberAndUnit,
                                std::string &number,
                                std::string &unit)
{
    boost::regex splitterRE("(\\d+)([^\\d\\s]*)");
    boost::smatch results;

    std::cout << "NumberAndUnit: " << numberAndUnit << std::endl;

    if (not boost::regex_search(numberAndUnit.begin(), numberAndUnit.end(),
                                    results, splitterRE)) {
        LOG_WARN(RedHawkFileReader, "Invalid format for " << numberAndUnit <<
                    ". Should be number then optional unit");
        return false;
    }

    number = std::string(results[1].first, results[1].second);
    unit = std::string(results[2].first, results[2].second);

    return true;
}
