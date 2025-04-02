#ifndef __GSF_JSON__
#define __GSF_JSON__
#include "cJSON.h"
#include "gsf.h"

char *gsfRecord_toJson(gsfDataID dataID, gsfRecords record);

#endif