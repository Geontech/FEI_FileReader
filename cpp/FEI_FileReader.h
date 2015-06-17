#ifndef FEI_FILEREADER_IMPL_H
#define FEI_FILEREADER_IMPL_H

#include "FEI_FileReader_base.h"

#include "RedHawkFileReader.h"

class FEI_FileReader_i : public FEI_FileReader_base
{
    ENABLE_LOGGING
    public:
        FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl);
        FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, char *compDev);
        FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities);
        FEI_FileReader_i(char *devMgr_ior, char *id, char *lbl, char *sftwrPrfl, CF::Properties capacities, char *compDev);
        ~FEI_FileReader_i();

        void initialize() throw (CF::LifeCycle::InitializeError, CORBA::SystemException);
        int serviceFunction();
        void start() throw (CF::Resource::StartError, CORBA::SystemException);
        void stop() throw (CF::Resource::StopError, CORBA::SystemException);

    protected:
        std::string getTunerType(const std::string& allocation_id);
        bool getTunerDeviceControl(const std::string& allocation_id);
        std::string getTunerGroupId(const std::string& allocation_id);
        std::string getTunerRfFlowId(const std::string& allocation_id);
        double getTunerCenterFrequency(const std::string& allocation_id);
        void setTunerCenterFrequency(const std::string& allocation_id, double freq);
        double getTunerBandwidth(const std::string& allocation_id);
        void setTunerBandwidth(const std::string& allocation_id, double bw);
        bool getTunerAgcEnable(const std::string& allocation_id);
        void setTunerAgcEnable(const std::string& allocation_id, bool enable);
        float getTunerGain(const std::string& allocation_id);
        void setTunerGain(const std::string& allocation_id, float gain);
        long getTunerReferenceSource(const std::string& allocation_id);
        void setTunerReferenceSource(const std::string& allocation_id, long source);
        bool getTunerEnable(const std::string& allocation_id);
        void setTunerEnable(const std::string& allocation_id, bool enable);
        double getTunerOutputSampleRate(const std::string& allocation_id);
        void setTunerOutputSampleRate(const std::string& allocation_id, double sr);
        std::string get_rf_flow_id(const std::string& port_name);
        void set_rf_flow_id(const std::string& port_name, const std::string& id);
        frontend::RFInfoPkt get_rfinfo_pkt(const std::string& port_name);
        void set_rfinfo_pkt(const std::string& port_name, const frontend::RFInfoPkt& pkt);

    private:
        ////////////////////////////////////////
        // Required device specific functions // -- to be implemented by device developer
        ////////////////////////////////////////

        // these are pure virtual, must be implemented here
        void deviceEnable(frontend_tuner_status_struct_struct &fts, size_t tuner_id);
        void deviceDisable(frontend_tuner_status_struct_struct &fts, size_t tuner_id);
        bool deviceSetTuning(const frontend::frontend_tuner_allocation_struct &request, frontend_tuner_status_struct_struct &fts, size_t tuner_id);
        bool deviceDeleteTuning(frontend_tuner_status_struct_struct &fts, size_t tuner_id);

        // Miscellaneous helper methods
        void AdvancedPropertiesChanged(const AdvancedProperties_struct *oldValue, const AdvancedProperties_struct *newValue);
        void construct();
        void filePathChanged(const std::string *oldValue, const std::string *newValue);
        void fileReaderDisable(size_t tunerId);
        void fileReaderEnable(size_t tunerId);
        std::string getStreamId(size_t tunerId);
        void playbackStateChanged(const std::string *oldValue, const std::string *newValue);
        void setPlaybackState(const std::string &value);
        void setPlaybackState(const std::string &value, const std::string &oldValue);

        void updateAvailableFilesVector();
        void updateAvailableFilesChanged(const bool *oldValue, const bool *newValue);
        void updateFileReaders();
        void updateRfFlowId(const std::string &rfFlowId);

    private:
        std::vector<RedHawkFileReader *> fileReaders;
        bool isPlaying;
        frontend::RFInfoPkt rfInfoPkt;
};

#endif // FEI_FILEREADER_IMPL_H
