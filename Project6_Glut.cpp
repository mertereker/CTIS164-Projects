/*****
   CTIS164
*****
STUDENT : Merter Eker
SECTION : 3
QUESTION 4 - Lighting Simulation - Streetlight
*****/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 500

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON       0 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
	float r, g, b;
} color_t;

typedef struct {
	int y;
} pole_t;

typedef struct {
	vec_t   pos;
	color_t color;
	vec_t   vel;
} light_t;

typedef struct {
	vec_t pos;
	vec_t N;
} vertex_t;

#define NUM 5
pole_t pole[5];
light_t light[5] =
{
  { { -325, 90 }, { 1 , 1 , 0.1  }, { 3, -2} },
  { { -125, 90 }, { 0.9 , 0.6 , 0.2 }, {-2, -1} },
  { { 2, 90 }, { 1, 0, 0 }, { 3,  2} },
  { { 125, 90 }, { 0.3 , 0 , 1 }, { 3, -2} },
  { { 325, 90 }, { 0.9 , 0.1 , 1  }, { 3, -2} }
};

color_t mulColor(float k, color_t c) {
	color_t tmp = { k * c.r, k * c.g, k * c.b };
	return tmp;
}

color_t addColor(color_t c1, color_t c2) {
	color_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
	return tmp;
}

float sum = 0;
int n;
// To add distance into calculation
// when distance is 0  => its impact is 1.0
// when distance is 350 => impact is 0.0
// Linear impact of distance on light calculation.
double ColorDensity(double d) {

	return (-1.0 / 400.0) * d + 1.0;

}

color_t calculateColor(light_t source, vertex_t v) {
	vec_t L = subV(source.pos, v.pos);
	vec_t uL = unitV(L);
	float factor = dotP(uL, v.N) * ColorDensity(magV(L));
	return mulColor(factor, source.color);
}


//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}



//
// To display onto window using OpenGL commands
//
void display() {

	float output[5];
	float xa, ya, x1, y1;
	int m;
	vec_t delta;
	
	for (m = 0; m < 5; m++) //color density finding for each one
	{
		delta.x = light[m].pos.x - light[m].pos.x;
		delta.y = light[m].pos.y - (-100);

		x1 = delta.x;
		y1 = delta.y;

		xa = x1 * x1;
		ya = y1 * y1;

		output[m] = ((-1.0 / 400.0) * (sqrt(xa + ya)) + 1.0);

	}

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//direk
	glColor3f(0.9, 0.9, 0.9);
	glRectf(0, -1000 + pole[2].y, 5, 89 + pole[2].y);

	glColor3f(0.9, 0.9, 0.9);
	glRectf(0, -1000 + pole[2].y, 5, 89 + pole[2].y);

	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_POLYGON);
	glVertex2f(210, 81 + pole[3].y);
	glVertex2f(125, 89 + pole[3].y);
	glVertex2f(200, 79 + pole[3].y);
	glVertex2f(205, 81 + pole[3].y);
	glEnd();

	glColor3f(0.9, 0.9, 0.9);
	glRectf(200, -1000 + pole[3].y, 205, 79 + pole[3].y);

	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_POLYGON);
	glVertex2f(410, 81 + pole[4].y);
	glVertex2f(325, 89 + pole[4].y);
	glVertex2f(400, 79 + pole[4].y);
	glVertex2f(405, 81 + pole[4].y);
	glEnd();

	glColor3f(0.9, 0.9, 0.9);
	glRectf(400, -1000 + pole[4].y, 405, 79 + pole[4].y);


	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_POLYGON);
	glVertex2f(-210, 81 + pole[1].y);
	glVertex2f(-125, 89 + pole[1].y);
	glVertex2f(-200, 79 + pole[1].y);
	glVertex2f(-205, 81 + pole[1].y);
	glEnd();

	glColor3f(0.9, 0.9, 0.9);
	glRectf(-200, -1000 + pole[1].y, -205, 79 + pole[1].y);


	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_POLYGON);
	glVertex2f(-410, 81 + pole[0].y);
	glVertex2f(-325, 89 + pole[0].y);
	glVertex2f(-400, 79 + pole[0].y);
	glVertex2f(-405, 81 + pole[0].y);
	glEnd();

	glColor3f(0.9, 0.9, 0.9);
	glRectf(-400, -1000 + pole[0].y, -405, 79 + pole[0].y);

	//direk
	
	
	for (m = 0; m < 5; m++)
	{
		glColor3f(light[m].color.r, light[m].color.g, light[m].color.b);
		vprint2(light[m].pos.x - 65, light[m].pos.y + 10, 0.15, "%.1f%%", (output[m] * 100));

	}

	for (m = 0; m < 3; m++)
	{
		glColor3f(light[m].color.r, light[m].color.g, light[m].color.b);
		vprint2(light[m].pos.x - 65, pole[m].y + 10, 0.11, "%.0f cm", (light[m].pos.y + 100) * 3);
	}
	for (m = 3; m < 5; m++)
	{
		glColor3f(light[m].color.r, light[m].color.g, light[m].color.b);
		vprint2(light[m].pos.x + 90, pole[m].y + 10, 0.11, "%.0f cm", (light[m].pos.y + 100) * 3);
	}

	// light source 
	for (int i = 0; i < NUM; i++) {
		glColor3f(light[i].color.r, light[i].color.g, light[i].color.b);
		circle(light[i].pos.x, light[i].pos.y, 10);
	}

	for (int x = -500; x <= 500; x++) {
		vertex_t P = { { x, -100 }, { 0, 1 } };

		color_t res = { 0, 0, 0 };
		for (int i = 0; i < NUM; i++) {
			res = addColor(res, calculateColor(light[i], P));
		}

		glBegin(GL_LINES);
		glColor3f(res.r, res.g, res.b);
		glVertex2f(x, -100);

		glColor3f(0, 0, 0);
		glVertex2f(x, -250);
		glEnd();

		//lanes
		glColor3f(0.9, 0.9, 0.9);
		glRectf(-200, -167, -273, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(-400, -167, -473, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(-520, -167, -493, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(-300, -167, -373, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(-200, -167, -273, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(-100, -167, -173, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(0, -167, -73, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(100, -167, 23, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(200, -167, 123, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(300, -167, 223, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(400, -167, 323, -185);
		glColor3f(0.9, 0.9, 0.9);
		glRectf(490, -167, 423, -185);
	
	}

	for (m = 0; m < NUM; m++)
	{
		
		if (light[m].pos.y < -20)
		{
			glColor3f(1 , 0 , 0.2);
			vprint2(-490, 230, 0.15, "Too low for");
			vprint2(-476, 200, 0.15, "car pass!");
		}

		if (light[m].pos.y > -100 && light[m].pos.y < 250)
		{
			if (100 - (100 * (output[0] + output[1] + output[2] + output[3] + output[4]) / 5) > 50)
			{
				glColor3f(0.5, 1, 1);
				vprint2(+305, -235, 0.15, "Covered area: %.0f%%", 2 * 100 * (output[0] + output[1] + output[2] + output[3] + output[4]) / 5);
			}
			else
			{
				glColor3f(0.5, 1, 1);
				vprint2(+305, -235, 0.15, "Covered area: %.0f%%", 2 * (100 - (100 * (output[0] + output[1] + output[2] + output[3] + output[4]) / 5)));
			}
			glColor3f(0.5, 1, 1);
			vprint2(-490, -235, 0.15, "Color Density: %.0f%%", 100 * (output[0] + output[1] + output[2] + output[3] + output[4]) / 5);

			if ((100 * (output[0] + output[1] + output[2] + output[3] + output[4]) / 5 < 30) || (2 * (100 - (100 * (output[0] + output[1] + output[2] + output[3] + output[4]) / 5))) < 10)
			{
				glColor3f(0.8, 0.1, 0.2);
				vprint2(-100, -235, 0.15, "NOT SAFE TO DRIVE!");
			}

				if ((light[1].pos.y < 175) && (light[2].pos.y < 175) && (light[3].pos.y < 175) && (light[4].pos.y < 175))
				{
					glColor3f(0.7 , 1 , 1);
					vprint2(305, +230, 0.15, "F(1-5) - Increase");
					vprint2(306, +200, 0.15, "(1-5) - Decrease");
					vprint2(10, +230, 0.15, "SPACE - Increase All");
					vprint2(-20, +200, 0.15, "BACKSPACE - Decrease All");
					glColor3f(0.8 , 0 , 0.5);
					vprint2(-312, +230, 0.15, "Merter Eker 21704343");
					vprint2(-300, +200, 0.15, "Streetlight Simulation");
				}
			
		}
	}

	glutSwapBuffers();

}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	
	for (int i = 0; i < NUM; i++) {
	
			if (key == ' ')
			{
				light[i].pos.y += 3;
				pole[i].y += 3;
			}

			if (key == 8)
			{
				light[i].pos.y -= 3;
				pole[i].y -= 3;
			}
			
	}

	switch (key)
	{
	case 49: light[0].pos.y -= 3; 
		pole[0].y -= 3; break;
	case 50: light[1].pos.y -= 3;
		pole[1].y -= 3; break;
	case 51: light[2].pos.y -= 3; 
		pole[2].y -= 3; break;
	case 52: light[3].pos.y -= 3; 
		pole[3].y -= 3; break;
	case 53: light[4].pos.y -= 3; 
		pole[4].y -= 3; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	case GLUT_KEY_F1: light[0].pos.y += 3;
		pole[0].y += 3; break;
	case GLUT_KEY_F2: light[1].pos.y += 3;
		pole[1].y += 3; break;
	case GLUT_KEY_F3: light[2].pos.y += 3;
		pole[2].y += 3; break;
	case GLUT_KEY_F4: light[3].pos.y += 3;
		pole[3].y += 3; break;
	case GLUT_KEY_F5: light[4].pos.y += 3;
		pole[4].y += 3; break;
	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	
	// to refresh the window it calls display() function
	glutPostRedisplay(); 

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Streetlight Simulation by Merter Eker");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}
