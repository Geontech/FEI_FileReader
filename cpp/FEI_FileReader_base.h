#ifndef FEI_FILEREADER_IMPL_BASE_H
#define FEI_FILEREADER_IMPL_BASE_H

#include <boost/thread.hpp>
#include <frontend/frontend.h>
#include <ossie/ThreadedComponent.h>

#include <frontend/frontend.h>
#include <bulkio/bulkio.h>
#include "struct_props.h"

#define BOOL_VALUE_HERE 0
#define DOUBLE_VALUE_HERE 0

class FEI_FileReader_base : public frontend::FrontendTunerDevice<frontend_tuner_status_struct_struct>, public virtual frontend::digital_tuner_delegation, public virtual frontend::rfinfo_delegation, protected ThreadedComponent
{
    public:
        FEI_FileReader_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl);
        FEI_FileReader_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev);
        FEI_FileReader_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities);
        FEI_FileReader_base(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev);
        ~FEI_FileReader_base();

        void start() throw (CF::Resource::StartError, CORBA::SystemException);

        void stop() throw (CF::Resource::StopError, CORBA::SystemException);

        void releaseObject() throw (CF::LifeCycle::ReleaseError, CORBA::SystemException);

        void loadProperties();
        void matchAllocationIdToStreamId(const std::string allocation_id, const std::string stream_id, const std::string port_name="");
        void removeAllocationIdRouting(const size_t tuner_id);
        void removeStreamIdRouting(const std::string stream_id, const std::string allocation_id="");

        virtual CF::Properties* getTunerStatus(const std::string& allocation_id);
        virtual void assignListener(const std::string& listen_alloc_id, const std::string& allocation_id);
        virtual void removeListener(const std::string& listen_alloc_id);
        void frontendTunerStatusChanged(const std::vector<frontend_tuner_status_struct_struct>* oldValue, const std::vector<frontend_tuner_status_struct_struct>* newValue);

    protected:
        void connectionTableChanged(const std::vector<connection_descriptor_struct>* oldValue, const std::vector<connection_descriptor_struct>* newValue);

        // Member variables exposed as properties
        std::string filePath;
        bool updateAvailableFiles;
        bool loop;
        AdvancedProperties_struct AdvancedProperties;
        std::vector<connection_descriptor_struct> connectionTable;
        std::vector<File_struct> availableFiles;

        // Ports
        frontend::InRFInfoPort *RFInfo_in;
        frontend::InDigitalTunerPort *DigitalTuner_in;
        bulkio::OutCharPort *dataChar_out;
        bulkio::OutOctetPort *dataOctet_out;
        bulkio::OutShortPort *dataShort_out;
        bulkio::OutUShortPort *dataUshort_out;
        bulkio::OutFloatPort *dataFloat_out;
        bulkio::OutULongPort *dataUlong_out;
        bulkio::OutLongPort *dataLong_out;
        bulkio::OutLongLongPort *dataLongLong_out;
        bulkio::OutULongLongPort *dataUlongLong_out;
        bulkio::OutDoublePort *dataDouble_out;

        std::map<std::string, std::string> listeners;

        virtual void setNumChannels(size_t num);

    private:
        void construct();
};
#endif // FEI_FILEREADER_IMPL_BASE_H
