/* Declaracoes do modulo CAMERA
 *
 * Autor: Pedro Mario Cruz e Silva
 * email: pmario@visgraf.impa.br
 * Data: 19 de outubro de 1999
 * Ultima alteracao: 26/10/99
 */

#include "glmath.h"

#ifndef CAMERA_H
#define CAMERA_H 1

typedef struct Camera {
	char proj_type;
	/* Parametros de definicao da camera */
	Vector3D eye;
	Vector3D at;
	Vector3D up;
	real fovy;
	real nearp;
	real farp;
	int screen_w;
	int screen_h;
	/* Sistema de coordenadas da camera */
	Vector3D xe, ye, ze;
	/* Canto inferior esquerdo do plano de projecao */
	Vector3D o;
	real factor_w, factor_h;
	real w, h;
} Camera;

Camera *cameraCreate(void);
int cameraReadFromFile(Camera *cam, char *filename);
void cameraAim(Camera *cam);
Vector3D cameraScrToPlane(Camera *cam, int x, int y);

#endif
