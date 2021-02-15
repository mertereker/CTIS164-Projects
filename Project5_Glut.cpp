/*****
   CTIS164
*****
STUDENT : Merter Eker
SECTION : 3
QUESTION 2 - Basic Vectors - Finding Angle
*****/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD   16 // Period for the timer.
#define TIMER_ON        1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
bool firstlaunch = true;

					/* Vector Library  - 2D */
typedef struct {
	double x, y;
} vec_t;

typedef struct {
	double magnitude, angle;
} polar_t;

double magV(vec_t v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

double angleV(vec_t v) {
	double angle = atan2(v.y, v.x) / D2R;
	return angle < 0 ? angle + 360 : angle;
}

vec_t addV(vec_t v1, vec_t v2) {
	vec_t tmp = { v1.x + v2.x, v1.y + v2.y };
	return tmp;
}

vec_t subV(vec_t v1, vec_t v2) {
	vec_t tmp = { v1.x - v2.x, v1.y - v2.y };
	return tmp;
}

vec_t mulV(double k, vec_t v) {
	vec_t tmp = { k * v.x, k * v.y };
	return tmp;
}

double dotP(vec_t v1, vec_t v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

vec_t unitV(vec_t v) {
	return mulV(1.0 / magV(v), v);
}

// convert from polar representation to rectangular representation
vec_t pol2rec(polar_t p) {
	return { p.magnitude * cos(p.angle * D2R),  p.magnitude * sin(p.angle * D2R) };
}

polar_t rec2pol(vec_t v) {
	return{ magV(v), angleV(v) };
}

/****
   V1 . V2 = ||V1|| . ||V2|| . Cos(angle)
*****/
double findAngle(vec_t v1, vec_t v2) {
	double magV1 = magV(v1);
	double magV2 = magV(v2);
	double dot = dotP(v1, v2);
	double angle = acos(dot / (magV1 * magV2)) / D2R; // in degree
	return angle;
}

/* Vector library Ends */

vec_t mouse;

typedef struct {
	double r, g, b;
} color_t;

/* User defined Types and variables for the application */
typedef struct {
	vec_t pos;
	polar_t polar;
	color_t color;   //color of the vector
	bool  visible;
} vector_t;

int vfx = -134, vfy = 99;
int vsx = -126, vsy = -115;

vector_t V[3] = {
   { { vfx, vfy }, {0, 0},  {0.4 , 1 , 0.1 }, true },
   { { vsx, vsy }, {0, 0},  {0, 1, 1}, true },
   { { 0, 0 }, {0, 0},  {1, 1, 0}, false }
};

int clickNo = 0, request=0;
char op;

double angleTwoVectors;

//
// to draw circle, center at (x,y)
//  radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	double angle;
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
	double angle;

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

void vprint2(int x, int y, double size, const char *string, ...) {
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

void drawBackground() {
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	//x-axis
	glVertex2f(-380, 0);
	glVertex2f(380, 0);
	//tip of arrow (left)
	glVertex2f(-360, 10);
	glVertex2f(-380, 0);
	glVertex2f(-360, -10);
	glVertex2f(-380, 0);
	//tip of arrow (right)
	glVertex2f(360, 10);
	glVertex2f(380, 0);
	glVertex2f(360, -10);
	glVertex2f(380, 0);
	//y-axis
	glVertex2f(0, 280);
	glVertex2f(0, -280);
	//tip of arrow (upper)
	glVertex2f(-10, 260);
	glVertex2f(0, 280);
	glVertex2f(10, 260);
	glVertex2f(0, 280);
	//tip of arrow (lower)
	glVertex2f(-10, -260);
	glVertex2f(0, -280);
	glVertex2f(10, -260);
	glVertex2f(0, -280);
	glEnd();
	vprint(-20, 240, GLUT_BITMAP_HELVETICA_18, "y");
	vprint(340, 10, GLUT_BITMAP_HELVETICA_18, "x");
	if (firstlaunch)
	{
		glColor3f(0.6, 0, 0.4);
		vprint(200, 265, GLUT_BITMAP_8_BY_13, "Press SPACE to draw another vector");
	}
	
	if (!firstlaunch)
	{
		glColor3f(0.5 , 0.1 , 0.5);
		vprint(400, 280, GLUT_BITMAP_8_BY_13, "(%.0f, %.0f)", mouse.x, mouse.y);
		vprint(-480, -265, GLUT_BITMAP_8_BY_13, "After drawing two vectors");
		vprint(-480, -250, GLUT_BITMAP_8_BY_13, "Click to add vector");
		vprint(-480, -280, GLUT_BITMAP_8_BY_13, "+/- to Add/Subtract vectors");
		glColor3f(0.7, 0.2, 0.1);
		vprint(40, -210, GLUT_BITMAP_8_BY_13, "1 - Find angle of first and third vector");
		vprint(40, -240, GLUT_BITMAP_8_BY_13, "2 - Find angle of second and third vector");
	}
}

vector_t setProperties(vector_t v) {
	v.polar = rec2pol(v.pos);
	v.visible = true;
	return v;
}

void drawVector(int no) {
	if (!V[no].visible) //if the vector is not constructed
		return;
	glColor3f(V[no].color.r, V[no].color.g, V[no].color.b);
	glBegin(GL_LINES);
	//vector
	glVertex2f(0, 0);
	glVertex2f(V[no].pos.x, V[no].pos.y);

	// arrow tip 1
	vec_t tip1 = pol2rec({ V[no].polar.magnitude * 0.95, V[no].polar.angle - 2 });
	glVertex2f(tip1.x, tip1.y);
	glVertex2f(V[no].pos.x, V[no].pos.y);

	// arrow tip 2
	vec_t tip2 = pol2rec({ V[no].polar.magnitude * 0.95, V[no].polar.angle + 2 });
	glVertex2f(tip2.x, tip2.y);
	glVertex2f(V[no].pos.x, V[no].pos.y);

	glEnd();

	vec_t textPos = pol2rec({ V[no].polar.magnitude + 25, V[no].polar.angle });
	
	if (firstlaunch)
	{
		vprint(-480, 280 - (no * 70), GLUT_BITMAP_8_BY_13,"V%d", no + 1);
		V[2].pos = addV(V[0].pos, V[1].pos);
		op = '+';
		V[2] = setProperties(V[2]);
		vprint(-480, 265 - (no * 70), GLUT_BITMAP_8_BY_13, "Rectangular: <%.0f, %.0f>", V[no].pos.x, V[no].pos.y);
		vprint(-480, 280 - (2 * 70), GLUT_BITMAP_8_BY_13, "V3 = V1 %c V2", op);
	}

	else
	{
		if (no == 2) {
			vprint(-480, 280 - (no * 70), GLUT_BITMAP_8_BY_13, "V3 = V1 %c V2", op);
		}
		else {
			vprint(-480, 280 - (no * 70), GLUT_BITMAP_8_BY_13, "V%d", no + 1);
		}
		vprint(-480, 265 - (no * 70), GLUT_BITMAP_8_BY_13, "Rectangular: <%.0f, %.0f>", V[no].pos.x, V[no].pos.y);
	}

}

void drawVectors() {
	for (int i = 0; i < 3; i++)
		drawVector(i);
	
	if (firstlaunch)
	{
		if (V[2].visible) {
			double angle = findAngle(V[0].pos, V[2].pos);
			vprint(40, -270, GLUT_BITMAP_8_BY_13, "Angle between First Point and Last Point : %.1f degree", angle);
		}
	}

	if (!firstlaunch)
	{
		if (V[2].visible) {
			double angle = findAngle(V[request].pos, V[2].pos);

			if(request == 0)
			vprint(40, -270, GLUT_BITMAP_8_BY_13, "Angle between First Point and Last Point : %.1f degree", angle);
			else if (request == 1)
			vprint(40, -270, GLUT_BITMAP_8_BY_13, "Angle between Second Point and Last Point : %.1f degree", angle);
		}
	}
	
}


void display()
{
	// clear window to black
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();
	drawVectors();

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

	switch (key) {
	case '+':
		V[2].pos = addV(V[0].pos, V[1].pos);
		op = '+';
		V[2] = setProperties(V[2]);

		break;
	case '-':
		V[2].pos = subV(V[0].pos, V[1].pos);
		op = '-';
		V[2] = setProperties(V[2]);
		break;
	}

	if (key == 50)
		request=1;
	if (key == 49)
		request=0;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == ' ')
	{
		firstlaunch = false;
		V[0].visible = V[1].visible = V[2].visible = false;
	}
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
	if(!firstlaunch)
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
		if (clickNo < 2) {
			V[clickNo].pos = mouse;
			V[clickNo] = setProperties(V[clickNo]);
			clickNo++;
		}
		else {
			V[0].visible = V[1].visible = V[2].visible = false;
			clickNo = 0;
		}
	}

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

	mouse = { x - winWidth / (double)2.0, winHeight / (double)2.0 - y };

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.


	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}


void main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Find Angle by Merter Eker");

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
