#ifndef __GSF_JSON__
#define __GSF_JSON__
#include "gsf.h"

typedef struct t_gsfJsonRecord
{
    int lastReturnValue;
    char *jsonRecord;
}
gsfJsonRecord;

char *gsfRecord_toJson(gsfDataID dataID, gsfRecords record);
struct t_gsfJsonRecord gsfNextJsonRecord(int handle, int desired_record);

#endif
