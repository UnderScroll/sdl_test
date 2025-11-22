#pragma once

#ifdef __GNUC__
#define UNUSED(x) __attribute__((__unused__)) x
#else
#define UNUSED(x) x
#endif
