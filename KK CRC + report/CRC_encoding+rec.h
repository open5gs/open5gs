#ifndef ENCODING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void signal_encode(int array_CRC[], int array_signal[], int array_signal_out[], int size_array_CRC, int size_array_signal, int size_array_signal_out);

#ifdef __cplusplus
}
#endif

#endif //ENCODING_H
