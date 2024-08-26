#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <windows.h>

#include "CRC_encoding+rec.h"
#include "CRC_encoding+rec.c"
#include "CRC_decoding+rec.h"
#include "CRC_decoding+rec.c"
#include "CRC_start+rec.h"
/*
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
*/
 void report_file(int array_CRC[], int array_signal[], int array_signal_out[], int size_array_CRC, int size_array_signal, int size_array_signal_out, FILE* outputFile)
{



    fprintf(outputFile, "\narray_CRC         ");
    for(int i=0; i<size_array_CRC; i++){
        fprintf(outputFile, "%d ", array_CRC[i]);
    }
    fprintf(outputFile, "               size_array_CRC =        %d", size_array_CRC);

    fprintf(outputFile, "\narray_signal      ");
    for(int i=0; i<size_array_signal; i++){
        fprintf(outputFile, "%d ", array_signal[i]);
    }
    fprintf(outputFile, "     size_array_signal =     %d", size_array_signal);

    fprintf(outputFile, "\narray_signal_out  ");
    for(int i=0; i<size_array_signal_out; i++){
        fprintf(outputFile, "%d ", array_signal_out[i]);
    }
    fprintf(outputFile, " size_array_signal_out = %d", size_array_signal_out);


    fprintf(outputFile, "\n");

}

int main(){

    SYSTEMTIME st;
    GetLocalTime(&st);

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

    FILE* outputFile = fopen("CRC_report.out", "a");

    fprintf(outputFile, "data saved at %d:%d on %d.%d.%d \n", st.wHour, st.wMinute, st.wDay, st.wMonth, st.wYear);

    fprintf(outputFile, "\npartial data from CRC_start");
    report_file(array_CRC, array_signal, array_signal_out, size_array_CRC, size_array_signal, size_array_signal_out, outputFile);

    signal_encode(array_CRC, array_signal, array_signal_out, size_array_CRC, size_array_signal, size_array_signal_out);

    fclose(outputFile);

    return 0;

    unknown_error_short_end:
    signal_code_control = 2;
    if(signal_code_control == 2) fprintf(outputFile, " (unknown error)\n");

    fclose(outputFile);

    return 0;
}
