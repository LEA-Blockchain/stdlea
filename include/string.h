#ifndef STRING_H
#define STRING_H
#include "feature_check.h"

#include "stddef.h"

void *memset(void *dest, int val, size_t len);
void *memcpy(void *dest, const void *src, size_t len);
void *memmove(void *dest, const void *src, size_t len);
int memcmp(const void *s1, const void *s2, size_t n);

#endif
