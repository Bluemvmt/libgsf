#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include "../cjson/cJSON.h"
#include "gsf_json.h"
#include "gsf.h"


gsfJsonFile gsfJsonFiles[GSF_MAX_OPEN_FILES];

static double epoch_double(struct timespec tv) {
  char time_str[32];

  sprintf(time_str, "%ld.%.9ld", tv.tv_sec, tv.tv_nsec);

  return atof(time_str);
}

static void add_double_array(cJSON *json, const char* name, double *array, int length) {
    if (array != NULL) {
        cJSON *json_array = cJSON_AddArrayToObject(json, name);
        for (int i = 0; i < length; i++) {
            cJSON *double_json = cJSON_CreateNumber(array[i]);
            cJSON_AddItemToArray(json_array, double_json);
        }
    }
}

static void add_char_array(cJSON *json, const char *name, unsigned char *array, int length) {
    if (array != NULL && *array != 0) {
        cJSON *json_array = cJSON_AddArrayToObject(json, name);
        for (int i = 0; i < length; i++) {
            cJSON *number_json = cJSON_CreateNumber(array[i]);
            cJSON_AddItemToArray(json_array, number_json);
        }
    }
}

static void add_short_array(cJSON *json, const char *name, unsigned short *array, int length) {
    if (array != NULL && *array != 0) {
        cJSON *json_array = cJSON_AddArrayToObject(json, name);
        for (int i = 0; i < length; i++) {
            cJSON *number_json = cJSON_CreateNumber(array[i]);
            cJSON_AddItemToArray(json_array, number_json);
        }
    }
}

cJSON *gsfComment_toJson(struct t_gsfComment comment, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_COMMENT);
    double epoch_time = epoch_double(comment.comment_time);
    if (gsfJsonFileInfo.include_denormalized_fields) {
        cJSON_AddNumberToObject(json, "timestamp", epoch_time);
        cJSON_AddStringToObject(json, "gsf_version", gsfJsonFileInfo.gsf_version);
        cJSON_AddStringToObject(json, "file_name", gsfJsonFileInfo.file_name);
    }
    cJSON *body_json = cJSON_AddObjectToObject(json, "json_record");
    cJSON_AddNumberToObject(body_json, "comment_length", comment.comment_length);
    cJSON_AddStringToObject(body_json, "comment", comment.comment);
    return json;
}

cJSON *gsfAttitude_toJson(struct t_gsfAttitude attitude, int include_denormalized_fields) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_ATTITUDE);
    double epoch_time = epoch_double(*attitude.attitude_time);
    if (include_denormalized_fields) {
        cJSON_AddNumberToObject(json, "timestamp", epoch_time);
    }
    cJSON *body_json = cJSON_AddObjectToObject(json, "json_record");
    cJSON_AddNumberToObject(body_json, "start_time", epoch_time);
    cJSON_AddNumberToObject(body_json, "num_measurements", attitude.num_measurements);
    cJSON_AddNumberToObject(body_json, "pitch", *attitude.pitch);
    cJSON_AddNumberToObject(body_json, "roll", *attitude.roll);
    cJSON_AddNumberToObject(body_json, "heave", *attitude.heave);
    cJSON_AddNumberToObject(body_json, "heading", *attitude.heading);

    return json;
}

cJSON *gsfSwathBathySummary_toJson(struct t_gsfSwathBathySummary summary, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();   
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_SWATH_BATHY_SUMMARY);

    if (gsfJsonFileInfo.include_denormalized_fields) {
        cJSON_AddStringToObject(json, "gsf_version", gsfJsonFileInfo.gsf_version);
        cJSON_AddStringToObject(json, "file_name", gsfJsonFileInfo.file_name);
        cJSON_AddNumberToObject(json, "timestamp", epoch_double(summary.start_time));
        cJSON_AddNumberToObject(json, "latitude", summary.min_latitude);
        cJSON_AddNumberToObject(json, "longitude", summary.min_longitude);
    }
    cJSON *body_json = cJSON_AddObjectToObject(json, "summary");
    cJSON_AddNumberToObject(body_json, "start_time", epoch_double(summary.start_time));
    cJSON_AddNumberToObject(body_json, "end_time", epoch_double(summary.end_time));
    cJSON_AddNumberToObject(body_json, "min_latitude", summary.min_latitude);
    cJSON_AddNumberToObject(body_json, "min_longitude", summary.min_longitude);
    cJSON_AddNumberToObject(body_json, "max_latitude", summary.max_latitude);
    cJSON_AddNumberToObject(body_json, "max_longitude", summary.max_longitude);
    cJSON_AddNumberToObject(body_json, "min_depth", summary.min_depth);
    cJSON_AddNumberToObject(body_json, "max_depth", summary.max_depth);
    return json;
}

static void gsfBRBIntensity_toJson(cJSON *json, gsfBRBIntensity *brb) {
    if (brb != NULL) {
        cJSON *brb_json = cJSON_AddObjectToObject(json, "brb_inten");
        cJSON_AddNumberToObject(brb_json, "bits_per_sample", (int) brb->bits_per_sample);
        cJSON_AddNumberToObject(brb_json, "applied_corrections", brb->applied_corrections);
    }
}

static void gsfEchotracSpecific_toJson(cJSON *json, t_gsfEchotracSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfEchotracSpecific");
    cJSON_AddNumberToObject(sensor_json, "navigation_error", (int) sensor_data.navigation_error);
    cJSON_AddNumberToObject(sensor_json, "mpp_source", (int) sensor_data.mpp_source);
    cJSON_AddNumberToObject(sensor_json, "tide_source", (int) sensor_data.tide_source);
}

static void gsfSBSensorSpecific_toJson(cJSON *json, gsfSBSensorSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfSBSensorSpecific");
    gsfEchotracSpecific_toJson(sensor_json, sensor_data.gsfEchotracSpecific);
    // t_gsfEchotracSpecific    gsfBathy2000Specific;
    // t_gsfMGD77Specific       gsfMGD77Specific;
    // t_gsfBDBSpecific         gsfBDBSpecific;
    // t_gsfNOSHDBSpecific      gsfNOSHDBSpecific;
}

static void gsfSBAmpSpecific_toJson(cJSON *json, t_gsfSBAmpSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfSBAmpSpecific");
    cJSON_AddNumberToObject(sensor_json, "hour", (int) sensor_data.hour);
    cJSON_AddNumberToObject(sensor_json, "minute", (int) sensor_data.minute);
    cJSON_AddNumberToObject(sensor_json, "second", (int) sensor_data.second);
    cJSON_AddNumberToObject(sensor_json, "hundreths", (int) sensor_data.hundredths);
    cJSON_AddNumberToObject(sensor_json, "block_number", (int) sensor_data.block_number);
    cJSON_AddNumberToObject(sensor_json, "avg_gate_depth", (int) sensor_data.avg_gate_depth);
}


static void gsfSeaBeamSpecific_toJson(cJSON *json, t_gsfSeaBeamSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfSeaBeamSpecific");
    cJSON_AddNumberToObject(sensor_json, "EclipseTime", (int) sensor_data.EclipseTime);
}

static void gsfEM100Specific_toJson(cJSON *json, t_gsfEM100Specific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfEM100Specific");
    cJSON_AddNumberToObject(sensor_json, "ship_pitch", sensor_data.ship_pitch);
    cJSON_AddNumberToObject(sensor_json, "transducer_pitch", sensor_data.transducer_pitch);
    cJSON_AddNumberToObject(sensor_json, "mode", sensor_data.mode);
    cJSON_AddNumberToObject(sensor_json, "power",  sensor_data.power);
    cJSON_AddNumberToObject(sensor_json, "attenuation", sensor_data.attenuation);
    cJSON_AddNumberToObject(sensor_json, "tvg", sensor_data.tvg);
    cJSON_AddNumberToObject(sensor_json, "pulse_length", sensor_data.pulse_length);
    cJSON_AddNumberToObject(sensor_json, "counter", sensor_data.counter);
}

static void gsfEM121ASpecific_toJson(cJSON *json, t_gsfEM121ASpecific sensor_data, const char *name) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, name);
    cJSON_AddNumberToObject(sensor_json, "ping_number", sensor_data.ping_number);
    cJSON_AddNumberToObject(sensor_json, "mode", sensor_data.mode);
    cJSON_AddNumberToObject(sensor_json, "valid_beams", sensor_data.valid_beams);
    cJSON_AddNumberToObject(sensor_json, "pulse_length", sensor_data.pulse_length);
    cJSON_AddNumberToObject(sensor_json, "beam_width", sensor_data.beam_width);
    cJSON_AddNumberToObject(sensor_json, "tx_power", sensor_data.tx_power);
    cJSON_AddNumberToObject(sensor_json, "tx_status", sensor_data.tx_status);
    cJSON_AddNumberToObject(sensor_json, "rx_status", sensor_data.rx_status);
    cJSON_AddNumberToObject(sensor_json, "surface_velocity", sensor_data.surface_velocity);
}

static void gsfSeaBatSpecific_toJson(cJSON *json, t_gsfSeaBatSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfSeaBatSpecific");
    cJSON_AddNumberToObject(sensor_json, "ping_number", sensor_data.ping_number);
    cJSON_AddNumberToObject(sensor_json, "surface_velocity", sensor_data.surface_velocity);
    cJSON_AddNumberToObject(sensor_json, "mode", sensor_data.mode);
    cJSON_AddNumberToObject(sensor_json, "sonar_range", sensor_data.sonar_range);
    cJSON_AddNumberToObject(sensor_json, "transmit_power", sensor_data.transmit_power);
    cJSON_AddNumberToObject(sensor_json, "receive_gain", sensor_data.receive_gain);
}

static void gsfEM950Specific_toJson(cJSON *json, t_gsfEM950Specific sensor_data, const char *name) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, name);
    cJSON_AddNumberToObject(sensor_json, "ping_number", sensor_data.ping_number);
    cJSON_AddNumberToObject(sensor_json, "mode", sensor_data.mode);
    cJSON_AddNumberToObject(sensor_json, "ping_quality", sensor_data.ping_quality);
    cJSON_AddNumberToObject(sensor_json, "ship_pitch", sensor_data.ship_pitch);
    cJSON_AddNumberToObject(sensor_json, "transducer_pitch", sensor_data.transducer_pitch);
    cJSON_AddNumberToObject(sensor_json, "surface_velocity", sensor_data.surface_velocity);
}

static void gsfSeamapSpecific_toJson(cJSON *json, t_gsfSeamapSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfSeamapSpecific");
    add_double_array(sensor_json, "portTransmitter", sensor_data.portTransmitter, 2);
    add_double_array(sensor_json, "stbdTransmitter", sensor_data.stbdTransmitter, 2);
    cJSON_AddNumberToObject(sensor_json, "portGain", sensor_data.portGain);
    cJSON_AddNumberToObject(sensor_json, "stbdGain", sensor_data.stbdGain);
    cJSON_AddNumberToObject(sensor_json, "portPulseLength", sensor_data.portPulseLength);
    cJSON_AddNumberToObject(sensor_json, "stbdPulseLength", sensor_data.stbdPulseLength);
    cJSON_AddNumberToObject(sensor_json, "pressureDepth", sensor_data.pressureDepth);
    cJSON_AddNumberToObject(sensor_json, "altitude", sensor_data.altitude);
    cJSON_AddNumberToObject(sensor_json, "temperature", sensor_data.temperature);
}

static void gsfCmpSassSpecific_toJson(cJSON *json, t_gsfCmpSassSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfCmpSassSpecific");
    cJSON_AddNumberToObject(sensor_json, "lfreq", sensor_data.lfreq);
    cJSON_AddNumberToObject(sensor_json, "lntens", sensor_data.lntens);
}

// static void gsfSeaBatIISpecific_toJson(cJSON *json, t_gsfSeaBatIISpecific sensor_data) {
//     int             ping_number;            /* 1 - 32767 */
//     double          surface_velocity;       /* meters/second */
//     int             mode;                   /* bit mapped, see macros below */
//     int             sonar_range;            /* meters */
//     int             transmit_power;
//     int             receive_gain;
//     double          fore_aft_bw;            /* fore/aft beam width in degrees */
//     double          athwart_bw;             /* athwartships beam width in degrees */
// }

static void gsfSensorSpecific_toJson(cJSON *json, gsfSensorSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
    gsfSBAmpSpecific_toJson(sensor_json, sensor_data.gsfSBAmpSpecific);
    gsfSeaBeamSpecific_toJson(sensor_json, sensor_data.gsfSeaBeamSpecific);
    gsfEM100Specific_toJson(sensor_json, sensor_data.gsfEM100Specific);
    gsfEM121ASpecific_toJson(sensor_json, sensor_data.gsfEM121ASpecific, "gsfEM121ASpecific");
    gsfEM121ASpecific_toJson(sensor_json, sensor_data.gsfEM121ASpecific, "gsfEM121Specific");
    gsfSeaBatSpecific_toJson(sensor_json, sensor_data.gsfSeaBatSpecific);
    gsfEM950Specific_toJson(sensor_json, sensor_data.gsfEM950Specific, "gsfEM950Specific");
    gsfEM950Specific_toJson(sensor_json, sensor_data.gsfEM1000Specific, "gsfEM1000Specific");
    gsfSeamapSpecific_toJson(sensor_json, sensor_data.gsfSeamapSpecific);
    gsfCmpSassSpecific_toJson(sensor_json, sensor_data.gsfCmpSassSpecific);
}

static cJSON *gsfSingleBeamPing_toJson(struct t_gsfSingleBeamPing ping) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_SINGLE_BEAM_PING);
    cJSON *body_json = cJSON_AddObjectToObject(json, "json_record");
    cJSON_AddNumberToObject(body_json, "ping_time", epoch_double(ping.ping_time));
    cJSON_AddNumberToObject(body_json, "latitude", ping.latitude);
    cJSON_AddNumberToObject(body_json, "longitude", ping.longitude);
    cJSON_AddNumberToObject(body_json, "tide_corrector", ping.tide_corrector);
    cJSON_AddNumberToObject(body_json, "depth_corrector", ping.depth_corrector);
    cJSON_AddNumberToObject(body_json, "heading", ping.heading);
    cJSON_AddNumberToObject(body_json, "pitch", ping.pitch);
    cJSON_AddNumberToObject(body_json, "roll", ping.roll);
    cJSON_AddNumberToObject(body_json, "heave", ping.heave);
    cJSON_AddNumberToObject(body_json, "depth", ping.depth);
    cJSON_AddNumberToObject(body_json, "sound_speed_correction", ping.sound_speed_correction);
    cJSON_AddNumberToObject(body_json, "positioning_system_type", ping.positioning_system_type);
    cJSON_AddNumberToObject(body_json, "sensor_id", ping.sensor_id);
    gsfSBSensorSpecific_toJson(body_json, ping.sensor_data);
    return json;
}

static cJSON *gsfSwathBathyPing_toJson(struct t_gsfSwathBathyPing ping, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();
    cJSON *body_json = cJSON_AddObjectToObject(json, "json_record");

    double epoch_time = epoch_double(ping.ping_time);
    if (gsfJsonFileInfo.include_denormalized_fields) {
        cJSON_AddStringToObject(json, "gsf_version", gsfJsonFileInfo.gsf_version);
        cJSON_AddStringToObject(json, "file_name", gsfJsonFileInfo.file_name);
        cJSON_AddNumberToObject(json, "timestamp", epoch_time);
        cJSON_AddNumberToObject(json, "latitude", ping.latitude);
        cJSON_AddNumberToObject(json, "longitude", ping.longitude);
    }
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_SWATH_BATHYMETRY_PING);
    cJSON_AddNumberToObject(body_json, "ping_time", epoch_double(ping.ping_time));
    cJSON_AddNumberToObject(body_json, "latitude", ping.latitude);
    cJSON_AddNumberToObject(body_json, "longitude", ping.longitude);
    cJSON_AddNumberToObject(body_json, "height", ping.height);  
    cJSON_AddNumberToObject(body_json, "sep", ping.sep);
    cJSON_AddNumberToObject(body_json, "number_beams", ping.number_beams);
    cJSON_AddNumberToObject(body_json, "center_beam", ping.center_beam);
    cJSON_AddNumberToObject(body_json, "ping_flags", ping.ping_flags);
    cJSON_AddNumberToObject(body_json, "reserved", ping.reserved);  
    cJSON_AddNumberToObject(body_json, "tide_corrector", ping.tide_corrector);
    cJSON_AddNumberToObject(body_json, "gps_tide_corrector", ping.gps_tide_corrector);
    cJSON_AddNumberToObject(body_json, "depth_corrector", ping.depth_corrector);
    cJSON_AddNumberToObject(body_json, "heading", ping.heading);
    cJSON_AddNumberToObject(body_json, "pitch", ping.pitch);
    cJSON_AddNumberToObject(body_json, "roll", ping.roll);
    cJSON_AddNumberToObject(body_json, "heave", ping.heave);
    cJSON_AddNumberToObject(body_json, "course", ping.course);
    cJSON_AddNumberToObject(body_json, "speed", ping.speed);
    add_double_array(body_json, "depth", ping.depth, ping.number_beams);
    add_double_array(body_json, "nominal_depth", ping.nominal_depth, ping.number_beams);
    add_double_array(body_json, "across_track", ping.across_track, ping.number_beams);
    add_double_array(body_json, "travel_time", ping.travel_time, ping.number_beams);
    add_double_array(body_json, "beam_angle", ping.beam_angle, ping.number_beams);
    add_double_array(body_json, "mc_amplitude", ping.mc_amplitude, ping.number_beams);
    add_double_array(body_json, "mr_amplitude", ping.mr_amplitude, ping.number_beams);
    add_double_array(body_json, "echo_width", ping.echo_width, ping.number_beams);
    add_double_array(body_json, "quality_factor", ping.quality_factor, ping.number_beams);
    add_double_array(body_json, "receive_heave", ping.receive_heave, ping.number_beams);
    add_double_array(body_json, "depth_error", ping.depth_error, ping.number_beams);
    add_double_array(body_json, "across_track_error", ping.across_track, ping.number_beams);
    add_double_array(body_json, "along_track_error", ping.along_track_error, ping.number_beams);
    add_char_array(body_json, "quality_flags", ping.quality_flags, ping.number_beams);
    add_char_array(body_json, "beam_flags", ping.beam_flags, ping.number_beams);
    add_double_array(body_json, "signal_to_noise", ping.signal_to_noise, ping.number_beams);
    add_double_array(body_json, "beam_angle_forward", ping.beam_angle_forward, ping.number_beams);
    add_double_array(body_json, "vertical_error", ping.vertical_error, ping.number_beams);
    add_double_array(body_json, "horizontal_error", ping.horizontal_error, ping.number_beams);
    add_short_array(body_json, "sector_number", ping.sector_number, ping.number_beams);
    add_short_array(body_json, "detection_info", ping.detection_info, ping.number_beams);
    add_double_array(body_json, "incident_beam_adj", ping.incident_beam_adj, ping.number_beams);
    add_short_array(body_json, "system_cleaning", ping.system_cleaning, ping.number_beams);
    add_double_array(body_json, "doppler_corr", ping.doppler_corr, ping.number_beams);
    add_double_array(body_json, "sonar_vert_uncert", ping.sonar_vert_uncert, ping.number_beams);
    add_double_array(body_json, "sonar_horz_uncert", ping.sonar_horz_uncert, ping.number_beams);
    add_double_array(body_json, "detection_window", ping.detection_window, ping.number_beams);
    add_double_array(body_json, "mean_abs_coeff", ping.mean_abs_coeff, ping.number_beams);
    cJSON_AddNumberToObject(body_json, "sensor_id", ping.sensor_id);
    gsfBRBIntensity_toJson(body_json, ping.brb_inten);
    gsfSensorSpecific_toJson(body_json, ping.sensor_data);
    return json;
}


char *gsfRecord_toJson(gsfDataID dataID, gsfRecords record, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json;
    switch (dataID.recordID) {
        case GSF_RECORD_COMMENT:
            json = gsfComment_toJson(record.comment, gsfJsonFileInfo);
            break;
        case GSF_RECORD_SWATH_BATHY_SUMMARY:
            json =  gsfSwathBathySummary_toJson(record.summary, gsfJsonFileInfo);
            break;
        case GSF_RECORD_SWATH_BATHYMETRY_PING:
            json = gsfSwathBathyPing_toJson(record.mb_ping, gsfJsonFileInfo);
            break;
        case GSF_RECORD_SINGLE_BEAM_PING:
            json = gsfSingleBeamPing_toJson(record.sb_ping);
            break;
        default:
            return NULL;
    }
    return cJSON_PrintUnformatted(json); 
}

int gsfOpenForJson(char *filename, const int mode, int *handle, int buf_size, int include_denormlized_fields) {
    int retvalue = gsfOpenBuffered(filename, mode, handle, buf_size);
    if (retvalue == 0) {
        memcpy(gsfJsonFiles[*handle].file_name, basename(filename), MAX_FILE_NAME_SIZE);
        gsfJsonFiles[*handle].include_denormalized_fields = include_denormlized_fields;
        gsfJsonFiles[*handle].gsf_version_set = 0;
    }
    return retvalue;
}

int gsfCloseForJson(int handle) {
    gsfJsonFiles[handle].file_name[0] = 0;
    gsfJsonFiles[handle].gsf_version_set = 0;
    gsfJsonFiles[handle].gsf_version[0] = 0;
    return gsfClose(handle);
}

struct t_gsfJsonRecord gsfNextJsonRecord(int handle, int desired_record, int include_denormalized_fields) {
    gsfDataID gsfID;
    gsfRecords gsfRec;
    struct t_gsfJsonRecord nextRecord;

    int bytes_read;
    bytes_read = gsfRead(handle, desired_record, &gsfID, &gsfRec, NULL, 0);
    if (bytes_read <= 0) {
        nextRecord.lastReturnValue = bytes_read;
        nextRecord.jsonRecord = NULL;
    } else {
        nextRecord.lastReturnValue = bytes_read;
        if (gsfID.recordID == GSF_RECORD_HEADER) {
            memcpy(gsfJsonFiles[handle].gsf_version, gsfRec.header.version, strlen(gsfRec.header.version));
            gsfJsonFiles[handle].gsf_version_set = 1;
            nextRecord.jsonRecord = NULL;
        } else {
            nextRecord.jsonRecord = gsfRecord_toJson(gsfID, gsfRec, gsfJsonFiles[handle]);
        }
    }

    return nextRecord;
}