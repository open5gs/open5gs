#ifndef DECODING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define UNKNOWN_ERROR 2
#define INCORRECT_SIGNAL 1
#define CORRECT_SIGNAL 0

void signal_decode(int array_CRC[], int array_signal[], int array_signal_out[], int size_array_CRC, int size_array_signal, int size_array_signal_out);

#ifdef __cplusplus
}
#endif

#endif //DECODING_H
