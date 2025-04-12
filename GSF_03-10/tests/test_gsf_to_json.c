#include <stdlib.h>
#include <stdio.h>
#include "cjson/cJSON.h"
#include "gsf_json.h"
#include "gsf.h"

int main(int argc, char **argv) {
    int gsfHandle;
    gsfDataID                  gsfID;
    gsfRecords                 gsfRec;

    int desired_record = atoi(argv[1]);
    int status = gsfOpen(argv[2], GSF_READONLY, &gsfHandle);
    if (status == 0) {
        int bytes;

        bytes = gsfRead(gsfHandle, desired_record, &gsfID, &gsfRec, NULL, 0);
        while (bytes > 0) {
            printf("bytes_read = %d\n", bytes);
            char *json_str = gsfRecord_toJson(gsfID, gsfRec, 1);
            if (json_str != NULL) {
                printf("cJSON = %s\n", json_str);
            } else {
                printf("record type %d not supported\n", gsfID.recordID);
            }
            bytes = gsfRead(gsfHandle, desired_record, &gsfID, &gsfRec, NULL, 0);
        }    
    }
}
