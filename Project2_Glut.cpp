/*********
   CTIS164 - Template Source Program
----------
STUDENT : Merter Eker
SECTION : 3
HOMEWORK: HW2
----------
PROBLEMS: ALL OF THE FUNCTIONS ARE WORKING PROPERLY. REQUIREMENTS DONE.
----------
ADDITIONAL FEATURES:
Each duck has a different color and *constantly* changing their colors.
There is a prize function that calculates point and determines the prize and showing right after the game.
The point bar change dynamically depend on the points, if point exceeds the bar new section is opening with a different color. Showing point.
There are two difficulty level for players. On easy mode user can move the arrow.
Ducks move depending on the speed option.
Player has 3 different timer option which are 15, 30 and 45 seconds.
The game has 5 second interval at start for user to read game properties.(*if the first 3-5 ENTER click doesn't work*).
Game state and enum operations used.
All of the screen filled with designs and working properly.
*********/


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define MAX 1000
#define  START 0
#define  RUN   1
#define  END   2
#define  DURATION   15
#define SHORT 1
#define LONG 2
#define VERYLONG 3
#define START_POINT -285
#define DSTART_POINT -700
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  1 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

enum position_t { TOP = 225, MIDDLE = 45, BOTTOM = -117 }; //to be user friendly
enum speed_t { FAST = 20, MODERATE = 25, SLOW = 30 }; //to be user friendly

typedef struct a {

	int xapos;
	int yapos = START_POINT;

} arrow_t;

typedef struct b {

	int xdpos;
	int ydpos = MIDDLE;
	arrow_t arrow;
	position_t position;
	speed_t speed;
} game_t;

/* Global Variables for Template File */

bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int x, r = 0, count = 0, d, c, b, cnt, randomset, xeksen, yeksen, speed = FAST; //Position adjustment, record status and speed
int  randoma[MAX], randomd[MAX], drt = LONG, check[MAX]; //array declerations
bool activeTimer = true; //movement check
bool arrowstatus = true; //arrow display check
bool arrowfire = false; //arrow firing check
bool easymode = true; //arrow firing check
game_t shape[MAX]; //main decleration for positions
int appStatus = START; //game state
int timeCounter = 0, startcheck = 0; //timer
double red[MAX], green[MAX], blue[MAX]; //coloring

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
		angle = 2 * PI * i / 100;
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
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
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
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
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

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
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

void duck()
{


	for (d = 0; d < 10; d++)
	{

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 0, shape[d].ydpos + 2, randomd[d] + (shape + d)->xdpos + 2, shape[d].ydpos + -4);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 2, shape[d].ydpos + -4, randomd[d] + (shape + d)->xdpos + 4, shape[d].ydpos + -8);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 4, shape[d].ydpos + -8, randomd[d] + (shape + d)->xdpos + 6, shape[d].ydpos + -12);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 6, shape[d].ydpos + -12, randomd[d] + (shape + d)->xdpos + 10, shape[d].ydpos + -14);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 10, shape[d].ydpos + -14, randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + -16);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + -14, randomd[d] + (shape + d)->xdpos + 30, shape[d].ydpos + -12);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 30, shape[d].ydpos + -12, randomd[d] + (shape + d)->xdpos + 32, shape[d].ydpos + -8);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 32, shape[d].ydpos + -8, randomd[d] + (shape + d)->xdpos + 34, shape[d].ydpos + -2);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 32, shape[d].ydpos + -2, randomd[d] + (shape + d)->xdpos + 30, shape[d].ydpos + 0);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 30, shape[d].ydpos + 0, randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 2);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 2, randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + 4);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 4, randomd[d] + (shape + d)->xdpos + 32, shape[d].ydpos + 6);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 32, shape[d].ydpos + 6, randomd[d] + (shape + d)->xdpos + 34, shape[d].ydpos + 8);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 32, shape[d].ydpos + 8, randomd[d] + (shape + d)->xdpos + 30, shape[d].ydpos + 12);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 30, shape[d].ydpos + 12, randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 14);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 14, randomd[d] + (shape + d)->xdpos + 22, shape[d].ydpos + 16);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 22, shape[d].ydpos + 12, randomd[d] + (shape + d)->xdpos + 20, shape[d].ydpos + 14);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 18, shape[d].ydpos + 12, randomd[d] + (shape + d)->xdpos + 20, shape[d].ydpos + 6);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 20, shape[d].ydpos + 6, randomd[d] + (shape + d)->xdpos + 22, shape[d].ydpos + 4);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 22, shape[d].ydpos + 4, randomd[d] + (shape + d)->xdpos + 24, shape[d].ydpos + 2);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 6, shape[d].ydpos + 0, randomd[d] + (shape + d)->xdpos + 18, shape[d].ydpos + -2);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 6, shape[d].ydpos + 2, randomd[d] + (shape + d)->xdpos + 4, shape[d].ydpos + 0);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 2, shape[d].ydpos + 2, randomd[d] + (shape + d)->xdpos + 4, shape[d].ydpos + 4);

		glColor3f((red[d] / 256), (green[d] / 256), (blue[d] / 256)); //cizgi
		glRectf(randomd[d] + (shape + d)->xdpos + 18, shape[d].ydpos + 2, randomd[d] + (shape + d)->xdpos + 22, shape[d].ydpos + 0);


		//boya

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 6, shape[d].ydpos + -2, randomd[d] + (shape + d)->xdpos + 30, shape[d].ydpos + -12);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 22, shape[d].ydpos + 2, randomd[d] + (shape + d)->xdpos + 24, shape[d].ydpos + 0);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + 2, randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 0);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 10, shape[d].ydpos + -12, randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + -14);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 18, shape[d].ydpos + -2, randomd[d] + (shape + d)->xdpos + 30, shape[d].ydpos + 0);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 30, shape[d].ydpos + -2, randomd[d] + (shape + d)->xdpos + 32, shape[d].ydpos + -8);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 4, shape[d].ydpos + -8, randomd[d] + (shape + d)->xdpos + 6, shape[d].ydpos + 0);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 2, shape[d].ydpos + 2, randomd[d] + (shape + d)->xdpos + 4, shape[d].ydpos + -4);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 22, shape[d].ydpos + 12, randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 14);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 24, shape[d].ydpos + 6, randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + 0);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 20, shape[d].ydpos + 12, randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + 6);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 20, shape[d].ydpos + 12, randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + 6);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 22, shape[d].ydpos + 4, randomd[d] + (shape + d)->xdpos + 24, shape[d].ydpos + 6);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + 4, randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 8);

		glColor3f(0.9, 0.4, 0); //agýz
		glRectf(randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 6, randomd[d] + (shape + d)->xdpos + 32, shape[d].ydpos + 8);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 12, randomd[d] + (shape + d)->xdpos + 30, shape[d].ydpos + 8);

		glColor3f(0.0, 0.2, 0.2); //goz
		glRectf(randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 10, randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + 8);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 12, randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + 10);

		glColor3f(0.9, 0.8, 0); //boya
		glRectf(randomd[d] + (shape + d)->xdpos + 28, shape[d].ydpos + 6, randomd[d] + (shape + d)->xdpos + 26, shape[d].ydpos + 8);

	}


}

void pointbar()
{

	glColor3f(0.6, 0.3, 0.8); //cerceve
	glRectf(-677, 202, -623, -152);

	glColor3f(0.8, 0.9, 1); //cerceve
	glRectf(-675, 200, -625, -150);

	glColor3ub(90, 100, 140);
	vprint2(-674, 223, 0.14, "POINT");
	glColor3ub(90, 100, 140);
	vprint2(-669, 205, 0.15, "BAR");

	if (((count * 9) > 10) && ((count * 9) < 30))
	{
		glColor3ub(90, 100, 140);
		vprint2(-675, -175, 0.10, "GO ON!");
	}
	else if (((count * 9) >= 30) && ((count * 9) <= 58))
	{
		glColor3ub(90, 100, 140);
		vprint2(-665, -175, 0.10, "WOW!");
	}

	if ((count * 9) <= 58)
	{
		glColor3f(0.7, 1, 0.2); //BAR
		glRectf(-675, -150, -625, -150 + ((count * 9) * 6));
	}
	else if ((count * 9) > 58)
	{
		glColor3f(0.7, 0, 0.3); //BAR
		glRectf(-675, -150, -625, -150 + (((count * 9) - 58) * 6));
		glColor3ub(90, 100, 140);
		vprint2(-675, -175, 0.10, "AMAZED!");
	}

	glColor3ub(90, 100, 140);
	vprint2(-663, 25, 0.16, "%d", count * 9);


}

void gamearea()
{
	int p;

	glColor3f(0.1, 0.6, 0.8); //background
	glRectf(-700, 350, 700, -275);



	for (int b = -630; b < 675; b += 80)
	{
		glBegin(GL_LINES); //cýzgý
		glColor3f(0, 0, 0);
		glVertex2f(b, 350);
		glVertex2f(b, -250);
		glEnd();
	}

	glColor3f(0, 0, 0); //path
	glRectf(-700, 231, 700, 168);
	glColor3f(0.7, 0.8, 0.8); //path
	glRectf(-700, 230, 700, 170);

	glColor3f(0, 0, 0); //path
	glRectf(-700, 71, 700, 8);
	glColor3f(0.7, 0.8, 0.8); //path
	glRectf(-700, 70, 700, 10);

	glColor3f(0, 0, 0); //path
	glRectf(-700, -89, 700, -152);
	glColor3f(0.7, 0.8, 0.8); //path
	glRectf(-700, -90, 700, -150);


	glBegin(GL_LINES); //serit0
	glColor3f(0.234, 0.324, 0.464);
	glVertex2f(-700, 210);
	glVertex2f(700, 210);
	glVertex2f(-700, 190);
	glVertex2f(700, 190);
	glVertex2f(-700, 50);
	glVertex2f(700, 50);
	glVertex2f(-700, 30);
	glVertex2f(700, 30);
	glVertex2f(-700, -110);
	glVertex2f(700, -110);
	glVertex2f(-700, -130);
	glVertex2f(700, -130);
	glEnd();


}

void bars()
{
	int p;



	glColor3f(0.9, 0.9, 0.7); //cizgi
	glRectf(-700, 350, -598, -350);
	glColor3f(0.9, 0.9, 1); //cizgi
	glRectf(-700, 350, -600, -350);

	glColor3f(0.9, 0.9, 0.7); //cizgi
	glRectf(598, 350, 700, -350);
	glColor3f(0.9, 0.9, 1); //cizgi
	glRectf(600, 350, 700, -350);

	for (p = -325; p < 325; p += 80)
	{
		glColor3f(0.9, 0.1, 0.3);
		glBegin(GL_QUADS);
		glVertex2f(600, p);
		glVertex2f(600, p + 25);
		glVertex2f(700, p + 50);
		glVertex2f(700, p + 25);
		glEnd();

		glColor3f(0.9, 0.1, 0.3);
		glBegin(GL_QUADS);
		glVertex2f(-600, p);
		glVertex2f(-600, p + 25);
		glVertex2f(-700, p + 50);
		glVertex2f(-700, p + 25);
		glEnd();
	} //candybar

	glColor3f(0.9, 0.8, 0); //UCGEN
	glRectf(-700, 275, -600, 325);

	glColor3f(0.9, 0.8, 0); //UCGEN
	glRectf(+700, 275, +600, 325);

	glColor3f(0.9, 0.8, 0); //UCGEN
	glRectf(+700, -225, +600, -250);

	glColor3f(0.9, 0.8, 0); //UCGEN
	glRectf(-700, -225, -600, -250);


	glColor3f(0.9, 0.8, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(-680, 275);
	glVertex2f(-620, 275);
	glVertex2f(-650, 250);
	glVertex2f(-620, 275);
	glVertex2f(-600, 275);
	glVertex2f(-600, 250);
	glVertex2f(-680, 275);
	glVertex2f(-700, 275);
	glVertex2f(-700, 250);

	glVertex2f(680, 275);
	glVertex2f(620, 275);
	glVertex2f(650, 250);
	glVertex2f(620, 275);
	glVertex2f(600, 275);
	glVertex2f(600, 250);
	glVertex2f(680, 275);
	glVertex2f(700, 275);
	glVertex2f(700, 250);

	glVertex2f(680, -225);
	glVertex2f(620, -225);
	glVertex2f(650, -200);
	glVertex2f(620, -225);
	glVertex2f(600, -225);
	glVertex2f(600, -200);
	glVertex2f(680, -225);
	glVertex2f(700, -225);
	glVertex2f(700, -200);

	glVertex2f(-680, -225);
	glVertex2f(-620, -225);
	glVertex2f(-650, -200);
	glVertex2f(-620, -225);
	glVertex2f(-600, -225);
	glVertex2f(-600, -200);
	glVertex2f(-680, -225);
	glVertex2f(-700, -225);
	glVertex2f(-700, -200);

	glEnd();



	glColor3f(0.9, 0.1, 0.3);
	glBegin(GL_QUADS);
	glVertex2f(-700, -350);
	glVertex2f(-700, -250);
	glVertex2f(0, -200);
	glVertex2f(0, -350);
	glEnd();

	glColor3f(0.9, 0.1, 0.3);
	glBegin(GL_QUADS);
	glVertex2f(0, -350);
	glVertex2f(0, -200);
	glVertex2f(700, -250);
	glVertex2f(700, -350);
	glEnd();


	for (p = -600; p < 700; p += 300)
	{
		glColor3f(1, 0.9, 0.2); //circle
		circle(p, 590, 308);
		glColor3f(0.9, 0.1, 0.3); //circle
		circle(p, 590, 304);
		glColor3f(0.2, 0.9, 1); //circle
		circle(p, 590, 300);

	}

	for (p = -725; p < 700; p += 63)
	{
		glColor3f(0.9, 0.1, 0.3); //circledekor
		circle(p, 365, 30);
	}

}

void arrow()
{

	glColor3f(0.1, 0.9, 0.7); //cizgi
	glRectf(shape->arrow.xapos + -4, shape->arrow.yapos + 4, shape->arrow.xapos + 3, shape->arrow.yapos + -60);

	glColor3f(0.9, 0.1, 0.3);
	glBegin(GL_QUADS);
	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + 8);
	glVertex2f(shape->arrow.xapos + -8, shape->arrow.yapos + 0);
	glVertex2f(shape->arrow.xapos + -4, shape->arrow.yapos + 0);
	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + 4);
	glEnd();
	glColor3f(0.9, 0.1, 0.3);
	glBegin(GL_QUADS);
	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + 8);
	glVertex2f(shape->arrow.xapos + 8, shape->arrow.yapos + 0);
	glVertex2f(shape->arrow.xapos + 4, shape->arrow.yapos + 0);
	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + 4);
	glEnd();
	glColor3f(0.9, 0.1, 0.3);
	glBegin(GL_QUADS);
	glVertex2f(shape->arrow.xapos + -4, shape->arrow.yapos + 0);
	glVertex2f(shape->arrow.xapos + -8, shape->arrow.yapos + -4);
	glVertex2f(shape->arrow.xapos + -6, shape->arrow.yapos + -6);
	glVertex2f(shape->arrow.xapos + -2, shape->arrow.yapos + -2);
	glEnd();
	glColor3f(0.9, 0.1, 0.3);
	glBegin(GL_QUADS);
	glVertex2f(shape->arrow.xapos + 4, shape->arrow.yapos + 0);
	glVertex2f(shape->arrow.xapos + 8, shape->arrow.yapos + -4);
	glVertex2f(shape->arrow.xapos + 6, shape->arrow.yapos + -6);
	glVertex2f(shape->arrow.xapos + 2, shape->arrow.yapos + -2);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2f(shape->arrow.xapos + -5, shape->arrow.yapos + -10);
	glVertex2f(shape->arrow.xapos + -6, shape->arrow.yapos + -8);
	glVertex2f(shape->arrow.xapos + -4, shape->arrow.yapos + -8);
	glVertex2f(shape->arrow.xapos + -3, shape->arrow.yapos + -10);

	glVertex2f(shape->arrow.xapos + -5, shape->arrow.yapos + -20);
	glVertex2f(shape->arrow.xapos + -6, shape->arrow.yapos + -18);
	glVertex2f(shape->arrow.xapos + -4, shape->arrow.yapos + -18);
	glVertex2f(shape->arrow.xapos + -3, shape->arrow.yapos + -20);

	glVertex2f(shape->arrow.xapos + -5, shape->arrow.yapos + -30);
	glVertex2f(shape->arrow.xapos + -6, shape->arrow.yapos + -28);
	glVertex2f(shape->arrow.xapos + -4, shape->arrow.yapos + -28);
	glVertex2f(shape->arrow.xapos + -3, shape->arrow.yapos + -30);

	glVertex2f(shape->arrow.xapos + -5, shape->arrow.yapos + -30);
	glVertex2f(shape->arrow.xapos + -6, shape->arrow.yapos + -28);
	glVertex2f(shape->arrow.xapos + -4, shape->arrow.yapos + -28);
	glVertex2f(shape->arrow.xapos + -3, shape->arrow.yapos + -30);

	glVertex2f(shape->arrow.xapos + -5, shape->arrow.yapos + -40);
	glVertex2f(shape->arrow.xapos + -6, shape->arrow.yapos + -38);
	glVertex2f(shape->arrow.xapos + -4, shape->arrow.yapos + -38);
	glVertex2f(shape->arrow.xapos + -3, shape->arrow.yapos + -40);

	glVertex2f(shape->arrow.xapos + -5, shape->arrow.yapos + -50);
	glVertex2f(shape->arrow.xapos + -6, shape->arrow.yapos + -48);
	glVertex2f(shape->arrow.xapos + -4, shape->arrow.yapos + -48);
	glVertex2f(shape->arrow.xapos + -3, shape->arrow.yapos + -50);

	glVertex2f(shape->arrow.xapos + 4, shape->arrow.yapos + -50);
	glVertex2f(shape->arrow.xapos + 5, shape->arrow.yapos + -48);
	glVertex2f(shape->arrow.xapos + 3, shape->arrow.yapos + -48);
	glVertex2f(shape->arrow.xapos + 2, shape->arrow.yapos + -50);

	glVertex2f(shape->arrow.xapos + 4, shape->arrow.yapos + -40);
	glVertex2f(shape->arrow.xapos + 5, shape->arrow.yapos + -38);
	glVertex2f(shape->arrow.xapos + 3, shape->arrow.yapos + -38);
	glVertex2f(shape->arrow.xapos + 2, shape->arrow.yapos + -40);

	glVertex2f(shape->arrow.xapos + 4, shape->arrow.yapos + -30);
	glVertex2f(shape->arrow.xapos + 5, shape->arrow.yapos + -28);
	glVertex2f(shape->arrow.xapos + 3, shape->arrow.yapos + -28);
	glVertex2f(shape->arrow.xapos + 2, shape->arrow.yapos + -30);

	glVertex2f(shape->arrow.xapos + 4, shape->arrow.yapos + -20);
	glVertex2f(shape->arrow.xapos + 5, shape->arrow.yapos + -18);
	glVertex2f(shape->arrow.xapos + 3, shape->arrow.yapos + -18);
	glVertex2f(shape->arrow.xapos + 2, shape->arrow.yapos + -20);

	glVertex2f(shape->arrow.xapos + 4, shape->arrow.yapos + -10);
	glVertex2f(shape->arrow.xapos + 5, shape->arrow.yapos + -8);
	glVertex2f(shape->arrow.xapos + 3, shape->arrow.yapos + -8);
	glVertex2f(shape->arrow.xapos + 2, shape->arrow.yapos + -10);
	glEnd();

	glColor3f(0.9, 0.1, 0.3);
	glBegin(GL_QUADS);
	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + -54);
	glVertex2f(shape->arrow.xapos + -6, shape->arrow.yapos + -60);
	glVertex2f(shape->arrow.xapos + -3, shape->arrow.yapos + -62);
	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + -60);

	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + -54);
	glVertex2f(shape->arrow.xapos + 6, shape->arrow.yapos + -60);
	glVertex2f(shape->arrow.xapos + 3, shape->arrow.yapos + -62);
	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + -60);
	glEnd();

	glColor3f(0.9, 0.1, 0.3);
	glBegin(GL_QUADS);
	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + -54);
	glVertex2f(shape->arrow.xapos + 6, shape->arrow.yapos + -56);
	glVertex2f(shape->arrow.xapos + -6, shape->arrow.yapos + -56);
	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + -62);

	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + -54);
	glVertex2f(shape->arrow.xapos + -6, shape->arrow.yapos + -56);
	glVertex2f(shape->arrow.xapos + 6, shape->arrow.yapos + -56);
	glVertex2f(shape->arrow.xapos + 0, shape->arrow.yapos + -62);

	glEnd();


}

void menu()
{
	glColor3f(1, 1, 1);
	vprint(-675, -283, GLUT_BITMAP_HELVETICA_12, "MERTER EKER");
	vprint(-675, -308, GLUT_BITMAP_HELVETICA_12, "21704343");
	vprint(-675, -333, GLUT_BITMAP_HELVETICA_12, "Dunk the Ducks");
	vprint(500, -283, GLUT_BITMAP_HELVETICA_12, "PRIZES");
	vprint(400, -308, GLUT_BITMAP_HELVETICA_12, "20: Baloon");
	vprint(550, -308, GLUT_BITMAP_HELVETICA_12, "30: Cinema Ticket");
	vprint(400, -333, GLUT_BITMAP_HELVETICA_12, "40: Tetris Game Boy");
	vprint(550, -333, GLUT_BITMAP_HELVETICA_12, "50: Teddy Bear");

	vprint(90, -283, GLUT_BITMAP_HELVETICA_12, "-GAME PROPERTIES-");
	vprint(0, -308, GLUT_BITMAP_HELVETICA_12, "TIMER : ");
	if (drt == SHORT)
		vprint(50, -308, GLUT_BITMAP_HELVETICA_12, " 15 sec");
	else if (drt == LONG)
		vprint(50, -308, GLUT_BITMAP_HELVETICA_12, " 30 sec");
	else if (drt == VERYLONG)
		vprint(50, -308, GLUT_BITMAP_HELVETICA_12, " 45 sec");

	vprint(25, -333, GLUT_BITMAP_HELVETICA_12, "DIFFICULTY : ");
	if (easymode)
		vprint(100, -333, GLUT_BITMAP_HELVETICA_12, " EASY (Arrow Can Move After Fired)");
	else
		vprint(100, -333, GLUT_BITMAP_HELVETICA_12, " HARD (Arrow Can Not Move After Fired)");

	vprint(225, -308, GLUT_BITMAP_HELVETICA_12, "SPEED : ");
	if (speed == FAST)
		vprint(275, -308, GLUT_BITMAP_HELVETICA_12, " FAST");
	else if (speed == MODERATE)
		vprint(275, -308, GLUT_BITMAP_HELVETICA_12, " MODERATE");
	else
		vprint(275, -308, GLUT_BITMAP_HELVETICA_12, " SLOW");

	vprint(-550, -283, GLUT_BITMAP_HELVETICA_12, "F1 : Change Timer");
	vprint(-425, -283, GLUT_BITMAP_HELVETICA_12, "F2 : Adjust Speed ");
	vprint(-550, -308, GLUT_BITMAP_HELVETICA_12, "SPACE : Shoot");
	vprint(-425, -308, GLUT_BITMAP_HELVETICA_12, "F3 : Change Difficulty");
	vprint(-515, -333, GLUT_BITMAP_HELVETICA_12, "<-  -> : Change Arrow Position");

	vprint2(-125, -250, 0.14, "PRESS ENTER TO START");
}


void prizes()
{

	glColor3f(0.1, 0.9, 0.7); //Prizebar
	glRectf(400, 232, -400, -150);

	if ((count * 9) >= 20)
	{
		glColor3f(1, 1, 1);
		vprint(-150, 150, GLUT_BITMAP_TIMES_ROMAN_24, "-= CONGRATULATIONS =-");

		if (((count * 9) >= 20) && ((count * 9) < 30))
		{
			vprint(-200, 75, GLUT_BITMAP_TIMES_ROMAN_24, "YOU WON BALOON WITH %d POINT!", count * 9);
			vprint(-85, 25, GLUT_BITMAP_TIMES_ROMAN_24, "DON'T MISS IT");

			vprint(-130, -100, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS F5 TO RESTART");
		}
		else if (((count * 9) >= 30) && ((count * 9) < 40))
		{
			vprint(-235, 75, GLUT_BITMAP_TIMES_ROMAN_24, "YOU WON CINEMA TICKET WITH %d POINT!", count * 9);
			vprint(-115, 25, GLUT_BITMAP_TIMES_ROMAN_24, "HAVE A GOOD TIME");

			vprint(-130, -100, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS F5 TO RESTART");
		}
		else if (((count * 9) >= 40) && ((count * 9) < 50))
		{
			vprint(-190, 75, GLUT_BITMAP_TIMES_ROMAN_24, "YOU WON TETRIS WITH %d POINT!", count * 9);
			vprint(-202, 25, GLUT_BITMAP_TIMES_ROMAN_24, "MAY THE UPRIGHT PART WITH YOU");

			vprint(-145, -100, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS F5 TO REACH TOP");
		}
		else if ((count * 9) >= 50)
		{
			vprint(-205, 75, GLUT_BITMAP_TIMES_ROMAN_24, "YOU WON TEDDY BEAR WITH %d POINT!", count * 9);
			vprint(-125, 25, GLUT_BITMAP_TIMES_ROMAN_24, "WHAT A NICE FRIEND");

			vprint(-225, -100, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS F5 TO BREAK THE WORLD RECORD");
		}

	}

	else
	{
		glColor3f(1, 1, 1);
		vprint(-75, 100, GLUT_BITMAP_TIMES_ROMAN_24, "YOU GOT : %d", count * 9);
		vprint(-115, 41, GLUT_BITMAP_TIMES_ROMAN_24, "PLEASE TRY AGAIN!");

		vprint(-190, -100, GLUT_BITMAP_TIMES_ROMAN_24, "WHEN YOU FEEL READY PRESS F5");

	}

}



//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	gamearea(); //default game area

	switch (appStatus) {

	case START:
		bars();
		menu();
		break;

	case RUN:
		duck();
		bars();
		pointbar();
		arrow();
		break;

	case END:
		prizes();
		timeCounter = 0;
		shape->arrow.yapos = START_POINT;
		bars();
		break;

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

	if (key == ' ') //fire
		arrowfire = !arrowfire;

	if (key == 13) //enter to start
	{
		if (startcheck > 180) //time check
			appStatus = RUN;

	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == 13) //enter to start
	{
		if (startcheck > 180) //time check
			appStatus = RUN;
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

	if (key == GLUT_KEY_F1) {
		if (drt == SHORT)
			drt = LONG;

		else if (drt == LONG)
			drt = VERYLONG;

		else if (drt == VERYLONG)
			drt = SHORT;
	}

	if (key == GLUT_KEY_F3) {
		easymode = !easymode; //open and close easy mode
	}

	if (key == GLUT_KEY_F2) { //changes duck speed
		if (speed == MODERATE)
			speed = SLOW;
		else if (speed == SLOW)
			speed = FAST;
		else if (speed == FAST)
			speed = MODERATE;
	}

	if (key == GLUT_KEY_F5) { //restart
		appStatus = START;
		count = 0; //count becomes 0 to start again
		shape->arrow.yapos = START_POINT;
		shape->xdpos = DSTART_POINT;
	}

	if (!easymode) //if easy mode close arrow cannot move after fired
	{
		if (!arrowfire)
		{
			if (key == GLUT_KEY_LEFT) { //change position of arrow
				shape->arrow.xapos--;
			}

			if (key == GLUT_KEY_RIGHT) {
				shape->arrow.xapos++;
			}

		}
	}

	else //if easy mode open arrow can move after fired
	{

		if (key == GLUT_KEY_LEFT) { //change position of arrow
			shape->arrow.xapos--;
		}

		if (key == GLUT_KEY_RIGHT) {
			shape->arrow.xapos++;
		}

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

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {


	glutTimerFunc(speed, onTimer, 0);

	startcheck++; //provides a time for user to read game properties

	if (arrowfire)
	{
		if (arrowstatus)
		{
			shape->arrow.yapos += 2;

			if (shape->arrow.yapos > 325) //if arrow arrives top take it to the beginning point
				shape->arrow.yapos = START_POINT;

		}
	}


	if (appStatus == RUN) {
		timeCounter++; //timer
		if (speed == MODERATE)
		{
			if (timeCounter > (drt * 575)) {
				// time expired
				appStatus = END;

			}
		}
		else if (speed == SLOW)
		{
			if (timeCounter > (drt * 500)) {
				// time expired
				appStatus = END;

			}
		}
		else if (speed == FAST)
		{
			if (timeCounter > (drt * 700)) {
				// time expired
				appStatus = END;

			}
		}
	}

	d = 0;
	x = 0;
	c = 0;
	cnt = 0;


	do
	{

		for (b = 0; b < d; b++) //all of the functions checks whether the arrow hit the duck or not and sends information to other functions. Arrow going to starting point if the user hits
		{

			red[b] = rand() % 256;
			green[b] = rand() % 256;
			blue[b] = rand() % 256;

			if (((shape[b].xdpos >= -700) && (shape[b].xdpos <= 700)) && shape[b].ydpos == BOTTOM)
			{

				if (((shape[b].xdpos + shape->arrow.xapos >= 685)) && (shape[b].xdpos + shape->arrow.xapos <= 715) && (shape[b].ydpos - shape->arrow.yapos >= -15) && (shape[b].ydpos - shape->arrow.yapos <= 20))
				{
					shape[b].xdpos = DSTART_POINT;
					shape->arrow.yapos = START_POINT;
					cnt++;
					count += powf(cnt, 0);
					d = 0;
				}
			}

			else if (((shape[b].xdpos >= 701) && (shape[b].xdpos <= 1799)) && shape[b].ydpos == BOTTOM)
			{

				if (((shape[b].xdpos + shape->arrow.xapos >= 1590)) && (shape[b].xdpos + shape->arrow.xapos <= 1610) && (shape[b].ydpos - shape->arrow.yapos >= -15) && (shape[b].ydpos - shape->arrow.yapos <= 20))
				{
					shape[b].xdpos = DSTART_POINT;
					shape->arrow.yapos = START_POINT;
					cnt++;
					count += powf(cnt, 0);
					d = 0;

				}

			}

			else if (((shape[b].xdpos > 1800) && shape[b].ydpos) && shape[b].ydpos == BOTTOM)
			{

				if (((shape[b].xdpos + shape->arrow.xapos >= 2090)) && (shape[b].xdpos + shape->arrow.xapos <= 2110) && (shape[b].ydpos - shape->arrow.yapos >= -15) && (shape[b].ydpos - shape->arrow.yapos <= 20))
				{
					shape[b].xdpos = DSTART_POINT;
					shape->arrow.yapos = START_POINT;
					cnt++;
					count += powf(cnt, 0);
					d = 0;
				}

			}

			else if (((shape[b].xdpos >= -700) && (shape[b].xdpos <= 900)) && shape[b].ydpos == MIDDLE)
			{

				if ((shape[b].xdpos + shape->arrow.xapos >= 790) && (shape[b].xdpos + shape->arrow.xapos <= 810) && (shape[b].ydpos - shape->arrow.yapos >= -15) && (shape[b].ydpos - shape->arrow.yapos <= 25))
				{
					shape[b].xdpos = DSTART_POINT;
					shape->arrow.yapos = START_POINT;
					cnt++;
					count += powf(cnt, 0);
					d = 0;

				}

			}

			else if (((shape[b].xdpos > 901) && (shape[b].xdpos <= 1300)) && shape[b].ydpos == MIDDLE)
			{

				if ((shape[b].xdpos + shape->arrow.xapos >= 1140) && (shape[b].xdpos + shape->arrow.xapos <= 1160) && (shape[b].ydpos - shape->arrow.yapos >= -15) && (shape[b].ydpos - shape->arrow.yapos <= 25))
				{
					shape[b].xdpos = DSTART_POINT;
					shape->arrow.yapos = START_POINT;
					cnt++;
					count += powf(cnt, 0);
					d = 0;
				}

			}

			else if (((shape[b].xdpos >= -700) && (shape[b].xdpos <= 500)) && shape[b].ydpos == TOP)
			{

				if ((shape[b].xdpos + shape->arrow.xapos >= 340) && (shape[b].xdpos + shape->arrow.xapos <= 360) && (shape[b].ydpos - shape->arrow.yapos >= -10) && (shape[b].ydpos - shape->arrow.yapos <= 25))
				{
					shape[b].xdpos = DSTART_POINT;
					shape->arrow.yapos = START_POINT;
					cnt++;
					count += powf(cnt, 0);
					d = 0;

				}

			}

			else if (((shape[b].xdpos > 501) && (shape[b].xdpos <= 2200)) && shape[b].ydpos == TOP)
			{

				if ((shape[b].xdpos + shape->arrow.xapos >= 2040) && (shape[b].xdpos + shape->arrow.xapos <= 2060) && (shape[b].ydpos - shape->arrow.yapos >= -10) && (shape[b].ydpos - shape->arrow.yapos <= 25))
				{
					shape[b].xdpos = DSTART_POINT;
					shape->arrow.yapos = START_POINT;
					cnt++;
					count += powf(cnt, 0);
					d = 0;

				}

			}


			else if (((shape[b].xdpos > 2250) && (shape[b].xdpos <= 3100)) && shape[b].ydpos == TOP)
			{

				if ((shape[b].xdpos + shape->arrow.xapos >= 2950) && (shape[b].xdpos + shape->arrow.xapos <= 2970) && (shape[b].ydpos - shape->arrow.yapos >= -10) && (shape[b].ydpos - shape->arrow.yapos <= 25))
				{
					shape[b].xdpos = DSTART_POINT;
					shape->arrow.yapos = START_POINT;
					cnt++;
					count += powf(cnt, 0);
					d = 0;

				}

			}

		}



		if ((shape[d].xdpos < 5000)) //random numbers creating here to determine duck position
		{
			x++;
			c++;
			srand(time(NULL));
			randoma[x] = (rand() % (101)) - 350;
		}



		if (c % 9 == 0) //position adjustments
		{
			randomd[d] = randoma[x] - 50;
			shape[d].ydpos = TOP;
		}
		else if (c % 9 == 1)
		{
			randomd[d] = randoma[x] - 850;
			shape[d].ydpos = MIDDLE;
		}
		else if (c % 9 == 2)
		{
			randomd[d] = randoma[x] - 450;
			shape[d].ydpos = BOTTOM;
		}
		else if (c % 9 == 3)
		{
			randomd[d] = randoma[x] - 1750;
			shape[d].ydpos = TOP;
		}
		else if (c % 9 == 4)
		{
			randomd[d] = randoma[x] - 1350;
			shape[d].ydpos = BOTTOM;
		}
		else if (c % 9 == 5)
		{
			randomd[d] = randoma[x] - 2650;
			shape[d].ydpos = TOP;
		}
		else if (c % 9 == 6)
		{
			randomd[d] = randoma[x] - 850;
			shape[d].ydpos = MIDDLE;
		}
		else if (c % 9 == 7)
		{
			randomd[d] = randoma[x] - 1850;
			shape[d].ydpos = BOTTOM;
		}
		else if (c % 9 == 8)
		{
			randomd[d] = randoma[x] - 550;
			shape[d].ydpos = MIDDLE;
		}

		d++;
		r++;

		if (activeTimer) {

			for (int l = 0; l < 500; l++)
			{


				shape[l].xdpos++; //position changer 



				if ((shape[l].xdpos > 4000)) //this statement resets the duck' position
				{
					shape[l].xdpos = DSTART_POINT;
					d = 0;
					x = 0;

				}
			}

		}




	} while (d <= 10);



	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()


}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Dunk the Ducks by Merter Eker");

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