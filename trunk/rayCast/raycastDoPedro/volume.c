/* Implementacoes do modulo VOLUME
 *
 * Autor: Pedro Mario Cruz e Silva
 * email: pmario@visgraf.impa.br
 * Data: 19 de outubro de 1999
 * Ultima alteracao: 23/11/99
 */

#include <GL/glut.h>
#include "volume.h"

Volume *volumeCreate(void)
{
  Volume *vol;
  
  vol = (Volume *)malloc(sizeof(Volume));
  if (!vol)
    return NULL;
  return vol;
}

int volumeReadFromFile(Volume *vol, char *filename)
{
  FILE *fp;
  int nx, ny, nz, offset;
  float dx, dy, dz;
  char data_file[256];
  char transf_file[256];

  fp = fopen(filename, "r");
  if (!fp)
    return 0;
  fscanf(fp, "%d %d %d %f %f %f %d\n", &nx, &ny, &nz, &dx, &dy, &dz, &offset);
  fscanf(fp, "%s\n", data_file);
	fscanf(fp, "%s\n", transf_file);
	/* Espaçamento entre as amostras */
  vol->delta_x = dx;
  vol->delta_y = dy;
  vol->delta_z = dz;
	/* Número de amostras em cada eixo */
  vol->nx = nx;
  vol->ny = ny;
  vol->nz = nz;
	/* Máximo de amostras por eixo */
	vol->max = nx;
	if (ny > vol->max)
		vol->max = ny;
	if (nz > vol->max)
		vol->max = nz;
	/* Dimensões do volume */
  vol->size_x = dx * nx;
  vol->size_y = dy * ny;
  vol->size_z = dz * nz;
  fclose(fp);
	/* Lendo o arquivo de dados */
  volumeReadData(vol, data_file, offset);
	/* Lendo a curva de transferência */
	volumeReadTransfering(vol, transf_file);
  return 1;
}

int volumeReadData(Volume *vol, char *filename, int offset)
{
  FILE *fp;
  int i, j, k;

  fp = fopen(filename, "rb");
  if (!fp)
    return 0;
  /* Alocando o buffer de dados volumetricos */
  vol->data = (char *)malloc(vol->nx * vol->ny * vol->nz);
  if (vol->data == NULL) {
		printf("Erro na alocação dos dados volumétricos!\n");
    return 0;
	}
  /* Passando o header */
  fseek(fp, offset, 0);
  /* Lendo os dados */
  for(k=0; k < vol->nz; k++)
    for(j=0; j < vol->ny; j++)
      for(i=0; i < vol->nx; i++)	
	volumePutVoxel(vol, i, j, k, (unsigned char)getc(fp));
  fclose(fp);
  return 1;
}

int volumeReadTransfering(Volume *vol, char *filename)
{ 
  int num;							/* Numero de amostras na curva de transferencia */
  float r1, g1, b1, a1; /* Valores de cor e opacidade da amostra */
  float r2, g2, b2, a2; /* Valores de cor e opacidade da amostra */
	float r, g, b, a;			/* Valores de cor e opacidade INTERPOLADOS */
	float mr, mg, mb, ma; /* Coeficientes de interpolacao */
  float t1, t2, dt;			/* Parametro amostrado */
  FILE *fp;
  int i, j, j1, j2;

  fp = fopen(filename, "r");
  if (fp == NULL)
		return 0;
  /* Lendo o número de amostras */
  fscanf(fp, "%d\n", &num);
	if (num < 2)
		return 0;
  /* Lendo a primeira amostra */
  fscanf(fp, "%f %f %f %f %f\n", &t1, &r1, &g1, &b1, &a1);
  j1 = (int)t1;
  /* Armazenando a cor da amostra */
  vol->color[j1].x[0] = r1;
	vol->color[j1].x[1] = g1;
	vol->color[j1].x[2] = b1;
	/* Armazenando a opacidade da amostra */
	vol->opac[j1] = a1;  
	/* Lendo as amostras restantes */
  for(i=1; i < num; i++) {
		fscanf(fp, "%f %f %f %f %f\n", &t2, &r2, &g2, &b2, &a2);
		j2 = (int)t2;
	  /* Armazenando a cor da amostra */
		vol->color[j2].x[0] = r2;
		vol->color[j2].x[1] = g2;
		vol->color[j2].x[2] = b2;
		/* Armazenando a opacidade da amostra */
		vol->opac[j2] = a2;  
		/* Interpolando entre t1 e t2 */
		dt = t2 - t1;
		mr = (r2 - r1) / dt;
		mg = (g2 - g1) / dt;
		mb = (b2 - b1) / dt;
		ma = (a2 - a1) / dt;
		r = r1; g = g1; b = b1; a = a1;
		for(j=j1+1; j < j2; j++) {
		  /* Armazenando a cor da amostra */
		  vol->color[j].x[0] = r; r += mr;
		  vol->color[j].x[1] = (g += mg);
		  vol->color[j].x[2] = (b += mb);
		  /* Armazenando a opacidade da amostra */
		  vol->opac[j] = (a += ma);
		}
		j1 = j2; t1 = t2; r1 = r2; g1 = g2; b1 = b2; a1 = a2;
  }
  fclose(fp);
  return 1;
}

int volumeGetVoxel(Volume *vol, int i, int j, int k)
{
  return vol->data[i + vol->nx * (j + vol->ny * k)];
}

void volumePutVoxel(Volume *vol, int i, int j, int k, int val)
{
  vol->data[i + vol->nx * (j + vol->ny * k)] = (unsigned char)val;
}

Vector3D volumeColor(Volume *vol, int i, int j, int k)
{
	return vol->color[volumeGetVoxel(vol, i,j,k)];
}

real volumeOpacity(Volume *vol, int i, int j, int k)
{
	return vol->opac[volumeGetVoxel(vol, i,j,k)];
}

Vector3D volumeGradient(Volume *vol, int i, int j, int k)
{
	real xa, xb;
	Vector3D grad;

	if ((i<vol->nx-1) && (j<vol->ny-1) && (k<vol->nz-1) &&
		  (i>0) && (j>0) && (k>0)) {
		xa = volumeGetVoxel(vol, i+1,j,k);
		xb = volumeGetVoxel(vol, i-1,j,k);
		grad.x[0] = xa - xb;
		xa = volumeGetVoxel(vol, i,j+1,k);
		xb = volumeGetVoxel(vol, i,j-1,k);
		grad.x[1] = xa - xb;
		xa = volumeGetVoxel(vol, i,j,k+1);
		xb = volumeGetVoxel(vol, i,j,k-1);
		grad.x[2] = xa - xb;
	}
	else {
		grad.x[0] = grad.x[1] = grad.x[2] = 0.0f;
	}
	return grad;
}

void volumeDrawGL(Volume *vol)
{
  float sx, sy, sz;
  sx = vol->size_x;
  sy = vol->size_y;
  sz = vol->size_z;
  glBegin(GL_QUADS);
    /* Faces do eixo X */
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, sy, 0.0f);
    glVertex3f(0.0f, sy, sz);
    glVertex3f(0.0f, 0.0f, sz);
  
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(sx, 0.0f, 0.0f);
    glVertex3f(sx, sy, 0.0f);
    glVertex3f(sx, sy, sz);
    glVertex3f(sx, 0.0f, sz);
  
    /* Faces do eixo Y */
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(sx, 0.0f, 0.0f);
    glVertex3f(sx, 0.0f, sz);
    glVertex3f(0.0f, 0.0f, sz);
  
    glVertex3f(0.0f, sy, 0.0f);
    glVertex3f(sx, sy, 0.0f);
    glVertex3f(sx, sy, sz);
    glVertex3f(0.0f, sy, sz);
  
    /* Faces do eixo Z */
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(sx, 0.0f, 0.0f);
    glVertex3f(sx, sy, 0.0f);
    glVertex3f(0.0f, sy, 0.0f);
  
    glVertex3f(0.0f, 0.0f, sz);
    glVertex3f(sx, 0.0f, sz);
    glVertex3f(sx, sy, sz);
    glVertex3f(0.0f, sy, sz);
  
  glEnd();
}

void volumeSaveHistogram(Volume *vol, char *filename)
{
	FILE *fp;
	int h[256];
	int i,j,k;
	
	fp = fopen(filename, "w");
	if (fp == NULL)
		return;
	/* Zerando o histograma */
	for(i=0; i < 256; i++)
		h[i] = 0;
	/* Calculando o histograma */
	for(k=0; k < vol->nz; k++)
		for(j=0; j < vol->ny; j++)
			for(i=0; i < vol->nx; i++)
				h[volumeGetVoxel(vol, i,j,k)]++;
	/* Gravando o histograma */
	for(i=0; i<256; i++)
		fprintf(fp, "%d %d\n", i, h[i]);
	fclose(fp);
}

