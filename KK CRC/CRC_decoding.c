#include "CRC_decoding.h"
#include "CRC_start.h"

int signal_code_control;

void signal_decode(int array_CRC[], int array_signal[], int array_signal_out[], int size_array_CRC, int size_array_signal, int size_array_signal_out) {

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

    printf("\npartial data from CRC_decoding.c");
    parts_control_print(array_CRC, array_signal, array_signal_out, size_array_CRC, size_array_signal, size_array_signal_out);

    if(signal_code_control == 0) printf("\ncorrect signal\n");
    if(signal_code_control == 1) printf("\nincorrect signal\n");

}


