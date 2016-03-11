#include "FEI_FileReader.h"

/*
 * Partial template specializations to only convert data if necessary
 */
template <>
void FEI_FileReader_i::convertAndCopyToAll(FileReaderContainer &container,
        std::vector<int8_t> &data)
{
    convertAndCopyPacket(this->dataOctet_out, data, container.uCharOutput);
    convertAndCopyPacket(this->dataShort_out, data, container.shortOutput);
    convertAndCopyPacket(this->dataUshort_out, data, container.uShortOutput);
    convertAndCopyPacket(this->dataLong_out, data, container.longOutput);
    convertAndCopyPacket(this->dataUlong_out, data, container.uLongOutput);
    convertAndCopyPacket(this->dataFloat_out, data, container.floatOutput);
    convertAndCopyPacket(this->dataLongLong_out, data, container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data, container.uLongLongOutput);
    convertAndCopyPacket(this->dataDouble_out, data, container.doubleOutput);
}

template <>
void FEI_FileReader_i::convertAndCopyToAll(FileReaderContainer &container,
        std::vector<uint8_t> &data)
{
    convertAndCopyPacket(this->dataChar_out, data, container.charOutput);
    convertAndCopyPacket(this->dataShort_out, data, container.shortOutput);
    convertAndCopyPacket(this->dataUshort_out, data, container.uShortOutput);
    convertAndCopyPacket(this->dataLong_out, data, container.longOutput);
    convertAndCopyPacket(this->dataUlong_out, data, container.uLongOutput);
    convertAndCopyPacket(this->dataFloat_out, data, container.floatOutput);
    convertAndCopyPacket(this->dataLongLong_out, data, container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data, container.uLongLongOutput);
    convertAndCopyPacket(this->dataDouble_out, data, container.doubleOutput);
}

template <>
void FEI_FileReader_i::convertAndCopyToAll(FileReaderContainer &container,
        std::vector<int16_t> &data)
{
    convertAndCopyPacket(this->dataChar_out, data, container.charOutput);
    convertAndCopyPacket(this->dataOctet_out, data, container.uCharOutput);
    convertAndCopyPacket(this->dataUshort_out, data, container.uShortOutput);
    convertAndCopyPacket(this->dataLong_out, data, container.longOutput);
    convertAndCopyPacket(this->dataUlong_out, data, container.uLongOutput);
    convertAndCopyPacket(this->dataFloat_out, data, container.floatOutput);
    convertAndCopyPacket(this->dataLongLong_out, data, container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data, container.uLongLongOutput);
    convertAndCopyPacket(this->dataDouble_out, data, container.doubleOutput);
}

template <>
void FEI_FileReader_i::convertAndCopyToAll(FileReaderContainer &container,
        std::vector<uint16_t> &data)
{
    convertAndCopyPacket(this->dataChar_out, data, container.charOutput);
    convertAndCopyPacket(this->dataOctet_out, data, container.uCharOutput);
    convertAndCopyPacket(this->dataShort_out, data, container.shortOutput);
    convertAndCopyPacket(this->dataLong_out, data, container.longOutput);
    convertAndCopyPacket(this->dataUlong_out, data, container.uLongOutput);
    convertAndCopyPacket(this->dataFloat_out, data, container.floatOutput);
    convertAndCopyPacket(this->dataLongLong_out, data, container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data, container.uLongLongOutput);
    convertAndCopyPacket(this->dataDouble_out, data, container.doubleOutput);
}

template <>
void FEI_FileReader_i::convertAndCopyToAll(FileReaderContainer &container,
        std::vector<int32_t> &data)
{
    convertAndCopyPacket(this->dataChar_out, data, container.charOutput);
    convertAndCopyPacket(this->dataOctet_out, data, container.uCharOutput);
    convertAndCopyPacket(this->dataShort_out, data, container.shortOutput);
    convertAndCopyPacket(this->dataUshort_out, data, container.uShortOutput);
    convertAndCopyPacket(this->dataUlong_out, data, container.uLongOutput);
    convertAndCopyPacket(this->dataFloat_out, data, container.floatOutput);
    convertAndCopyPacket(this->dataLongLong_out, data, container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data, container.uLongLongOutput);
    convertAndCopyPacket(this->dataDouble_out, data, container.doubleOutput);
}

template <>
void FEI_FileReader_i::convertAndCopyToAll(FileReaderContainer &container,
        std::vector<uint32_t> &data)
{
    convertAndCopyPacket(this->dataChar_out, data, container.charOutput);
    convertAndCopyPacket(this->dataOctet_out, data, container.uCharOutput);
    convertAndCopyPacket(this->dataShort_out, data, container.shortOutput);
    convertAndCopyPacket(this->dataUshort_out, data, container.uShortOutput);
    convertAndCopyPacket(this->dataFloat_out, data, container.floatOutput);
    convertAndCopyPacket(this->dataLongLong_out, data, container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data, container.uLongLongOutput);
    convertAndCopyPacket(this->dataDouble_out, data, container.doubleOutput);
}

template <>
void FEI_FileReader_i::convertAndCopyToAll(FileReaderContainer &container,
        std::vector<float> &data)
{
    convertAndCopyPacket(this->dataChar_out, data, container.charOutput);
    convertAndCopyPacket(this->dataOctet_out, data, container.uCharOutput);
    convertAndCopyPacket(this->dataShort_out, data, container.shortOutput);
    convertAndCopyPacket(this->dataUshort_out, data, container.uShortOutput);
    convertAndCopyPacket(this->dataUlong_out, data, container.uLongOutput);
    convertAndCopyPacket(this->dataLongLong_out, data, container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data, container.uLongLongOutput);
    convertAndCopyPacket(this->dataDouble_out, data, container.doubleOutput);
}

template <>
void FEI_FileReader_i::convertAndCopyToAll(FileReaderContainer &container,
        std::vector<int64_t> &data)
{
    convertAndCopyPacket(this->dataChar_out, data, container.charOutput);
    convertAndCopyPacket(this->dataOctet_out, data, container.uCharOutput);
    convertAndCopyPacket(this->dataShort_out, data, container.shortOutput);
    convertAndCopyPacket(this->dataUshort_out, data, container.uShortOutput);
    convertAndCopyPacket(this->dataUlong_out, data, container.uLongOutput);
    convertAndCopyPacket(this->dataFloat_out, data, container.floatOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data, container.uLongLongOutput);
    convertAndCopyPacket(this->dataDouble_out, data, container.doubleOutput);
}

template <>
void FEI_FileReader_i::convertAndCopyToAll(FileReaderContainer &container,
        std::vector<uint64_t> &data)
{
    convertAndCopyPacket(this->dataChar_out, data, container.charOutput);
    convertAndCopyPacket(this->dataOctet_out, data, container.uCharOutput);
    convertAndCopyPacket(this->dataShort_out, data, container.shortOutput);
    convertAndCopyPacket(this->dataUshort_out, data, container.uShortOutput);
    convertAndCopyPacket(this->dataUlong_out, data, container.uLongOutput);
    convertAndCopyPacket(this->dataFloat_out, data, container.floatOutput);
    convertAndCopyPacket(this->dataLongLong_out, data, container.longLongOutput);
    convertAndCopyPacket(this->dataDouble_out, data, container.doubleOutput);
}

template <>
void FEI_FileReader_i::convertAndCopyToAll(FileReaderContainer &container,
        std::vector<double> &data)
{
    convertAndCopyPacket(this->dataChar_out, data, container.charOutput);
    convertAndCopyPacket(this->dataOctet_out, data, container.uCharOutput);
    convertAndCopyPacket(this->dataShort_out, data, container.shortOutput);
    convertAndCopyPacket(this->dataUshort_out, data, container.uShortOutput);
    convertAndCopyPacket(this->dataUlong_out, data, container.uLongOutput);
    convertAndCopyPacket(this->dataFloat_out, data, container.floatOutput);
    convertAndCopyPacket(this->dataLongLong_out, data, container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data, container.uLongLongOutput);
}

/*
 * Partial template specialization to push to active ports
 */
template <>
void FEI_FileReader_i::pushPacketToAll<int8_t>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    this->dataChar_out->pushPacket(container.charOutput, T, EOS, streamID);

    pushPacketIfActive(this->dataOctet_out, container.uCharOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out, container.uLongLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput, T, EOS,
            streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<uint8_t>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    this->dataOctet_out->pushPacket(container.uCharOutput, T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out, container.uLongLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput, T, EOS,
            streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<int16_t>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    this->dataShort_out->pushPacket(container.shortOutput, T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out, container.uLongLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput, T, EOS,
            streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<uint16_t>(
        FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    this->dataUshort_out->pushPacket(container.uShortOutput, T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out, container.uLongLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput, T, EOS,
            streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<int32_t>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    this->dataLong_out->pushPacket(container.longOutput, T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out, container.uLongLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput, T, EOS,
            streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<uint32_t>(
        FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    this->dataUlong_out->pushPacket(container.uLongOutput, T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out, container.uLongLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput, T, EOS,
            streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<float>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    this->dataFloat_out->pushPacket(container.floatOutput, T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out, container.uLongLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput, T, EOS,
            streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<int64_t>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    this->dataLongLong_out->pushPacket(container.longLongOutput, T, EOS,
            streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out, container.uLongLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput, T, EOS,
            streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<uint64_t>(
        FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    this->dataUlongLong_out->pushPacket(container.uLongLongOutput, T, EOS,
            streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput, T, EOS,
            streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<double>(
        FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    this->dataDouble_out->pushPacket(container.doubleOutput, T, EOS,
            streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput, T, EOS,
            streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput, T,
            EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out, container.uLongLongOutput, T,
            EOS, streamID);
}
