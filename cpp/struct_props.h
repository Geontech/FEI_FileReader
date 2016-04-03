#ifndef STRUCTPROPS_H
#define STRUCTPROPS_H

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

*******************************************************************************************/

#include <ossie/CorbaUtils.h>
#include <CF/cf.h>
#include <ossie/PropertyMap.h>
#include <bulkio/bulkio.h>
typedef bulkio::connection_descriptor_struct connection_descriptor_struct;

#include <frontend/fe_tuner_struct_props.h>

struct AdvancedProperties_struct {
    AdvancedProperties_struct ()
    {
        maxOutputRate = 1e12;
        minOutputRate = 1.0;
        newStreamAfterLoop = false;
        packetSize = 1887432;
        sampleRateForSRI = false;
    };

    static std::string getId() {
        return std::string("AdvancedProperties");
    };

    double maxOutputRate;
    double minOutputRate;
    bool newStreamAfterLoop;
    CORBA::ULong packetSize;
    bool sampleRateForSRI;
};

inline bool operator>>= (const CORBA::Any& a, AdvancedProperties_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(*temp);
    if (props.contains("AdvancedProperties::maxOutputRate")) {
        if (!(props["AdvancedProperties::maxOutputRate"] >>= s.maxOutputRate)) return false;
    }
    if (props.contains("AdvancedProperties::minOutputRate")) {
        if (!(props["AdvancedProperties::minOutputRate"] >>= s.minOutputRate)) return false;
    }
    if (props.contains("AdvancedProperties::newStreamAfterLoop")) {
        if (!(props["AdvancedProperties::newStreamAfterLoop"] >>= s.newStreamAfterLoop)) return false;
    }
    if (props.contains("AdvancedProperties::packetSize")) {
        if (!(props["AdvancedProperties::packetSize"] >>= s.packetSize)) return false;
    }
    if (props.contains("AdvancedProperties::sampleRateForSRI")) {
        if (!(props["AdvancedProperties::sampleRateForSRI"] >>= s.sampleRateForSRI)) return false;
    }
    return true;
}

inline void operator<<= (CORBA::Any& a, const AdvancedProperties_struct& s) {
    redhawk::PropertyMap props;
 
    props["AdvancedProperties::maxOutputRate"] = s.maxOutputRate;
 
    props["AdvancedProperties::minOutputRate"] = s.minOutputRate;
 
    props["AdvancedProperties::newStreamAfterLoop"] = s.newStreamAfterLoop;
 
    props["AdvancedProperties::packetSize"] = s.packetSize;
 
    props["AdvancedProperties::sampleRateForSRI"] = s.sampleRateForSRI;
    a <<= props;
}

inline bool operator== (const AdvancedProperties_struct& s1, const AdvancedProperties_struct& s2) {
    if (s1.maxOutputRate!=s2.maxOutputRate)
        return false;
    if (s1.minOutputRate!=s2.minOutputRate)
        return false;
    if (s1.newStreamAfterLoop!=s2.newStreamAfterLoop)
        return false;
    if (s1.packetSize!=s2.packetSize)
        return false;
    if (s1.sampleRateForSRI!=s2.sampleRateForSRI)
        return false;
    return true;
}

inline bool operator!= (const AdvancedProperties_struct& s1, const AdvancedProperties_struct& s2) {
    return !(s1==s2);
}

struct frontend_tuner_status_struct_struct : public frontend::default_frontend_tuner_status_struct_struct {
    frontend_tuner_status_struct_struct () : frontend::default_frontend_tuner_status_struct_struct()
    {
    };

    static std::string getId() {
        return std::string("FRONTEND::tuner_status_struct");
    };

    std::string stream_id;
};

inline bool operator>>= (const CORBA::Any& a, frontend_tuner_status_struct_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(*temp);
    if (props.contains("FRONTEND::tuner_status::allocation_id_csv")) {
        if (!(props["FRONTEND::tuner_status::allocation_id_csv"] >>= s.allocation_id_csv)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::bandwidth")) {
        if (!(props["FRONTEND::tuner_status::bandwidth"] >>= s.bandwidth)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::center_frequency")) {
        if (!(props["FRONTEND::tuner_status::center_frequency"] >>= s.center_frequency)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::enabled")) {
        if (!(props["FRONTEND::tuner_status::enabled"] >>= s.enabled)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::group_id")) {
        if (!(props["FRONTEND::tuner_status::group_id"] >>= s.group_id)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::rf_flow_id")) {
        if (!(props["FRONTEND::tuner_status::rf_flow_id"] >>= s.rf_flow_id)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::sample_rate")) {
        if (!(props["FRONTEND::tuner_status::sample_rate"] >>= s.sample_rate)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::tuner_type")) {
        if (!(props["FRONTEND::tuner_status::tuner_type"] >>= s.tuner_type)) return false;
    }
    if (props.contains("FRONTEND::tuner_status::stream_id")) {
        if (!(props["FRONTEND::tuner_status::stream_id"] >>= s.stream_id)) return false;
    }
    return true;
}

inline void operator<<= (CORBA::Any& a, const frontend_tuner_status_struct_struct& s) {
    redhawk::PropertyMap props;
 
    props["FRONTEND::tuner_status::allocation_id_csv"] = s.allocation_id_csv;
 
    props["FRONTEND::tuner_status::bandwidth"] = s.bandwidth;
 
    props["FRONTEND::tuner_status::center_frequency"] = s.center_frequency;
 
    props["FRONTEND::tuner_status::enabled"] = s.enabled;
 
    props["FRONTEND::tuner_status::group_id"] = s.group_id;
 
    props["FRONTEND::tuner_status::rf_flow_id"] = s.rf_flow_id;
 
    props["FRONTEND::tuner_status::sample_rate"] = s.sample_rate;
 
    props["FRONTEND::tuner_status::tuner_type"] = s.tuner_type;
 
    props["FRONTEND::tuner_status::stream_id"] = s.stream_id;
    a <<= props;
}

inline bool operator== (const frontend_tuner_status_struct_struct& s1, const frontend_tuner_status_struct_struct& s2) {
    if (s1.allocation_id_csv!=s2.allocation_id_csv)
        return false;
    if (s1.bandwidth!=s2.bandwidth)
        return false;
    if (s1.center_frequency!=s2.center_frequency)
        return false;
    if (s1.enabled!=s2.enabled)
        return false;
    if (s1.group_id!=s2.group_id)
        return false;
    if (s1.rf_flow_id!=s2.rf_flow_id)
        return false;
    if (s1.sample_rate!=s2.sample_rate)
        return false;
    if (s1.tuner_type!=s2.tuner_type)
        return false;
    if (s1.stream_id!=s2.stream_id)
        return false;
    return true;
}

inline bool operator!= (const frontend_tuner_status_struct_struct& s1, const frontend_tuner_status_struct_struct& s2) {
    return !(s1==s2);
}

struct File_struct {
    File_struct ()
    {
        path = "";
        size = 0LL;
    };

    static std::string getId() {
        return std::string("availableFiles::File");
    };

    std::string path;
    CORBA::ULongLong size;
};

inline bool operator>>= (const CORBA::Any& a, File_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    const redhawk::PropertyMap& props = redhawk::PropertyMap::cast(*temp);
    if (props.contains("availableFiles::path")) {
        if (!(props["availableFiles::path"] >>= s.path)) return false;
    }
    if (props.contains("availableFiles::size")) {
        if (!(props["availableFiles::size"] >>= s.size)) return false;
    }
    return true;
}

inline void operator<<= (CORBA::Any& a, const File_struct& s) {
    redhawk::PropertyMap props;
 
    props["availableFiles::path"] = s.path;
 
    props["availableFiles::size"] = s.size;
    a <<= props;
}

inline bool operator== (const File_struct& s1, const File_struct& s2) {
    if (s1.path!=s2.path)
        return false;
    if (s1.size!=s2.size)
        return false;
    return true;
}

inline bool operator!= (const File_struct& s1, const File_struct& s2) {
    return !(s1==s2);
}

#endif // STRUCTPROPS_H
