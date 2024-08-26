#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

#include "CRC_encoding.h"
#include "CRC_encoding.c"
#include "CRC_decoding.h"
#include "CRC_decoding.c"
#include "CRC_start.h"

void parts_control_print(int array_CRC[], int array_signal[], int array_signal_out[], int size_array_CRC, int size_array_signal, int size_array_signal_out){

    printf("\nsize_array_CRC = %d\n", size_array_CRC);
    printf("size_array_signal = %d\n", size_array_signal);
    printf("size_array_signal_out = %d\n", size_array_signal_out);

    printf("\narray_CRC         ");
    for(int i=0; i<size_array_CRC; i++){
        printf("%d ", array_CRC[i]);
    }
    printf("\narray_signal      ");
    for(int i=0; i<size_array_signal; i++){
        printf("%d ", array_signal[i]);
    }
    printf("\narray_signal_out  ");
    for(int i=0; i<size_array_signal_out; i++){
        printf("%d ", array_signal_out[i]);
    }
    printf("\n");
}

int main(){

    int array_CRC[3]={1,0,1};
    int array_signal[8]={1,1,0,1,0,1,1,0};
    int array_signal_out[10]={};
    /*
    int s;
    for(int i=0; i<8; i++){
        scanf("%d", &s);
        array_signal[i]=s;
    }
    */
    int size_array_CRC = sizeof array_CRC / sizeof (int);
    int size_array_signal = sizeof array_signal / sizeof (int);
    int size_array_signal_out = sizeof array_signal_out / sizeof (int);

    if(size_array_signal_out - size_array_signal != size_array_CRC - 1) goto unknown_error_short_end;

    printf("\npartial data from CRC_start.c");
    parts_control_print(array_CRC, array_signal, array_signal_out, size_array_CRC, size_array_signal, size_array_signal_out);

    signal_encode(array_CRC, array_signal, array_signal_out, size_array_CRC, size_array_signal, size_array_signal_out);

return 0;

    unknown_error_short_end:
    signal_code_control = 2;
    if(signal_code_control == 2) printf(" (unknown error)\n");

return 0;
}
