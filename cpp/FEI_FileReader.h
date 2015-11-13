#ifndef FEI_FILEREADER_IMPL_H
#define FEI_FILEREADER_IMPL_H

#include "FEI_FileReader_base.h"

#include "FormattedFileReader.h"

/*
 * A structure which includes extra data associated with a file reader
 */
struct FileReaderContainer {
    const FilePacket *currentPacket;
    FormattedFileReader *fileReader;
    boost::system_time firstSeen;
    boost::mutex *lock;
    BULKIO::StreamSRI sri;
    boost::posix_time::time_duration timeDuration;
    BULKIO::PrecisionUTCTime timestamp;
    bool updateSRI;

    std::vector<int8_t> charOutput;
    std::vector<uint8_t> uCharOutput;
    std::vector<int16_t> shortOutput;
    std::vector<uint16_t> uShortOutput;
    std::vector<int32_t> longOutput;
    std::vector<uint32_t> uLongOutput;
    std::vector<float> floatOutput;
    std::vector<int64_t> longLongOutput;
    std::vector<uint64_t> uLongLongOutput;
    std::vector<double> doubleOutput;
};

/*
 * A typedef for file reader container vector iteration
 */
typedef std::vector<FileReaderContainer>::iterator FileReaderIterator;

class FEI_FileReader_i : public FEI_FileReader_base
{
    ENABLE_LOGGING
    public:
        FEI_FileReader_i(
                char *devMgr_ior,
                char *id,
                char *lbl,
                char *sftwrPrfl);

        FEI_FileReader_i(
                char *devMgr_ior,
                char *id,
                char *lbl,
                char *sftwrPrfl,
                char *compDev);

        FEI_FileReader_i(
                char *devMgr_ior,
                char *id,
                char *lbl,
                char *sftwrPrfl,
                CF::Properties capacities);

        FEI_FileReader_i(
                char *devMgr_ior,
                char *id,
                char *lbl,
                char *sftwrPrfl,
                CF::Properties capacities,
                char *compDev);

        ~FEI_FileReader_i();

        // REDHAWK LifeCycle functions
        void initialize() throw (
                CF::LifeCycle::InitializeError,
                CORBA::SystemException);

        int serviceFunction();

        void stop() throw (CF::Resource::StopError, CORBA::SystemException);

    private:
        ////////////////////////////////////////
        // Required device specific functions //
        ////////////////////////////////////////

        // These are pure virtual and must be implemented here
        void deviceEnable(
                frontend_tuner_status_struct_struct &fts,
                size_t tuner_id);

        void deviceDisable(
                frontend_tuner_status_struct_struct &fts,
                size_t tuner_id);

        bool deviceSetTuning(
                const frontend::frontend_tuner_allocation_struct &request,
                frontend_tuner_status_struct_struct &fts,
                size_t tuner_id);

        bool deviceDeleteTuning(
                frontend_tuner_status_struct_struct &fts,
                size_t tuner_id);

        // Property change listeners
        void AdvancedPropertiesChanged(
                const AdvancedProperties_struct *oldValue,
                const AdvancedProperties_struct *newValue);

        void filePathChanged(
                const std::string *oldValue,
                const std::string *newValue);

        void loopChanged(const bool *oldValue, const bool *newValue);

        void updateAvailableFilesChanged(
                const bool *oldValue,
                const bool *newValue);

        // Connection listeners
        template <typename T>
        void connectionAdded(const char *connectionId);

        template <typename T>
        void connectionRemoved(const char *connectionId);

        // Miscellaneous helper methods
        void construct();

        void fileReaderDisable(size_t tunerId);

        void fileReaderEnable(size_t tunerId);

        std::string getStreamId(size_t tunerId);

        void initializeOutputVectorByType(FileReaderContainer &container);

        void initializeOutputVectors();

        void pushPacketByType(
                FileReaderContainer &container,
                BULKIO::PrecisionUTCTime &T,
                bool EOS, const std::string &streamID);

        void pushSRIByType(BULKIO::StreamSRI &sri, const std::string &type);

        void setPacketSizes(size_t packetSize);

        void setQueueSizes(size_t queueSize);

        size_t sizeFromType(const std::string &type);

        const std::string typeFromTypeInfo(const std::type_info &typeInfo);

        void updateAvailableFilesVector();

        void updateFileReaders();

        void updateRfFlowId(const std::string &rfFlowId);

    protected:
        // Tuner Control and RfInfo port functions
        std::string getTunerType(const std::string& allocation_id);

        bool getTunerDeviceControl(const std::string& allocation_id);

        std::string getTunerGroupId(const std::string& allocation_id);

        std::string getTunerRfFlowId(const std::string& allocation_id);

        double getTunerCenterFrequency(const std::string& allocation_id);

        void setTunerCenterFrequency(
                const std::string& allocation_id,
                double freq);

        double getTunerBandwidth(const std::string& allocation_id);

        void setTunerBandwidth(const std::string& allocation_id, double bw);

        bool getTunerAgcEnable(const std::string& allocation_id);

        void setTunerAgcEnable(const std::string& allocation_id, bool enable);

        float getTunerGain(const std::string& allocation_id);

        void setTunerGain(const std::string& allocation_id, float gain);

        long getTunerReferenceSource(const std::string& allocation_id);

        void setTunerReferenceSource(
                const std::string& allocation_id,
                long source);

        bool getTunerEnable(const std::string& allocation_id);

        void setTunerEnable(const std::string& allocation_id, bool enable);

        double getTunerOutputSampleRate(const std::string& allocation_id);

        void setTunerOutputSampleRate(
                const std::string& allocation_id,
                double sr);

        std::string get_rf_flow_id(const std::string& port_name);

        void set_rf_flow_id(
                const std::string& port_name,
                const std::string& id);

        frontend::RFInfoPkt get_rfinfo_pkt(const std::string& port_name);

        void set_rfinfo_pkt(
                const std::string& port_name,
                const frontend::RFInfoPkt& pkt);

    private:
        std::map<std::string, std::vector<std::string> > streamIdToPortType;
        std::vector<FileReaderContainer> fileReaderContainers;
        uint64_t fractionalResolution;
        bool isPlaying;
        frontend::RFInfoPkt rfInfoPkt;
        bool useMaxOutputRate;
};

#endif // FEI_FILEREADER_IMPL_H
