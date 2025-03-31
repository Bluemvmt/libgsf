#include <stdlib.h>
#include <cjson/cJSON.h>
#include "gsf_json.h"
#include "gsf.h"


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

cJSON *gsfAttitude_toJson(struct t_gsfAttitude attitude) {
    cJSON *json = cJSON_CreateObject();
    return json;
}

cJSON *gsfSwathBathySummary_toJson(struct t_gsfSwathBathySummary summary) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "start_time", epoch_double(summary.start_time));
    cJSON_AddNumberToObject(json, "end_time", epoch_double(summary.end_time));
    cJSON_AddNumberToObject(json, "min_latitude", summary.min_latitude);
    cJSON_AddNumberToObject(json, "min_longitude", summary.min_longitude);
    cJSON_AddNumberToObject(json, "max_latitude", summary.max_latitude);
    cJSON_AddNumberToObject(json, "max_longitude", summary.max_longitude);
    cJSON_AddNumberToObject(json, "max_depth", summary.min_depth);
    cJSON_AddNumberToObject(json, "max_depth", summary.max_depth);
    return json;
}

void gsfBRBIntensity_toJson(cJSON *json, gsfBRBIntensity *brb) {
    if (brb != NULL) {
        cJSON *brb_json = cJSON_AddObjectToObject(json, "brb_inten");
        cJSON_AddNumberToObject(brb_json, "bits_per_sample", (int) brb->bits_per_sample);
        cJSON_AddNumberToObject(brb_json, "applied_corrections", brb->applied_corrections);
        char *json_str = cJSON_PrintUnformatted(brb_json); 
        printf("BRB = %s\n", json_str);
    }
}
void gsfSBAmpSpecific_toJson(cJSON *json, t_gsfSBAmpSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfSBAmpSpecific");
    cJSON_AddNumberToObject(sensor_json, "hour", (int) sensor_data.hour);
    cJSON_AddNumberToObject(sensor_json, "minute", (int) sensor_data.minute);
    cJSON_AddNumberToObject(sensor_json, "second", (int) sensor_data.second);
    cJSON_AddNumberToObject(sensor_json, "hundreths", (int) sensor_data.hundredths);
    cJSON_AddNumberToObject(sensor_json, "block_number", (int) sensor_data.block_number);
    cJSON_AddNumberToObject(sensor_json, "avg_gate_depth", (int) sensor_data.avg_gate_depth);
}

void gsfEchotracSpecific_toJson(cJSON *json, t_gsfEchotracSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfEchotracSpecific");
    cJSON_AddNumberToObject(sensor_json, "navigation_error", (int) sensor_data.navigation_error);
    cJSON_AddNumberToObject(sensor_json, "mpp_source", (int) sensor_data.mpp_source);
    cJSON_AddNumberToObject(sensor_json, "tide_source", (int) sensor_data.tide_source);
}

void gsfSeaBeamSpecific_toJson(cJSON *json, t_gsfSeaBeamSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfSeaBeamSpecific");
    cJSON_AddNumberToObject(sensor_json, "EclipseTime", (int) sensor_data.EclipseTime);
}

void gsfEM100Specific_toJson(cJSON *json, t_gsfEM100Specific sensor_data) {
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

void gsfSBSensorSpecific_toJson(cJSON *json, gsfSBSensorSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "gsfSBSensorSpecific");
    gsfEchotracSpecific_toJson(sensor_json, sensor_data.gsfEchotracSpecific);
    // t_gsfEchotracSpecific    gsfBathy2000Specific;
    // t_gsfMGD77Specific       gsfMGD77Specific;
    // t_gsfBDBSpecific         gsfBDBSpecific;
    // t_gsfNOSHDBSpecific      gsfNOSHDBSpecific;
}

void gsfSensorSpecific_toJson(cJSON *json, gsfSensorSpecific sensor_data) {
    cJSON *sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
    gsfSBAmpSpecific_toJson(sensor_json, sensor_data.gsfSBAmpSpecific);
}

cJSON *gsfSingleBeamPing_toJson(struct t_gsfSingleBeamPing ping) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "ping_time", epoch_double(ping.ping_time));
    cJSON_AddNumberToObject(json, "latitude", ping.latitude);
    cJSON_AddNumberToObject(json, "longitude", ping.longitude);
    cJSON_AddNumberToObject(json, "tide_corrector", ping.tide_corrector);
    cJSON_AddNumberToObject(json, "depth_corrector", ping.depth_corrector);
    cJSON_AddNumberToObject(json, "heading", ping.heading);
    cJSON_AddNumberToObject(json, "pitch", ping.pitch);
    cJSON_AddNumberToObject(json, "roll", ping.roll);
    cJSON_AddNumberToObject(json, "heave", ping.heave);
    cJSON_AddNumberToObject(json, "depth", ping.depth);
    cJSON_AddNumberToObject(json, "sound_speed_correction", ping.sound_speed_correction);
    cJSON_AddNumberToObject(json, "positioning_system_type", ping.positioning_system_type);
    cJSON_AddNumberToObject(json, "sensor_id", ping.sensor_id);
    gsfSBSensorSpecific_toJson(json, ping.sensor_data);
    char *json_str = cJSON_PrintUnformatted(json); 
    printf("ping = %s\n", json_str);
    return json;
}

cJSON *gsfSwathBathyPing_toJson(struct t_gsfSwathBathyPing ping) {
    cJSON *json = cJSON_CreateObject();

    cJSON_AddNumberToObject(json, "ping_time", epoch_double(ping.ping_time));
    cJSON_AddNumberToObject(json, "latitude", ping.latitude);
    cJSON_AddNumberToObject(json, "longitude", ping.longitude);
    cJSON_AddNumberToObject(json, "height", ping.height);  
    cJSON_AddNumberToObject(json, "sep", ping.sep);
    cJSON_AddNumberToObject(json, "number_beams", ping.number_beams);
    cJSON_AddNumberToObject(json, "center_beam", ping.center_beam);
    cJSON_AddNumberToObject(json, "ping_flags", ping.ping_flags);
    cJSON_AddNumberToObject(json, "reserved", ping.reserved);  
    cJSON_AddNumberToObject(json, "tide_corrector", ping.tide_corrector);
    cJSON_AddNumberToObject(json, "gps_tide_corrector", ping.gps_tide_corrector);
    cJSON_AddNumberToObject(json, "depth_corrector", ping.depth_corrector);
    cJSON_AddNumberToObject(json, "heading", ping.heading);
    cJSON_AddNumberToObject(json, "pitch", ping.pitch);
    cJSON_AddNumberToObject(json, "roll", ping.roll);
    cJSON_AddNumberToObject(json, "heave", ping.heave);
    cJSON_AddNumberToObject(json, "course", ping.course);
    cJSON_AddNumberToObject(json, "speed", ping.speed);
    add_double_array(json, "depth", ping.depth, ping.number_beams);
    add_double_array(json, "nominal_depth", ping.nominal_depth, ping.number_beams);
    add_double_array(json, "across_track", ping.across_track, ping.number_beams);
    add_double_array(json, "travel_time", ping.travel_time, ping.number_beams);
    add_double_array(json, "beam_angle", ping.beam_angle, ping.number_beams);
    add_double_array(json, "mc_amplitude", ping.mc_amplitude, ping.number_beams);
    add_double_array(json, "mr_amplitude", ping.mr_amplitude, ping.number_beams);
    add_double_array(json, "echo_width", ping.echo_width, ping.number_beams);
    add_double_array(json, "quality_factor", ping.quality_factor, ping.number_beams);
    add_double_array(json, "receive_heave", ping.receive_heave, ping.number_beams);
    add_double_array(json, "depth_error", ping.depth_error, ping.number_beams);
    add_double_array(json, "across_track_error", ping.across_track, ping.number_beams);
    add_double_array(json, "along_track_error", ping.along_track_error, ping.number_beams);
    add_char_array(json, "quality_flags", ping.quality_flags, ping.number_beams);
    add_char_array(json, "beam_flags", ping.beam_flags, ping.number_beams);
    add_double_array(json, "signal_to_noise", ping.signal_to_noise, ping.number_beams);
    add_double_array(json, "beam_angle_forward", ping.beam_angle_forward, ping.number_beams);
    add_double_array(json, "vertical_error", ping.vertical_error, ping.number_beams);
    add_double_array(json, "horizontal_error", ping.horizontal_error, ping.number_beams);
    add_short_array(json, "sector_number", ping.sector_number, ping.number_beams);
    // unsigned short    *detection_info;     /* Array of values that specify the method of bottom detection */
    add_double_array(json, "incident_beam_adj", ping.incident_beam_adj, ping.number_beams);
    // unsigned short    *system_cleaning;    /* Array of values that specify data cleaning information from the sensor system */
    add_double_array(json, "doppler_corr", ping.doppler_corr, ping.number_beams);
    add_double_array(json, "sonar_vert_uncert", ping.sonar_vert_uncert, ping.number_beams);
    add_double_array(json, "sonar_horz_uncert", ping.sonar_horz_uncert, ping.number_beams);
    add_double_array(json, "detection_window", ping.detection_window, ping.number_beams);
    add_double_array(json, "mean_abs_coeff", ping.mean_abs_coeff, ping.number_beams);
    cJSON_AddNumberToObject(json, "sensor_id", ping.sensor_id);
    gsfBRBIntensity_toJson(json, ping.brb_inten);
    gsfSensorSpecific_toJson(json, ping.sensor_data);
    // gsfSensorSpecific  sensor_data;        /* union of known sensor specific data */
    // gsfBRBIntensity   *brb_inten;          /* Structure containing bathymetric receive beam time series intensities */
    // char *json_str = cJSON_PrintUnformatted(json); 
    // printf("ping = %s\n", json_str);
    return json;
}

cJSON *gsfHeader_toJson(struct t_gsfHeader header) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "header", header.version);
    return json;
}

cJSON *gsfRecord_toJson(gsfDataID dataID, gsfRecords record) {
    switch (dataID.recordID) {
        case GSF_RECORD_HEADER:
            return gsfHeader_toJson(record.header);
        case GSF_RECORD_SWATH_BATHY_SUMMARY:
            return gsfSwathBathySummary_toJson(record.summary);
        case GSF_RECORD_SWATH_BATHYMETRY_PING:
            return gsfSwathBathyPing_toJson(record.mb_ping);
        case GSF_RECORD_SINGLE_BEAM_PING:
            return gsfSingleBeamPing_toJson(record.sb_ping);
    }
    return NULL;
}