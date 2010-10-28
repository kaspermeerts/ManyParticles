#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "main.h"
#include "render.h"
#include "system.h"

#define SCREEN_W 1280
#define SCREEN_H 800

SDL_Surface *surface;

GLfloat light_pos[] = {  3.0, 1.0, 0.0, 0.0 };
GLfloat light_diff[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light_spec[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat light_ambi[] = { 1.0, 1.0, 1.0, 1.0 };
void renderSphere(float x, float y, float z, float r);

int initRender(void)
{
	int flags = 0;
	float s = config.numBox * config.boxSize;
	const SDL_VideoInfo *vidinfo;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		die(SDL_GetError());
	
	vidinfo = SDL_GetVideoInfo();
	if (vidinfo == NULL)
		die(SDL_GetError());
	
	flags |= SDL_OPENGL;
	flags |= SDL_HWPALETTE;
	flags |= (vidinfo->hw_available ? SDL_HWSURFACE : SDL_SWSURFACE);
	if (vidinfo->blit_hw) flags |= SDL_HWACCEL;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

	surface = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 24, flags);
	if (surface == NULL)
		die(SDL_GetError());

	/*SDL_WM_ToggleFullScreen(surface);	*/

	atexit(SDL_Quit);

	/* OpenGL Init */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv( GL_LIGHT0, GL_POSITION, light_pos );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diff );
	glLightfv( GL_LIGHT0, GL_SPECULAR, light_spec );
	glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambi );

	glClearColor(0.0, 0.0, 0.2, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, SCREEN_W/(double)SCREEN_H, s/2, 4*s);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return 0;
}

int render(void)
{
	int i;
	float s;
	static float theta;

	s = config.numBox * config.boxSize;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	theta += 0.00;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -s*2.5);
	glRotatef(theta, 0, 1, 0);


	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-s/2, -s/2, -s/2);
		glVertex3f(-s/2, -s/2, +s/2);
		glVertex3f(-s/2, +s/2, +s/2);
		glVertex3f(-s/2, +s/2, -s/2);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(+s/2, -s/2, -s/2);
		glVertex3f(+s/2, -s/2, +s/2);
		glVertex3f(+s/2, +s/2, +s/2);
		glVertex3f(+s/2, +s/2, -s/2);
	glEnd();

#ifdef BROWNIAN
	renderSphere(huge.pos.x - s/2, huge.pos.y - s/2, huge.pos.z - s/2, huge.r);
#endif
	glColor3f(0.0, 0.7, 0.0);
	/*glPointSize(2);*/
	/*glBegin(GL_POINTS);*/
	for (i = 0; i < config.numParticles; i++)
	{
		Particle *p = &world.parts[i];
		renderSphere(p->pos.x - s/2, p->pos.y - s/2, p->pos.z - s/2, p->r);
		/*glVertex3f(p->pos.x - s/2, p->pos.y - s/2, p->pos.z - s/2);*/
		glColor3f(1.0, 0.0, 0.0);
	}
	/*glEnd();*/

	SDL_GL_SwapBuffers();

	return 0;
}

void renderSphere(float x, float y, float z, float r)
{
	const int stacks = 8;
	const int slices = 8;
	int i, j;

	glBegin(GL_QUAD_STRIP);

	for (i = 0; i < stacks; i++)
	{
		float theta1, theta2;
		float r1, h1, r2, h2;

		theta1 = M_PI * ((float) (i + 0) / stacks) - M_PI/2;
		theta2 = M_PI * ((float) (i + 1) / stacks) - M_PI/2;

		h1 = r * sin(theta1);
		r1 = sqrt(r*r - h1*h1);

		h2 = r * sin(theta2);
		r2 = sqrt(r*r - h2*h2);

		for (j = 0; j <= slices; j++)
		{
			float phi;
			float x1, y1, z1, x2, y2, z2;

			phi = 2 * M_PI * j / slices;

			x1 = r1 * cos(phi);
			y1 = r1 * sin(phi);
			z1 = h1;

			x2 = r2 * cos(phi);
			y2 = r2 * sin(phi);
			z2 = h2;

			glVertex3f(x + x1, y + y1, z + z1);
			glNormal3f(x1, y1, z1); 
			glVertex3f(x + x2, y + y2, z + z2);
			glNormal3f(x2, y2, z2);
		}
	}

	glEnd();

	return;
}
