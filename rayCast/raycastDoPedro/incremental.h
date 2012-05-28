/* Declaracoes do modulo INCREMENTAL
 *
 * Autor: Pedro Mario Cruz e Silva
 * email: pmario@visgraf.impa.br
 * Data: 2 de dezembro de 1999
 * Ultima alteracao: 2/12/99
 */

#include "volume.h"
#include "glmath.h"
#include "camera.h"

#ifndef INCREMENTAL
#define INCREMENTAL 1

Vector3D INC_rayContribution(Camera *cam, Volume *vol, 
			     Vector3D p0, Vector3D n);

#endif