/* Declaracoes do modulo RAYCAST
 *
 * Autor: Pedro Mario Cruz e Silva
 * email: pmario@visgraf.impa.br
 * Data: 11 de novembro de 1999
 * Ultima alteracao: 11/11/99
 */

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "volume.h"
#include "camera.h"

#ifndef RAYCAST
#define RAYCAST 1

typedef struct Ray {
  Vector3D orig;
  Vector3D dir;
} Ray;

void castRays(Camera *camera, Volume *volume);

#endif
