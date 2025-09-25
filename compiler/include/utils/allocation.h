#ifndef SELENA_UTILS_ALLOCATION_H_
#define SELENA_UTILS_ALLOCATION_H_

#include <stddef.h>
#include <stdio.h>

/**
 * @brief Allocates elements with a certain size.
 * 
 * @param[in] num number of elements.
 * @param[in] size_of_element size of each element.
 * @param[in] error_stream stream for messages about errors.
 * @param[in] function_info name of the function and etc. For debug.
 */
void* sln_util_alloc(size_t num, size_t size_of_element, FILE* error_stream, const char* function_info);

/**
 * @brief Macros for debuggin name of the function
 */
#define SLN_ALLOC(num, type, stream) \
    (type*)sln_util_alloc((num), sizeof(type), (stream), __PRETTY_FUNCTION__)


#endif // SELENA_UTILS_ALLOCATION_H_