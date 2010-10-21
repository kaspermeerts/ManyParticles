#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "render.h"
#include "system.h"

#define SCREEN_W 600
#define SCREEN_H 600

float theta = 0;
SDL_Surface *surface;

void die(const char *msg);

void die(const char *msg)
{
	fprintf(stderr, "%s\n", msg);
}

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

	s = config.numBox * config.boxSize;

	glClear(GL_COLOR_BUFFER_BIT);

	theta += 0.00;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -s*2.5);
	/*glTranslatef(-s/2, -s/2, -s/2);*/
	glRotatef(theta, 0, 1, 1/2.);


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

	glColor3f(1.0, 0.5, 0.0);
	glPointSize(2);
	glBegin(GL_POINTS);
	for (i = 0; i < config.numParticles; i++)
	{
		Particle *p = &world.parts[i];
		glVertex3f(p->pos.x - s/2, p->pos.y - s/2, p->pos.z - s/2);
	}
	glEnd();

	SDL_GL_SwapBuffers();

	return 0;
}
