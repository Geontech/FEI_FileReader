/*
 * FormattedFileReader.cpp
 *
 *  Created on: Jun 15, 2015
 *      Author: patrick
 */

#include "FormattedFileReader.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

PREPARE_LOGGING(FormattedFileReader)

/*
 * Initialize the members.  Will call MetaFileReader()
 */
FormattedFileReader::FormattedFileReader()
{
    LOG_TRACE(FormattedFileReader, __PRETTY_FUNCTION__);
}

/*
 * Will call ~MetaFileReader()
 */
FormattedFileReader::~FormattedFileReader()
{
    LOG_TRACE(FormattedFileReader, __PRETTY_FUNCTION__);
}

/*
 * Static method to check if this type of file reader is applicable to the
 * given file path
 */
bool FormattedFileReader::isValid(const std::string &filePath)
{
    LOG_TRACE(FormattedFileReader, __PRETTY_FUNCTION__);

    // Dummy variables
    std::string fileHandle;
    std::string cf;
    std::string sr;
    std::string bw;
    std::string type;
    std::string cx;

    return FormattedFileReader::fromFileName(filePath, fileHandle, cf, sr, bw,
            type, cx);
}

/*
 * Set the file path and then attempt to read the center frequency, sample
 * rate, bandwidth, file type, and complexity from the file name
 */
bool FormattedFileReader::setFilePath(const std::string &newFilePath)
{
    LOG_TRACE(FormattedFileReader, __PRETTY_FUNCTION__);

    if (not MappedFileReader::setFilePath(newFilePath)) {
        LOG_WARN(FormattedFileReader, "Ignoring file name as configuration");
        return false;
    }

    // Get the name of the file from the path
    boost::filesystem::path pathObject(newFilePath);
    const std::string fileName = pathObject.filename().c_str();

    // Attempt to extract metadata from the file name
    std::string fileHandle, cf, sr, bw, type, cx;

    if (not FormattedFileReader::fromFileName(fileName, fileHandle, cf, sr, bw,
            type, cx)) {
        LOG_WARN(FormattedFileReader, "Unable to extract metadata from file "
                    << fileName);
        return true;
    }

    // Take the string values and turn them into doubles
    std::string number, unit;
    std::stringstream buffer;

    if (cf != "" && FormattedFileReader::splitNumberAndUnit(cf, number,
            unit)) {
        buffer << number;
        buffer >> this->centerFrequency;
        this->centerFrequency *=
                FormattedFileReader::multiplierFromUnit(unit);
    } else {
        this->centerFrequency = -1;
    }

    buffer.clear();

    if (sr != "" && FormattedFileReader::splitNumberAndUnit(sr, number,
            unit)) {
        buffer << number;
        buffer >> this->sampleRate;
        this->sampleRate *=
                FormattedFileReader::multiplierFromUnit(unit);
    } else {
        this->sampleRate = -1;
    }

    buffer.clear();

    if (bw != "" && FormattedFileReader::splitNumberAndUnit(bw, number,
            unit)) {
        buffer << number;
        buffer >> this->bandwidth;
        this->bandwidth *=
                FormattedFileReader::multiplierFromUnit(unit);
    } else {
        this->bandwidth = -1;
    }

    this->complex = (cx == "complex");

    if (type == "B") {
        this->type = CHAR;
    } else if (type == "UB") {
        this->type = OCTET;
    } else if (type == "I") {
        this->type = SHORT;
    } else if (type == "UI") {
        this->type = USHORT;
    } else if (type == "L") {
        this->type = LONG;
    } else if (type == "UL") {
        this->type = ULONG;
    } else if (type == "F") {
        this->type = FLOAT;
    } else if (type == "X") {
        this->type = LONGLONG;
    } else if (type == "UX") {
        this->type = ULONGLONG;
    } else if (type == "D") {
        this->type = DOUBLE;
    } else {
        this->type = FORMAT_UNKNOWN;
    }

    return true;
}

/* Attempt to parse the file name as input for configuring the stream data
 * The format is fileHandle_CF_SR<_BW>.TYPE<R> where:
 * - fileHandle: Any handle used to identify the data
 * - CF: The center frequency of the data
 * - SR: The sample rate of the data
 * - BW: The optional bandwidth.  If not included, use optional complex
 *      specifier to determine bandwidth
 * - TYPE: The data type of the data (B, I, UB) based on MIDAS format
 * - R: When present, the data is treated as real, otherwise, complex
 */
bool FormattedFileReader::fromFileName(const std::string &fileName,
                                    std::string &fileHandle,
                                    std::string &cf, std::string &sr,
                                    std::string &bw, std::string &type,
                                    std::string &cx)
{
    LOG_TRACE(FormattedFileReader, __PRETTY_FUNCTION__);

    // Specify the regular expression used to parse the file name
    boost::regex splitterRE("([^_]+)_([^_]+)_([^_]+)_?(.*)$");
    boost::smatch results;

    if (not boost::regex_search(fileName.begin(), fileName.end(),
            results, splitterRE)) {
        LOG_WARN(FormattedFileReader, "Invalid format for " << fileName <<
                    ". Should be fileHandle_CF_SR<_BW>.TYPE<R>");
        return false;
    }

    // If the regular expression search succeeded, these two values are
    // guaranteed to be present
    fileHandle = std::string(results[1].first, results[1].second);
    cf = std::string(results[2].first, results[2].second);

    // Specify the regular expression used to split on the period
    boost::regex finalSplitterRE("([^\\.]+)\\.(.*)");
    boost::smatch innerResults;

    // Test the fourth value to determine what the contents of these strings
    // are
    const std::string thirdValue(results[3].first, results[3].second);
    const std::string fourthValue(results[4].first, results[4].second);

    // In this case, the bandwidth hasn't been specified
    if (fourthValue == "") {
        if (not boost::regex_search(thirdValue.begin(), thirdValue.end(),
                                        innerResults, finalSplitterRE)) {
            LOG_WARN(FormattedFileReader, "Invalid format");
            return false;
        }

        sr = std::string(innerResults[1].first, innerResults[1].second);
        bw = "";
        type = std::string(innerResults[2].first, innerResults[2].second);
    } else {
        sr = thirdValue;

        if (not boost::regex_search(fourthValue.begin(), fourthValue.end(),
                innerResults, finalSplitterRE)) {
            LOG_WARN(FormattedFileReader, "Invalid format");
            return false;
        }

        bw = std::string(innerResults[1].first, innerResults[1].second);
        type = std::string(innerResults[2].first, innerResults[2].second);
    }

    // Check the last character to determine the data complexity
    if (type.substr(type.size() - 1) == "r" ||
            type.substr(type.size() - 1) == "R") {
        cx = "real";

        type = type.substr(0, type.size() - 1);
    } else if (type.substr(type.size() - 1) == "c" ||
            type.substr(type.size() - 1) == "C") {
        cx = "complex";

        type = type.substr(0, type.size() - 1);
    } else {
        cx = "complex";
    }

    return true;
}

/*
 * Given a unit such as MHz, kSps, GHz, etc., return the multiplier associated
 * with the prefix
 */
double FormattedFileReader::multiplierFromUnit(const std::string &unit)
{
    LOG_TRACE(FormattedFileReader, __PRETTY_FUNCTION__);

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

/*
 * Given a number and unit such as 10MSps or 99.1MHz, split the number and unit
 * into two separate strings
 */
bool FormattedFileReader::splitNumberAndUnit(const std::string &numberAndUnit,
                                std::string &number,
                                std::string &unit)
{
    LOG_TRACE(FormattedFileReader, __PRETTY_FUNCTION__);

    // Specify the regular expression used to split the number and unit
    boost::regex splitterRE("(\\d*\\.?\\d*)([^\\d\\s]*)");
    boost::smatch results;

    if (not boost::regex_search(numberAndUnit.begin(), numberAndUnit.end(),
                                    results, splitterRE)) {
        LOG_WARN(FormattedFileReader, "Invalid format for " << numberAndUnit <<
                    ". Should be number then optional unit");
        return false;
    }

    number = std::string(results[1].first, results[1].second);
    unit = std::string(results[2].first, results[2].second);

    return true;
}
