#pragma once
#include <cstdlib>

#ifndef NUMGENH
#define NUMGENH

inline float rand_float() {
	return rand() / (RAND_MAX + 1.0);
}

#endif