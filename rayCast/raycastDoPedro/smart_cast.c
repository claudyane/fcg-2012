#include "incremental.h"
#include "smart_cast.h"

#define TOL 0.000001

void drawPoint(Camera *cam, Vector3D p)
{
	real dx, dy;
	int x,y;

	p = vector3DSub(p, cam->o);
	dx = vector3DDot(p, cam->xe);
	dy = vector3DDot(p, cam->ye);
	x = (int)(dx / cam->factor_w);
	y = (int)(dy / cam->factor_h);
	glVertex2i(x,y);
}

void cast_X_plane(Camera *cam, Volume *vol, int i)
{
	int j, k;
	Vector3D cor;
	Vector3D p0;
	Vector3D n;
	Vector3D pp;
	real t;

	glBegin(GL_POINTS);
	for(j=0; j < vol->ny; j++)
		for(k = 0; k < vol->nz; k++) {
			/* Calculando o ponto inicial */
			p0.x[0] = (i + 0.5) * vol->delta_x;
			p0.x[1] = (j + 0.5) * vol->delta_y;
			p0.x[2] = (k + 0.5) * vol->delta_z;
			/* Calculando a direcao do raio */
			if (cam->proj_type == 'p')
				n = vector3DSub(p0, cam->eye);
			else
				n = vector3DScale(-1.0f, cam->ze);
			/* Calculando a contribuicao do raio */
			cor = INC_rayContribution(cam, vol, p0, n);
			/* Calculando o pixel que recebera a contribuicao */
			t = vector3DDot(vector3DSub(cam->o, p0), cam->ze) /
					vector3DDot(n, cam->ze);
			pp = vector3DAdd(p0, vector3DScale(t, n));
			/* Plotando */
			glColor3f(cor.x[0], cor.x[1], cor.x[2]);
			//glVertex3f(pp.x[0], pp.x[1], pp.x[2]);
			drawPoint(cam, pp);
		}
	glEnd();
}

void cast_Y_plane(Camera *cam, Volume *vol, int j)
{
	int i, k;
	Vector3D cor;
	Vector3D p0;
	Vector3D n;
	Vector3D pp;
	real t;

	glBegin(GL_POINTS);
	for(i=0; i < vol->nx; i++)
		for(k = 0; k < vol->nz; k++) {
			/* Calculando o ponto inicial */
			p0.x[0] = (i + 0.5) * vol->delta_x;
			p0.x[1] = (j + 0.5) * vol->delta_y;
			p0.x[2] = (k + 0.5) * vol->delta_z;
			/* Calculando a direcao do raio */
			if (cam->proj_type == 'p')
				n = vector3DSub(p0, cam->eye);
			else
				n = vector3DScale(-1.0f, cam->ze);
			/* Calculando a contribuicao do raio */
			cor = INC_rayContribution(cam, vol, p0, n);
			/* Calculando o pixel que recebera a contribuicao */
			t = vector3DDot(vector3DSub(cam->o, p0), cam->ze) /
					vector3DDot(n, cam->ze);
			pp = vector3DAdd(p0, vector3DScale(t, n));
			/* Plotando */
			glColor3f(cor.x[0], cor.x[1], cor.x[2]);
			//glVertex3f(pp.x[0], pp.x[1], pp.x[2]);
			drawPoint(cam, pp);
		}
	glEnd();
}

void cast_Z_plane(Camera *cam, Volume *vol, int k)
{
	int i, j;
	Vector3D cor;
	Vector3D p0;
	Vector3D n;
	Vector3D pp;
	real t;

	glBegin(GL_POINTS);
	for(i=0; i < vol->nx; i++)
		for(j = 0; j < vol->ny; j++) {
			/* Calculando o ponto inicial */
			p0.x[0] = (i + 0.5) * vol->delta_x;
			p0.x[1] = (j + 0.5) * vol->delta_y;
			p0.x[2] = (k + 0.5) * vol->delta_z;
			/* Calculando a direcao do raio */
			if (cam->proj_type == 'p')
				n = vector3DSub(p0, cam->eye);
			else
				n = vector3DScale(-1.0f, cam->ze);
			/* Calculando a contribuicao do raio */
			cor = INC_rayContribution(cam, vol, p0, n);
			/* Calculando o pixel que recebera a contribuicao */
			t = vector3DDot(vector3DSub(cam->o, p0), cam->ze) /
					vector3DDot(n, cam->ze);
			pp = vector3DAdd(p0, vector3DScale(t, n));
			/* Plotando */
			glColor3f(cor.x[0], cor.x[1], cor.x[2]);
			//glVertex3f(pp.x[0], pp.x[1], pp.x[2]);
			drawPoint(cam, pp);
		}
	glEnd();
}

void smartCast(Camera *cam, Volume *vol)
{	
	Vector3D bg;

  printf("Tracando raios...");
  fflush(stdout);
  /* Preparando o OpenGL para desenhar Pixels (x,y)
     Obs.: A origem e' o canto inferior esquerdo do viewport BOTTOM_LEFT */
  bg.x[0] = bg.x[1] = bg.x[2] = 1.0f;
  glClearColor(bg.x[0], bg.x[1], bg.x[2], 0.0f);
  glClearDepth(cam->farp);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glFlush();
  /* Zerando as matrizes */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, cam->screen_w, 0, cam->screen_h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  glDisable(GL_DEPTH_TEST);
	/* Testando a posicao da camera com relacao aos planos x = cte */
	if (fabs(cam->ze.x[0]) > TOL) {
	  if (cam->ze.x[0] < 0)
		  /* Renderiza o plano x = 0 */
		  cast_X_plane(cam, vol, 0);
		else
			/* Renderiza o plano x = 0 */
		  cast_X_plane(cam, vol, vol->nx-1);
	}
	/* Testando a posicao da camera com relacao aos planos y = cte */
	if (fabs(cam->ze.x[1]) > TOL) {
	  if (cam->ze.x[1] < 0)
		  /* Renderiza o plano x = 0 */
		  cast_Y_plane(cam, vol, 0);
		else
			/* Renderiza o plano x = 0 */
		  cast_Y_plane(cam, vol, vol->ny-1);
	}
	/* Testando a posicao da camera com relacao aos planos z = cte */
	if (fabs(cam->ze.x[2]) > TOL) {
	  if (cam->ze.x[2] < 0)
		  /* Renderiza o plano x = 0 */
		  cast_Z_plane(cam, vol, 0);
		else
			/* Renderiza o plano x = 0 */
		  cast_Z_plane(cam, vol, vol->nz-1);
	}
	printf("Ok!");
}