#ifndef __STRING_H_
#define __STRING_H_

#include <string.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	char *a = (char *)src;
	char *b = (char *)dest;

	for (int i = 0; i < n; i++) {
		b[i] = a[i];
	}
}

#endif // __STRING_H_