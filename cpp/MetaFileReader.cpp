/*
 * MetaFileReader.cpp
 *
 *  Created on: Mar 17, 2016
 *      Author: pwolfram
 */

#include "MetaFileReader.h"

PREPARE_LOGGING(MetaFileReader)

/*
 * Initialize the members
 */
MetaFileReader::MetaFileReader() :
    bandwidth(-1),
    complex(true),
    centerFrequency(-1),
    sampleRate(-1),
    type(FORMAT_UNKNOWN)
{
    LOG_TRACE(MetaFileReader, __PRETTY_FUNCTION__);
}

/*
 * Will call ~MappedFileReader
 */
MetaFileReader::~MetaFileReader()
{
    LOG_TRACE(MetaFileReader, __PRETTY_FUNCTION__);
}
