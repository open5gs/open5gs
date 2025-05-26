#ifndef CVECTOR_UTILS_H_
#define CVECTOR_UTILS_H_

/**
 * @brief cvector_for_each_in - for header to iterate over vector each element's address
 * @param it - iterator of type pointer to vector element
 * @param vec - the vector
 * @return void
 */
#define cvector_for_each_in(it, vec) \
    for (it = cvector_begin(vec); it < cvector_end(vec); it++)

/**
 * @brief cvector_for_each - call function func on each element of the vector
 * @param vec - the vector
 * @param func - function to be called on each element that takes each element as argument
 * @return void
 */
#define cvector_for_each(vec, func)                   \
    do {                                              \
        if ((vec) && (func) != NULL) {                \
            size_t i;                                 \
            for (i = 0; i < cvector_size(vec); i++) { \
                func((vec)[i]);                       \
            }                                         \
        }                                             \
    } while (0)

#endif /* CVECTOR_UTILS_H_ */
