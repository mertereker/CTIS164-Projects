//164 MIDTERM QUESTION -TIC TOC-
/*******
STUDENT : Merter Eker
SECTION : 3
HOMEWORK: MIDTERM
Time should be entered with : sign like (hh:mm).
I want to use trigonometric functions' benefits on C dynamically. This module can be used in games and applications for different purposes (setting alarm, solving puzzle etc.).
Time should be set as clockwisely.
----------
PROBLEMS: ALL OF THE FUNCTIONS ARE WORKING PROPERLY.
----------
ADDITIONAL FEATURES:
Minute-hand movement stops when it reaches the exact time.
It works like a analog watch so it doesn't do calculations by AM/PM system, logically.
Time difference is showing.
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 400

#define  TIMER_PERIOD  16 // 60fps
#define  TIMER_ON       1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, spacebar = false;
int  winWidth, winHeight; // current Window width and height

/* User defined Types and Variables */
typedef struct a{
	float x, y;
} point_t;

typedef struct b{

	int currenthour, showhourc, currentmin, current;
	int requesthour, showhourr, requestmin, request;
	int diff, degree, rotate = 0;

} clock_t;

typedef struct {
	point_t pos;
	clock_t time;// position of the object
	float   angle;  // view angle 
	float   Bangle;  // view angle 
	float   r;
} hand_t;

bool check = true;
hand_t h = { {0,0}, 45, 20 };
clock_t time;

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


void clock() {

	glColor3f(1.0, 0.3, 0.3);
	circle(-150, 0, 77);
	glColor3f(1, 0.9, 0.3);
	circle(-150, 0, 75);

	glColor3f(0, 0, 0);
	vprint(-158, 55, GLUT_BITMAP_8_BY_13, "12");
	vprint(-155, -70, GLUT_BITMAP_8_BY_13, "6");
	vprint(-220, -10, GLUT_BITMAP_8_BY_13, "9");
	vprint(-90, -10, GLUT_BITMAP_8_BY_13, "3");
	vprint(-100, -35, GLUT_BITMAP_8_BY_13, "4");
	vprint(-123, -59, GLUT_BITMAP_8_BY_13, "5");
	vprint(-210, -35, GLUT_BITMAP_8_BY_13, "8");
	vprint(-185, -59, GLUT_BITMAP_8_BY_13, "7");
	vprint(-213, 22, GLUT_BITMAP_8_BY_13, "10");
	vprint(-195, 46, GLUT_BITMAP_8_BY_13, "11");
	vprint(-120, 46, GLUT_BITMAP_8_BY_13, "1");
	vprint(-97, 22, GLUT_BITMAP_8_BY_13, "2");

}


void drawhand(hand_t tp) {

	int i = 0;
	glColor3f(0, 1, 0);
	glLineWidth(3);


	if (h.angle == 90)
	{
		h.Bangle -= 30; //this move the hour hand one forward
	}


	glBegin(GL_LINES);
	glVertex2f(-150, -5);
	glVertex2f(-150 + 50 * cos(tp.angle * D2R), -5 + 50 * sin(tp.angle * D2R)); //minute-hand takes position regarding to the sin cos values
	glEnd();
	glLineWidth(1);

	glColor3f(1, 0, 1);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(-150, -5);
	glVertex2f(-150 + 40 * cos(tp.Bangle * D2R), -5 + 40 * sin(tp.Bangle * D2R)); //hour-hand takes position regarding to the sin cos values
	glEnd();
	glLineWidth(1);
	i++;
	glColor3f(1, 1, 1);
	vprint(tp.pos.x - 50, tp.pos.y - 100, GLUT_BITMAP_8_BY_13, "You need to rotate %d degree clock-wisely", time.degree);
	vprint(tp.pos.x - 50, tp.pos.y - 25, GLUT_BITMAP_8_BY_13, "Initial time: %0.2d:%0.2d ", time.showhourc, time.currentmin);
	vprint(tp.pos.x - 50, tp.pos.y - 50, GLUT_BITMAP_8_BY_13, "Target time: %0.2d:%0.2d ", time.showhourr, time.requestmin);
	vprint(tp.pos.x - 50, tp.pos.y - 75, GLUT_BITMAP_8_BY_13, "Time Difference: %0.2d:%0.2d", time.diff / 60, time.diff % 60);
	vprint(tp.pos.x - 50, tp.pos.y + 75, GLUT_BITMAP_8_BY_13, "Press -> to move minute-hand");
	vprint(tp.pos.x - 50, tp.pos.y + 50, GLUT_BITMAP_8_BY_13, "Merter Eker");
	vprint(tp.pos.x - 50, tp.pos.y + 25, GLUT_BITMAP_8_BY_13, "21704343");
	vprint(tp.pos.x - 50, tp.pos.y + 0, GLUT_BITMAP_8_BY_13, "MIDTERM QUESTION");
	if (!check)
		vprint(tp.pos.x - 50, tp.pos.y - 125, GLUT_BITMAP_8_BY_13, "DONE!");

}

void display()
{

	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	clock();
	drawhand(h);

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
	if (key == ' ')
		spacebar = true;
	// to refresh the window it calls display() function
	//glutPostRedisplay() ;
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ')
		spacebar = false;
	// to refresh the window it calls display() function
	//glutPostRedisplay();
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
   //glutPostRedisplay() ;
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
   //glutPostRedisplay() ;
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
	//glutPostRedisplay() ; 
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
	//glutPostRedisplay() ;
}

void onMove(int x, int y) {
	// Write your codes here.



	 // to refresh the window it calls display() function
	 //glutPostRedisplay() ;
}

void turnHand(hand_t *tp, float inc) {
	tp->angle += inc;
	if (tp->angle > 360) tp->angle -= 360;
	if (tp->angle < 0) tp->angle += 360;
}


#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	if (right) {


		if (check)
		{
			turnHand(&h, -1);

			if (time.degree > 0)
				time.degree--; //degree check

			else
				check = false;
		}
	}

	if (left) {
		turnHand(&h, 1);
		time.degree++; //if user press <- increase the needed degree
	}


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
	glutCreateWindow("Tic Toc by Merter Eker");

	do {
		printf("Enter current time (Enter hh:mm): ");
		scanf_s("%d : %d", &time.currenthour, &time.currentmin);

		if (time.currenthour >= 12) //this is for AM/PM difference
		{
			time.showhourc = time.currenthour;
			time.currenthour -= 12;
		}

		else
			time.showhourc = time.currenthour;

		} while (!((time.currenthour < 24) && (time.currentmin < 60))); //data validation

	do {
		printf("Enter target time (Enter hh:mm): ");
		scanf_s("%d : %d", &time.requesthour, &time.requestmin);

		if (time.requesthour >= 12) //this is for AM/PM difference
		{
			time.showhourr = time.requesthour;
			time.requesthour -= 12;
		}

		else
			time.showhourr = time.requesthour;

	} while (!((time.requesthour < 24) && (time.requestmin < 60))); //data validation

	h.angle = 90 - (30 * time.currentmin / 5); //initial point of minute-hand

	time.current = (time.currenthour * 60) + time.currentmin; //calculate in minute form
	time.request = (time.requesthour * 60) + time.requestmin; //calculate in minute form

	if (time.request > time.current) //comparison
		time.diff = time.request - time.current;

	else
		time.diff = time.current - time.request;

	time.degree = time.diff * 30 / 5; //this calculates how many degree is needed to reach

	if (time.currenthour != 12) // hour-hand check
		h.Bangle = 90 - (30 * time.currenthour);

	else
		h.Bangle = 90;

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
