#ifndef _INCLUDE_ONCE_UTIL
#define _INCLUDE_ONCE_UTIL

#define TODO(x)                                                                         \
	do                                                                              \
	{                                                                               \
		printf("\n########################################################");   \
		printf("\nTODO: %s", x);                                                \
		printf("\n########################################################\n"); \
	} while (0);

typedef void (*fun_ptr_t)();

#endif // _INCLUDE_ONCE_UTIL