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

cJSON *gsfComment_flatten(struct t_gsfComment comment, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_COMMENT);
    double timestamp = epoch_double(comment.comment_time);
    cJSON_AddNumberToObject(json, "timestamp", timestamp);
    cJSON_AddStringToObject(json, "gsf_version", gsfJsonFileInfo.gsf_version);
    cJSON_AddStringToObject(json, "file_name", gsfJsonFileInfo.file_name);
    cJSON_AddNumberToObject(json, "comment_length", comment.comment_length);
    cJSON_AddStringToObject(json, "comment", comment.comment);

    return json;
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

cJSON *gsfHistory_flatten(struct t_gsfHistory history, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_HISTORY);
    cJSON_AddStringToObject(json, "gsf_version", gsfJsonFileInfo.gsf_version);
    cJSON_AddStringToObject(json, "file_name", gsfJsonFileInfo.file_name);
    double timestamp = epoch_double(history.history_time);
    cJSON_AddNumberToObject(json, "timestamp", timestamp);
    cJSON_AddNumberToObject(json, "history_time", timestamp);
    cJSON_AddStringToObject(json, "history_host_name", history.host_name);
    cJSON_AddStringToObject(json, "history_operator_name", history.operator_name);
    cJSON_AddStringToObject(json, "history_command_line", history.command_line);
    cJSON_AddStringToObject(json, "history_comment", history.comment);

    return json;
}

cJSON *gsfHistory_toJson(struct t_gsfHistory history, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_HISTORY);
    double epoch_time = epoch_double(history.history_time);
    if (gsfJsonFileInfo.include_denormalized_fields) {
        cJSON_AddStringToObject(json, "gsf_version", gsfJsonFileInfo.gsf_version);
        cJSON_AddStringToObject(json, "file_name", gsfJsonFileInfo.file_name);
        cJSON_AddNumberToObject(json, "timestamp", epoch_time);
    }
    cJSON *body_json = cJSON_AddObjectToObject(json, "json_record");
    cJSON_AddNumberToObject(body_json, "history_time", epoch_time);
    cJSON_AddStringToObject(body_json, "host_name", history.host_name);
    cJSON_AddStringToObject(body_json, "operator_name", history.operator_name);
    cJSON_AddStringToObject(body_json, "command_line", history.command_line);
    cJSON_AddStringToObject(body_json, "comment", history.comment);
    return json;
}

static cJSON *gsfAttitude_flatten(struct t_gsfAttitude attitude, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_ATTITUDE);
    double timestamp = epoch_double(*attitude.attitude_time);
    cJSON_AddNumberToObject(json, "timestamp", timestamp);
    cJSON_AddNumberToObject(json, "attitude_time", timestamp);
    cJSON_AddNumberToObject(json, "attitude_num_measurements", attitude.num_measurements);
    cJSON_AddNumberToObject(json, "attitude_pitch", *attitude.pitch);
    cJSON_AddNumberToObject(json, "attitude_roll", *attitude.roll);
    cJSON_AddNumberToObject(json, "attitude_heave", *attitude.heave);
    cJSON_AddNumberToObject(json, "attitude_heading", *attitude.heading);
    
    return json;
}

static cJSON *gsfAttitude_toJson(struct t_gsfAttitude attitude, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_ATTITUDE);
    double epoch_time = epoch_double(*attitude.attitude_time);
    if (gsfJsonFileInfo.include_denormalized_fields) {
        cJSON_AddNumberToObject(json, "timestamp", epoch_time);
    }
    cJSON *body_json = cJSON_AddObjectToObject(json, "json_record");
    cJSON_AddNumberToObject(body_json, "attitude_time", epoch_time);
    cJSON_AddNumberToObject(body_json, "num_measurements", attitude.num_measurements);
    cJSON_AddNumberToObject(body_json, "pitch", *attitude.pitch);
    cJSON_AddNumberToObject(body_json, "roll", *attitude.roll);
    cJSON_AddNumberToObject(body_json, "heave", *attitude.heave);
    cJSON_AddNumberToObject(body_json, "heading", *attitude.heading);

    return json;
}


static cJSON *gsfSwathBathySummary_flatten(struct t_gsfSwathBathySummary summary, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();   
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_SWATH_BATHY_SUMMARY);

    cJSON_AddStringToObject(json, "gsf_version", gsfJsonFileInfo.gsf_version);
    cJSON_AddStringToObject(json, "file_name", gsfJsonFileInfo.file_name);
    cJSON_AddNumberToObject(json, "latitude", summary.min_latitude);
    cJSON_AddNumberToObject(json, "longitude", summary.min_longitude);
    double timestamp = epoch_double(summary.start_time);
    cJSON_AddNumberToObject(json, "timestamp", timestamp);
    cJSON_AddNumberToObject(json, "summary_start_time", timestamp);
    cJSON_AddNumberToObject(json, "summary_end_time", epoch_double(summary.end_time));
    cJSON_AddNumberToObject(json, "summary_min_latitude", summary.min_latitude);
    cJSON_AddNumberToObject(json, "summary_min_longitude", summary.min_longitude);
    cJSON_AddNumberToObject(json, "summary_max_latitude", summary.max_latitude);
    cJSON_AddNumberToObject(json, "summary_max_longitude", summary.max_longitude);
    cJSON_AddNumberToObject(json, "summary_min_depth", summary.min_depth);
    cJSON_AddNumberToObject(json, "summary_max_depth", summary.max_depth);
    return json;
}

static cJSON *gsfSwathBathySummary_toJson(struct t_gsfSwathBathySummary summary, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();   
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_SWATH_BATHY_SUMMARY);

    if (gsfJsonFileInfo.include_denormalized_fields) {
        cJSON_AddStringToObject(json, "gsf_version", gsfJsonFileInfo.gsf_version);
        cJSON_AddStringToObject(json, "file_name", gsfJsonFileInfo.file_name);
        cJSON_AddNumberToObject(json, "timestamp", epoch_double(summary.start_time));
        cJSON_AddNumberToObject(json, "latitude", summary.min_latitude);
        cJSON_AddNumberToObject(json, "longitude", summary.min_longitude);
    }
    cJSON *body_json = cJSON_AddObjectToObject(json, "json_record");
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

static void gsfEMRunTime_toJson(cJSON *sensor_json, t_gsfEMRunTime runtime) {
    cJSON_AddNumberToObject(sensor_json, "model_number", runtime.model_number);
    cJSON_AddNumberToObject(sensor_json, "dg_time", epoch_double(runtime.dg_time));
    cJSON_AddNumberToObject(sensor_json, "ping_counter", runtime.ping_counter);
    cJSON_AddNumberToObject(sensor_json, "serial_number", runtime.serial_number);
    cJSON_AddNumberToObject(sensor_json, "operator_station_status", runtime.operator_station_status);
    cJSON_AddNumberToObject(sensor_json, "processing_unit_status", runtime.processing_unit_status);
    cJSON_AddNumberToObject(sensor_json, "bsp_status", runtime.bsp_status);
    cJSON_AddNumberToObject(sensor_json, "head_transceiver_status", runtime.head_transceiver_status);
    cJSON_AddNumberToObject(sensor_json, "mode", runtime.mode);
    cJSON_AddNumberToObject(sensor_json, "filter_id", runtime.filter_id);
    cJSON_AddNumberToObject(sensor_json, "min_depth", runtime.min_depth);
    cJSON_AddNumberToObject(sensor_json, "max_depth", runtime.max_depth);
    cJSON_AddNumberToObject(sensor_json, "absorption", runtime.absorption);
    cJSON_AddNumberToObject(sensor_json, "tx_pulse_length", runtime.tx_pulse_length);
    cJSON_AddNumberToObject(sensor_json, "tx_beam_width", runtime.tx_beam_width);
    cJSON_AddNumberToObject(sensor_json, "tx_power_re_max", runtime.tx_power_re_max);
    cJSON_AddNumberToObject(sensor_json, "rx_beam_width", runtime.rx_beam_width);
    cJSON_AddNumberToObject(sensor_json, "rx_bandwidth", runtime.rx_bandwidth);
    cJSON_AddNumberToObject(sensor_json, "rx_fixed_gain", runtime.rx_fixed_gain);
    cJSON_AddNumberToObject(sensor_json, "tvg_cross_over_angle", runtime.tvg_cross_over_angle);
    switch (runtime.ssv_source) {
        case 0:
            cJSON_AddStringToObject(sensor_json, "ssv_source", "sensor");
            break;
        case 1:
            cJSON_AddStringToObject(sensor_json, "ssv_source", "manual");
            break;
        case 2:
            cJSON_AddStringToObject(sensor_json, "ssv_source", "profile");
            break;
    }
    cJSON_AddNumberToObject(sensor_json, "max_port_swath_width", runtime.max_port_swath_width);
    switch (runtime.beam_spacing) {
        case 0:
            cJSON_AddStringToObject(sensor_json, "beam_spacing", "beamwidth");
            break;
        case 1:
            cJSON_AddStringToObject(sensor_json, "beam_spacing", "equiangle");
            break;
        case 2:
            cJSON_AddStringToObject(sensor_json, "beam_spacing", "equidistant");
            break;
        case 3:
            cJSON_AddStringToObject(sensor_json, "beam_spacing", "intermediate");
            break;
    }
    cJSON_AddNumberToObject(sensor_json, "max_port_coverage", runtime.max_port_coverage);
    cJSON_AddNumberToObject(sensor_json, "stabilization", runtime.stabilization);
    cJSON_AddNumberToObject(sensor_json, "max_stbd_coverage", runtime.max_stbd_coverage);
    cJSON_AddNumberToObject(sensor_json, "max_stbd_swath_width", runtime.max_stbd_swath_width);
    cJSON_AddNumberToObject(sensor_json, "durotong_speed", runtime.durotong_speed);
    cJSON_AddNumberToObject(sensor_json, "hi_low_absorption_ratio", runtime.hi_low_absorption_ratio);
    cJSON_AddNumberToObject(sensor_json, "tx_along_tilt", runtime.tx_along_tilt);
    cJSON_AddNumberToObject(sensor_json, "filter_id_2", runtime.filter_id_2);
}

static cJSON *gsfEM3Runtime_toJson(struct t_gsfEM3RunTime runtime) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "model_number", runtime.model_number);
    cJSON_AddNumberToObject(json, "dg_tome", epoch_double(runtime.dg_time));
    cJSON_AddNumberToObject(json, "ping_number", runtime.ping_number);
    cJSON_AddNumberToObject(json, "serial_number", runtime.serial_number);
    cJSON_AddNumberToObject(json, "system_status", runtime.system_status);
    switch (runtime.mode) {
        case 0:
            cJSON_AddStringToObject(json, "mode", "nearfield");
            break;
        case 1:
            cJSON_AddStringToObject(json, "mode", "normal");
            break;
        case 2:
            cJSON_AddStringToObject(json, "mode", "target");
            break;
        case 3:
            cJSON_AddStringToObject(json, "mode", "deep");
            break;
        case 4:
            cJSON_AddStringToObject(json, "mode", "very deep");
            break;
    }   
    cJSON_AddNumberToObject(json, "filter_id", runtime.filter_id);
    cJSON_AddNumberToObject(json, "min_depth", runtime.min_depth);
    cJSON_AddNumberToObject(json, "max_depth", runtime.max_depth);
    cJSON_AddNumberToObject(json, "absorption", runtime.absorption);
    cJSON_AddNumberToObject(json, "pulse_length", runtime.pulse_length);
    cJSON_AddNumberToObject(json, "transmit_beam_width", runtime.transmit_beam_width);
    cJSON_AddNumberToObject(json, "power_reduction", runtime.power_reduction);
    cJSON_AddNumberToObject(json, "receive_beam_width", runtime.receive_beam_width);
    cJSON_AddNumberToObject(json, "receive_bandwidth", runtime.receive_bandwidth);
    cJSON_AddNumberToObject(json, "receive_gain", runtime.receive_gain);
    cJSON_AddNumberToObject(json, "cross_over_angle", runtime.cross_over_angle);
    switch (runtime.ssv_source) {
        case 0:
            cJSON_AddStringToObject(json, "ssv_source", "sensor");
            break;
        case 1:
            cJSON_AddStringToObject(json, "ssv_source", "manual");
            break;
        case 2:
            cJSON_AddStringToObject(json, "ssv_source", "profile");
            break;
    }
    cJSON_AddNumberToObject(json, "swath_width", runtime.swath_width);
    switch (runtime.beam_spacing) {
        case 0:
            cJSON_AddStringToObject(json, "beam_spacing", "beamwidth");
            break;
        case 1:
            cJSON_AddStringToObject(json, "beam_spacing", "equiangle");
            break;
        case 2:
            cJSON_AddStringToObject(json, "beam_spacing", "equidistant");
            break;
        case 3:
            cJSON_AddStringToObject(json, "beam_spacing", "intermediate");
            break;
    }
    cJSON_AddNumberToObject(json, "coverage_sector", runtime.coverage_sector);
    cJSON_AddNumberToObject(json, "stabilization", runtime.stabilization);
    cJSON_AddNumberToObject(json, "port_swath_width", runtime.port_swath_width);
    cJSON_AddNumberToObject(json, "stbd_swath_width", runtime.stbd_swath_width);
    cJSON_AddNumberToObject(json, "port_coverage_sector", runtime.port_coverage_sector);
    cJSON_AddNumberToObject(json, "stbd_coverage_sector", runtime.stbd_coverage_sector);
    cJSON_AddNumberToObject(json, "hilo_freq_absorp_ratio", runtime.hilo_freq_absorp_ratio);
    return json;
}

static void gsfEM3Specific_toJson(cJSON *sensor_json, t_gsfEM3Specific sensor_data) {
    cJSON_AddNumberToObject(sensor_json, "model_number", sensor_data.model_number);
    cJSON_AddNumberToObject(sensor_json, "ping_number", sensor_data.ping_number);
    cJSON_AddNumberToObject(sensor_json, "serial_number", sensor_data.serial_number);
    cJSON_AddNumberToObject(sensor_json, "surface_velocity", sensor_data.surface_velocity);
    cJSON_AddNumberToObject(sensor_json, "transducer_depth", sensor_data.transducer_depth);
    cJSON_AddNumberToObject(sensor_json, "valid_beams", sensor_data.valid_beams);
    cJSON_AddNumberToObject(sensor_json, "sample_rate", sensor_data.sample_rate);
    cJSON_AddNumberToObject(sensor_json, "depth_difference", sensor_data.depth_difference);
    cJSON_AddNumberToObject(sensor_json, "offset_multiplier", sensor_data.offset_multiplier);
    cJSON *gsfEM3RuntimeArray = cJSON_AddArrayToObject(sensor_json, "gsfEM3RunTime");
    cJSON_AddItemToArray(gsfEM3RuntimeArray, gsfEM3Runtime_toJson(sensor_data.run_time[0]));
    cJSON_AddItemToArray(gsfEM3RuntimeArray, gsfEM3Runtime_toJson(sensor_data.run_time[1]));
}

static void gsfEM121ASpecific_toJson(cJSON *sensor_json, t_gsfEM121ASpecific sensor_data) {
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

static void gsfEM950Specific_toJson(cJSON *sensor_json, t_gsfEM950Specific sensor_data) {
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

static cJSON *gsfEM4TxSector_toJson(t_gsfEM4TxSector sector) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "tilt_angle", sector.tilt_angle);
    cJSON_AddNumberToObject(json, "focus_range", sector.focus_range);
    cJSON_AddNumberToObject(json, "signal_length", sector.signal_length);
    cJSON_AddNumberToObject(json, "transmit_delay", sector.transmit_delay);
    cJSON_AddNumberToObject(json, "center_frequency", sector.center_frequency);
    cJSON_AddNumberToObject(json, "mean_absorption", sector.mean_absorption);
    switch (sector.waveform_id) {
        case 0:
            cJSON_AddStringToObject(json, "waveform_id", "CW");
            break;
        case 1:
            cJSON_AddStringToObject(json, "waveform_id", "FM upsweep");
            break;
        case 2:
            cJSON_AddStringToObject(json, "waveform_id", "FM downsweep");
            break;
    }
    cJSON_AddNumberToObject(json, "sector_number", sector.sector_number);
    cJSON_AddNumberToObject(json, "signal_bandwidth", sector.signal_bandwidth);
    return json;
}

static void gsfEMPUStatus_toJson(cJSON *status_json, t_gsfEMPUStatus status) {
    cJSON_AddNumberToObject(status_json, "pu_cpu_load", status.pu_cpu_load);
    cJSON_AddNumberToObject(status_json, "sensor_status", status.sensor_status);
    cJSON_AddNumberToObject(status_json, "achieved_port_coverage", status.achieved_port_coverage);
    cJSON_AddNumberToObject(status_json, "achieved_stbd_coverage", status.achieved_stbd_coverage);
    cJSON_AddNumberToObject(status_json, "yaw_stabilization", status.yaw_stabilization);
}

static void gsfEM4Specific_toJson(cJSON *sensor_json, t_gsfEM4Specific sensor_data) {
    /* values from the XYZ datagram and raw range datagram */
    cJSON_AddNumberToObject(sensor_json, "model_number", sensor_data.model_number);
    cJSON_AddNumberToObject(sensor_json, "ping_counter", sensor_data.ping_counter);
    cJSON_AddNumberToObject(sensor_json, "serial_number", sensor_data.serial_number);
    cJSON_AddNumberToObject(sensor_json, "surface_velocity", sensor_data.surface_velocity);
    cJSON_AddNumberToObject(sensor_json, "transducer_depth", sensor_data.transducer_depth);
    cJSON_AddNumberToObject(sensor_json, "valid_detections", sensor_data.valid_detections);
    cJSON_AddNumberToObject(sensor_json, "sampling_frequency", sensor_data.sampling_frequency);
    cJSON_AddNumberToObject(sensor_json, "doppler_corr_scale", sensor_data.doppler_corr_scale);
    cJSON_AddNumberToObject(sensor_json, "vehicle_depth", sensor_data.vehicle_depth);
    cJSON_AddNumberToObject(sensor_json, "transmit_sectors", sensor_data.transmit_sectors);
    cJSON *gsfEM4TxSectorArray = cJSON_AddArrayToObject(sensor_json, "sector");
    for (int i = 0; i < GSF_MAX_EM4_SECTORS; i++) {
        cJSON_AddItemToArray(gsfEM4TxSectorArray, gsfEM4TxSector_toJson(sensor_data.sector[i]));
    }
    cJSON *runtime_json = cJSON_AddObjectToObject(sensor_json, "run_time");
    gsfEMRunTime_toJson(runtime_json, sensor_data.run_time);
    cJSON *status_json = cJSON_AddObjectToObject(sensor_json, "pu_status");
    gsfEMPUStatus_toJson(status_json, sensor_data.pu_status);
}

static void gsfCmpSassSpecific_toJson(cJSON *sensor_json, t_gsfCmpSassSpecific sensor_data) {
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

static void gsfSensorSpecific_toJson(cJSON *json, int sensor_id, gsfSwathBathyPing ping) {
    cJSON *sensor_json = NULL;

    gsfSensorSpecific sensor_data = ping.sensor_data;
    switch (sensor_id) {
        case (GSF_SWATH_BATHY_SUBRECORD_UNKNOWN):
            cJSON_AddStringToObject(json, "sensor_name", gsfGetSonarTextName(&ping));
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_SEABEAM_SPECIFIC):
            sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
            gsfSeaBeamSpecific_toJson(sensor_json, sensor_data.gsfSeaBeamSpecific);
            cJSON_AddStringToObject(json, "sensor_name", gsfGetSonarTextName(&ping));
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_EM12_SPECIFIC):
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_EM100_SPECIFIC):
            sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
            gsfEM100Specific_toJson(sensor_json, sensor_data.gsfEM100Specific);
            cJSON_AddStringToObject(json, "sensor_name", gsfGetSonarTextName(&ping));
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_EM950_SPECIFIC):
            sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
            gsfEM950Specific_toJson(sensor_json, sensor_data.gsfEM950Specific);
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_EM121A_SPECIFIC):
            sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
            gsfEM121ASpecific_toJson(sensor_json, sensor_data.gsfEM121ASpecific);
            cJSON_AddStringToObject(json, "sensor_name", gsfGetSonarTextName(&ping));
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_EM121_SPECIFIC):
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_SASS_SPECIFIC):
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_SEAMAP_SPECIFIC):
            sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
            gsfSeamapSpecific_toJson(sensor_json, sensor_data.gsfSeamapSpecific);
            cJSON_AddStringToObject(json, "sensor_name", gsfGetSonarTextName(&ping));
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_SEABAT_SPECIFIC):
            sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
            gsfSeaBatSpecific_toJson(sensor_json, sensor_data.gsfSeaBatSpecific);
            cJSON_AddStringToObject(json, "sensor_name", gsfGetSonarTextName(&ping));
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_EM1000_SPECIFIC):
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_TYPEIII_SEABEAM_SPECIFIC):
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_SB_AMP_SPECIFIC):
            sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
            gsfSBAmpSpecific_toJson(sensor_json, sensor_data.gsfSBAmpSpecific);
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_SEABAT_II_SPECIFIC):
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_SEABAT_8101_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_SEABEAM_2112_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_ELAC_MKII_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_CMP_SASS_SPECIFIC):
            sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
            gsfCmpSassSpecific_toJson(sensor_json, sensor_data.gsfCmpSassSpecific);
            cJSON_AddStringToObject(json, "sensor_name", gsfGetSonarTextName(&ping));
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_EM2000_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM3000_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM1002_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM300_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM120_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM3002_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM3002D_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM121A_SIS_SPECIFIC):
            sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
            gsfEM3Specific_toJson(sensor_json, sensor_data.gsfEM3Specific);
            cJSON_AddStringToObject(json, "sensor_name", gsfGetSonarTextName(&ping));
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_EM2000_RAW_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM3000_RAW_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM1002_RAW_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM300_RAW_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM120_RAW_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM3002_RAW_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM3000D_RAW_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM3002D_RAW_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM121A_SIS_RAW_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;


        case (GSF_SWATH_BATHY_SUBRECORD_RESON_8101_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_RESON_8111_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_RESON_8124_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_RESON_8125_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_RESON_8150_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_RESON_8160_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_RESON_7125_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_RESON_TSERIES_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_EM710_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM302_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM122_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_EM2040_SPECIFIC):
            sensor_json = cJSON_AddObjectToObject(json, "sensor_data");
            gsfEM4Specific_toJson(sensor_json, ping.sensor_data.gsfEM4Specific);
            cJSON_AddStringToObject(json, "sensor_name", gsfGetSonarTextName(&ping));
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_GEOSWATH_PLUS_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_KLEIN_5410_BSS_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_DELTA_T_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_R2SONIC_2020_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_R2SONIC_2022_SPECIFIC):
        case (GSF_SWATH_BATHY_SUBRECORD_R2SONIC_2024_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SB_SUBRECORD_ECHOTRAC_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SB_SUBRECORD_BATHY2000_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SB_SUBRECORD_MGD77_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SB_SUBRECORD_BDB_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SB_SUBRECORD_NOSHDB_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SB_SUBRECORD_PDD_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SB_SUBRECORD_NAVISOUND_SPECIFIC):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;

        case (GSF_SWATH_BATHY_SUBRECORD_INTENSITY_SERIES_ARRAY):
            cJSON_AddStringToObject(json, "sensor_name", "UNIMPLEMENTED");
            break;
    }
}

static cJSON *gsfSingleBeamPing_flatten(struct t_gsfSingleBeamPing ping) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_SINGLE_BEAM_PING);
    double timestamp = epoch_double(ping.ping_time);
    cJSON_AddNumberToObject(json, "timestamp", timestamp);
   cJSON_AddNumberToObject(json, "latitude", ping.latitude);
    cJSON_AddNumberToObject(json, "longitude", ping.longitude);
    cJSON_AddNumberToObject(json, "sb_tide_corrector", ping.tide_corrector);
    cJSON_AddNumberToObject(json, "sb_depth_corrector", ping.depth_corrector);
    cJSON_AddNumberToObject(json, "sb_heading", ping.heading);
    cJSON_AddNumberToObject(json, "sb_pitch", ping.pitch);
    cJSON_AddNumberToObject(json, "sb_roll", ping.roll);
    cJSON_AddNumberToObject(json, "sb_heave", ping.heave);
    cJSON_AddNumberToObject(json, "sb_depth", ping.depth);
    cJSON_AddNumberToObject(json, "sb_sound_speed_correction", ping.sound_speed_correction);
    cJSON_AddNumberToObject(json, "sb_positioning_system_type", ping.positioning_system_type);
    cJSON_AddNumberToObject(json, "sensor_id", ping.sensor_id);
    gsfSBSensorSpecific_toJson(json,ping.sensor_data);
    return json;
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

static cJSON *gsfSwathBathyPing_flatten(struct t_gsfSwathBathyPing ping, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json = cJSON_CreateObject();

    cJSON_AddStringToObject(json, "gsf_version", gsfJsonFileInfo.gsf_version);
    cJSON_AddStringToObject(json, "file_name", gsfJsonFileInfo.file_name);
    double timestamp = epoch_double(ping.ping_time);
    cJSON_AddNumberToObject(json, "timestamp", timestamp);
    cJSON_AddNumberToObject(json, "latitude", ping.latitude);
    cJSON_AddNumberToObject(json, "longitude", ping.longitude);
    cJSON_AddNumberToObject(json, "record_type", GSF_RECORD_SWATH_BATHYMETRY_PING);
    cJSON_AddNumberToObject(json, "height", ping.height);
    cJSON_AddNumberToObject(json, "sep", ping.sep);
    cJSON_AddNumberToObject(json, "mb_number_beams", ping.number_beams);
    cJSON_AddNumberToObject(json, "mb_center_beam", ping.center_beam);
    cJSON_AddNumberToObject(json, "mb_ping_flags", ping.ping_flags);
    cJSON_AddNumberToObject(json, "mb_tide_corrector", ping.tide_corrector);
    cJSON_AddNumberToObject(json, "mb_gps_tide_corrector", ping.gps_tide_corrector);
    cJSON_AddNumberToObject(json, "mb_depth_corrector", ping.depth_corrector);
    cJSON_AddNumberToObject(json, "mb_heading", ping.heading);
    cJSON_AddNumberToObject(json, "mb_pitch", ping.pitch);
    cJSON_AddNumberToObject(json, "mb_roll", ping.roll);
    cJSON_AddNumberToObject(json, "mb_heave", ping.heave);
    cJSON_AddNumberToObject(json, "mb_course", ping.course);
    cJSON_AddNumberToObject(json, "mb_speed", ping.speed);
    add_double_array(json, "mb_depth", ping.depth, ping.number_beams);
    add_double_array(json, "mb_nominal_depth", ping.nominal_depth, ping.number_beams);
    add_double_array(json, "mb_across_track", ping.across_track, ping.number_beams);
    add_double_array(json, "mb_travel_time", ping.travel_time, ping.number_beams);
    add_double_array(json, "mb_beam_angle", ping.beam_angle, ping.number_beams);
    add_double_array(json, "mb_mc_amplitude", ping.mc_amplitude, ping.number_beams);
    add_double_array(json, "mb_mr_amplitude", ping.mr_amplitude, ping.number_beams);
    add_double_array(json, "mb_echo_width", ping.echo_width, ping.number_beams);
    add_double_array(json, "mb_quality_factor", ping.quality_factor, ping.number_beams);
    add_double_array(json, "mb_receive_heave", ping.receive_heave, ping.number_beams);
    add_double_array(json, "mb_depth_error", ping.depth_error, ping.number_beams);
    add_double_array(json, "mb_across_track_error", ping.across_track, ping.number_beams);
    add_double_array(json, "mb_along_track_error", ping.along_track_error, ping.number_beams);
    add_char_array(json, "mb_uality_flags", ping.quality_flags, ping.number_beams);
    add_char_array(json, "mb_beam_flags", ping.beam_flags, ping.number_beams);
    add_double_array(json, "mb_signal_to_noise", ping.signal_to_noise, ping.number_beams);
    add_double_array(json, "mb_beam_angle_forward", ping.beam_angle_forward, ping.number_beams);
    add_double_array(json, "mb_vertical_error", ping.vertical_error, ping.number_beams);
    add_double_array(json, "mb_horizontal_error", ping.horizontal_error, ping.number_beams);
    add_short_array(json, "mb_sector_number", ping.sector_number, ping.number_beams);
    add_short_array(json, "mb_detection_info", ping.detection_info, ping.number_beams);
    add_double_array(json, "mb_incident_beam_adj", ping.incident_beam_adj, ping.number_beams);
    add_short_array(json, "mb_system_cleaning", ping.system_cleaning, ping.number_beams);
    add_double_array(json, "mb_doppler_corr", ping.doppler_corr, ping.number_beams);
    add_double_array(json, "mb_sonar_vert_uncert", ping.sonar_vert_uncert, ping.number_beams);
    add_double_array(json, "mb_sonar_horz_uncert", ping.sonar_horz_uncert, ping.number_beams);
    add_double_array(json, "mb_detection_window", ping.detection_window, ping.number_beams);
    add_double_array(json, "mb_mean_abs_coeff", ping.mean_abs_coeff, ping.number_beams);
    cJSON_AddNumberToObject(json, "sensor_id", ping.sensor_id);
    cJSON_AddStringToObject(json, "sensor_name", gsfGetSonarTextName(&ping));
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
    gsfSensorSpecific_toJson(body_json, ping.sensor_id, ping);
    return json;
}


char *gsfRecord_toJson(gsfDataID dataID, gsfRecords record, gsfJsonFile gsfJsonFileInfo) {
    cJSON *json;
    switch (dataID.recordID) {
        case GSF_RECORD_COMMENT:
            if (gsfJsonFileInfo.flatten) {
                json = gsfComment_flatten(record.comment, gsfJsonFileInfo);
            } else {
                json = gsfComment_toJson(record.comment, gsfJsonFileInfo);
            }
            break;
        case GSF_RECORD_HISTORY:
            if (gsfJsonFileInfo.flatten) {
                json = gsfHistory_flatten(record.history, gsfJsonFileInfo);
            } else {
                json = gsfHistory_toJson(record.history, gsfJsonFileInfo);
            }
            break;
        case GSF_RECORD_SWATH_BATHY_SUMMARY:
            if (gsfJsonFileInfo.flatten) {
                json =  gsfSwathBathySummary_flatten(record.summary, gsfJsonFileInfo);
            } else {
                json =  gsfSwathBathySummary_toJson(record.summary, gsfJsonFileInfo);
            }
            break;
        case GSF_RECORD_SWATH_BATHYMETRY_PING:
            if (gsfJsonFileInfo.flatten) {
                json = gsfSwathBathyPing_flatten(record.mb_ping, gsfJsonFileInfo);
            } else {
                json = gsfSwathBathyPing_toJson(record.mb_ping, gsfJsonFileInfo);
            }
            break;
        case GSF_RECORD_ATTITUDE:
            if (gsfJsonFileInfo.flatten) {
                json = gsfAttitude_flatten(record.attitude, gsfJsonFileInfo);
            } else {
                json = gsfAttitude_toJson(record.attitude, gsfJsonFileInfo);
            }
            break;
        case GSF_RECORD_SINGLE_BEAM_PING:
            if (gsfJsonFileInfo.flatten) {
                json = gsfSingleBeamPing_flatten(record.sb_ping);
            } else {
                json = gsfSingleBeamPing_toJson(record.sb_ping);
            }
            break;
        default:
            return NULL;
    }
    return cJSON_PrintUnformatted(json); 
}

int gsfOpenForJson(char *filename, const int mode, int *handle, int buf_size, int include_denormlized_fields, int flatten) {
    int retvalue = gsfOpenBuffered(filename, mode, handle, buf_size);
    if (retvalue == 0) {
        memcpy(gsfJsonFiles[*handle].file_name, basename(filename), MAX_FILE_NAME_SIZE);
        gsfJsonFiles[*handle].include_denormalized_fields = include_denormlized_fields;
        gsfJsonFiles[*handle].flatten = flatten;
        gsfJsonFiles[*handle].gsf_version_set = 0;
        strncpy(gsfJsonFiles[*handle].gsf_version, "NA", 3);
    }
    return retvalue;
}

int gsfCloseForJson(int handle) {
    gsfJsonFiles[handle].file_name[0] = 0;
    gsfJsonFiles[handle].gsf_version_set = 0;
    gsfJsonFiles[handle].gsf_version[0] = 0;
    return gsfClose(handle);
}

struct t_gsfJsonRecord gsfNextJsonRecord(int handle, int desired_record) {
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