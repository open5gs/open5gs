#include <stdio.h>

#include "CRC_decoding+rec.h"
#include "CRC_start+rec.h"

int signal_code_control;

void signal_decode(int array_CRC[], int array_signal[], int array_signal_out[], int size_array_CRC, int size_array_signal, int size_array_signal_out) {

    FILE* outputFile = fopen("CRC_report.out", "a");

    for(int j=0; j<size_array_signal+1; j++){
        if(array_signal_out[j]==1){
            for(int i=0; i<size_array_CRC; i++){
                array_signal_out[j+i]=array_signal_out[j+i]^array_CRC[i];
            }
        }
    }

    for(int i=0; i<size_array_signal_out; i++){
        if(array_signal_out[i]!=0){
            signal_code_control = 1;
        }
    }

    fprintf(outputFile, "\npartial data from CRC_decoding");
    report_file(array_CRC, array_signal, array_signal_out, size_array_CRC, size_array_signal, size_array_signal_out, outputFile);

    if(signal_code_control == 0) fprintf(outputFile, "\ncorrect signal\n\n");
    if(signal_code_control == 1) fprintf(outputFile, "\nincorrect signal\n\n");

}


