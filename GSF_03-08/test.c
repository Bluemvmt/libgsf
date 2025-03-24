#include <stdlib.h>
#include <stdio.h>
#include "gsf.h"

int main(int argc, char **argv) {
    int gsfHandle;
    gsfDataID                  gsfID;
    gsfRecords                 gsfRec;

    int status = gsfOpen(argv[1], GSF_READONLY, &gsfHandle);
    printf("status = %d\n", status);
    if (status == 0) {
        int bytes;

	printf("hello\n");
        bytes = gsfRead(gsfHandle, 0, &gsfID, &gsfRec, NULL, 0);
        printf("bytes = %d", bytes);
        while (bytes > 0) {
            if (gsfID.recordID == GSF_RECORD_SWATH_BATHYMETRY_PING) {
                for (int i = 0; i < gsfRec.mb_ping.number_beams; i++) {      
                    printf("mb_ping[%d] = %f\n", i, gsfRec.mb_ping.mr_amplitude[i]);
                }
            }
            bytes = gsfRead(gsfHandle, 0, &gsfID, &gsfRec, NULL, 0);
        }    
    }
}
