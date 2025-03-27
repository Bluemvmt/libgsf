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
    printf("name = %s\n", name);
    if (array != NULL) {
        for (int i = 0; i < length; i++) {
            printf("%s ", array[i]);
        }
        printf("\n");
    }
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
    // unsigned short    *sector_number;      /* Array of values that specify the transit sector for this beam */
    // unsigned short    *detection_info;     /* Array of values that specify the method of bottom detection */
    add_double_array(json, "incident_beam_adj", ping.incident_beam_adj, ping.number_beams);
    // unsigned short    *system_cleaning;    /* Array of values that specify data cleaning information from the sensor system */
    add_double_array(json, "doppler_corr", ping.doppler_corr, ping.number_beams);
    add_double_array(json, "sonar_vert_uncert", ping.sonar_vert_uncert, ping.number_beams);
    add_double_array(json, "sonar_horz_uncert", ping.sonar_horz_uncert, ping.number_beams);
    add_double_array(json, "detection_window", ping.detection_window, ping.number_beams);
    add_double_array(json, "mean_abs_coeff", ping.mean_abs_coeff, ping.number_beams);
    add_double_array(json, "TVG_dB", ping.TVG_dB, ping.number_beams);
    cJSON_AddNumberToObject(json, "sensor_id", ping.sensor_id);
    // gsfSensorSpecific  sensor_data;        /* union of known sensor specific data */
    // gsfBRBIntensity   *brb_inten;          /* Structure containing bathymetric receive beam time series intensities */
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
    }
    if (dataID.recordID == GSF_RECORD_HEADER) {
    }
    return NULL;
}