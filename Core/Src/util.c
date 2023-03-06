/*
 * util.c
 *
 *  Created on: Mar 3, 2023
 *      Author: Kazuki Hamada
 */

#include "util.h"

#include "math.h"

#define EPS (1.0e-6F)

float saturation(const float value, const float min, const float max) {
	if (value < min) {
		return min;
	} else if (max < value) {
		return max;
	} else {
		return value;
	}
}

BOOL util_isnonzero(const float value) {
	return (EPS < fabsf(value)) ? (TRUE) : (FALSE);
}
