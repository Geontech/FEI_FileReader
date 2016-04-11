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
    convertAndCopyPacket(this->dataLongLong_out, data,
            container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data,
            container.uLongLongOutput);
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
    convertAndCopyPacket(this->dataLongLong_out, data,
            container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data,
            container.uLongLongOutput);
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
    convertAndCopyPacket(this->dataLongLong_out, data,
            container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data,
            container.uLongLongOutput);
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
    convertAndCopyPacket(this->dataLongLong_out, data,
            container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data,
            container.uLongLongOutput);
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
    convertAndCopyPacket(this->dataLongLong_out, data,
            container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data,
            container.uLongLongOutput);
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
    convertAndCopyPacket(this->dataLongLong_out, data,
            container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data,
            container.uLongLongOutput);
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
    convertAndCopyPacket(this->dataLongLong_out, data,
            container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data,
            container.uLongLongOutput);
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
    convertAndCopyPacket(this->dataUlongLong_out, data,
            container.uLongLongOutput);
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
    convertAndCopyPacket(this->dataLongLong_out, data,
            container.longLongOutput);
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
    convertAndCopyPacket(this->dataLongLong_out, data,
            container.longLongOutput);
    convertAndCopyPacket(this->dataUlongLong_out, data,
            container.uLongLongOutput);
}

/*
 * Partial template specialization to push to active ports
 */
template <>
void FEI_FileReader_i::pushPacketToAll<int8_t>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    size_t dataSize = container.currentPacket.dataSize;

    this->dataChar_out->pushPacket(container.charOutput.data(), dataSize, T,
            EOS, streamID);

    pushPacketIfActive(this->dataOctet_out, container.uCharOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput.data(),
            dataSize / sizeof(int16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput.data(),
            dataSize / sizeof(uint16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput.data(),
            dataSize / sizeof(int32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput.data(),
            dataSize / sizeof(uint32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput.data(),
            dataSize / sizeof(float), T, EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput.data(),
            dataSize / sizeof(int64_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out,
            container.uLongLongOutput.data(), dataSize / sizeof(uint64_t), T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput.data(),
            dataSize / sizeof(double), T, EOS, streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<uint8_t>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    size_t dataSize = container.currentPacket.dataSize;

    this->dataOctet_out->pushPacket(container.uCharOutput.data(), dataSize, T,
            EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput.data(),
            dataSize / sizeof(int16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput.data(),
            dataSize / sizeof(uint16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput.data(),
            dataSize / sizeof(int32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput.data(),
            dataSize / sizeof(uint32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput.data(),
            dataSize / sizeof(float), T, EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput.data(),
            dataSize / sizeof(int64_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out,
            container.uLongLongOutput.data(), dataSize / sizeof(uint64_t), T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput.data(),
            dataSize / sizeof(double), T, EOS, streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<int16_t>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    size_t dataSize = container.currentPacket.dataSize;

    this->dataShort_out->pushPacket(container.shortOutput.data(),
            dataSize / sizeof(int16_t), T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput.data(),
            dataSize / sizeof(uint16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput.data(),
            dataSize / sizeof(int32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput.data(),
            dataSize / sizeof(uint32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput.data(),
            dataSize / sizeof(float), T, EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput.data(),
            dataSize / sizeof(int64_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out,
            container.uLongLongOutput.data(), dataSize / sizeof(uint64_t), T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput.data(),
            dataSize / sizeof(double), T, EOS, streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<uint16_t>(
        FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    size_t dataSize = container.currentPacket.dataSize;

    this->dataUshort_out->pushPacket(container.uShortOutput.data(),
            dataSize / sizeof(uint16_t), T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput.data(),
            dataSize / sizeof(int16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput.data(),
            dataSize / sizeof(int32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput.data(),
            dataSize / sizeof(uint32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput.data(),
            dataSize / sizeof(float), T, EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput.data(),
            dataSize / sizeof(int64_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out,
            container.uLongLongOutput.data(), dataSize / sizeof(uint64_t), T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput.data(),
            dataSize / sizeof(double), T, EOS, streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<int32_t>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    size_t dataSize = container.currentPacket.dataSize;

    this->dataLong_out->pushPacket(container.longOutput.data(),
            dataSize / sizeof(int32_t), T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput.data(),
            dataSize / sizeof(int16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput.data(),
            dataSize / sizeof(uint16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput.data(),
            dataSize / sizeof(uint32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput.data(),
            dataSize / sizeof(float), T, EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput.data(),
            dataSize / sizeof(int64_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out,
            container.uLongLongOutput.data(), dataSize / sizeof(uint64_t), T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput.data(),
            dataSize / sizeof(double), T, EOS, streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<uint32_t>(
        FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    size_t dataSize = container.currentPacket.dataSize;

    this->dataUlong_out->pushPacket(container.uLongOutput.data(),
            dataSize / sizeof(uint32_t), T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput.data(),
            dataSize / sizeof(int16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput.data(),
            dataSize / sizeof(uint16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput.data(),
            dataSize / sizeof(int32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput.data(),
            dataSize / sizeof(float), T, EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput.data(),
            dataSize / sizeof(int64_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out,
            container.uLongLongOutput.data(), dataSize / sizeof(uint64_t), T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput.data(),
            dataSize / sizeof(double), T, EOS, streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<float>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    size_t dataSize = container.currentPacket.dataSize;

    this->dataFloat_out->pushPacket(container.floatOutput.data(),
            dataSize / sizeof(float), T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput.data(),
            dataSize / sizeof(int16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput.data(),
            dataSize / sizeof(uint16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput.data(),
            dataSize / sizeof(int32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput.data(),
            dataSize / sizeof(uint32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput.data(),
            dataSize / sizeof(int64_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out,
            container.uLongLongOutput.data(), dataSize / sizeof(uint64_t), T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput.data(),
            dataSize / sizeof(double), T, EOS, streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<int64_t>(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    size_t dataSize = container.currentPacket.dataSize;

    this->dataLongLong_out->pushPacket(container.longLongOutput.data(),
            dataSize / sizeof(int64_t), T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput.data(),
            dataSize / sizeof(int16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput.data(),
            dataSize / sizeof(uint16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput.data(),
            dataSize / sizeof(int32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput.data(),
            dataSize / sizeof(uint32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput.data(),
            dataSize / sizeof(float), T, EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out,
            container.uLongLongOutput.data(), dataSize / sizeof(uint64_t), T,
            EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput.data(),
            dataSize / sizeof(double), T, EOS, streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<uint64_t>(
        FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    size_t dataSize = container.currentPacket.dataSize;

    this->dataUlongLong_out->pushPacket(container.uLongLongOutput.data(),
            dataSize / sizeof(uint64_t), T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput.data(),
            dataSize / sizeof(int16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput.data(),
            dataSize / sizeof(uint16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput.data(),
            dataSize / sizeof(int32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput.data(),
            dataSize / sizeof(uint32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput.data(),
            dataSize / sizeof(float), T, EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput.data(),
            dataSize / sizeof(int64_t), T, EOS, streamID);
    pushPacketIfActive(this->dataDouble_out, container.doubleOutput.data(),
            dataSize / sizeof(double), T, EOS, streamID);
}

template <>
void FEI_FileReader_i::pushPacketToAll<double>(
        FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T, bool EOS,
        const std::string &streamID)
{
    size_t dataSize = container.currentPacket.dataSize;

    this->dataDouble_out->pushPacket(container.doubleOutput.data(),
            dataSize / sizeof(double), T, EOS, streamID);

    pushPacketIfActive(this->dataChar_out, container.charOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataOctet_out, container.uCharOutput.data(),
            dataSize, T, EOS, streamID);
    pushPacketIfActive(this->dataShort_out, container.shortOutput.data(),
            dataSize / sizeof(int16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUshort_out, container.uShortOutput.data(),
            dataSize / sizeof(uint16_t), T, EOS, streamID);
    pushPacketIfActive(this->dataLong_out, container.longOutput.data(),
            dataSize / sizeof(int32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlong_out, container.uLongOutput.data(),
            dataSize / sizeof(uint32_t), T, EOS, streamID);
    pushPacketIfActive(this->dataFloat_out, container.floatOutput.data(),
            dataSize / sizeof(float), T, EOS, streamID);
    pushPacketIfActive(this->dataLongLong_out, container.longLongOutput.data(),
            dataSize / sizeof(int64_t), T, EOS, streamID);
    pushPacketIfActive(this->dataUlongLong_out,
            container.uLongLongOutput.data(), dataSize / sizeof(uint64_t), T,
            EOS, streamID);
}
