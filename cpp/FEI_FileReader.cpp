/**************************************************************************

    This is the device code. This file contains the child class where
    custom functionality can be added to the device. Custom
    functionality to the base class can be extended here. Access to
    the ports can also be done from this class

**************************************************************************/

#include "FEI_FileReader.h"

#include <boost/filesystem.hpp>

PREPARE_LOGGING(FEI_FileReader_i)

FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl)
{
    construct();
}

FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl, compDev)
{
    construct();
}

FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl, capacities)
{
    construct();
}

FEI_FileReader_i::FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev) :
    FEI_FileReader_base(devMgr_ior, id, lbl, sftwrPrfl, capacities, compDev)
{
    construct();
}

FEI_FileReader_i::~FEI_FileReader_i()
{
    for (size_t id = 0; id < this->fileReaders.size(); ++id) {
        delete this->fileReaders[id];
    }
}

void FEI_FileReader_i::initialize() throw (CF::LifeCycle::InitializeError, CORBA::SystemException)
{
    FEI_FileReader_base::initialize();

    // Start the device after initialization
    start();
}

void FEI_FileReader_i::start() throw (CF::Resource::StartError, CORBA::SystemException)
{
    if (not Resource_impl::started()) {
        Resource_impl::start();
    }
}

void FEI_FileReader_i::stop() throw (CF::Resource::StopError, CORBA::SystemException)
{
    // Iterate through file readers to disable any enabled ones
    for (size_t tunerId = 0; tunerId < this->fileReaders.size(); ++tunerId) {
        deviceDisable(this->frontend_tuner_status[tunerId], tunerId);
    }

    if (Resource_impl::started()) {
        Resource_impl::stop();
    }
}

/***********************************************************************************************

    Basic functionality:

        The service function is called by the serviceThread object (of type ProcessThread).
        This call happens immediately after the previous call if the return value for
        the previous call was NORMAL.
        If the return value for the previous call was NOOP, then the serviceThread waits
        an amount of time defined in the serviceThread's constructor.
        
    SRI:
        To create a StreamSRI object, use the following code:
                std::string stream_id = "testStream";
                BULKIO::StreamSRI sri = bulkio::sri::create(stream_id);

    Time:
        To create a PrecisionUTCTime object, use the following code:
                BULKIO::PrecisionUTCTime tstamp = bulkio::time::utils::now();

        
    Ports:

        Data is passed to the serviceFunction through the getPacket call (BULKIO only).
        The dataTransfer class is a port-specific class, so each port implementing the
        BULKIO interface will have its own type-specific dataTransfer.

        The argument to the getPacket function is a floating point number that specifies
        the time to wait in seconds. A zero value is non-blocking. A negative value
        is blocking.  Constants have been defined for these values, bulkio::Const::BLOCKING and
        bulkio::Const::NON_BLOCKING.

        Each received dataTransfer is owned by serviceFunction and *MUST* be
        explicitly deallocated.

        To send data using a BULKIO interface, a convenience interface has been added 
        that takes a std::vector as the data input

        NOTE: If you have a BULKIO dataSDDS or dataVITA49  port, you must manually call 
              "port->updateStats()" to update the port statistics when appropriate.

        Example:
            // this example assumes that the device has two ports:
             *
            //  A provides (input) port of type bulkio::InShortPort called short_in
            //  A uses (output) port of type bulkio::OutFloatPort called float_out
            // The mapping between the port and the class is found
            // in the device base class header file

            bulkio::InShortPort::dataTransfer *tmp = short_in->getPacket(bulkio::Const::BLOCKING);
            if (not tmp) { // No data is available
                return NOOP;
            }

            std::vector<float> outputData;
            outputData.resize(tmp->dataBuffer.size());
            for (unsigned int i=0; i<tmp->dataBuffer.size(); i++) {
                outputData[i] = (float)tmp->dataBuffer[i];
            }

            // NOTE: You must make at least one valid pushSRI call
            if (tmp->sriChanged) {
                float_out->pushSRI(tmp->SRI);
            }
            float_out->pushPacket(outputData, tmp->T, tmp->EOS, tmp->streamID);
td::cout << *i << std::endl;
            delete tmp; // IMPORTANT: MUST RELEASE THE RECEIVED DATA BLOCK
            return NORMAL;

        If working with complex data (i.e., the "mode" on the SRI is set to
        true), the std::vector passed from/to BulkIO can be typecast to/from
        std::vector< std::complex<dataType> >.  For example, for short data:

            bulkio::InShortPort::dataTransfer *tmp = myInput->getPacket(bulkio::Const::BLOCKING);
            std::vector<std::complex<short> >* intermediate = (std::vector<std::complex<short> >*) &(tmp->dataBuffer);
            // do work here
            std::vector<short>* output = (std::vector<short>*) intermediate;
            myOutput->pushPacket(*output, tmp->T, tmp->EOS, tmp->streamID);

        Interactions with non-BULKIO ports are left up to the device developer's discretion

    Properties:
        
        Properties are accessed directly as member variables. For example, if the
        property name is "baudRate", it may be accessed within member functions as
        "baudRate". Unnamed properties are given the property id as its name.
        Property types are mapped to the nearest C++ type, (e.g. "string" becomes
        "std::string"). All generated properties are declared in the base class
        (FEI_FileReader_base).
    
        Simple sequence properties are mapped to "std::vector" of the simple type.
        Struct properties, if used, are mapped to C++ structs defined in the
        generated file "struct_props.h". Field names are taken from the name in
        the properties file; if no name is given, a generated name of the form
        "field_n" is used, where "n" is the ordinal number of the field.
        
        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            //  - A boolean called scaleInput
              
            if (scaleInput) {
                dataOut[i] = dataIn[i] * scaleValue;
            } else {
                dataOut[i] = dataIn[i];
            }
            
        Callback methods can be associated with a property so that the methods are
        called each time the property value changes.  This is done by calling 
        addPropertyChangeListener(<property name>, this, &FEI_FileReader_i::<callback method>)
        in the constructor.

        Callback methods should take two arguments, both const pointers to the value
        type (e.g., "const float *"), and return void.

        Example:
            // This example makes use of the following Properties:
            //  - A float value called scaleValue
            
        //Add to FEI_FileReader.cpp
        FEI_FileReader_i::FEI_FileReader_i(const char *uuid, const char *label) :
            FEI_FileReader_base(uuid, label)
        {
            addPropertyChangeListener("scaleValue", this, &FEI_FileReader_i::scaleChanged);
        }

        void FEI_FileReader_i::scaleChanged(const float *oldValue, const float *newValue)
        {
            std::cout << "scaleValue changed from" << *oldValue << " to " << *newValue
                      << std::endl;
        }
            
        //Add to FEI_FileReader.h
        void scaleChanged(const float* oldValue, const float* newValue);
        
    Allocation:
    
        Allocation callbacks are available to customize the Device's response to 
        allocation requests. For example, if the Device contains the allocation 
        property "my_alloc" of type string, the allocation and deallocation
        callbacks follow the pattern (with arbitrileReader_i, "Reading file at: " << this->filepath);ary function names
        my_alloc_fn and my_dealloc_fn):
        
        bool FEI_FileReader_i::my_alloc_fn(const std::string &value)
        {
            // perform logic
            return true; // successful allocation
        }
        void FEI_FileReader_i::my_dealloc_fn(const std::string &value)
        {
            // perform logic
        }
        
        The allocation and deallocation functions are then registered with the Device
        base class with the setAllocationImpl call:
        
        this->setAllocationImpl("my_alloc", this, &FEI_FileReader_i::my_alloc_fn, &FEI_FileReader_i::my_dealloc_fn);
        
        

************************************************************************************************/
int FEI_FileReader_i::serviceFunction()
{
    /*if (not this->isPlaying) {
        return NOOP;
    }

    if (not fileReader.isReady()) {
        setPlaybackState("STOP");
        return NOOP;
    }

    FilePacket * const packet = this->fileReader.getNextPacket();
    
    if (packet == NULL) {
        return NOOP;
    }

    LOG_INFO(FEI_FileReader_i, "Got packet of size " << packet->dataSize << " bytes");

    this->fileReader.replacePacket(packet);*/

    return NORMAL;
}

void FEI_FileReader_i::AdvancedPropertiesChanged(const AdvancedProperties_struct *oldValue, const AdvancedProperties_struct *newValue)
{
    /*if (oldValue->PacketSize != newValue->PacketSize) {
        this->fileReader.setPacketSize(newValue->PacketSize);
    }

    if (oldValue->QueueSize != newValue->QueueSize) {
        this->fileReader.setQueueSize(newValue->QueueSize);
    }*/
}

void FEI_FileReader_i::construct()
{
    addPropertyChangeListener("AdvancedProperties", this, &FEI_FileReader_i::AdvancedPropertiesChanged);
    addPropertyChangeListener("filePath", this, &FEI_FileReader_i::filePathChanged);
    addPropertyChangeListener("playbackState", this, &FEI_FileReader_i::playbackStateChanged);
    addPropertyChangeListener("updateAvailableFiles", this, &FEI_FileReader_i::updateAvailableFilesChanged);

    // Initialize the RF Info Packet with very large ranges
    this->rfInfoPkt.rf_flow_id = "FEI_FILEREADER_FLOW_ID_NOT_SET";
    this->rfInfoPkt.rf_center_freq = 50e9;
    this->rfInfoPkt.rf_bandwidth = 100e9;
    this->rfInfoPkt.if_center_freq = 0;
}

void FEI_FileReader_i::filePathChanged(const std::string *oldValue, const std::string *newValue)
{
    if (not boost::filesystem::exists(*newValue)) {
        LOG_WARN(FEI_FileReader_i, "Invalid file path");
        this->filePath = *oldValue;
        return;
    }

    updateAvailableFilesVector();

    LOG_INFO(FEI_FileReader_i, "Found " << this->fileReaders.size() << " files to read");
}

void FEI_FileReader_i::fileReaderDisable(size_t tunerId)
{
    bool previouslyEnabled = this->frontend_tuner_status[tunerId].enabled;
    this->frontend_tuner_status[tunerId].enabled = false;

    this->fileReaders[tunerId]->stop();
}

void FEI_FileReader_i::fileReaderEnable(size_t tunerId)
{
    bool previouslyEnabled = this->frontend_tuner_status[tunerId].enabled;
    this->frontend_tuner_status[tunerId].enabled = true;

    // get stream id (creates one if not already created for this tuner)
    std::string streamId = getStreamId(tunerId);

    if (not previouslyEnabled) {
        BULKIO::StreamSRI sri = create(streamId, this->frontend_tuner_status[tunerId]);

        // TODO: Push SRI on all ports or be smart and figure out data type?
    }

    this->fileReaders[tunerId]->start();
}

std::string FEI_FileReader_i::getStreamId(size_t tunerId)
{
    if (tunerId >= this->fileReaders.size())
        return "ERR: INVALID TUNER ID";

    if (this->frontend_tuner_status[tunerId].stream_id.empty()){
        std::ostringstream id;

        id << "tuner_freq_" <<
                long(this->frontend_tuner_status[tunerId].center_frequency) <<
                "_Hz_" << frontend::uuidGenerator();

        frontend_tuner_status[tunerId].stream_id = id.str();
        //usrp_tuners[tunerId].update_sri = true;
    }

    return this->frontend_tuner_status[tunerId].stream_id;
}

void FEI_FileReader_i::playbackStateChanged(const std::string *oldValue, const std::string *newValue)
{
    setPlaybackState(*newValue, *oldValue);
}

void FEI_FileReader_i::setPlaybackState(const std::string &value)
{
    setPlaybackState(value, this->playbackState);
}

void FEI_FileReader_i::setPlaybackState(const std::string &value, const std::string &oldValue)
{
    std::string finalValue = value;

    if (oldValue == "PLAY") {
        if (value == "STOP") {
            //this->fileReader.stop();
            this->isPlaying = false;
        } else if (value == "PAUSE") {
            this->isPlaying = false;
        }
    } else if (oldValue == "STOP") {
        if (value == "PLAY") {
            //this->fileReader.start();
            this->isPlaying = true;
        } else if (value == "PAUSE") {
            this->isPlaying = false;
            finalValue = "STOP";
        }
    } else if (oldValue == "PAUSE") {
        if (value == "PLAY") {
            //this->fileReader.start();
            this->isPlaying = true;
        } else if (value == "STOP") {
            //this->fileReader.stop();
            this->isPlaying = false;
        }
    }

    LOG_INFO(FEI_FileReader_i, "Setting playback state to " << finalValue);
    this->playbackState = finalValue;
}

void FEI_FileReader_i::setNumChannels(size_t numChannels)
{
    FEI_FileReader_base::setNumChannels(numChannels);

    for (size_t id = 0; id < this->fileReaders.size(); ++id) {
        delete this->fileReaders[id];
    }

    this->fileReaders.clear();
    this->fileReaders.resize(numChannels);

    for (size_t id = 0; id < this->fileReaders.size(); ++id) {
        this->fileReaders[id] = new RedHawkFileReader();
        this->fileReaders[id]->setFilePath(this->availableFiles[id].path);
    }
}

void FEI_FileReader_i::updateAvailableFilesVector()
{
    this->availableFiles.clear();

    if (boost::filesystem::is_regular_file(this->filePath)) {
        File_struct file;
        file.path = this->filePath;
        file.size = boost::filesystem::file_size(this->filePath);

        this->availableFiles.push_back(file);
    } else if (boost::filesystem::is_directory(this->filePath)) {
        for (boost::filesystem::directory_iterator i(this->filePath); i != boost::filesystem::directory_iterator(); i++) {
            boost::filesystem::directory_entry e(*i);

            if (boost::filesystem::is_regular_file(e)) {
                File_struct file;
                file.path = e.path().string();
                file.size = boost::filesystem::file_size(file.path);

                this->availableFiles.push_back(file);
            }
        }
    } else {
        LOG_WARN(FEI_FileReader_i, "Unsupported file type (symbolic link, empty file/directory, etc.");
    }

    setNumChannels(this->availableFiles.size());

    for (size_t tunerId = 0; tunerId < this->fileReaders.size(); ++tunerId) {
        this->frontend_tuner_status[tunerId].allocation_id_csv = "";
        this->frontend_tuner_status[tunerId].bandwidth = 256000;
        this->frontend_tuner_status[tunerId].center_frequency = 99100000;
        this->frontend_tuner_status[tunerId].enabled = false;
        this->frontend_tuner_status[tunerId].group_id = "";
        this->frontend_tuner_status[tunerId].rf_flow_id = "";
        this->frontend_tuner_status[tunerId].sample_rate = 256000;
        this->frontend_tuner_status[tunerId].stream_id = "";
        this->frontend_tuner_status[tunerId].tuner_type = "RX_DIGITIZER";
    }
}

void FEI_FileReader_i::updateAvailableFilesChanged(const bool *oldValue, const bool *newValue)
{
    if (this->updateAvailableFiles) {
        updateAvailableFilesVector();
    }

    this->updateAvailableFiles = false;
}

/*************************************************************
Functions supporting tuning allocation
*************************************************************/
void FEI_FileReader_i::deviceEnable(frontend_tuner_status_struct_struct &fts, size_t tuner_id)
{
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    Make sure to set the 'enabled' member of fts to indicate that tuner as enabled
    ************************************************************/
    fileReaderEnable(tuner_id);
    return;
}

void FEI_FileReader_i::deviceDisable(frontend_tuner_status_struct_struct &fts, size_t tuner_id)
{
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    Make sure to reset the 'enabled' member of fts to indicate that tuner as disabled
    ************************************************************/
    fileReaderDisable(tuner_id);
    return;
}

bool FEI_FileReader_i::deviceSetTuning(const frontend::frontend_tuner_allocation_struct &request, frontend_tuner_status_struct_struct &fts, size_t tuner_id){
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    return true if the tuning succeeded, and false if it failed
    ************************************************************/
    if (fts.tuner_type == "RX_DIGITIZER") {
        try {
            if (not frontend::validateRequestVsDevice(request, this->rfInfoPkt, true, 0, 10e9, 10e9, 10e9)) {
                throw FRONTEND::BadParameterException("INVALID REQUEST -- falls outside of file capabilities");
            }
        } catch (FRONTEND::BadParameterException &e) {
            LOG_INFO(FEI_FileReader_i,"deviceSetTuning|BadParameterException - " << e.msg);
            throw;
        }

        // Specify the parameters of the request for the purposes
        // of throttling
    }

    return true;
}
bool FEI_FileReader_i::deviceDeleteTuning(frontend_tuner_status_struct_struct &fts, size_t tuner_id) {
    /************************************************************
    modify fts, which corresponds to this->frontend_tuner_status[tuner_id]
    return true if the tune deletion succeeded, and false if it failed
    ************************************************************/
    #warning deviceDeleteTuning(): Deallocate an allocated tuner  *********
    return BOOL_VALUE_HERE;
}

/*************************************************************
Functions servicing the tuner control port
*************************************************************/
std::string FEI_FileReader_i::getTunerType(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].tuner_type;
}

bool FEI_FileReader_i::getTunerDeviceControl(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if (getControlAllocationId(idx) == allocation_id)
        return true;
    return false;
}

std::string FEI_FileReader_i::getTunerGroupId(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].group_id;
}

std::string FEI_FileReader_i::getTunerRfFlowId(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].rf_flow_id;
}

void FEI_FileReader_i::setTunerCenterFrequency(const std::string& allocation_id, double freq) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if(allocation_id != getControlAllocationId(idx))
        throw FRONTEND::FrontendException(("ID "+allocation_id+" does not have authorization to modify the tuner").c_str());
    if (freq<0) throw FRONTEND::BadParameterException();
    // set hardware to new value. Raise an exception if it's not possible
    this->frontend_tuner_status[idx].center_frequency = freq;
}

double FEI_FileReader_i::getTunerCenterFrequency(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].center_frequency;
}

void FEI_FileReader_i::setTunerBandwidth(const std::string& allocation_id, double bw) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if(allocation_id != getControlAllocationId(idx))
        throw FRONTEND::FrontendException(("ID "+allocation_id+" does not have authorization to modify the tuner").c_str());
    if (bw<0) throw FRONTEND::BadParameterException();
    // set hardware to new value. Raise an exception if it's not possible
    this->frontend_tuner_status[idx].bandwidth = bw;
}

double FEI_FileReader_i::getTunerBandwidth(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].bandwidth;
}

void FEI_FileReader_i::setTunerAgcEnable(const std::string& allocation_id, bool enable)
{
    throw FRONTEND::NotSupportedException("setTunerAgcEnable not supported");
}

bool FEI_FileReader_i::getTunerAgcEnable(const std::string& allocation_id)
{
    throw FRONTEND::NotSupportedException("getTunerAgcEnable not supported");
}

void FEI_FileReader_i::setTunerGain(const std::string& allocation_id, float gain)
{
    throw FRONTEND::NotSupportedException("setTunerGain not supported");
}

float FEI_FileReader_i::getTunerGain(const std::string& allocation_id)
{
    throw FRONTEND::NotSupportedException("getTunerGain not supported");
}

void FEI_FileReader_i::setTunerReferenceSource(const std::string& allocation_id, long source)
{
    throw FRONTEND::NotSupportedException("setTunerReferenceSource not supported");
}

long FEI_FileReader_i::getTunerReferenceSource(const std::string& allocation_id)
{
    throw FRONTEND::NotSupportedException("getTunerReferenceSource not supported");
}

void FEI_FileReader_i::setTunerEnable(const std::string& allocation_id, bool enable) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if(allocation_id != getControlAllocationId(idx))
        throw FRONTEND::FrontendException(("ID "+allocation_id+" does not have authorization to modify the tuner").c_str());
    // set hardware to new value. Raise an exception if it's not possible
    this->frontend_tuner_status[idx].enabled = enable;
}

bool FEI_FileReader_i::getTunerEnable(const std::string& allocation_id) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].enabled;
}

void FEI_FileReader_i::setTunerOutputSampleRate(const std::string& allocation_id, double sr) {
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    if(allocation_id != getControlAllocationId(idx))
        throw FRONTEND::FrontendException(("ID "+allocation_id+" does not have authorization to modify the tuner").c_str());
    if (sr<0) throw FRONTEND::BadParameterException();
    // set hardware to new value. Raise an exception if it's not possible
    this->frontend_tuner_status[idx].sample_rate = sr;
}

double FEI_FileReader_i::getTunerOutputSampleRate(const std::string& allocation_id){
    long idx = getTunerMapping(allocation_id);
    if (idx < 0) throw FRONTEND::FrontendException("Invalid allocation id");
    return frontend_tuner_status[idx].sample_rate;
}

/*************************************************************
Functions servicing the RFInfo port(s)
- port_name is the port over which the call was received
*************************************************************/
std::string FEI_FileReader_i::get_rf_flow_id(const std::string& port_name)
{
    return std::string("none");
}

void FEI_FileReader_i::set_rf_flow_id(const std::string& port_name, const std::string& id)
{
}

frontend::RFInfoPkt FEI_FileReader_i::get_rfinfo_pkt(const std::string& port_name)
{
    frontend::RFInfoPkt pkt;
    return pkt;
}

void FEI_FileReader_i::set_rfinfo_pkt(const std::string& port_name, const frontend::RFInfoPkt &pkt)
{
}

