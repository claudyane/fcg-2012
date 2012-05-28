/*
 * Ray.h
 *
 *  Created on: May 12, 2012
 *      Author: walther
 */

#ifndef RAY_H_
#define RAY_H_

#include "Vector4D.h"

typedef struct t_ray
{
    Vector4D origin;
    Vector4D direction;
}Ray;

#endif /* RAY_H_ */
