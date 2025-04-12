#include <stdlib.h>
#include <stdio.h>
#include "cjson/cJSON.h"
#include "../gsf_json.h"
#include "../gsf.h"

int main(int argc, char **argv) {
    int gsfHandle;
    struct t_gsfJsonRecord nextRec;

    int desired_record = atoi(argv[1]);
    int status = gsfOpenForJson(argv[2], GSF_READONLY, &gsfHandle, 0, 1);
    if (status == 0) {
        int bytes;

        nextRec = gsfNextJsonRecord(gsfHandle, desired_record, 1);
        while (nextRec.lastReturnValue > 0) {
            if (nextRec.jsonRecord != NULL) {
                printf("cJSON = %s\n", nextRec.jsonRecord);
            } else {
                printf("record type not supported\n");
            }
            nextRec = gsfNextJsonRecord(gsfHandle, desired_record, 1);
        }    
        gsfCloseForJson(gsfHandle);
    }
}
