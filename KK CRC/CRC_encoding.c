#include "CRC_encoding.h"
#include "CRC_start.h"

void signal_encode(int array_CRC[], int array_signal[], int array_signal_out[], int size_array_CRC, int size_array_signal, int size_array_signal_out){

    for(int j=0; j<size_array_signal; j++){
        array_signal_out[j]=array_signal[j];
    }

    for(int j=0; j<size_array_signal+1; j++){
        if(array_signal_out[j]==1){
            for(int i=0; i<size_array_CRC; i++){
                array_signal_out[j+i]=array_signal_out[j+i]^array_CRC[i];
            }
        }
    }

    for(int j=0; j<size_array_signal; j++){
            array_signal_out[j]=array_signal[j];
    }

    printf("\npartial data from CRC_encoding.c");
    parts_control_print(array_CRC, array_signal, array_signal_out, size_array_CRC, size_array_signal, size_array_signal_out);

    signal_decode(array_CRC, array_signal, array_signal_out, size_array_CRC, size_array_signal, size_array_signal_out);
}

