/* Implementacoes do modulo INCREMENTAL
 *
 * Autor: Pedro Mario Cruz e Silva
 * email: pmario@visgraf.impa.br
 * Data: 2 de dezembro de 1999
 * Ultima alteracao: 2/12/99
 */

#include <math.h>
#include "glmath.h"
#include "camera.h"
#include "volume.h"

#define MAX_T 0.999

int min3(real t0, real t1, real t2)
{
  if (t0 < t1) {
    if (t0 < t2)
      return 0;  /* t0 e' o menor */
    else
      return 2;  /* t2 e' o menor */
  }
  else {
    if (t1 < t2)
      return 1;  /* t1 e' o menor */
    else
      return 2;  /* t2 e' o menor */
  } 
} /* Esta funcao recebe tres valores reais t0, t1, t2 e 
     retorna i se ti e' o menor entre os tres */

int sgn(real x)
{
	if (x > 0)
		return 1;
	else
		if (x < 0)
	    return -1;
		else
			return 0;
}

void INC_initialParam(real delta, real x0, int idx, real nx, int sx,
		      real *t, real *dt)
{
  if (sx > 0) {
    *t = ((idx+1) * delta - x0) / nx;
    *dt = (delta / nx);
  }
  else {
    if (sx < 0) {
      *t = (idx * delta - x0) / nx;
      *dt = (delta * sx) / nx;
    }
    else {
      *t = 1.0e30; /* Significa que nao ha intersecao com planos (MAXFLOAT = 3.7e38) */
      *dt = 0.0f;
    }
  }
} /* Essa funcao retorna por referencia o valor do primeiro t
     e o dt para um eixo, utilizando:
     dx - Espacamento entre as amostras no eixo
     x0 - Coordenada, no eixo, do ponto de entrada do raio no volume
     nx - Coordenada, no eixo, da direcao do raio
     sx - Sinal de nx
     */

Vector3D INC_voxColor(Volume *vol, Camera *cam, int i, int j, int k)
{
  Vector3D c;     /* Cor do voxel (com iluminacao) */
  Vector3D grad;  /* Vetor gradiente do voxel */
  Vector3D light; /* Direcao da fonte de iluminacao */
  Vector3D p;     /* Centroide do voxel */
  real norm;      /* Norma do vetor gradiente */
  real dot;       /* Cosseno do angulo entre o gradiente e a fonte de luz */

  /* Calculando o vetor gradiente */
  grad = volumeGradient(vol, i,j,k);
  /* Calculando a norma do vetor gradiente */
  norm = vector3DNorm(grad);
  if (!ZERO(norm)) {
    /* Normalizando o gradiente */
    grad = vector3DScale(1.0f / norm, grad);
		/* Pegando a cor do voxel */
		c = volumeColor(vol, i,j,k);
    /* Calculando o centroide do voxel */
    p.x[0] = (i + 0.5) * vol->delta_x;
    p.x[1] = (j + 0.5) * vol->delta_y;
    p.x[2] = (k + 0.5) * vol->delta_z;
    /* Calculando a direcao da fonte de luz */
    light = vector3DNormalized(vector3DSub(cam->eye, p));
    /* Calculando o cosseno entre o gradiente e a fonte de luz */
    dot = vector3DDot(grad, light);
    if (dot < 0.0f)
      dot = -dot;
		/* printf("||Luz|| = %.2f, ||grad|| = %.2f, dot = %.2f\n", vector3DNorm(light), norm, dot); */
		c = vector3DScale(dot, c);
  }
  else
    c.x[0] = c.x[1] = c.x[2] = 0.0f;
  return c;
} /* Esta funcao calcula a cor no centroide de um voxel
     levando em consideracao a iluminacao */

Vector3D INC_rayContribution(Camera *cam, Volume *vol, 
			     Vector3D p0, Vector3D n)
{
  real tx, ty, tz;    /* Parametros de intersecao */
  real dtx, dty, dtz; /* Incrementos dos parametros */
  int sx, sy, sz;     /* sinais das coordenadas do vetor direcao */
  int i0, j0, k0;     /* Indices do voxel atual */
	int i1, j1, k1;     /* Indices do proximo voxel */
  int min;
  Vector3D c;         /* Contribuicao total do raio */
  Vector3D c_vox;     /* Contribuicao do Voxel atual */
  real trp, alfa;     /* Coeficientes da integracao numerica */
  real t0, t1;        /* Parametros de entrada e saida do raio no voxel */
  const real tol = 0.001f;
  Vector3D bg;        /* Cor do background */
	real norm;          /* Norma do maximo do vetor de cor */
	int done;
	const real teta = vol->delta_x * vol->delta_y * vol->delta_z;
	
  /* Inicializando a transparencia acumulada */
  trp = 1.0f;
  /* Inicializando a cor da contribuicao do raio */
  c.x[0] = c.x[1] = c.x[2] = 0.0f;
  bg.x[0] = bg.x[1] = bg.x[2] = 1.0f;

  /* Calculando os sinais das coordenadas do vetor direcao */
  sx = sgn(n.x[0]); sy = sgn(n.x[1]); sz = sgn(n.x[2]);

  /* Calculando os indices do voxel inicial */
	/*
	i1 = i0 = floor( (p0.x[0] / vol->size_x) * (vol->nx - 1) );
  j1 = j0 = floor( (p0.x[1] / vol->size_y) * (vol->ny - 1) );
  k1 = k0 = floor( (p0.x[2] / vol->size_z) * (vol->nz - 1) );
	*/
	i1 = i0 = (p0.x[0] / vol->delta_x);
	if (i0 >= vol->nx) i1 = i0 = vol->nx-1;
	j1 = j0 = (p0.x[1] / vol->delta_y);
	if (j0 >= vol->ny) j1 = j0 = vol->ny-1;
	k1 = k0 = (p0.x[2] / vol->delta_z);
	if (k0 >= vol->nz) k1 = k0 = vol->nz-1;

		if ((i0 < 0) || (j0 < 0) || (k0 < 0) ||
			(i0 > vol->nx-1) || (j0 > vol->ny-1) || (k0 > vol->nz-1)) {
			done = 1;
			printf("Fora do Voxel! (%d,%d,%d)\n", i0,j0,k0);
		}
	
  /* Calculando os valores iniciais de tx, ty, tz, dtx, dty e dtz */
  INC_initialParam(vol->delta_x, p0.x[0], i0, n.x[0], sx, &tx, &dtx);
  INC_initialParam(vol->delta_y, p0.x[1], j0, n.x[1], sy, &ty, &dty);
  INC_initialParam(vol->delta_z, p0.x[2], k0, n.x[2], sz, &tz, &dtz);

	/* Calculando o t0 de entrada do primeiro voxel */
  min = min3(tx, ty, tz);
  switch (min) {
  case 0: t0 = tx; tx += dtx; break;
  case 1: t0 = ty; ty += dty; break;
  case 2: t0 = tz; tz += dtz; break;
  }
  //printf("\nPrimeiro Voxel (%d, %d, %d) (%.3f, %.3f, %.3f)",i0,j0,k0,tx,ty,tz);
	done = (t0 < MAX_T)?0:1;

  /* Iterando */
  while (!done) {
    min = min3(tx, ty, tz);
    /* Calculando o t1 de saida do voxel corrente */
    switch (min) {
    case 0: t1 = tx; i1 = i0 + sx; break;
    case 1: t1 = ty; j1 = j0 + sy; break;
    case 2: t1 = tz; k1 = k0 + sz; break;
    }
		/* Testando se o raio ainda esta dentro do volume */
		if ((i0 < 0) || (j0 < 0) || (k0 < 0) ||
			(i0 > vol->nx-1) || (j0 > vol->ny-1) || (k0 > vol->nz-1)) {
			done = 1;
			// printf("Fora do voxel!\n");
		}
		else {
			//printf("\n    Opacidade_IN (%d, %d, %d) (%.3f, %.3f, %.3f)",i0,j0,k0,tx,ty,tz);
			alfa = volumeOpacity(vol, i0,j0,k0);
		  //printf("- OUT -");
			if (alfa > tol) {
				/* Calculando a cor do voxel */
				c_vox = INC_voxColor(vol, cam, i0,j0,k0);
				/* Somando a contribuicao total do raio */
				c = vector3DAdd(c, vector3DScale(trp * alfa, c_vox));
				/* Atualizando a transparencia */
				trp *= (1.0f - alfa);
			}
			/* Passando para o proximo voxel */
			if (trp > tol) {
				switch (min) {
				case 0: tx += dtx; break;
				case 1: ty += dty; break;
				case 2: tz += dtz; break;
				}
				t0 = t1; i0 = i1; j0 = j1; k0 = k1;
			}
			else
				done = 1;
		}
  }
	/* Adicionando o background */
	if (trp > tol)
		c = vector3DAdd(c, vector3DScale(trp, bg));
	norm = vector3DMax(c);
	if (norm > 1.0f)
		c = vector3DScale(1.0f / norm, c);
	return c;
}





