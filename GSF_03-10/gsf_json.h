#ifndef __GSF_JSON__
#define __GSF_JSON__
#include "gsf.h"

#define MAX_FILE_NAME_SIZE 1024

typedef struct t_gsfJsonRecord
{
    int lastReturnValue;
    char *jsonRecord;
}
gsfJsonRecord;

typedef struct t_gsfJsonFile {
    char file_name[MAX_FILE_NAME_SIZE];
    char gsf_version[GSF_VERSION_SIZE];
    int gsf_version_set;
    int include_denormalized_fields;    
}
gsfJsonFile;

int gsfOpenForJson(char *filename, const int mode, int *handle, int buf_size, int include_denormalized_fields);
int gsfCloseForJson(const int handle);
char *gsfRecord_toJson(gsfDataID dataID, gsfRecords record, gsfJsonFile gsfJsonFileInfo);
struct t_gsfJsonRecord gsfNextJsonRecord(int handle, int desired_record);

#endif
