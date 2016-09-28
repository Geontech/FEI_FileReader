/**************************************************************************

    This is the device code. This file contains the child class where
    custom functionality can be added to the device. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

**************************************************************************/

#include "FEI_FileReader.h"

#include "FormattedFileReader.h"

#include <boost/date_time/time_defs.hpp>
#include <boost/filesystem.hpp>

PREPARE_LOGGING(FEI_FileReader_i)

/*
 * Construct the device by calling the construct method
 */
FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl,
                                    char *sftwrPrfl) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl),
    fractionalResolution(1)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);
}

/*
 * Construct the device by calling the construct method
 */
FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl,
                                    char *sftwrPrfl, char *compDev) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl, compDev),
    fractionalResolution(1)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);
}

/*
 * Construct the device by calling the construct method
 */
FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl,
                                    char *sftwrPrfl,
                                    CF::Properties capacities) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl, capacities),
    fractionalResolution(1)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);
}

/*
 * Construct the device by calling the construct method
 */
FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl,
                                    char *sftwrPrfl,
                                    CF::Properties capacities, char *compDev) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl, capacities, compDev),
    fractionalResolution(1)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);
}

/*
 * Destroy the device by cleaning up the file reader pointers
 */
FEI_FileReader_i::~FEI_FileReader_i()
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    for (FileReaderIterator i = this->fileReaderContainers.begin();
            i != this->fileReaderContainers.end(); ++i) {
        delete i->fileReader;
    }
}

/*
 * Override the initialize method to start the device automatically
 */
void FEI_FileReader_i::initialize() throw (CF::LifeCycle::InitializeError,
                                        CORBA::SystemException)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    FEI_FileReader_base::initialize();

    // Start the device after initialization
    start();
}

/*
 * Iterate through the file reader containers and push the data, if all
 * conditions are met
 */
int FEI_FileReader_i::serviceFunction()
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    bulkio::InFilePort::DataTransferType *pkt = this->dataFile_in->
            getPacket(bulkio::Const::BLOCKING);

    if (not pkt) {
        return NOOP;
    }

    // Attempt to set the new file path
    std::string oldValue = this->filePath;
    this->filePath = pkt->dataBuffer;

    if (not setFilePath(this->filePath)) {
        LOG_WARN(FEI_FileReader_i, "Unable to set file path, reverting");
        this->filePath = oldValue;
    }

    delete pkt;

    return NORMAL;
}

/*
 * Disable all of the file readers
 */
void FEI_FileReader_i::stop() throw (CF::Resource::StopError,
                                        CORBA::SystemException)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    // Iterate through file readers to disable any enabled ones
    for (size_t tunerId = 0; tunerId < this->fileReaderContainers.size();
            ++tunerId) {
        deviceDisable(this->frontend_tuner_status[tunerId], tunerId);
    }

    if (FEI_FileReader_base::started()) {
        FEI_FileReader_base::stop();
    }
}

/*************************************************************
Functions supporting tuning allocation
*************************************************************/
void FEI_FileReader_i::deviceEnable(frontend_tuner_status_struct_struct &fts,
                                        size_t tuner_id)
{
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    Make sure to set the 'enabled' member of fts to indicate that tuner as
    enabled
    ************************************************************/
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    fts.enabled = true;
    return;
}

void FEI_FileReader_i::deviceDisable(frontend_tuner_status_struct_struct &fts,
                                        size_t tuner_id)
{
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    Make sure to reset the 'enabled' member of fts to indicate that tuner as
    disabled
    ************************************************************/
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    fts.enabled = false;
    return;
}

bool FEI_FileReader_i::deviceSetTuning(
        const frontend::frontend_tuner_allocation_struct &request,
        frontend_tuner_status_struct_struct &fts, size_t tuner_id)
{
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    return true if the tuning succeeded, and false if it failed
    ************************************************************/
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (tuner_id >= this->fileReaderContainers.size()) {
        LOG_ERROR(FEI_FileReader_i, "ERR: INVALID TUNER ID");
        return false;
    }

    if (fts.tuner_type == "RX_DIGITIZER") {
        try {
            FileReaderContainer &container =
                    this->fileReaderContainers[tuner_id];

            if (not frontend::validateRequestVsDevice(request, this->rfInfoPkt,
                    container.fileReader->getComplex(),
                    container.fileReader->getCenterFrequency(),
                    container.fileReader->getCenterFrequency(),
                    container.bandwidth,
                    container.fileReader->getSampleRate())) {
                throw FRONTEND::BadParameterException("INVALID REQUEST --" \
                                "falls outside of file capabilities");
            }
        } catch (FRONTEND::BadParameterException &e) {
            LOG_DEBUG(FEI_FileReader_i,"deviceSetTuning|" \
                    "BadParameterException - " << e.msg);
            return false;
        }

        FileReaderContainer &container = this->fileReaderContainers[tuner_id];

        container.updateSRI = true;

        if (not container.thread) {
            boost::mutex::scoped_lock lock(*container.lock);

            container.thread = new boost::thread(
                    &FEI_FileReader_i::threadFunction, this, tuner_id,
                    request.allocation_id);
        }
    }

    return true;
}

bool FEI_FileReader_i::deviceDeleteTuning(
        frontend_tuner_status_struct_struct &fts, size_t tuner_id)
{
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    return true if the tune deletion succeeded, and false if it failed
    ************************************************************/
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (tuner_id >= this->fileReaderContainers.size()) {
        LOG_ERROR(FEI_FileReader_i, "ERR: INVALID TUNER ID");
        return false;
    }

    FileReaderContainer &container = this->fileReaderContainers[tuner_id];

    if (container.thread) {
        boost::mutex::scoped_lock lock(*container.lock);

        container.thread->interrupt();
        container.thread->join();
        delete container.thread;
        container.thread = NULL;
    }

    return true;
}

/*
 * Update the packet and queue size for every file reader
 */
void FEI_FileReader_i::AdvancedPropertiesChanged(
        const AdvancedProperties_struct *oldValue,
        const AdvancedProperties_struct *newValue)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (not setAdvancedProperties(*newValue)) {
        LOG_WARN(FEI_FileReader_i, "Unable to set Advanced Properties, " \
                "reverting");
        this->AdvancedProperties = *oldValue;
    }

    // Update all SRI values
    if (newValue->maxOutputRate != oldValue->maxOutputRate ||
            newValue->minOutputRate != oldValue->minOutputRate ||
            newValue->sampleRateForSRI != oldValue->sampleRateForSRI) {
        for (size_t i = 0; i < this->fileReaderContainers.size(); ++i) {
            this->fileReaderContainers[i].updateSRI = true;
        }
    }
}

/*
 * Initialize property change listeners, set the fractional resolution,
 * and initialize the rfinfo packet
 */
void FEI_FileReader_i::constructor()
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    // Setup based on initial property values
    setAdvancedProperties(this->AdvancedProperties);
    setFilePath(this->filePath);
    setGroupId(this->group_id);

    if (this->updateAvailableFiles) {
        this->updateAvailableFiles = false;
    }

    // Initialize property change listeners
    addPropertyChangeListener("AdvancedProperties", this,
            &FEI_FileReader_i::AdvancedPropertiesChanged);
    addPropertyChangeListener("filePath", this,
            &FEI_FileReader_i::filePathChanged);
    addPropertyChangeListener("group_id", this,
            &FEI_FileReader_i::group_idChanged);
    addPropertyChangeListener("updateAvailableFiles", this,
            &FEI_FileReader_i::updateAvailableFilesChanged);

    // Initialize the fractional resolution value
    switch (boost::posix_time::time_duration::resolution()) {
        case boost::date_time::nano:
            this->fractionalResolution = 1e9;
            break;

        case boost::date_time::micro:
            this->fractionalResolution = 1e6;
            break;

        case boost::date_time::milli:
            this->fractionalResolution = 1e3;
            break;

        default:
            this->fractionalResolution = 1;
    }

    // Initialize the RF Info Packet with very large ranges
    this->rfInfoPkt.rf_flow_id = "FEI_FILEREADER_FLOW_ID_NOT_SET";
    this->rfInfoPkt.rf_center_freq = 50e9;
    this->rfInfoPkt.rf_bandwidth = 100e9;
    this->rfInfoPkt.if_center_freq = 0;
}

/*
 * Given a container, use its type info to perform a copy to the correct vector
 * and then call a templated conversion function to conditionally copy to other
 * vector types based on the state of the associated port
 */
void FEI_FileReader_i::convertAndCopy(FileReaderContainer &container)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    const MetaFileType type = container.fileReader->getType();

    if (type == CHAR) {
        memcpy(container.charOutput.data(), container.currentPacket.data,
                container.currentPacket.dataSize);

        convertAndCopyToAll(container, container.charOutput);
    } else if (type == OCTET) {
        memcpy(container.uCharOutput.data(), container.currentPacket.data,
                container.currentPacket.dataSize);

        convertAndCopyToAll(container, container.uCharOutput);
    } else if (type == SHORT) {
        memcpy(container.shortOutput.data(), container.currentPacket.data,
                container.currentPacket.dataSize);

        convertAndCopyToAll(container, container.shortOutput);
    } else if (type == USHORT) {
        memcpy(container.uShortOutput.data(), container.currentPacket.data,
                container.currentPacket.dataSize);

        convertAndCopyToAll(container, container.uShortOutput);
    } else if (type == LONG) {
        memcpy(container.longOutput.data(), container.currentPacket.data,
                container.currentPacket.dataSize);

        convertAndCopyToAll(container, container.longOutput);
    } else if (type == ULONG) {
        memcpy(container.uLongOutput.data(), container.currentPacket.data,
                container.currentPacket.dataSize);

        convertAndCopyToAll(container, container.uLongOutput);
    } else if (type == FLOAT) {
        memcpy(container.floatOutput.data(), container.currentPacket.data,
                container.currentPacket.dataSize);

        convertAndCopyToAll(container, container.floatOutput);
    } else if (type == LONGLONG) {
        memcpy(container.longLongOutput.data(), container.currentPacket.data,
                container.currentPacket.dataSize);

        convertAndCopyToAll(container, container.longLongOutput);
    } else if (type == ULONGLONG) {
        memcpy(container.uLongLongOutput.data(), container.currentPacket.data,
                container.currentPacket.dataSize);

        convertAndCopyToAll(container, container.uLongLongOutput);
    } else if (type == DOUBLE) {
        memcpy(container.doubleOutput.data(), container.currentPacket.data,
                container.currentPacket.dataSize);

        convertAndCopyToAll(container, container.doubleOutput);
    } else {
        LOG_WARN(FEI_FileReader_i, "Unrecognized file type");
    }
}

/*
 * If the new file path exists, set the new file path appropriately
 */
void FEI_FileReader_i::filePathChanged(const std::string *oldValue,
        const std::string *newValue)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (not setFilePath(*newValue)) {
        LOG_WARN(FEI_FileReader_i, "Unable to set file path, reverting");
        this->filePath = *oldValue;
    }
}

/*
 * Set the group ID appropriately
 */
void FEI_FileReader_i::group_idChanged(const std::string *oldValue,
        const std::string *newValue)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (not setGroupId(*newValue)) {
        LOG_WARN(FEI_FileReader_i, "Unable to set group ID, reverting");
        this->group_id = *oldValue;
    }
}

/*
 * Get the stream ID for the given tuner ID, or create one if it doesn't
 * already exist
 */
std::string FEI_FileReader_i::getStreamId(size_t tunerId)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (tunerId >= this->fileReaderContainers.size()) {
        return "ERR: INVALID TUNER ID";
    }

    if (this->frontend_tuner_status[tunerId].stream_id.empty()) {
        std::ostringstream id;

        id << "tuner_freq_" <<
                long(this->frontend_tuner_status[tunerId].center_frequency) <<
                "_Hz_" << frontend::uuidGenerator();

        frontend_tuner_status[tunerId].stream_id = id.str();
        this->fileReaderContainers[tunerId].updateSRI = true;
    }

    return this->frontend_tuner_status[tunerId].stream_id;
}

/*
 * Given a type, resize the appropriate vector for container
 */
void FEI_FileReader_i::initializeOutputVectorByType(
        FileReaderContainer &container)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    size_t bytes = this->AdvancedProperties.packetSize;
    const MetaFileType type = container.fileReader->getType();
    size_t typeSize = container.typeSize;

    // First, clear out all of the vectors
    container.charOutput.resize(0);
    container.uCharOutput.resize(0);
    container.shortOutput.resize(0);
    container.uShortOutput.resize(0);
    container.longOutput.resize(0);
    container.uLongOutput.resize(0);
    container.floatOutput.resize(0);
    container.longLongOutput.resize(0);
    container.uLongLongOutput.resize(0);
    container.doubleOutput.resize(0);

    // Now resize only the relevant vector
    if (type == CHAR) {
        container.charOutput.resize(bytes);
    } else if (type == OCTET) {
        container.uCharOutput.resize(bytes);
    } else if (type == SHORT) {
        container.shortOutput.resize(bytes / typeSize);
    } else if (type == USHORT) {
        container.uShortOutput.resize(bytes / typeSize);
    } else if (type == LONG) {
        container.longOutput.resize(bytes / typeSize);
    } else if (type == ULONG) {
        container.uLongOutput.resize(bytes / typeSize);
    } else if (type == FLOAT) {
        container.floatOutput.resize(bytes / typeSize);
    } else if (type == LONGLONG) {
        container.longLongOutput.resize(bytes / typeSize);
    } else if (type == ULONGLONG) {
        container.uLongLongOutput.resize(bytes / typeSize);
    } else if (type == DOUBLE) {
        container.doubleOutput.resize(bytes / typeSize);
    } else {
        LOG_WARN(FEI_FileReader_i, "Unrecognized file type");
    }
}

/*
 * Initialize the output vector of each file reader by the file type
 */
void FEI_FileReader_i::initializeOutputVectors()
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    for (FileReaderIterator i = this->fileReaderContainers.begin();
            i != this->fileReaderContainers.end(); ++i) {
        initializeOutputVectorByType(*i);
    }
}

/*
 * Given a file reader container, push packets to active ports
 */
void FEI_FileReader_i::pushPacket(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T,
        bool EOS,
        const std::string &streamID)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    const MetaFileType type = container.fileReader->getType();

    if (type == CHAR) {
        pushPacketToAll<int8_t>(container, T, EOS, streamID);
    } else if (type == OCTET) {
        pushPacketToAll<uint8_t>(container, T, EOS, streamID);
    } else if (type == SHORT) {
        pushPacketToAll<int16_t>(container, T, EOS, streamID);
    } else if (type == USHORT) {
        pushPacketToAll<uint16_t>(container, T, EOS, streamID);
    } else if (type == LONG) {
        pushPacketToAll<int32_t>(container, T, EOS, streamID);
    } else if (type == ULONG) {
        pushPacketToAll<uint32_t>(container, T, EOS, streamID);
    } else if (type == FLOAT) {
        pushPacketToAll<float>(container, T, EOS, streamID);
    } else if (type == LONGLONG) {
        pushPacketToAll<int64_t>(container, T, EOS, streamID);
    } else if (type == ULONGLONG) {
        pushPacketToAll<uint64_t>(container, T, EOS, streamID);
    } else if (type == DOUBLE) {
        pushPacketToAll<double>(container, T, EOS, streamID);
    } else {
        LOG_WARN(FEI_FileReader_i, "Unrecognized file type");
    }
}

/*
 * Given an SRI object, push it to all ports
 */
void FEI_FileReader_i::pushSRI(BULKIO::StreamSRI &sri)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    this->dataChar_out->pushSRI(sri);
    this->dataOctet_out->pushSRI(sri);
    this->dataShort_out->pushSRI(sri);
    this->dataUshort_out->pushSRI(sri);
    this->dataLong_out->pushSRI(sri);
    this->dataUlong_out->pushSRI(sri);
    this->dataFloat_out->pushSRI(sri);
    this->dataLongLong_out->pushSRI(sri);
    this->dataUlongLong_out->pushSRI(sri);
    this->dataDouble_out->pushSRI(sri);
}

/*
 * Set the AdvancedProperties member
 */
bool FEI_FileReader_i::setAdvancedProperties(
        const AdvancedProperties_struct &newValue)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    // Verify the requested rates are valid
    if ((long) newValue.maxOutputRate <= 0) {
        LOG_WARN(FEI_FileReader_i, "Maximum sample rate must be greater " <<
                "than 0, setting to 1");
        this->AdvancedProperties.maxOutputRate = 1;
    }
    
    if ((long) newValue.minOutputRate <= 0) {
        LOG_WARN(FEI_FileReader_i, "Minimum sample rate must be greater " <<
                "than 0, setting to 1");
        this->AdvancedProperties.minOutputRate = 1;
    }

    if (newValue.minOutputRate > newValue.maxOutputRate) {
        LOG_WARN(FEI_FileReader_i, "Minimum sample rate must be less than " <<
                "or equal to the maximum sample rate, reverting");
        this->AdvancedProperties.minOutputRate =
                this->AdvancedProperties.maxOutputRate;
    }

    setPacketSizes(newValue.packetSize);

    return true;
}

/*
 * Set the filePath member, failing if it is invalid
 */
bool FEI_FileReader_i::setFilePath(const std::string &newValue)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (not boost::filesystem::exists(newValue)) {
        LOG_WARN(FEI_FileReader_i, "Invalid file path");
        return false;
    }

    updateAvailableFilesVector();

    LOG_DEBUG(FEI_FileReader_i, "Found " <<
            this->fileReaderContainers.size() << " files to read");

    return true;
}

/*
 * Set the group_id member, failing if it is invalid
 */
bool FEI_FileReader_i::setGroupId(const std::string &newValue)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    for (size_t i = 0; i < this->frontend_tuner_status.size(); ++i) {
        this->frontend_tuner_status[i].group_id = newValue;
    }

    return true;
}

/*
 * Set the packet size for each file reader and reinitialize the output vectors
 */
void FEI_FileReader_i::setPacketSizes(size_t packetSize)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    for (FileReaderIterator i = this->fileReaderContainers.begin();
            i != this->fileReaderContainers.end(); ++i) {
        i->fileReader->setPacketSize(packetSize);
    }

    initializeOutputVectors();
}

/*
 * Return the size in bytes of the specified type
 */
size_t FEI_FileReader_i::sizeFromType(const MetaFileType &type)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (type == CHAR) {
        return sizeof(int8_t);
    } else if (type == OCTET) {
        return sizeof(uint8_t);
    } else if (type == SHORT) {
        return sizeof(int16_t);
    } else if (type == USHORT) {
        return sizeof(uint16_t);
    } else if (type == LONG) {
        return sizeof(int32_t);
    } else if (type == ULONG) {
        return sizeof(uint32_t);
    } else if (type == FLOAT) {
        return sizeof(float);
    } else if (type == LONGLONG) {
        return sizeof(int64_t);
    } else if (type == ULONGLONG) {
        return sizeof(uint64_t);
    } else if (type == DOUBLE) {
        return sizeof(double);
    } else {
        LOG_WARN(FEI_FileReader_i, "Unrecognized file type");
    }

    return 0;
}

void FEI_FileReader_i::threadFunction(const size_t &tunerId,
        const std::string &allocationId)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    FileReaderContainer &container = this->fileReaderContainers[tunerId];
    boost::system_time firstSeen;
    std::string oldStreamId;
    boost::posix_time::time_duration pushDelay, timeBetweenPackets;
    BULKIO::PrecisionUTCTime timestamp;

    // This loop allows for EOS to be sent between loop iterations, if enabled
    while (true) {
        // Get a new stream ID
        std::string streamId = getStreamId(tunerId);

        // Clear the old connection table entries
        if (not oldStreamId.empty()) {
            removeStreamIdRouting(oldStreamId, allocationId);
        }

        // Enable multi-out capability
        matchAllocationIdToStreamId(allocationId, streamId, "");

        // Push initial SRI
        container.sri = create(streamId,
                this->frontend_tuner_status[tunerId]);

        pushSRI(container.sri);

        // Check to see if the channel is allocated before requesting a data
        // packet
        while (true) {
            if (boost::this_thread::interruption_requested()) {
                break;
            }

            if (this->tuner_allocation_ids[tunerId].control_allocation_id.
                            empty()
                    || not this->frontend_tuner_status[tunerId].enabled) {
                boost::posix_time::time_duration
                        timeToSleep(boost::posix_time::milliseconds(250));

                // Catch the exception to ensure EOS is sent
                try {
                    boost::this_thread::sleep(timeToSleep);
                } catch (boost::thread_interrupted &e) {
                    break;
                } catch(...) {
                    LOG_WARN(FEI_FileReader_i, "An unexpected error" <<
                            " occurred while thread waited to be enabled");
                    break;
                }

                continue;
            }

            boost::mutex::scoped_lock lock(*container.lock);

            container.currentPacket = container.
                    fileReader->getNextPacket(
                            this->AdvancedProperties.packetSize);

            if (container.currentPacket.data == NULL) {
                LOG_WARN(FEI_FileReader_i, "Got NULL data for tuner number " <<
                        tunerId);
                continue;
            }

            // If the update SRI flag is set, push the SRI packet
            if (container.updateSRI) {
                container.sri = create(streamId,
                        this->frontend_tuner_status[tunerId]);
                container.sri.mode = container.fileReader->getComplex();

                double fileSampleRate = this->frontend_tuner_status[tunerId].
                                            sample_rate;

                if (this->AdvancedProperties.sampleRateForSRI) {
                    if (fileSampleRate >
                            this->AdvancedProperties.maxOutputRate) {
                        fileSampleRate =
                                this->AdvancedProperties.maxOutputRate;
                    } else if (fileSampleRate <
                            this->AdvancedProperties.minOutputRate) {
                        fileSampleRate =
                                this->AdvancedProperties.minOutputRate;
                    }
                }

                container.sri.xdelta = 1.0 / fileSampleRate;

                pushSRI(container.sri);

                container.updateSRI = false;
            }

            // Get the current time for keeping track of when to push
            firstSeen = boost::get_system_time();
            timestamp = bulkio::time::utils::now();

            // Calculate the time duration for this packet based on the
            // number of samples, the requested sample rate, and the complexity
            size_t bytes = container.currentPacket.dataSize;

            size_t samples = bytes / container.typeSize;

            if (container.fileReader->getComplex()) {
                samples /= 2;
            }

            double sampleRate = this->frontend_tuner_status[tunerId].
                                            sample_rate;

            if (sampleRate > this->AdvancedProperties.maxOutputRate) {
                sampleRate = this->AdvancedProperties.maxOutputRate;
            } else if (sampleRate < this->AdvancedProperties.minOutputRate) {
                sampleRate = this->AdvancedProperties.minOutputRate;
            }

            double timeDuration = samples / sampleRate;
            int seconds = timeDuration;
            int fractional = this->fractionalResolution *
                    (timeDuration - seconds);

            timeBetweenPackets = boost::posix_time::time_duration(0, 0,
                    seconds, fractional);

            // Convert the packet now
            convertAndCopy(container);

            // Take note of the current time to calculate the pushPacket delay
            boost::system_time prePushTime = boost::get_system_time();

            pushPacket(container, timestamp, false, streamId);

            pushDelay = (boost::get_system_time() - prePushTime);

            lock.unlock();

            if (container.currentPacket.lastPacket && not this->loop) {
                LOG_DEBUG(FEI_FileReader_i, "Last packet and not looping");
                break;
            } else if (container.currentPacket.lastPacket &&
                    this->AdvancedProperties.newStreamAfterLoop) {
                LOG_DEBUG(FEI_FileReader_i, "Last packet and new stream");
                break;
            }

            boost::posix_time::time_duration timeToWait =
                    timeBetweenPackets -
                    (boost::get_system_time() - firstSeen) -
                    pushDelay;

            // Catch the exception to ensure the EOS is sent
            try {
                boost::this_thread::sleep(timeToWait);
            } catch (boost::thread_interrupted &e) {
                break;
            } catch(...) {
                LOG_WARN(FEI_FileReader_i, "An unexpected error occurred" <<
                        " while thread waited for next push");
                break;
            }
        }

        // Send the EOS at this point
        pushSRI(container.sri);

        container.updateSRI = false;

        // Set the data size to zero for the EOS packet
        container.currentPacket.dataSize = 0;

        BULKIO::PrecisionUTCTime T = bulkio::time::utils::now();

        pushPacket(container, T, true, streamId);

        this->frontend_tuner_status[tunerId].stream_id = "";

        // Break if necessary
        if (boost::this_thread::interruption_requested() ||
                not this->AdvancedProperties.newStreamAfterLoop) {
            break;
        }

        // Save the old stream ID for cleaning up the connection table
        oldStreamId = streamId;
    }
}

/*
 * Return the string associated with each type
 */
const MetaFileType FEI_FileReader_i::typeFromTypeInfo(
        const std::type_info &typeInfo)
{
    if (typeInfo == typeid(int8_t)) {
        return CHAR;
    } else if (typeInfo == typeid(uint8_t)) {
        return OCTET;
    } else if (typeInfo == typeid(int16_t)) {
        return SHORT;
    } else if (typeInfo == typeid(uint16_t)) {
        return USHORT;
    } else if (typeInfo == typeid(int32_t)) {
        return LONG;
    } else if (typeInfo == typeid(uint32_t)) {
        return ULONG;
    } else if (typeInfo == typeid(float)) {
        return FLOAT;
    } else if (typeInfo == typeid(int64_t)) {
        return LONGLONG;
    } else if (typeInfo == typeid(uint64_t)) {
        return ULONGLONG;
    } else if (typeInfo == typeid(double)) {
        return DOUBLE;
    } else {
        LOG_WARN(FEI_FileReader_i, "Unrecognized data type");
    }

    return FORMAT_UNKNOWN;
}

/*
 * Clear the current available files vector and use the current file path to
 * read the path as either a single file or a directory of files
 */
void FEI_FileReader_i::updateAvailableFilesVector()
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    // Deallocate all current tuners
    for (size_t i = 0; i < this->frontend_tuner_status.size(); ++i) {
        enableTuner(i, false);
        removeTunerMapping(i);
        this->frontend_tuner_status[i].allocation_id_csv =
                createAllocationIdCsv(i);
    }

    this->availableFiles.clear();

    // Read the file(s)
    if (boost::filesystem::is_regular_file(this->filePath)) {
        File_struct file;
        file.path = this->filePath;
        file.size = boost::filesystem::file_size(this->filePath);

        this->availableFiles.push_back(file);
    } else if (boost::filesystem::is_directory(this->filePath)) {
        for (boost::filesystem::directory_iterator i(this->filePath);
                i != boost::filesystem::directory_iterator(); i++) {
            boost::filesystem::directory_entry e(*i);

            if (boost::filesystem::is_regular_file(e)) {
                File_struct file;
                file.path = e.path().string();
                file.size = boost::filesystem::file_size(file.path);

                this->availableFiles.push_back(file);
            }
        }
    } else {
        LOG_WARN(FEI_FileReader_i, "Unsupported file type (symbolic link," \
                                        "empty file/directory, etc.)");
    }

    // Use the new list of available files to update the list
    updateFileReaders();

    // Initialize the frontend tuner status structures
    setNumChannels(this->fileReaderContainers.size());

    for (size_t tunerId = 0; tunerId < this->fileReaderContainers.size();
            ++tunerId) {
        this->frontend_tuner_status[tunerId].allocation_id_csv = "";
        this->frontend_tuner_status[tunerId].enabled = false;
        this->frontend_tuner_status[tunerId].group_id = this->group_id;
        this->frontend_tuner_status[tunerId].rf_flow_id = "";
        this->frontend_tuner_status[tunerId].stream_id = "";
        this->frontend_tuner_status[tunerId].tuner_type = "RX_DIGITIZER";

        FileReaderContainer &container = this->fileReaderContainers[tunerId];

        this->frontend_tuner_status[tunerId].center_frequency =
                container.fileReader->getCenterFrequency();
        this->frontend_tuner_status[tunerId].sample_rate =
                container.fileReader->getSampleRate();

        // If the bandwidth isn't available, attempt to use the sample rate
        // and complexity to set the bandwidth
        if (container.fileReader->getBandwidth() == -1) {
            if (container.fileReader->getSampleRate() != -1) {
                if (container.fileReader->getComplex()) {
                    this->frontend_tuner_status[tunerId].bandwidth =
                            container.fileReader->getSampleRate();
                    container.bandwidth =
                            container.fileReader->getSampleRate();
                } else {
                    this->frontend_tuner_status[tunerId].bandwidth =
                            container.fileReader->getSampleRate() / 2;
                    container.bandwidth =
                            container.fileReader->getSampleRate() / 2;
                }
            } else {
                this->frontend_tuner_status[tunerId].bandwidth = -1;
                container.bandwidth = -1;
            }
        } else {
            this->frontend_tuner_status[tunerId].bandwidth =
                    container.fileReader->getBandwidth();
            container.bandwidth = container.fileReader->getBandwidth();
        }

        // Set the frontend_tuner_status with the available bandwidth, center
        // frequency, and sample rate using the CSV format with one entry each
        std::stringstream sstream;

        sstream << std::fixed <<
                this->frontend_tuner_status[tunerId].bandwidth;
        sstream >> this->frontend_tuner_status[tunerId].available_bandwidth;

        sstream.clear();

        sstream << std::fixed <<
                this->frontend_tuner_status[tunerId].center_frequency;
        sstream >> this->frontend_tuner_status[tunerId].available_frequency;

        sstream.clear();

        sstream << std::fixed <<
                this->frontend_tuner_status[tunerId].sample_rate;
        sstream >> this->frontend_tuner_status[tunerId].available_sample_rate;
    }
}

/*
 * If the flag changes to true, update the available files
 */
void FEI_FileReader_i::updateAvailableFilesChanged(const bool *oldValue,
                                                    const bool *newValue)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (this->updateAvailableFiles) {
        updateAvailableFilesVector();
    }

    this->updateAvailableFiles = false;
}

/*
 * Clear the current list of file readers and then initialize new ones with the
 * available files list
 */
void FEI_FileReader_i::updateFileReaders()
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    for (size_t id = 0; id < this->fileReaderContainers.size(); ++id) {
        delete this->fileReaderContainers[id].fileReader;
        delete this->fileReaderContainers[id].lock;
    }

    this->fileReaderContainers.clear();

    MetaFileReader *newFileReader = NULL;

    for (size_t id = 0; id < this->availableFiles.size(); ++id) {
        FileReaderContainer container;

        // Check if file meta information is in the file name
        if (FormattedFileReader::isValid(this->availableFiles[id].path)) {
            newFileReader = new FormattedFileReader;

            newFileReader->setFilePath(this->availableFiles[id].path);
            newFileReader->setPacketSize(this->AdvancedProperties.packetSize);

            container.fileReader = newFileReader;
            container.lock = new boost::mutex;
            container.thread = NULL;
            container.typeSize = sizeFromType(newFileReader->getType());

            this->fileReaderContainers.push_back(container);
        } else {
            LOG_WARN(FEI_FileReader_i, "Unable to extract met information " \
                    "for file: " << this->availableFiles[id].path);
        }
    }

    initializeOutputVectors();
}

/*
 * Update the RF flow id for each file reader
 */
void FEI_FileReader_i::updateRfFlowId(const std::string &rfFlowId)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    for (size_t tunerId = 0; tunerId < this->frontend_tuner_status.size();
            ++tunerId) {
        frontend_tuner_status[tunerId].rf_flow_id = rfFlowId;
        this->fileReaderContainers[tunerId].updateSRI = true;
    }
}

/*************************************************************
Functions servicing the tuner control port
*************************************************************/
std::string FEI_FileReader_i::getTunerType(const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    long idx = getTunerMapping(allocation_id);

    if (idx < 0) {
        throw FRONTEND::FrontendException("Invalid allocation id");
    }

    return frontend_tuner_status[idx].tuner_type;
}

bool FEI_FileReader_i::getTunerDeviceControl(const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    long idx = getTunerMapping(allocation_id);

    if (idx < 0) {
        throw FRONTEND::FrontendException("Invalid allocation id");
    }

    if (getControlAllocationId(idx) == allocation_id) {
        return true;
    }

    return false;
}

std::string FEI_FileReader_i::getTunerGroupId(const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    long idx = getTunerMapping(allocation_id);

    if (idx < 0) {
        throw FRONTEND::FrontendException("Invalid allocation id");
    }

    return frontend_tuner_status[idx].group_id;
}

std::string FEI_FileReader_i::getTunerRfFlowId(
        const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    long idx = getTunerMapping(allocation_id);

    if (idx < 0) {
        throw FRONTEND::FrontendException("Invalid allocation id");
    }

    return frontend_tuner_status[idx].rf_flow_id;
}

void FEI_FileReader_i::setTunerCenterFrequency(
        const std::string& allocation_id, double freq)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    throw FRONTEND::NotSupportedException(
            "setTunerCenterFrequency not supported");
}

double FEI_FileReader_i::getTunerCenterFrequency(
        const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    long idx = getTunerMapping(allocation_id);

    if (idx < 0) {
        throw FRONTEND::FrontendException("Invalid allocation id");
    }

    return frontend_tuner_status[idx].center_frequency;
}

void FEI_FileReader_i::setTunerBandwidth(const std::string& allocation_id,
                                            double bw)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    throw FRONTEND::NotSupportedException("setTunerBandwidth not supported");
}

double FEI_FileReader_i::getTunerBandwidth(const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    long idx = getTunerMapping(allocation_id);

    if (idx < 0) {
        throw FRONTEND::FrontendException("Invalid allocation id");
    }

    return frontend_tuner_status[idx].bandwidth;
}

void FEI_FileReader_i::setTunerAgcEnable(const std::string& allocation_id,
                                            bool enable)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    throw FRONTEND::NotSupportedException("setTunerAgcEnable not supported");
}

bool FEI_FileReader_i::getTunerAgcEnable(const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    throw FRONTEND::NotSupportedException("getTunerAgcEnable not supported");
}

void FEI_FileReader_i::setTunerGain(const std::string& allocation_id,
                                        float gain)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    throw FRONTEND::NotSupportedException("setTunerGain not supported");
}

float FEI_FileReader_i::getTunerGain(const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    throw FRONTEND::NotSupportedException("getTunerGain not supported");
}

void FEI_FileReader_i::setTunerReferenceSource(
        const std::string& allocation_id, long source)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    throw FRONTEND::NotSupportedException("setTunerReferenceSource " \
                                            "not supported");
}

long FEI_FileReader_i::getTunerReferenceSource(
        const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    throw FRONTEND::NotSupportedException("getTunerReferenceSource " \
                                            "not supported");
}

void FEI_FileReader_i::setTunerEnable(const std::string& allocation_id,
                                        bool enable)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    long idx = getTunerMapping(allocation_id);

    if (idx < 0) {
        throw FRONTEND::FrontendException("Invalid allocation id");
    }

    if (allocation_id != getControlAllocationId(idx)) {
        throw FRONTEND::FrontendException(("ID " + allocation_id +
                " does not have authorization to modify the tuner").c_str());
    }

    // Set hardware to new value. Raise an exception if it's not possible
    this->frontend_tuner_status[idx].enabled = enable;
}

bool FEI_FileReader_i::getTunerEnable(const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    long idx = getTunerMapping(allocation_id);

    if (idx < 0) {
        throw FRONTEND::FrontendException("Invalid allocation id");
    }

    return frontend_tuner_status[idx].enabled;
}

void FEI_FileReader_i::setTunerOutputSampleRate(
        const std::string& allocation_id, double sr)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    throw FRONTEND::NotSupportedException("setTunerOutputSampleRate " \
                                            "not supported");
}

double FEI_FileReader_i::getTunerOutputSampleRate(
        const std::string& allocation_id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    long idx = getTunerMapping(allocation_id);

    if (idx < 0) {
        throw FRONTEND::FrontendException("Invalid allocation id");
    }

    return frontend_tuner_status[idx].sample_rate;
}

/*************************************************************
Functions servicing the RFInfo port(s)
- port_name is the port over which the call was received
*************************************************************/
std::string FEI_FileReader_i::get_rf_flow_id(const std::string& port_name)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (port_name == "RFInfo_in") {
        return this->rfInfoPkt.rf_flow_id;
    } else {
        return "";
    }
}

void FEI_FileReader_i::set_rf_flow_id(const std::string& port_name,
                                        const std::string& id)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (port_name == "RFInfo_in") {
        updateRfFlowId(id);
        this->rfInfoPkt.rf_flow_id = id;
    }
}

frontend::RFInfoPkt FEI_FileReader_i::get_rfinfo_pkt(
        const std::string& port_name)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    frontend::RFInfoPkt pkt;

    if (port_name == "RFInfo_in") {
        pkt = this->rfInfoPkt;
    }

    return pkt;
}

void FEI_FileReader_i::set_rfinfo_pkt(const std::string& port_name,
                                        const frontend::RFInfoPkt &pkt)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (port_name == "RFInfo_in") {
        updateRfFlowId(pkt.rf_flow_id);
        this->rfInfoPkt = pkt;
    }
}

