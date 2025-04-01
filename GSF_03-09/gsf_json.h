#ifndef __GSF_JSON__
#define __GSF_JSON__
#include <cjson/cJSON.h>
#include "gsf.h"

cJSON *gsfRecord_toJson(gsfDataID dataID, gsfRecords record);

_libgsf.gsfClose.argtypes = [c_int]
_libgsf.gsfClose.restype = c_int

#endif