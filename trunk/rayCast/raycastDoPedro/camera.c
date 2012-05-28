/* Implementacoes do modulo CAMERA
 *
 * Autor: Pedro Mario Cruz e Silva
 * email: pmario@visgraf.impa.br
 * Data: 19 de outubro de 1999
 * Ultima alteracao: 26/10/99
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "camera.h"
#include "math.h"

#define PI 3.1415926536

const double deg_to_rad = PI / 180;

Camera *cameraCreate(void)
{
  Camera *cam;

  cam = (Camera *)malloc(sizeof(Camera));
  return cam;
} /* Esta funcao aloca um objeto da classe camera */

int cameraReadFromFile(Camera *cam, char *filename)
{
  FILE *fp;
  float x, y, z;
  int n;
  char param_name[16];
  
  fp = fopen(filename, "r");
  if (!fp)
    return 0;
  /* Lendo o tipo de projecao */
  fscanf(fp, "%s ", param_name);
  if (strcmp(param_name, "ptype=") == 0)
    fscanf(fp, "%c", &(cam->proj_type));
  else
    return 0;
  /* Lendo a posicao do observador */
  fscanf(fp, "%s ", param_name);
  if (strcmp(param_name, "eye=") == 0) {
    fscanf(fp, "%f %f %f", &x, &y, &z);
    cam->eye.x[0] = x;
    cam->eye.x[1] = y;
    cam->eye.x[2] = z;
  }
  else
    return 0;  
  /* Lendo a direcao de visao */
  fscanf(fp, "%s ", param_name);
  if (strcmp(param_name, "at=") == 0) {
    fscanf(fp, "%f %f %f", &x, &y, &z);
    cam->at.x[0] = x;
    cam->at.x[1] = y;
    cam->at.x[2] = z;
  }
  else
    return 0;  
  /* Lendo o "view-up" */
  fscanf(fp, "%s ", param_name);
  if (strcmp(param_name, "up=") == 0) {
    fscanf(fp, "%f %f %f", &x, &y, &z);
    cam->up.x[0] = x;
    cam->up.x[1] = y;
    cam->up.x[2] = z;
  }
  else
    return 0;  
  /* Lendo o fovy */
  fscanf(fp, "%s ", param_name);
  if (strcmp(param_name, "fovy=") == 0) {
    fscanf(fp, "%f ", &x);
    cam->fovy = x;
  }
  else
    return 0;  
  /* Lendo o near plane */
  fscanf(fp, "%s ", param_name);
  if (strcmp(param_name, "nearp=") == 0) {
    fscanf(fp, "%f ", &x);
    cam->nearp = x;
  }
  else
    return 0;  
  /* Lendo o far plane */
  fscanf(fp, "%s ", param_name);
  if (strcmp(param_name, "farp=") == 0) {
    fscanf(fp, "%f ", &x);
    cam->farp = x;
  }
  else
    return 0;  
  /* Lendo a largura da janela */
  fscanf(fp, "%s ", param_name);
  if (strcmp(param_name, "screenW=") == 0) {
    fscanf(fp, "%d ", &n);
    cam->screen_w = n;
  }
  else
    return 0;  
  /* Lendo a altura da janela */
  fscanf(fp, "%s ", param_name);
  if (strcmp(param_name, "screenH=") == 0) {
    fscanf(fp, "%d ", &n);
    cam->screen_h = n;
  }
  else
    return 0;  
  fclose(fp);
  return 1;
} /* Esta funcao le os parametros de camera de um 
     arquivo texto ( .cam ). */

void cameraAim(Camera *cam) {
  double aspect; /* Razao de aspecto (w/h) */
  double w;      /* Metade da largura da janela de projecao */
  double h;      /* Metade da altura da janela de projecao */

  aspect = (double)cam->screen_w / (double)cam->screen_h;
  h = cam->nearp * tan((cam->fovy/2) * deg_to_rad);
  w = h * aspect;
  cam->factor_w = (2.0f * w) / cam->screen_w;
  cam->factor_h = (2.0f * h) / cam->screen_h;
  /* Configurando a projecao */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (cam->proj_type == 'p') {
    /* Proj. em Perspectiva */
    glFrustum(-w, w, -h, h, cam->nearp, cam->farp);
  }
  else {
    /* Proj. Ortografica */
    glOrtho(-w, w, -h, h, cam->nearp, cam->farp);
  }
  /* Setando a base de vetores do espaco da camera */
  cam->ze = vector3DNormalized(vector3DSub(cam->eye, cam->at));
  cam->xe = vector3DNormalized(vector3DCross(cam->up, cam->ze));
  cam->ye = vector3DCross(cam->ze, cam->xe);
  cam->o = vector3DAdd(cam->eye, vector3DScale(-cam->nearp, cam->ze));
  cam->o = vector3DAdd(cam->o, vector3DScale(-w + (cam->factor_w / 2), cam->xe));
  cam->o = vector3DAdd(cam->o, vector3DScale(-h + (cam->factor_h / 2), cam->ye));
  cam->w = 2.0f * w;
  cam->h = 2.0f * h;
  /* Posicionando a camera */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(cam->eye.x[0], cam->eye.x[1], cam->eye.x[2],
	    cam->at.x[0], cam->at.x[1], cam->at.x[2],
	    cam->up.x[0], cam->up.x[1], cam->up.x[2]);
  glViewport(0, 0, cam->screen_w, cam->screen_h);
} /* Esta funcao carrega as matrizes do OpenGL com os parametros
     de camera, alem disto calcula parametros que sao necessarios
     para fazer transformacoes entre o plano da tela e o plano de
     projecao */

Vector3D cameraScrToPlane(Camera *cam, int x, int y)
{
  Vector3D p;
  real px, py;
  
  px = (real)x * cam->factor_w;
  py = (real)y * cam->factor_h;
  p = vector3DAdd(cam->o, vector3DScale(px, cam->xe));
  p = vector3DAdd(p, vector3DScale(py, cam->ye));
  return p;
}







