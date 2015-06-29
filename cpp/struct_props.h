#ifndef STRUCTPROPS_H
#define STRUCTPROPS_H

/*******************************************************************************************

    AUTO-GENERATED CODE. DO NOT MODIFY

*******************************************************************************************/

#include <ossie/CorbaUtils.h>
#include <CF/cf.h>
#include <bulkio/bulkio.h>
typedef bulkio::connection_descriptor_struct connection_descriptor_struct;

#include <frontend/fe_tuner_struct_props.h>

struct AdvancedProperties_struct {
    AdvancedProperties_struct ()
    {
        PacketSize = 1887440;
        QueueSize = 25;
    };

    static std::string getId() {
        return std::string("AdvancedProperties");
    };

    CORBA::ULong PacketSize;
    CORBA::ULong QueueSize;
};

inline bool operator>>= (const CORBA::Any& a, AdvancedProperties_struct& s) {
    CF::Properties* temp;
    if (!(a >>= temp)) return false;
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("AdvancedProperties::PacketSize", props[idx].id)) {
            if (!(props[idx].value >>= s.PacketSize)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
        else if (!strcmp("AdvancedProperties::QueueSize", props[idx].id)) {
            if (!(props[idx].value >>= s.QueueSize)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const AdvancedProperties_struct& s) {
    CF::Properties props;
    props.length(2);
    props[0].id = CORBA::string_dup("AdvancedProperties::PacketSize");
    props[0].value <<= s.PacketSize;
    props[1].id = CORBA::string_dup("AdvancedProperties::QueueSize");
    props[1].value <<= s.QueueSize;
    a <<= props;
};

inline bool operator== (const AdvancedProperties_struct& s1, const AdvancedProperties_struct& s2) {
    if (s1.PacketSize!=s2.PacketSize)
        return false;
    if (s1.QueueSize!=s2.QueueSize)
        return false;
    return true;
};

inline bool operator!= (const AdvancedProperties_struct& s1, const AdvancedProperties_struct& s2) {
    return !(s1==s2);
};

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
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("FRONTEND::tuner_status::allocation_id_csv", props[idx].id)) {
            if (!(props[idx].value >>= s.allocation_id_csv)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
        else if (!strcmp("FRONTEND::tuner_status::bandwidth", props[idx].id)) {
            if (!(props[idx].value >>= s.bandwidth)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
        else if (!strcmp("FRONTEND::tuner_status::center_frequency", props[idx].id)) {
            if (!(props[idx].value >>= s.center_frequency)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
        else if (!strcmp("FRONTEND::tuner_status::enabled", props[idx].id)) {
            if (!(props[idx].value >>= s.enabled)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
        else if (!strcmp("FRONTEND::tuner_status::group_id", props[idx].id)) {
            if (!(props[idx].value >>= s.group_id)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
        else if (!strcmp("FRONTEND::tuner_status::rf_flow_id", props[idx].id)) {
            if (!(props[idx].value >>= s.rf_flow_id)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
        else if (!strcmp("FRONTEND::tuner_status::sample_rate", props[idx].id)) {
            if (!(props[idx].value >>= s.sample_rate)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
        else if (!strcmp("FRONTEND::tuner_status::tuner_type", props[idx].id)) {
            if (!(props[idx].value >>= s.tuner_type)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
        else if (!strcmp("FRONTEND::tuner_status::stream_id", props[idx].id)) {
            if (!(props[idx].value >>= s.stream_id)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const frontend_tuner_status_struct_struct& s) {
    CF::Properties props;
    props.length(9);
    props[0].id = CORBA::string_dup("FRONTEND::tuner_status::allocation_id_csv");
    props[0].value <<= s.allocation_id_csv;
    props[1].id = CORBA::string_dup("FRONTEND::tuner_status::bandwidth");
    props[1].value <<= s.bandwidth;
    props[2].id = CORBA::string_dup("FRONTEND::tuner_status::center_frequency");
    props[2].value <<= s.center_frequency;
    props[3].id = CORBA::string_dup("FRONTEND::tuner_status::enabled");
    props[3].value <<= s.enabled;
    props[4].id = CORBA::string_dup("FRONTEND::tuner_status::group_id");
    props[4].value <<= s.group_id;
    props[5].id = CORBA::string_dup("FRONTEND::tuner_status::rf_flow_id");
    props[5].value <<= s.rf_flow_id;
    props[6].id = CORBA::string_dup("FRONTEND::tuner_status::sample_rate");
    props[6].value <<= s.sample_rate;
    props[7].id = CORBA::string_dup("FRONTEND::tuner_status::tuner_type");
    props[7].value <<= s.tuner_type;
    props[8].id = CORBA::string_dup("FRONTEND::tuner_status::stream_id");
    props[8].value <<= s.stream_id;
    a <<= props;
};

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
};

inline bool operator!= (const frontend_tuner_status_struct_struct& s1, const frontend_tuner_status_struct_struct& s2) {
    return !(s1==s2);
};

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
    CF::Properties& props = *temp;
    for (unsigned int idx = 0; idx < props.length(); idx++) {
        if (!strcmp("availableFiles::path", props[idx].id)) {
            if (!(props[idx].value >>= s.path)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
        else if (!strcmp("availableFiles::size", props[idx].id)) {
            if (!(props[idx].value >>= s.size)) {
                CORBA::TypeCode_var typecode = props[idx].value.type();
                if (typecode->kind() != CORBA::tk_null) {
                    return false;
                }
            }
        }
    }
    return true;
};

inline void operator<<= (CORBA::Any& a, const File_struct& s) {
    CF::Properties props;
    props.length(2);
    props[0].id = CORBA::string_dup("availableFiles::path");
    props[0].value <<= s.path;
    props[1].id = CORBA::string_dup("availableFiles::size");
    props[1].value <<= s.size;
    a <<= props;
};

inline bool operator== (const File_struct& s1, const File_struct& s2) {
    if (s1.path!=s2.path)
        return false;
    if (s1.size!=s2.size)
        return false;
    return true;
};

inline bool operator!= (const File_struct& s1, const File_struct& s2) {
    return !(s1==s2);
};

#endif // STRUCTPROPS_H
