/*
 * util.h
 *
 *  Created on: Mar 1, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

float saturation(const float value, const float min, const float max) {
	if (value < min) {
		return min;
	} else if (max < value) {
		return max;
	} else {
		return value;
	}
}

#endif /* INC_UTIL_H_ */
