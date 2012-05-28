/* Implementacoes do modulo RAYCAST
 *
 * Autor: Pedro Mario Cruz e Silva
 * email: pmario@visgraf.impa.br
 * Data: 11 de novembro de 1999
 * Ultima alteracao: 19/11/99
 */

#include "ray_cast.h"
#include "incremental.h"

int clipt(float d, float n, float *tE, float *tS)
{
  int in = 1;
  float t;
  
  if (d < 0.0f) { /* intersecao PE */
    t = n / d;
    if (t > *tS) 
      in = 0;
    else
      if (t > *tE) 
	*tE = t;
  }
  else
    if (d > 0.0f) { /* intersecao PS */     
      t = n / d;
      if (t < *tE) 
	in = 0;
      else
	if (t < *tS) 
	  *tS = t;
    }
    else
      if (n < 0.0f)    /* linha esta fora */
	in = 0; 
  return (in);
}

int cyrusBeck3D(float *x0, float *y0, float *z0, 
		float *x1, float* y1, float *z1,
		float left, float right, 
		float bottom, float top, 
		float nea, float fa)
{
  float dx = *x1 - *x0, dy = *y1 - *y0, dz = *z1 - *z0;
  float tE = 0.0f, tS = 1.0e32; /* tS = 1.0f */
  int visible = 0;
  
  if ((dx == 0.0f) && (dy == 0.0f) && (dz == 0.0f)) /* Clip de ponto */
    ;
  else {
    if (clipt(-dx, *x0-left, &tE, &tS))          /* LEFT   */
      if (clipt(dx, right-*x0, &tE, &tS))        /* RIGHT  */
	if (clipt(-dy, *y0-bottom, &tE, &tS))    /* BOTTOM */
	  if (clipt(dy, top-*y0, &tE, &tS))      /* TOP    */
	    if (clipt(-dz, *z0-nea, &tE, &tS))   /* NEAR   */
	      if (clipt(dz, fa-*z0, &tE, &tS)) { /* FAR    */
                visible = 1;
		(*x1) = (*x0) + tS * dx; 
		(*y1) = (*y0) + tS * dy; 
		(*z1) = (*z0) + tS * dz; 
		if (tE > 0.0f) { 
		  *x0 = *x0 + tE * dx; 
		  *y0 = *y0 + tE * dy; 
		  *z0 = *z0 + tE * dz; 
		}
	      }
  }
  return (visible);
} /* Nesta funcao o algoritmo de Cyrus-Beck foi alterado para realizar o
     cliping de uma semi-reta (rayo) com um volume paralelo aos eixos
     coordenados. Foi mudada a inicializacao da variavel tS. Foi mudada
     tambem a atualizacao de (x1, y1, z1) que passa a ocorrer sempre. */

int volumeClip(Volume *vol, Vector3D *p0, Vector3D *p1)
{
  return cyrusBeck3D(&(p0->x[0]), &(p0->x[1]), &(p0->x[2]),
		     &(p1->x[0]), &(p1->x[1]), &(p1->x[2]),
		     0.0f, vol->size_x,
		     0.0f, vol->size_y,
		     0.0f, vol->size_z);  
} /* Esta funcao faz o cliping de uma SEMIRETA, com origem em p0 e
     com a direcao P1-P0, contra um volume. */

/* VARIAVEIS GLOBAIS --------------------------------------------------------*/

int i,j,k;	 /* Indices do voxel */
real di, dj, dk; /* Coordenadas de interpolacao (dentro do voxel) */
real factor[8];	 /* Fatores de interpolacao */
real voxel_val;  /* Valor do voxel */
Vector3D bg;	 /* Cor do background */
real dens_norm;  /* Norma do vetor gradiente da funcao DENSIDADE */

void calculateFactors(real x1, real y1, real z1)
{
  real x0, y0, z0;
  
  x0 = 1.0f - x1;
  y0 = 1.0f - y1;
  z0 = 1.0f - z1;
  factor[0] = x0 * y0 * z0; /* 000 */
  factor[1] = x1 * y0 * z0; /* 100 */
  factor[2] = x0 * y1 * z0; /* 010 */
  factor[3] = x1 * y1 * z0; /* 110 */
  factor[4] = x0 * y0 * z1; /* 001 */
  factor[5] = x1 * y0 * z1; /* 101 */
  factor[6] = x0 * y1 * z1; /* 011 */
  factor[7] = x1 * y1 * z1; /* 111 */
}

real interpolOpacity(Volume *vol)
{
  if ((i<vol->nx-1) && (j<vol->ny-1) && (k<vol->nz-1))
    return (factor[0] * volumeOpacity(vol, i,j,k) +
	    factor[1] * volumeOpacity(vol, i+1,j,k) +
	    factor[2] * volumeOpacity(vol, i,j+1,k) +
	    factor[3] * volumeOpacity(vol, i+1,j+1,k) +
	    factor[4] * volumeOpacity(vol, i,j,k+1) +
	    factor[5] * volumeOpacity(vol, i+1,j,k+1) +
	    factor[6] * volumeOpacity(vol, i,j+1,k+1) +
	    factor[7] * volumeOpacity(vol, i+1,j+1,k+1));
  else
    return 0.0f;
}

Vector3D interpolColor(Volume *vol)
{
  Vector3D c;
  if ((i<vol->nx-1) && (j<vol->ny-1) && (k<vol->nz-1)) {
    c = vector3DScale(factor[0], volumeColor(vol, i,j,k));
    c = vector3DAdd(c, vector3DScale(factor[1], volumeColor(vol, i,j,k)));
    c = vector3DAdd(c, vector3DScale(factor[2], volumeColor(vol, i+1,j,k)));
    c = vector3DAdd(c, vector3DScale(factor[3], volumeColor(vol, i,j+1,k)));
    c = vector3DAdd(c, vector3DScale(factor[4], volumeColor(vol, i+1,j+1,k)));
    c = vector3DAdd(c, vector3DScale(factor[5], volumeColor(vol, i+1,j,k+1)));
    c = vector3DAdd(c, vector3DScale(factor[6], volumeColor(vol, i,j+1,k+1)));
    c = vector3DAdd(c,vector3DScale(factor[7],volumeColor(vol, i+1,j+1,k+1)));
    return c;
  }
  else {
    c.x[0] = c.x[1] = c.x[2] = 0.0f;
    return c;
  }	
}

Vector3D interpolGradient(Volume *vol)
{
  Vector3D c;
  if ((i<vol->nx-1) && (j<vol->ny-1) && (k<vol->nz-1)) {
    c = vector3DScale(factor[0], volumeGradient(vol, i,j,k));
    c = vector3DAdd(c, vector3DScale(factor[1], volumeGradient(vol, i,j,k)));
    c = vector3DAdd(c, vector3DScale(factor[2], volumeGradient(vol, i+1,j,k)));
    c = vector3DAdd(c, vector3DScale(factor[3], volumeGradient(vol, i,j+1,k)));
    c=vector3DAdd(c, vector3DScale(factor[4], volumeGradient(vol, i+1,j+1,k)));
    c=vector3DAdd(c, vector3DScale(factor[5], volumeGradient(vol, i+1,j,k+1)));
    c=vector3DAdd(c, vector3DScale(factor[6], volumeGradient(vol, i,j+1,k+1)));
    c=vector3DAdd(c,vector3DScale(factor[7],volumeGradient(vol, i+1,j+1,k+1)));
    return c;
  }
  else {
    c.x[0] = c.x[1] = c.x[2] = 0.0f;
    return c;
  }	
}

real opacityGradientNorm(Volume *vol, int i, int j, int k)
{	
  real norm;
  real xa, xb;
  
  norm = 0.0f;
  if ((i>0)&&(j>0)&&(k>0) &&
      (i < vol->nx-1) && (i < vol->nx-1) && (i < vol->nx-1)) {
    xa = volumeOpacity(vol, i+1, j, k);
    xb = volumeOpacity(vol, i-1, j, k);
    norm = (xa - xb)*(xa - xb);
    xa = volumeOpacity(vol, i, j+1, k);
    xb = volumeOpacity(vol, i, j-1, k);
    norm += (xa - xb)*(xa - xb);
    xa = volumeOpacity(vol, i, j, k+1);
    xb = volumeOpacity(vol, i, j, k-1);
    norm += (xa - xb)*(xa - xb);
    return sqrt(norm / 3.0f);
  }
  else
    return 0.0f;
} /* Esta funcaoo e uma aproximacao da norma do gradiente da funcao
     OPACIDADE */

Vector3D sampleContribution(Camera *cam, Volume *vol, Vector3D p)
{
  Vector3D color; /* Vetor RGB */
  Vector3D grad;  /* Vetor gradiente */
  Vector3D light; /* Direcao da luz */
  real dot;       /* cosseno do angulo entre grad e light */
  
  /* Calculando o gradiente */
  grad = interpolGradient(vol);
  /* grad = volumeGradient(vol, i,j,k); */
  dens_norm = vector3DNorm(grad);
  if (!ZERO(dens_norm)) {
    /* Pegando a cor do voxel */
    /* color = interpolColor(vol); */
    color = volumeColor(vol, i,j,k);
    grad = vector3DScale(1.0f/dens_norm, grad);
    /* Calculando a direcao da luz */
    light = vector3DNormalized(vector3DSub(cam->eye, p));
    dot = vector3DDot(grad, light);
    if (dot < 0.0f)
      dot = -dot;
    color = vector3DScale(dot, color);
  }
  else {
    color.x[0] = color.x[1] = color.x[2] = 0.0f;
  }
  return color;
}

Vector3D rayContribution(Camera *cam, Volume *vol, Vector3D p0, Vector3D p1)
{
  const real tol = 0.001f; /* Tolerancia */
  Vector3D d;   /* Vetor d = P1 - P0 */
  Vector3D c;   /* Contribuicao do raio (cor) */
  Vector3D cor; /* Contribuicao de cada amostra */
  real norm;    /* Norma de 'c' */
  real alpha;   /* Opacidade da amostra */
  real trp;	    /* Fator de transparencia */
  int n;        /* Contador */
  int max;      /* Numero de amostras por raio */
  
  max = vol->max;
  d = vector3DScale((1.0f/max), vector3DSub(p1, p0));
  p0 = vector3DAdd(p0, vector3DScale(0.5f, d));
  /* Inicializando a contribuicao do raio como PRETO */
  c.x[0] = c.x[1] = c.x[2] = 0.0f;
  /* Inicializando a atenuacao como 0% */
  trp = 1.0f;
  for(n=1; (n < max) && (trp > tol); n++) {
    i = (int)floor(di = (vol->nx - 1) * (p0.x[0] / vol->size_x));
    j = (int)floor(dj = (vol->ny - 1) * (p0.x[1] / vol->size_y));
    k = (int)floor(dk = (vol->nz - 1) * (p0.x[2] / vol->size_z));
    if (i<0) i=0; if (j<0) j=0; if (k<0) k=0;
    di -= i; dj -= j; dk -= k;
    calculateFactors(di, dj, dk);
    /* Calculando a opacidade da amostra */
    /* alpha = interpolOpacity(vol); */
    alpha = volumeOpacity(vol, i,j,k);
    if (alpha > tol) {
      /* Calculando a contribuicao da amostra */
      cor = sampleContribution(cam, vol, p0);
      /* Enfatizando as bordas com um fator proporcional a norma do gradiente*/
      /* alpha *= (dens_norm / (255 * sqrt(3))); */
      /* Adicionando a contrinuicao da amostra `a contribuicao total do raio */
      c = vector3DAdd(c, vector3DScale(trp * alpha, cor));
      /* Atualizando o fator de atenuacao */
      trp *= (1.0f - alpha);
    }
    /* Movendo para a proxima amostra */
    p0 = vector3DAdd(p0, d);
  }
  /* Normalizando a cotribuicao do raio */
  c = vector3DAdd(c, vector3DScale(trp, bg));
  norm = vector3DMax(c);
  if (norm > 1.0f)
    c = vector3DScale(1.0f/norm, c);
  return c;
} /* Esta funcao faz uma amostragem uniforme na porcao do raio
     compreendida entre p0 e p1 */

void castRays(Camera *camera, Volume *volume)
{
  Vector3D left, p, p0, p1, dx, dy;
  int i, j;
  Vector3D color;
  
  printf("Tracando raios...");
  fflush(stdout);
  /* Preparando o OpenGL para desenhar Pixels (x,y)
     Obs.: A origem e' o canto inferior esquerdo do viewport BOTTOM_LEFT */
  bg.x[0] = bg.x[1] = bg.x[2] = 1.0f;
  glClearColor(bg.x[0], bg.x[1], bg.x[2], 0.0f);
  glClearDepth(camera->farp);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glFlush();
  glutSwapBuffers();
  /* Zerando as matrizes */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, camera->screen_w, 0, camera->screen_h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glDisable(GL_DEPTH_TEST);
  /* Calculando os vetores de deslocamento da origem do raio */
  dx = vector3DScale(camera->factor_w, camera->xe);
  dy = vector3DScale(camera->factor_h, camera->ye);
  left = camera->o;
  for(j = 0; j < camera->screen_h; j++) {
    glBegin(GL_POINTS);
    p = left;
    for(i = 0; i < camera->screen_w; i++) {
      p0 = p;
      /* Calculando p1 */
      if (camera->proj_type == 'o')
	      p1 = vector3DSub(p0, camera->ze);
      else
	      p1 = vector3DAdd(p0, vector3DSub(p0, camera->eye));
      if (volumeClip(volume, &p0, &p1)) {
	    /* Calculando a intersecao do raio com o volume */
	    //color = rayContribution(camera, volume, p0, p1);
			color = INC_rayContribution(camera, volume, p0, vector3DSub(p1,p0));
	    /* Plotando o resultado */
	    glColor3f(color.x[0], color.x[1], color.x[2]);
	    glVertex2f(i, j);
      }
      p = vector3DAdd(p, dx);
    } 
    left = vector3DAdd(left, dy);
    glEnd();
    glFlush();
    glutSwapBuffers();
  }
  glEnable(GL_DEPTH_TEST);
  cameraAim(camera);
  printf("Ok!\n");
  fflush(stdout);
}








