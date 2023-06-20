#ifndef __STDBOOL_H_
#define __STDBOOL_H_

#ifndef __cplusplus

#if defined(__GNUC__) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901)

#define false 0
#define true 1

#else

typedef enum {
	false = 0,
	true = 1
} _Bool;

#define false false
#define true true

#endif

#define bool _Bool

#else // __cplusplus

#define _Bool bool
#define bool bool
#define false false
#define true true

#endif // __cplusplus

#endif // __STDBOOL_H_