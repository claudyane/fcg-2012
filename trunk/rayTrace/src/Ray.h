/*
 * Ray.h
 *
 *  Created on: May 12, 2012
 *      Author: walther
 */

#ifndef RAY_H_
#define RAY_H_

#include "Vector3D.h"

typedef struct t_ray
{
    Vector3D origin;
    Vector3D direction;
}Ray;

#endif /* RAY_H_ */
