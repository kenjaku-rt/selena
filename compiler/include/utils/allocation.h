#ifndef SELENA_UTILS_ALLOCATION_H_
#define SELENA_UTILS_ALLOCATION_H_

#include <stddef.h>
#include <stdio.h>

/**
 * @brief Sets the stream that will be used for allocation error messages.
 * 
 * @param[in] error_stream stream for error messages.
 * @returns 0 if OK, 1 otherwise.
 */
int sln_utils_alloc_set_stream(FILE* error_stream);

/**
 * @brief Allocates elements with a certain size.
 * 
 * @param[in] num number of elements.
 * @param[in] size_of_element size of each element.
 * @param[in] function_info name of the function and etc. For debug.
 */
void* sln_utils_alloc(size_t num, size_t size_of_element, const char* function_info);

/**
 * @brief Macros for debuggin name of the function
 */
#define SLN_ALLOC(num, type) \
    (type*)sln_utils_alloc((num), sizeof(type), __PRETTY_FUNCTION__)


#endif // SELENA_UTILS_ALLOCATION_H_