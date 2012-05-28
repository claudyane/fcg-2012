/* Declaracoes do modulo VOLUME
 *
 * Autor: Pedro Mario Cruz e Silva
 * email: pmario@visgraf.impa.br
 * Data: 19 de outubro de 1999
 * Ultima alteracao: 23/11/99
 */

#include <stdio.h>
#include <stdlib.h>
#include "glmath.h"

#ifndef VOLUME_H
#define VOLUME_H 1

#define MAX 256 /* Numero maximo de amostras na curva de transferencia */

typedef struct Volume {
	/* Distancias entre os voxels */
	real delta_x, delta_y, delta_z;
	real size_x, size_y, size_z;
	/* numero de voxels por eixo */
	int nx, ny, nz;
	int max;
	/* Dados volumetricos */
	unsigned char *data;
	/* Curva de transferencia */
	Vector3D color[MAX];
	real opac[MAX];
	/*? Deveria depender do numero de bits da quantizacao */
} Volume;

Volume *volumeCreate(void);
int volumeReadFromFile(Volume *vol, char *filename);
int volumeReadData(Volume *vol, char *filename, int offset);
int volumeReadTransfering(Volume *vol, char *filename);
int volumeGetVoxel(Volume *vol, int i, int j, int k);
void volumePutVoxel(Volume *vol, int i, int j, int k, int val);
Vector3D volumeColor(Volume *vol, int i, int j, int k);
real volumeOpacity(Volume *vol, int i, int j, int k);
Vector3D volumeGradient(Volume *vol, int i, int j, int k);
void volumeDrawGL(Volume *vol);
void volumeSaveHistogram(Volume *vol, char *filename);

#endif
