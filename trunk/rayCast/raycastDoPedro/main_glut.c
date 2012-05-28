#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include "volume.h"
#include "camera.h"
#include "ray_cast.h"
#include "smart_cast.h"

/* Variaveis globais */

int rotate = 0;
int old_x, old_y;

Volume *volume;
Camera *camera;

/* Callbacks da GLUT */

void displayCall(void)
{	
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(camera->farp);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  volumeDrawGL(volume);
  
  glFlush();
  glutSwapBuffers();
} /* Callback de REDISPLAY */

void reshapeCall(int width, int height)
{
  camera->screen_w = width;
  camera->screen_h = height;
  glViewport(0, 0, camera->screen_w, camera->screen_h);
  cameraAim(camera);
} /* Callback de RESHAPE */

void mouseCall(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    old_x = x;
    old_y = y;
    rotate = 1;
  }
  else 
    rotate = 0;
} /* Callback de mouse CLICK */

void moveCamera(int x0, int y0, int x1, int y1)
{
  double dx, dy;
  
  dx = x1 - x0;
  dy = y1 - y0;
}

void motionCall(int x, int y)
{
  if (rotate) {
    moveCamera(old_x, old_y, x, y);
  }
  old_x = x;
  old_y = y;
  glutPostRedisplay();
} /* Callback de mouse MOTION */

void keyboardCall(unsigned char key, int x, int y)
{
  switch (key) {
	case 'q':
		smartCast(camera, volume);
		break;
	case 's':
		volumeSaveHistogram(volume, "histo.txt");
		break;
  case 13: /* Enter */
    castRays(camera, volume);
    break;
  case 27: /* Esc */
    exit(0);
  }
}

void idleCall(void)
{
} /* Callback de IDLE */

void main(int argc, char **argv)
{
  volume = volumeCreate();

  /* Lendo os dados volumetricos */
  printf("Lendo os dados volumetricos...\n");
  fflush(stdout);
  if (!volumeReadFromFile(volume, argv[1])) {
    printf("Erro na leitura do arquivo %s", argv[2]);
    exit(0);
  }    

  camera = cameraCreate();
  /* Lendo o arquivo de configuracao de camera */
  printf("Lendo os parametros da camera...\n");
  fflush(stdout);
  if (!cameraReadFromFile(camera, argv[2])) {
    printf("Erro na leitura do arquivo %s", argv[3]);
    exit(0);
  }
  
  /* GLUT - Initialization */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
  glutInitWindowSize(camera->screen_w, camera->screen_h);
  glutCreateWindow("Ray-Casting");
  
  /* Armando a camera */
  cameraAim(camera);

  /* Definindo o tipo de rendering */
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
  
  /* Registrando callbacks */
  glutDisplayFunc(displayCall);
  glutReshapeFunc(reshapeCall);
  glutMouseFunc(mouseCall);
  glutMotionFunc(motionCall);
  glutKeyboardFunc(keyboardCall);
  glutIdleFunc(idleCall);
  
  /* GLUT main loop */
  glutMainLoop();
}











