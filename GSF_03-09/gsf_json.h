#ifndef __GSF_JSON__
#define __GSF_JSON__
#include <cjson/cJSON.h>
#include "gsf.h"

cJSON *gsfRecord_toJson(gsfDataID dataID, gsfRecords record);

#endif