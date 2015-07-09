/**************************************************************************

    This is the device code. This file contains the child class where
    custom functionality can be added to the device. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

**************************************************************************/

#include "FEI_FileReader.h"

#include <boost/date_time/time_defs.hpp>
#include <boost/filesystem.hpp>

PREPARE_LOGGING(FEI_FileReader_i)

/*
 * Construct the device by calling the construct method
 */
FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl,
                                    char *sftwrPrfl) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    construct();
}

/*
 * Construct the device by calling the construct method
 */
FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl,
                                    char *sftwrPrfl, char *compDev) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl, compDev)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    construct();
}

/*
 * Construct the device by calling the construct method
 */
FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl,
                                    char *sftwrPrfl,
                                    CF::Properties capacities) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl, capacities)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    construct();
}

/*
 * Construct the device by calling the construct method
 */
FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl,
                                    char *sftwrPrfl,
                                    CF::Properties capacities, char *compDev) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl, capacities, compDev)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    construct();
}

/*
 * Destruct the device by cleaning up the file reader pointers
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

    bool rx_data = false;

    for (size_t tunerId = 0; tunerId < this->fileReaderContainers.size();
            ++tunerId) {
        // Check to see if the channel is allocated before requesting a data
        // packet
        if (getControlAllocationId(tunerId).empty()) {
            continue;
        }

        // Check to see if the output is enabled
        if (not this->frontend_tuner_status[tunerId].enabled) {
            continue;
        }

        // Get the type of data the file reader is pulling in
        const std::string type =
                this->fileReaderContainers[tunerId].
                        fileReader->getType();

        // Check if we're currently waiting for a packet to be ready for
        // throttling
        if (fileReaderContainers[tunerId].currentPacket == NULL) {
            this->fileReaderContainers[tunerId].currentPacket =
                    this->fileReaderContainers[tunerId].
                            fileReader->getNextPacket();

            if (this->fileReaderContainers[tunerId].currentPacket == NULL) {
                continue;
            }

            this->fileReaderContainers[tunerId].firstSeen =
                    boost::get_system_time();
            this->fileReaderContainers[tunerId].timestamp =
                    bulkio::time::utils::now();

            // Calculate the time duration for this packet based on the
            // number of samples, the requested sample rate, and the complexity
            size_t bytes = this->fileReaderContainers[tunerId].
                                    currentPacket->dataSize;

            size_t samples = bytes / sizeFromType(type);

            if (this->fileReaderContainers[tunerId].fileReader->getComplex()) {
                samples /= 2;
            }

            double sampleRate = this->frontend_tuner_status[tunerId].
                                        sample_rate;
            double timeDuration = samples / sampleRate;
            int seconds = timeDuration;
            int fractional = this->fractionalResolution *
                    (timeDuration - seconds);

            this->fileReaderContainers[tunerId].timeDuration =
                    boost::posix_time::time_duration(0, 0,
                            seconds, fractional);

            // Update the delay, if necessary
            if ((this->fileReaderContainers[tunerId].
                    timeDuration.total_nanoseconds() / 1.0e9) < getThreadDelay()) {
                setThreadDelay((this->fileReaderContainers[tunerId].
                        timeDuration.total_nanoseconds() / 1.0e9) / 10);
            }
        }

        // Check if the amount of time has elapsed for this packet
        if ((boost::get_system_time() -
                    this->fileReaderContainers[tunerId].firstSeen) <
                    this->fileReaderContainers[tunerId].timeDuration) {
            continue;
        }

        rx_data = true;

        std::string streamId = getStreamId(tunerId);

        // If the update SRI flag is set, push the SRI packet
        if (this->fileReaderContainers[tunerId].updateSRI) {
            BULKIO::StreamSRI sri = create(streamId, this->frontend_tuner_status[tunerId]);
            sri.mode = this->fileReaderContainers[tunerId].
                    fileReader->getComplex();

            pushSRIByType(sri, type);

            this->fileReaderContainers[tunerId].updateSRI = false;
        }

        pushPacketByType(this->fileReaderContainers[tunerId],
                this->fileReaderContainers[tunerId].timestamp, false,
                streamId);

        this->fileReaderContainers[tunerId].
                fileReader->replacePacket(this->fileReaderContainers[tunerId].
                        currentPacket);

        this->fileReaderContainers[tunerId].currentPacket = NULL;
    }

    if (rx_data) {
        return NORMAL;
    }

    return NOOP;
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
    Make sure to set the 'enabled' member of fts to indicate that tuner as enabled
    ************************************************************/
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    fileReaderEnable(tuner_id);
    return;
}

void FEI_FileReader_i::deviceDisable(frontend_tuner_status_struct_struct &fts,
                                        size_t tuner_id)
{
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    Make sure to reset the 'enabled' member of fts to indicate that tuner as disabled
    ************************************************************/
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    fileReaderDisable(tuner_id);
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

    if (fts.tuner_type == "RX_DIGITIZER") {
        try {
            if (not frontend::validateRequestVsDevice(request, this->rfInfoPkt,
                    this->fileReaderContainers[tuner_id].
                            fileReader->getComplex(),
                    this->frontend_tuner_status[tuner_id].center_frequency,
                    this->frontend_tuner_status[tuner_id].center_frequency,
                    this->frontend_tuner_status[tuner_id].bandwidth,
                    this->frontend_tuner_status[tuner_id].sample_rate)) {
                throw FRONTEND::BadParameterException("INVALID REQUEST --" \
                                "falls outside of file capabilities");
            }
        } catch (FRONTEND::BadParameterException &e) {
            LOG_DEBUG(FEI_FileReader_i,"deviceSetTuning|" \
                    "BadParameterException - " << e.msg);
            return false;
        }

        // Specify the parameters of the request for the purposes
        // of throttling

        // Create a stream id if not already created for this file
        std::string streamId = getStreamId(tuner_id);

        // Enable multi-out capability
        matchAllocationIdToStreamId(request.allocation_id, streamId, "");

        // Enable the file reader
        this->fileReaderContainers[tuner_id].fileReader->start();

        this->fileReaderContainers[tuner_id].updateSRI = true;
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

    this->fileReaderContainers[tuner_id].fileReader->stop();

    std::string streamId = getStreamId(tuner_id);
    BULKIO::StreamSRI sri = create(streamId,
                                    this->frontend_tuner_status[tuner_id]);

    sri.mode = this->fileReaderContainers[tuner_id].fileReader->getComplex();

    pushSRIByType(sri, this->fileReaderContainers[tuner_id].
            fileReader->getType());

    this->fileReaderContainers[tuner_id].updateSRI = false;

    if (this->fileReaderContainers[tuner_id].currentPacket != NULL) {
        this->fileReaderContainers[tuner_id].fileReader->replacePacket(
                this->fileReaderContainers[tuner_id].currentPacket);
    }

    FilePacket tempPacket;
    tempPacket.dataSize = 0;
    this->fileReaderContainers[tuner_id].currentPacket = &tempPacket;

    BULKIO::PrecisionUTCTime T = bulkio::time::utils::now();

    pushPacketByType(this->fileReaderContainers[tuner_id], T, true, streamId);

    this->fileReaderContainers[tuner_id].currentPacket = NULL;

    fts.stream_id = "";

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

    if (oldValue->PacketSize != newValue->PacketSize) {
        setPacketSizes(newValue->PacketSize);
    }

    if (oldValue->QueueSize != newValue->QueueSize) {
        setQueueSizes(newValue->QueueSize);
    }
}

/*
 * Initialize property change listeners, set the fractional resolution,
 * and initialize the rfinfo packet
 */
void FEI_FileReader_i::construct()
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    // Initialize property change listeners
    addPropertyChangeListener("AdvancedProperties", this,
            &FEI_FileReader_i::AdvancedPropertiesChanged);
    addPropertyChangeListener("filePath", this,
            &FEI_FileReader_i::filePathChanged);
    addPropertyChangeListener("loop", this,
            &FEI_FileReader_i::loopChanged);
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
 * If the new file path exists, set the new file path appropriately
 */
void FEI_FileReader_i::filePathChanged(const std::string *oldValue,
        const std::string *newValue)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (not boost::filesystem::exists(*newValue)) {
        LOG_WARN(FEI_FileReader_i, "Invalid file path");
        this->filePath = *oldValue;
        return;
    }

    updateAvailableFilesVector();

    LOG_DEBUG(FEI_FileReader_i, "Found " <<
            this->fileReaderContainers.size() << " files to read");
}

/*
 * Disable the file reader with the given tuner ID and clean up
 */
void FEI_FileReader_i::fileReaderDisable(size_t tunerId)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    bool previouslyEnabled = this->frontend_tuner_status[tunerId].enabled;
    this->frontend_tuner_status[tunerId].enabled = false;

    if (previouslyEnabled) {
        //TODO: LOCK THIS
        this->fileReaderContainers[tunerId].fileReader->stop();

        if (this->fileReaderContainers[tunerId].currentPacket != NULL) {
            this->fileReaderContainers[tunerId].
                fileReader->replacePacket(this->fileReaderContainers[tunerId].
                        currentPacket);

            this->fileReaderContainers[tunerId].currentPacket = NULL;
        }
    }
}

/*
 * Enable the file reader with the given tuner ID and push SRI
 */
void FEI_FileReader_i::fileReaderEnable(size_t tunerId)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    bool previouslyEnabled = this->frontend_tuner_status[tunerId].enabled;
    this->frontend_tuner_status[tunerId].enabled = true;

    std::string streamId = getStreamId(tunerId);

    if (not previouslyEnabled) {
        BULKIO::StreamSRI sri = create(streamId,
                this->frontend_tuner_status[tunerId]);

        pushSRIByType(sri, this->fileReaderContainers[tunerId].
                fileReader->getType());

        this->fileReaderContainers[tunerId].fileReader->start();
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

    size_t bytes = this->AdvancedProperties.PacketSize;
    const std::string type = container.fileReader->getType();

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
    if (type == "B") {
        container.charOutput.resize(bytes);
    } else if (type == "UB") {
        container.uCharOutput.resize(bytes);
    } else if (type == "I") {
        container.shortOutput.resize(bytes / sizeFromType(type));
    } else if (type == "UI") {
        container.uShortOutput.resize(bytes / sizeFromType(type));
    } else if (type == "L") {
        container.longOutput.resize(bytes / sizeFromType(type));
    } else if (type == "UL") {
        container.uLongOutput.resize(bytes / sizeFromType(type));
    } else if (type == "F") {
        container.floatOutput.resize(bytes / sizeFromType(type));
    } else if (type == "X") {
        container.longLongOutput.resize(bytes / sizeFromType(type));
    } else if (type == "UX") {
        container.uLongLongOutput.resize(bytes / sizeFromType(type));
    } else if (type == "D") {
        container.doubleOutput.resize(bytes / sizeFromType(type));
    } else {
        LOG_WARN(FEI_FileReader_i, "Unrecognized file type: " << type);
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
 * Set the looping value on all of the file readers
 */
void FEI_FileReader_i::loopChanged(const bool *oldValue, const bool *newValue)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    for (FileReaderIterator i = this->fileReaderContainers.begin();
            i != this->fileReaderContainers.end(); ++i) {
        i->fileReader->setLoopingEnabled(*newValue);
    }
}

/*
 * Given a file reader container, push its packet to the port that matches
 * the file's data type
 * TODO: May optimize this by having the appropriate vector pre-initialized
 * with the packet size so all that is necessary is a copy
 */
void FEI_FileReader_i::pushPacketByType(FileReaderContainer &container,
        BULKIO::PrecisionUTCTime &T,
        bool EOS,
        const std::string &streamId)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    const std::string type = container.fileReader->getType();

    if (type == "B") {
        std::copy(container.currentPacket->data,
                container.currentPacket->data +
                container.currentPacket->dataSize,
                container.charOutput.data());


        this->dataChar_out->pushPacket(container.charOutput, T, EOS, streamId);
    } else if (type == "UB") {
        std::copy(container.currentPacket->data,
                container.currentPacket->data +
                container.currentPacket->dataSize,
                container.uCharOutput.data());

        this->dataOctet_out->pushPacket(container.uCharOutput, T, EOS,
                streamId);
    } else if (type == "I") {
        std::copy(container.currentPacket->data,
                container.currentPacket->data +
                container.currentPacket->dataSize,
                (int16_t *) container.shortOutput.data());

        this->dataShort_out->pushPacket(container.shortOutput, T, EOS,
                streamId);
    } else if (type == "UI") {
        std::copy(container.currentPacket->data,
                container.currentPacket->data +
                container.currentPacket->dataSize,
                (uint16_t *) container.shortOutput.data());

        this->dataUshort_out->pushPacket(container.uShortOutput, T, EOS,
                streamId);
    } else if (type == "L") {
        std::copy(container.currentPacket->data,
                container.currentPacket->data +
                container.currentPacket->dataSize,
                (int32_t *) container.shortOutput.data());

        this->dataLong_out->pushPacket(container.longOutput, T, EOS, streamId);
    } else if (type == "UL") {
        std::copy(container.currentPacket->data,
                container.currentPacket->data +
                container.currentPacket->dataSize,
                (uint32_t *) container.shortOutput.data());

        this->dataUlong_out->pushPacket(container.uLongOutput, T, EOS,
                streamId);
    } else if (type == "F") {
        std::copy(container.currentPacket->data,
                container.currentPacket->data +
                container.currentPacket->dataSize,
                (float *) container.shortOutput.data());

        this->dataFloat_out->pushPacket(container.floatOutput, T, EOS,
                streamId);
    } else if (type == "X") {
        std::copy(container.currentPacket->data,
                container.currentPacket->data +
                container.currentPacket->dataSize,
                (int64_t *) container.shortOutput.data());

        this->dataLongLong_out->pushPacket(container.longLongOutput, T, EOS,
                streamId);
    } else if (type == "UX") {
        std::copy(container.currentPacket->data,
                container.currentPacket->data +
                container.currentPacket->dataSize,
                (uint64_t *) container.shortOutput.data());

        this->dataUlongLong_out->pushPacket(container.uLongLongOutput, T, EOS,
                streamId);
    } else if (type == "D") {
        std::copy(container.currentPacket->data,
                container.currentPacket->data +
                container.currentPacket->dataSize,
                (double *) container.shortOutput.data());

        this->dataDouble_out->pushPacket(container.doubleOutput, T, EOS,
                streamId);
    } else {
        LOG_WARN(FEI_FileReader_i, "Unrecognized file type: " << type);
    }
}

/*
 * Given an SRI object, push it to the port that matches the file's data type
 */
void FEI_FileReader_i::pushSRIByType(BULKIO::StreamSRI &sri,
        const std::string &type)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (type == "B") {
        this->dataChar_out->pushSRI(sri);
    } else if (type == "UB") {
        this->dataOctet_out->pushSRI(sri);
    } else if (type == "I") {
        this->dataShort_out->pushSRI(sri);
    } else if (type == "UI") {
        this->dataUshort_out->pushSRI(sri);
    } else if (type == "L") {
        this->dataLong_out->pushSRI(sri);
    } else if (type == "UL") {
        this->dataUlong_out->pushSRI(sri);
    } else if (type == "F") {
        this->dataFloat_out->pushSRI(sri);
    } else if (type == "X") {
        this->dataLongLong_out->pushSRI(sri);
    } else if (type == "UX") {
        this->dataUlongLong_out->pushSRI(sri);
    } else if (type == "D") {
        this->dataDouble_out->pushSRI(sri);
    } else {
        LOG_WARN(FEI_FileReader_i, "Unrecognized file type: " << type);
    }
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
 * Set the queue size for each file reader
 */
void FEI_FileReader_i::setQueueSizes(size_t queueSize)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    for (FileReaderIterator i = this->fileReaderContainers.begin();
            i != this->fileReaderContainers.end(); ++i) {
        i->fileReader->setQueueSize(queueSize);
    }
}

/*
 * Return the size in bytes of the specified type
 */
size_t FEI_FileReader_i::sizeFromType(const std::string &type)
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

    if (type == "B") {
        return sizeof(int8_t);
    } else if (type == "UB") {
        return sizeof(uint8_t);
    } else if (type == "I") {
        return sizeof(int16_t);
    } else if (type == "UI") {
        return sizeof(uint16_t);
    } else if (type == "L") {
        return sizeof(int32_t);
    } else if (type == "UL") {
        return sizeof(uint32_t);
    } else if (type == "F") {
        return sizeof(float);
    } else if (type == "X") {
        return sizeof(int64_t);
    } else if (type == "UX") {
        return sizeof(uint64_t);
    } else if (type == "D") {
        return sizeof(double);
    } else {
        LOG_WARN(FEI_FileReader_i, "Unrecognized file type: " << type);
    }

    return 0;
}

/*
 * Clear the current available files vector and use the current file path to
 * read the path as either a single file or a directory of files
 */
void FEI_FileReader_i::updateAvailableFilesVector()
{
    LOG_TRACE(FEI_FileReader_i, __PRETTY_FUNCTION__);

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
        this->frontend_tuner_status[tunerId].group_id = "";
        this->frontend_tuner_status[tunerId].rf_flow_id = "";
        this->frontend_tuner_status[tunerId].stream_id = "";
        this->frontend_tuner_status[tunerId].tuner_type = "RX_DIGITIZER";

        this->frontend_tuner_status[tunerId].center_frequency =
                this->fileReaderContainers[tunerId].
                        fileReader->getCenterFrequency();
        this->frontend_tuner_status[tunerId].sample_rate =
                this->fileReaderContainers[tunerId].
                        fileReader->getSampleRate();

        // If the bandwidth isn't available, attempt to use the sample rate
        // and complexity to set the bandwidth
        if (this->fileReaderContainers[tunerId].
                    fileReader->getBandwidth() == -1) {
            if (this->fileReaderContainers[tunerId].
                        fileReader->getSampleRate() != -1) {
                if (this->fileReaderContainers[tunerId].
                            fileReader->getComplex()) {
                    this->frontend_tuner_status[tunerId].bandwidth =
                            this->fileReaderContainers[tunerId].
                                    fileReader->getSampleRate();
                } else {
                    this->frontend_tuner_status[tunerId].bandwidth =
                            this->fileReaderContainers[tunerId].
                                    fileReader->getSampleRate() / 2;
                }
            } else {
                this->frontend_tuner_status[tunerId].bandwidth = -1;
            }
        } else {
            this->frontend_tuner_status[tunerId].bandwidth =
                    this->fileReaderContainers[tunerId].
                            fileReader->getBandwidth();
        }
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
    }

    this->fileReaderContainers.clear();

    FormattedFileReader *newFileReader = NULL;

    for (size_t id = 0; id < this->availableFiles.size(); ++id) {
        FileReaderContainer container;

        newFileReader = new FormattedFileReader;

        if (newFileReader->setFilePath(this->availableFiles[id].path)) {
            newFileReader->setLoopingEnabled(this->loop);
            newFileReader->setPacketSize(this->AdvancedProperties.PacketSize);
            newFileReader->setQueueSize(this->AdvancedProperties.QueueSize);

            container.currentPacket = NULL;
            container.fileReader = newFileReader;

            this->fileReaderContainers.push_back(container);
        } else {
            delete newFileReader;
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

    throw FRONTEND::NotSupportedException("setTunerCenterFrequency not supported");
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

