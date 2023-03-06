/*
 * util.h
 *
 *  Created on: Mar 1, 2023
 *      Author: Kazuki Hamada
 */

#ifndef INC_UTIL_H_
#define INC_UTIL_H_

typedef enum {
	FALSE = 0U,
	TRUE
} BOOL;

float saturation(const float value, const float min, const float max);
BOOL util_isnonzero(const float value);

#endif /* INC_UTIL_H_ */
