#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <cmath>
#include <math.h>
#include <iostream>

# define WIDTH	1024
# define HEIGHT 512
# define P2 M_PI/2
# define P3 3 * M_PI/2
# define DR 0.0174533

float px, py;
float dtx, dty; // delta x && delta y
float pa; // player angle
int	mapX = 8;
int mapY = 8;
int mapS = 64;

int	map[] = {
	1,1,1,1,1,1,1,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,1,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1,
};

void	drawMap2D()
{
	int x;
	int y;
	int xo;
	int yo;

	y = -1;
	while (++y < mapY)
	{
		x = -1;
		while (++x < mapX)
		{
			if (map[y*mapX+x] == 1)
				glColor3f(1, 1, 1);
			else
				glColor3f(0, 0, 0);
			xo = x * mapS;
			yo = y * mapS;
			glBegin(GL_QUADS);
			glVertex2i(xo + 1, yo + 1);
			glVertex2i(xo + 1, yo + mapS - 1);
			glVertex2i(xo + mapS - 1, yo + mapS - 1);
			glVertex2i(xo + mapS - 1, yo + 1);
			glEnd();
		}
	}
}

float	get_dist(float ax, float ay, float bx, float by, float ang)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}


void get_lines()
{
	int r, mx, my, mp, dof;
	float rx, ry, ra, xo, yo, aTan, nTan, disH, disV ,hx, hy, vx, vy, disT, lineH, lineO, ca;

	r = -1;
	ra = pa - DR * 30;
	if (ra < 0)
		ra += 2 * M_PI;
	if (ra > 2 * M_PI)
		ra -= 2 * M_PI;
	while (++r < 60)
	{
		disH = 1000000;
		hx = px;
		hy = py;
		dof = 0;
		aTan = -1/tan(ra);
		if (ra > M_PI)
		{
			ry = (((int) py >> 6) << 6) - 0.0001;
			rx = (py - ry) * aTan + px;
			yo =-64;
			xo =-yo * aTan;
		}
		if (ra < M_PI)
		{
			ry = (((int) py >> 6) << 6) + 64;
			rx = (py - ry) * aTan + px;
			yo = 64;
			xo =-yo*aTan;
		}
		if (ra == 0 || ra == (float)M_PI)
		{
			rx = px;
			ry = py;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = (int)(rx) >> 6;
			my = (int)ry >> 6;
			mp = my * mapY + mx;
			if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
			{
				hx = rx;
				hy=ry;
				disH = get_dist(px, py, hx, hy, ra);
				dof = 8;
			}
			else
			{
				rx += xo;
				ry += yo;
				dof++;
			}
		}
	dof = 0;
	nTan = -tan(ra);
	if (ra>P2 && ra < P3)
	{
		rx = (((int)px>>6)<<6)-0.0001;
		ry = (px - rx) * nTan + py;
		xo = -64;
		yo = -xo * nTan;
	}
	if (ra < P2 || ra > P3)
	{
		rx = (((int)px>>6)<<6) + 64;
		ry = (px - rx) * nTan + py;
		xo = 64;
		yo = -xo * nTan;
	}
	if (ra == 0 || ra == (float)M_PI)
	{
		rx = px;
		ry = py;
		dof = 8;
	}
	disV = 1000000;
	vx = px;
	vy = py;
	while (dof < 8)
	{
		mx = (int)(rx) >> 6;
		my = (int)ry >> 6;
		mp = my * mapX + mx;
		if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
		{
			vx = rx;
			vy = ry;
			disV = get_dist(px, py, vx, vy, ra);
			dof = 8;
		}
		else
		{
			rx += xo;
			ry += yo;
			dof++;
		}
		}
		if (disV < disH)
		{
			rx = vx;
			ry = vy;
			disT = disV;
			glColor3f(1,0,0);
		}
		if (disH < disV)
		{
			rx = hx;
			ry = hy;
			disT = disH;
			glColor3f(0,1,0);
		}
		glLineWidth(3);
		glBegin(GL_LINES);
		glVertex2i(px, py);
		glVertex2i(rx, ry);
		glEnd();

		// draw the 3D Map
		ca = pa -ra;
		if (ca < 0)
			ca+= 2 * M_PI;
		if (ca > 2 * M_PI)
			ca -= 2 * M_PI;
		disT = disT * cos(ca);
		lineH = mapS * 320 / disT;
		if (lineH > 320)
			lineH = 320;
		lineO = 160 - lineH/2;
		glLineWidth(8);
		glBegin(GL_LINES);
		glVertex2i(r * 8 + 530, lineO);
		glVertex2i(r * 8 + 530, lineH + lineO);
		glEnd();
		ra+=DR;
		if (ra < 0)
			ra += 2 * M_PI;
		if (ra > 2 * M_PI)
			ra -= 2 * M_PI;
	}
}

void	drawRays3D()
{
	get_lines();
}


void buttons(unsigned char k, int x, int y)
{
	if (k == 'a')
	{
		pa -= .1;
		if (pa < 0)
			pa += 2 * M_PI;
		dtx = cos(pa) * 5;
		dty = sin(pa) * 5;
	}
	if (k == 'd')
	{
		pa += .1;
		if (pa > 2 * M_PI)
			pa -= 2 * M_PI;
		dtx = cos(pa) * 5;
		dty = sin(pa) * 5;
	}
	if (k == 'w')
	{
		px += dtx;
		py += dty;
	}
	if (k == 's')
	{
		px -= dtx;
		py -= dty;
	}
	if (k == 27)
		exit(0);
	glutPostRedisplay();
}

void drawplayer()
{
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(px, py);
	glEnd();

	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2i(px, py);
	glVertex2i(px+dtx * 5, py + dty * 5);
	glEnd();
}

void	display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap2D();
	drawRays3D();
	drawplayer();
	glutSwapBuffers();
}

void init()
{
	glClearColor(0.3,0.3,0.3,0.3);
	gluOrtho2D(0, 1024, 512, 0);
	px = 300;
	py = 300;
	dtx = cos(pa) * 5;
	dty = sin(pa) * 5;
	pa = 0.1;
}

int main(int ac, char **av) {
	glutInit(&ac, av);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(buttons);
	glutMainLoop();
	return (0);
}
