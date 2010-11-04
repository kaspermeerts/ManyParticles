#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <math.h>
#include "main.h"
#include "render.h"
#include "system.h"

#define SCREEN_W 1100
#define SCREEN_H 1100

typedef struct {
	GLfloat x, y, z;
} Vertex3;

const GLfloat light_pos[]  = {2.0, 1.0, 2.0, 0.0};
const GLfloat light_diff[] = {1.0, 1.0, 1.0, 0.0};
const GLfloat light_spec[] = {1.0, 0.0, 0.0, 0.0};
const GLfloat light_ambi[] = {0.8, 0.8, 0.8, 0.0};

static int numVertices;
static int numIndices;
static Vertex3 *sphereVertex;
static GLushort *sphereIndex;
static SDL_Surface *surface;
static float angle;

static void createSphere(int slices);
static void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, 
		GLdouble zFar);
static void calcFps(void);
#ifdef BROWNIAN
static void renderSphere(float x, float y, float z, float r);
#endif

static void calcFps()
{
	static int tock = 0, frames;
	int tick;
	char string[32];

	frames++;
	tick = SDL_GetTicks();

	if (tick - tock > 1000)
	{
		tock = tick;
		sprintf(string, "%u FPS\n", frames);
		SDL_WM_SetCaption(string, string);
		frames = 0;
	}
	return;
}

int renderLoop(void)
{
	SDL_Event event;

	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;
			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return 0;
					break;
				case SDLK_LEFT:
					angle--;
					break;
				case SDLK_RIGHT:
					angle++;
					break;
				case SDLK_UP:
					config.timeStep *= 2;
					break;
				case SDLK_DOWN:
					config.timeStep /= 2;
					break;
				default:
					break;
				}
			}
		}

		render();
		calcFps();
		stepWorld();
	}
}

static void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, 
		GLdouble zFar)
{
	GLdouble xMin, xMax, yMin, yMax;

	yMax = zNear * tan(fovy * M_PI / 360.0);
	yMin = -yMax;

	xMin = yMin * aspect;
	xMax = yMax * aspect;

	glFrustum(xMin, xMax, yMin, yMax, zNear, zFar);
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

	SDL_EnableKeyRepeat(1, 
			SDL_DEFAULT_REPEAT_INTERVAL);

	/*SDL_WM_ToggleFullScreen(surface);	*/

	atexit(SDL_Quit);

	/* OpenGL Init */
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambi);

	glClearColor(1.0, 1.0, 1.0, 0.0);

	createSphere(16);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3), sphereVertex);
	glNormalPointer(   GL_FLOAT, sizeof(Vertex3), sphereVertex);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35, SCREEN_W/(double)SCREEN_H, s/2, 4*s);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return 0;
}

int render(void)
{
	int i;
	float s;

	s = config.numBox * config.boxSize;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*	angle += 0.10;*/

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -s*2.5);
	glRotatef(angle, 0, 1, 0);

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
#if 0
	renderSphere(huge.pos.x - s/2, huge.pos.y - s/2, huge.pos.z - s/2,
			config.radiusHuge);
#else
	Particle *p = &huge;
	glPushMatrix();
		glTranslatef(p->pos.x - s/2, 
				p->pos.y - s/2, p->pos.z - s/2);
		glScalef(config.radiusHuge, config.radiusHuge, config.radiusHuge);
		glDrawElements(GL_TRIANGLES, numIndices, 
				GL_UNSIGNED_SHORT, sphereIndex);
	glPopMatrix();
#endif

#endif
	glColor3f(0.0, 0.7, 0.0);
	glPointSize(2);
	for (i = 0; i < config.numParticles; i++)
	{
		Particle *p = &world.parts[i];

#if 1
		glPushMatrix();
			glTranslatef(p->pos.x - s/2, 
					p->pos.y - s/2, p->pos.z - s/2);
			glScalef(config.radius, config.radius, config.radius);
			glDrawElements(GL_TRIANGLES, numIndices, 
					GL_UNSIGNED_SHORT, sphereIndex);
		glPopMatrix();
#else
		renderSphere(p->pos.x - s/2, p->pos.y - s/2, p->pos.z - s/2, 
				config.radius);
#endif
		glColor3f(1.0, 0.0, 0.0);
	}

	
	SDL_GL_SwapBuffers();

	return 0;
}

static void createSphere(int slices)
{
	int i, j, k;
	float x, y, z;
	float r;
	int stacks;

	stacks = slices;
	slices *= 2;

	/* Plus two for the poles */
	numVertices = (stacks - 1) * slices + 2;
	sphereVertex = calloc(numVertices, sizeof(Vertex3));

	/* All but the top and bottom stack */
	for (i = 1; i < stacks; i++)
	{
		float phi = M_PI * i / (float) stacks - 2*M_PI;
		
		z = cos(phi);
		r = sqrt(1 - z*z);

		for (j = 0; j < slices; j++)
		{
			float theta = 2*M_PI*j/(float) slices;
			x = r * sin(theta);
			y = r * cos(theta);

			sphereVertex[(i-1) * slices + j + 1].x = x;
			sphereVertex[(i-1) * slices + j + 1].y = y;
			sphereVertex[(i-1) * slices + j + 1].z = z;
		}
	}

	/* Top and bottom */
	sphereVertex[0].x = 0;
	sphereVertex[0].y = 0;
	sphereVertex[0].z = 1;

	sphereVertex[numVertices-1].x = 0;
	sphereVertex[numVertices-1].y = 0;
	sphereVertex[numVertices-1].z = -1;

	numIndices = (stacks - 1) * slices * 6;
	sphereIndex = calloc(numIndices, sizeof(GLushort));

	k = 0;

	for (i = 1; i < slices; i++)
	{
		sphereIndex[k++] = 0;
		sphereIndex[k++] = i;
		sphereIndex[k++] = i+1;
	}
	sphereIndex[k++] = 0;
	sphereIndex[k++] = 1;
	sphereIndex[k++] = slices;
	
	for (i = 0; i < slices - 1; i++)
	{
		sphereIndex[k++] = numVertices - 1;
		sphereIndex[k++] = (numVertices - 1 - slices) + i;
		sphereIndex[k++] = (numVertices - 1 - slices) + i + 1;
	}
	sphereIndex[k++] = numVertices - 1;
	sphereIndex[k++] = numVertices - 1 - 1;
	sphereIndex[k++] = numVertices - 1 - slices + 0;

	for (i = 1; i < stacks - 1; i++)
	{
		int base = 1 + (i - 1) * slices;

		for (j = 0; j < slices - 1; j++)
		{
			sphereIndex[k++] = base + j;
			sphereIndex[k++] = base + slices + j;
			sphereIndex[k++] = base + slices + j + 1;

			sphereIndex[k++] = base + j;
			sphereIndex[k++] = base + j + 1;
			sphereIndex[k++] = base + slices + j + 1;
		}

		sphereIndex[k++] = base;
		sphereIndex[k++] = base + slices - 1;
		sphereIndex[k++] = base + slices;

		sphereIndex[k++] = base + slices - 1;
		sphereIndex[k++] = base + slices;
		sphereIndex[k++] = base + slices + slices - 1;
	}

	return;
}

#ifdef BROWNIAN
static void renderSphere(float x, float y, float z, float r)
{
	const int stacks = 16;
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
#endif
