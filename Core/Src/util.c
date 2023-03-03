/*
 * util.c
 *
 *  Created on: Mar 3, 2023
 *      Author: Kazuki Hamada
 */

#include "util.h"

float saturation(const float value, const float min, const float max) {
	if (value < min) {
		return min;
	} else if (max < value) {
		return max;
	} else {
		return value;
	}
}
