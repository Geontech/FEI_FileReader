#include "FEI_FileReader_base.h"

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

    The following class functions are for the base class for the device class. To
    customize any of these functions, do not modify them here. Instead, overload them
    on the child class

******************************************************************************************/

FEI_FileReader_base::FEI_FileReader_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl) :
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl),
    ThreadedComponent()
{
    construct();
}

FEI_FileReader_base::FEI_FileReader_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev) :
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl, compDev),
    ThreadedComponent()
{
    construct();
}

FEI_FileReader_base::FEI_FileReader_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities) :
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl, capacities),
    ThreadedComponent()
{
    construct();
}

FEI_FileReader_base::FEI_FileReader_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev) :
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>(devMgr_ior, id, lbl, sftwrPrfl, capacities, compDev),
    ThreadedComponent()
{
    construct();
}

FEI_FileReader_base::~FEI_FileReader_base()
{
    delete RFInfo_in;
    RFInfo_in = 0;
    delete DigitalTuner_in;
    DigitalTuner_in = 0;
    delete dataChar_out;
    dataChar_out = 0;
    delete dataOctet_out;
    dataOctet_out = 0;
    delete dataShort_out;
    dataShort_out = 0;
    delete dataUshort_out;
    dataUshort_out = 0;
    delete dataFloat_out;
    dataFloat_out = 0;
    delete dataUlong_out;
    dataUlong_out = 0;
    delete dataLong_out;
    dataLong_out = 0;
    delete dataLongLong_out;
    dataLongLong_out = 0;
    delete dataUlongLong_out;
    dataUlongLong_out = 0;
    delete dataDouble_out;
    dataDouble_out = 0;
}

void FEI_FileReader_base::construct()
{
    loadProperties();

    RFInfo_in = new frontend::InRFInfoPort("RFInfo_in", this);
    addPort("RFInfo_in", RFInfo_in);
    DigitalTuner_in = new frontend::InDigitalTunerPort("DigitalTuner_in", this);
    addPort("DigitalTuner_in", DigitalTuner_in);
    dataChar_out = new bulkio::OutCharPort("dataChar_out");
    addPort("dataChar_out", dataChar_out);
    dataOctet_out = new bulkio::OutOctetPort("dataOctet_out");
    addPort("dataOctet_out", dataOctet_out);
    dataShort_out = new bulkio::OutShortPort("dataShort_out");
    addPort("dataShort_out", dataShort_out);
    dataUshort_out = new bulkio::OutUShortPort("dataUshort_out");
    addPort("dataUshort_out", dataUshort_out);
    dataFloat_out = new bulkio::OutFloatPort("dataFloat_out");
    addPort("dataFloat_out", dataFloat_out);
    dataUlong_out = new bulkio::OutULongPort("dataUlong_out");
    addPort("dataUlong_out", dataUlong_out);
    dataLong_out = new bulkio::OutLongPort("dataLong_out");
    addPort("dataLong_out", dataLong_out);
    dataLongLong_out = new bulkio::OutLongLongPort("dataLongLong_out");
    addPort("dataLongLong_out", dataLongLong_out);
    dataUlongLong_out = new bulkio::OutULongLongPort("dataUlongLong_out");
    addPort("dataUlongLong_out", dataUlongLong_out);
    dataDouble_out = new bulkio::OutDoublePort("dataDouble_out");
    addPort("dataDouble_out", dataDouble_out);

    this->addPropertyListener(connectionTable, this, &FEI_FileReader_base::connectionTableChanged);

}

/*******************************************************************************************
    Framework-level functions
    These functions are generally called by the framework to perform housekeeping.
*******************************************************************************************/
void FEI_FileReader_base::start() throw (CORBA::SystemException, CF::Resource::StartError)
{
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>::start();
    ThreadedComponent::startThread();
}

void FEI_FileReader_base::stop() throw (CORBA::SystemException, CF::Resource::StopError)
{
    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>::stop();
    if (!ThreadedComponent::stopThread()) {
        throw CF::Resource::StopError(CF::CF_NOTSET, "Processing thread did not die");
    }
}

void FEI_FileReader_base::releaseObject() throw (CORBA::SystemException, CF::LifeCycle::ReleaseError)
{
    // This function clears the device running condition so main shuts down everything
    try {
        stop();
    } catch (CF::Resource::StopError& ex) {
        // TODO - this should probably be logged instead of ignored
    }

    frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>::releaseObject();
}

void FEI_FileReader_base::connectionTableChanged(const std::vector<connection_descriptor_struct>* oldValue, const std::vector<connection_descriptor_struct>* newValue)
{
    dataChar_out->updateConnectionFilter(*newValue);
    dataOctet_out->updateConnectionFilter(*newValue);
    dataShort_out->updateConnectionFilter(*newValue);
    dataUshort_out->updateConnectionFilter(*newValue);
    dataFloat_out->updateConnectionFilter(*newValue);
    dataUlong_out->updateConnectionFilter(*newValue);
    dataLong_out->updateConnectionFilter(*newValue);
    dataLongLong_out->updateConnectionFilter(*newValue);
    dataUlongLong_out->updateConnectionFilter(*newValue);
    dataDouble_out->updateConnectionFilter(*newValue);
}

void FEI_FileReader_base::loadProperties()
{
    device_kind = "FRONTEND::TUNER";
    device_model = "FILE";
    addProperty(filePath,
                "",
                "filePath",
                "",
                "readwrite",
                "",
                "external",
                "property");

    addProperty(updateAvailableFiles,
                false,
                "updateAvailableFiles",
                "updateAvailableFiles",
                "readwrite",
                "",
                "external",
                "property");

    addProperty(loop,
                false,
                "loop",
                "",
                "readwrite",
                "",
                "external",
                "property");

    frontend_listener_allocation = frontend::frontend_listener_allocation_struct();
    frontend_tuner_allocation = frontend::frontend_tuner_allocation_struct();
    addProperty(AdvancedProperties,
                AdvancedProperties_struct(),
                "AdvancedProperties",
                "",
                "readwrite",
                "",
                "external",
                "property");

    addProperty(connectionTable,
                "connectionTable",
                "",
                "readonly",
                "",
                "external",
                "property");

    addProperty(availableFiles,
                "availableFiles",
                "availableFiles",
                "readonly",
                "",
                "external",
                "property");

}

/* This sets the number of entries in the frontend_tuner_status struct sequence property
 * as well as the tuner_allocation_ids vector. Call this function during initialization
 */
void FEI_FileReader_base::setNumChannels(size_t num)
{
    this->setNumChannels(num, "RX_DIGITIZER");
}
/* This sets the number of entries in the frontend_tuner_status struct sequence property
 * as well as the tuner_allocation_ids vector. Call this function during initialization
 */

void FEI_FileReader_base::setNumChannels(size_t num, std::string tuner_type)
{
    frontend_tuner_status.clear();
    frontend_tuner_status.resize(num);
    tuner_allocation_ids.clear();
    tuner_allocation_ids.resize(num);
    for (std::vector<frontend_tuner_status_struct_struct>::iterator iter=frontend_tuner_status.begin(); iter!=frontend_tuner_status.end(); iter++) {
        iter->enabled = false;
        iter->tuner_type = tuner_type;
    }
}

void FEI_FileReader_base::frontendTunerStatusChanged(const std::vector<frontend_tuner_status_struct_struct>* oldValue, const std::vector<frontend_tuner_status_struct_struct>* newValue)
{
    this->tuner_allocation_ids.resize(this->frontend_tuner_status.size());
}

CF::Properties* FEI_FileReader_base::getTunerStatus(const std::string& allocation_id)
{
    CF::Properties* tmpVal = new CF::Properties();
    long tuner_id = getTunerMapping(allocation_id);
    if (tuner_id < 0)
        throw FRONTEND::FrontendException(("ERROR: ID: " + std::string(allocation_id) + " IS NOT ASSOCIATED WITH ANY TUNER!").c_str());
    CORBA::Any prop;
    prop <<= *(static_cast<frontend_tuner_status_struct_struct*>(&this->frontend_tuner_status[tuner_id]));
    prop >>= tmpVal;

    CF::Properties_var tmp = new CF::Properties(*tmpVal);
    return tmp._retn();
}

void FEI_FileReader_base::assignListener(const std::string& listen_alloc_id, const std::string& allocation_id)
{
    // find control allocation_id
    std::string existing_alloc_id = allocation_id;
    std::map<std::string,std::string>::iterator existing_listener;
    while ((existing_listener=listeners.find(existing_alloc_id)) != listeners.end())
        existing_alloc_id = existing_listener->second;
    listeners[listen_alloc_id] = existing_alloc_id;

    std::vector<connection_descriptor_struct> old_table = connectionTable;
    std::vector<connection_descriptor_struct> new_entries;
    for (std::vector<connection_descriptor_struct>::iterator entry=connectionTable.begin();entry!=connectionTable.end();entry++) {
        if (entry->connection_id == existing_alloc_id) {
            connection_descriptor_struct tmp;
            tmp.connection_id = listen_alloc_id;
            tmp.stream_id = entry->stream_id;
            tmp.port_name = entry->port_name;
            new_entries.push_back(tmp);
        }
    }
    for (std::vector<connection_descriptor_struct>::iterator new_entry=new_entries.begin();new_entry!=new_entries.end();new_entry++) {
        bool foundEntry = false;
        for (std::vector<connection_descriptor_struct>::iterator entry=connectionTable.begin();entry!=connectionTable.end();entry++) {
            if (entry == new_entry) {
                foundEntry = true;
                break;
            }
        }
        if (!foundEntry) {
            connectionTable.push_back(*new_entry);
        }
    }
    connectionTableChanged(&old_table, &connectionTable);
}

void FEI_FileReader_base::removeListener(const std::string& listen_alloc_id)
{
    if (listeners.find(listen_alloc_id) != listeners.end()) {
        listeners.erase(listen_alloc_id);
    }
    std::vector<connection_descriptor_struct> old_table = this->connectionTable;
    std::vector<connection_descriptor_struct>::iterator entry = this->connectionTable.begin();
    while (entry != this->connectionTable.end()) {
        if (entry->connection_id == listen_alloc_id) {
            entry = this->connectionTable.erase(entry);
        } else {
            entry++;
        }
    }
    ExtendedCF::UsesConnectionSequence_var tmp;
    // Check to see if port "dataChar_out" has a connection for this listener
    tmp = this->dataChar_out->connections();
    for (unsigned int i=0; i<this->dataChar_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataChar_out->disconnectPort(connection_id.c_str());
        }
    }
    // Check to see if port "dataOctet_out" has a connection for this listener
    tmp = this->dataOctet_out->connections();
    for (unsigned int i=0; i<this->dataOctet_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataOctet_out->disconnectPort(connection_id.c_str());
        }
    }
    // Check to see if port "dataShort_out" has a connection for this listener
    tmp = this->dataShort_out->connections();
    for (unsigned int i=0; i<this->dataShort_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataShort_out->disconnectPort(connection_id.c_str());
        }
    }
    // Check to see if port "dataUshort_out" has a connection for this listener
    tmp = this->dataUshort_out->connections();
    for (unsigned int i=0; i<this->dataUshort_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataUshort_out->disconnectPort(connection_id.c_str());
        }
    }
    // Check to see if port "dataFloat_out" has a connection for this listener
    tmp = this->dataFloat_out->connections();
    for (unsigned int i=0; i<this->dataFloat_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataFloat_out->disconnectPort(connection_id.c_str());
        }
    }
    // Check to see if port "dataUlong_out" has a connection for this listener
    tmp = this->dataUlong_out->connections();
    for (unsigned int i=0; i<this->dataUlong_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataUlong_out->disconnectPort(connection_id.c_str());
        }
    }
    // Check to see if port "dataLong_out" has a connection for this listener
    tmp = this->dataLong_out->connections();
    for (unsigned int i=0; i<this->dataLong_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataLong_out->disconnectPort(connection_id.c_str());
        }
    }
    // Check to see if port "dataLongLong_out" has a connection for this listener
    tmp = this->dataLongLong_out->connections();
    for (unsigned int i=0; i<this->dataLongLong_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataLongLong_out->disconnectPort(connection_id.c_str());
        }
    }
    // Check to see if port "dataUlongLong_out" has a connection for this listener
    tmp = this->dataUlongLong_out->connections();
    for (unsigned int i=0; i<this->dataUlongLong_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataUlongLong_out->disconnectPort(connection_id.c_str());
        }
    }
    // Check to see if port "dataDouble_out" has a connection for this listener
    tmp = this->dataDouble_out->connections();
    for (unsigned int i=0; i<this->dataDouble_out->connections()->length(); i++) {
        std::string connection_id = ossie::corba::returnString(tmp[i].connectionId);
        if (connection_id == listen_alloc_id) {
            this->dataDouble_out->disconnectPort(connection_id.c_str());
        }
    }
    this->connectionTableChanged(&old_table, &this->connectionTable);
}

void FEI_FileReader_base::removeAllocationIdRouting(const size_t tuner_id) {
    std::string allocation_id = getControlAllocationId(tuner_id);
    std::vector<connection_descriptor_struct> old_table = this->connectionTable;
    std::vector<connection_descriptor_struct>::iterator itr = this->connectionTable.begin();
    while (itr != this->connectionTable.end()) {
        if (itr->connection_id == allocation_id) {
            itr = this->connectionTable.erase(itr);
            continue;
        }
        itr++;
    }
    for (std::map<std::string, std::string>::iterator listener=listeners.begin();listener!=listeners.end();listener++) {
        if (listener->second == allocation_id) {
            std::vector<connection_descriptor_struct>::iterator itr = this->connectionTable.begin();
            while (itr != this->connectionTable.end()) {
                if (itr->connection_id == listener->first) {
                    itr = this->connectionTable.erase(itr);
                    continue;
                }
                itr++;
            }
        }
    }
    this->connectionTableChanged(&old_table, &this->connectionTable);
}

void FEI_FileReader_base::removeStreamIdRouting(const std::string stream_id, const std::string allocation_id) {
    std::vector<connection_descriptor_struct> old_table = this->connectionTable;
    std::vector<connection_descriptor_struct>::iterator itr = this->connectionTable.begin();
    while (itr != this->connectionTable.end()) {
        if (allocation_id == "") {
            if (itr->stream_id == stream_id) {
                itr = this->connectionTable.erase(itr);
                continue;
            }
        } else {
            if ((itr->stream_id == stream_id) and (itr->connection_id == allocation_id)) {
                itr = this->connectionTable.erase(itr);
                continue;
            }
        }
        itr++;
    }
    for (std::map<std::string, std::string>::iterator listener=listeners.begin();listener!=listeners.end();listener++) {
        if (listener->second == allocation_id) {
            std::vector<connection_descriptor_struct>::iterator itr = this->connectionTable.begin();
            while (itr != this->connectionTable.end()) {
                if ((itr->connection_id == listener->first) and (itr->stream_id == stream_id)) {
                    itr = this->connectionTable.erase(itr);
                    continue;
                }
                itr++;
            }
        }
    }
    this->connectionTableChanged(&old_table, &this->connectionTable);
}

void FEI_FileReader_base::matchAllocationIdToStreamId(const std::string allocation_id, const std::string stream_id, const std::string port_name) {
    if (port_name != "") {
        for (std::vector<connection_descriptor_struct>::iterator prop_itr = this->connectionTable.begin(); prop_itr!=this->connectionTable.end(); prop_itr++) {
            if ((*prop_itr).port_name != port_name)
                continue;
            if ((*prop_itr).stream_id != stream_id)
                continue;
            if ((*prop_itr).connection_id != allocation_id)
                continue;
            // all three match. This is a repeat
            return;
        }
        std::vector<connection_descriptor_struct> old_table = this->connectionTable;
        connection_descriptor_struct tmp;
        tmp.connection_id = allocation_id;
        tmp.port_name = port_name;
        tmp.stream_id = stream_id;
        this->connectionTable.push_back(tmp);
        this->connectionTableChanged(&old_table, &this->connectionTable);
        return;
    }
    std::vector<connection_descriptor_struct> old_table = this->connectionTable;
    connection_descriptor_struct tmp;
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataChar_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataOctet_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataShort_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataUshort_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataFloat_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataUlong_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataLong_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataLongLong_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataUlongLong_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    tmp.connection_id = allocation_id;
    tmp.port_name = "dataDouble_out";
    tmp.stream_id = stream_id;
    this->connectionTable.push_back(tmp);
    this->connectionTableChanged(&old_table, &this->connectionTable);
}

