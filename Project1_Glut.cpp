/**********
   CTIS164 - Template Source Program
----------
STUDENT : Merter Eker 21704343
SECTION : 03
HOMEWORK: HW1
----------
PROBLEMS: ALL OF THE FUNCTIONS ARE WORKING PROPERLY. REQUIREMENTS DONE.
----------
ADDITIONAL FEATURES: 

***Rats are deployable only on the ground and table	reasonably.
***There is a threat level on the refrigerator which changes depending on the rat count.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  1// Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532
#define MAX 10
#define MAXRAT 10

typedef struct {

	int xpos;
	int ypos;

} kitchen_t;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int u, c, n, o , xeksen, yeksen; //rat area
int ratcount = 0; //rat number
bool activeTimer = false; //movement check
bool ratview = false; //rat display check
kitchen_t *shape;



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
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
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

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
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

void shadows()
{

	glColor3f(0.9 , 0.9 , 0.8);
	glRectf(-675, 42, 610, 38);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(618, 42, 624, 38);
	
	glColor3f(0.9, 0.9, 0.8);
	glRectf(630, 42, 642, 38);
	
	glColor3f(0.9, 0.9, 0.8);
	glRectf(648, 42, 654, 38);
	
	glColor3f(0.9, 0.9, 0.8);
	glRectf(664, 42, 674, 38);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(-650, 20, -630, 10);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(-633, 17, -627, -25);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(-633, -25, -630, -35);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(-633, -40, -630, -43);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(-633, -50, -630, -53);



	glColor3f(0.9, 0.9, 0.8);
	glRectf(650, 20, 630, 10);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(633, 17, 627, -25);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(633, -25, 630, -35);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(633, -40, 630, -43);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(633, -50, 630, -53);

	//*

	glColor3f(0.9, 0.9, 0.8);
	glRectf(610, 20, 610, 10);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(593, 17, 604, -25);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(593, -25, 610, -35);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(593, -40, 610, -43);

	glColor3f(0.9, 0.9, 0.8);
	glRectf(593, -50, 610, -53);



	for (int b = 250; b > 170; b -= 40)
	{
		glColor3f(0.9, 0.9, 0.8);
		glRectf(b, 20, b - 20, 10);

		glColor3f(0.9, 0.9, 0.8);
		glRectf(b - 17, 17, b - 23, -25);

		glColor3f(0.9, 0.9, 0.8);
		glRectf(b - 17, -25, b - 20, -35);

		glColor3f(0.9, 0.9, 0.8);
		glRectf(b - 17, -40, b - 20, -43);

		glColor3f(0.9, 0.9, 0.8);
		glRectf(b - 20, -50, b - 20, -53);
	}

	for (int b = -110; b > -230; b -= 40)
	{
		glColor3f(0.9, 0.9, 0.8);
		glRectf(b, 20, b - 20, 10);

		glColor3f(0.9, 0.9, 0.8);
		glRectf(b - 17, 17, b - 23, -25);

		glColor3f(0.9, 0.9, 0.8);
		glRectf(b - 17, -25, b - 20, -35);

		glColor3f(0.9, 0.9, 0.8);
		glRectf(b - 17, -40, b - 20, -43);

		glColor3f(0.9, 0.9, 0.8);
		glRectf(b - 20, -50, b - 20, -53);
	}

	

	for (int m = -639; m < 700; m += 80)
	{
		glColor3f(0.9, 0.9, 0.8);
		glRectf(m, -322, m+12, -330);
	
	}
	
	for (int m = -677; m < 700; m += 80)
	{
		glColor3f(0.9, 0.9, 0.8);
		glRectf(m, -287, m+12, -295);
	
	}
	

}

void refri()
{

	glColor3f(0, 0, 0); //�st kapak
	glRectf(-601, 276, -299, 74);

	glColor3f(0.515, 0.82, 0.72); //�st kapak
	glRectf(-600, 275, -300, 75);

	glColor3f(0,0,0); // altkapak
	glRectf(-601, 77, -299, -275);
	glColor3f(0.515, 0.82, 0.72); // altkapak
	glRectf(-600, 75, -300, -275);

	glBegin(GL_POLYGON);
	glColor3ub(0, 0, 0);  //tut1
	glVertex2f(-320, -5);
	glVertex2f(-315, -8);
	glVertex2f(-310, 67);
	glVertex2f(-325, 64);
	glEnd();
	
	glBegin(GL_POLYGON);
	glColor3ub(0, 0, 0);  //tut2
	glVertex2f(-320, 100);
	glVertex2f(-315, 97);
	glVertex2f(-310, 172);
	glVertex2f(-325, 169);
	glEnd();


	glColor3f(0, 0, 0); // magnet00
	glRectf(-431, -4, -369, -76);
	glColor3f(0.9, 0.9, 1); // magnet1
	glRectf(-430, -5, -370, -75);
	glColor3f(0,0,0); //magnet
	circle(-400, -5, 9);
	

	glColor3f(0,0,0); // magnet1
	glRectf(-471, 26, -409, -46);
	glColor3f(0.9, 0.9, 1); // magnet1
	glRectf(-470, 25, -410, -45);
	glColor3f(0,0,0); //magnet
	circle(-440, 25, 9);

	glBegin(GL_TRIANGLES);
	glVertex2f(-500, 197);
	glVertex2f(-520, 207);
	glVertex2f(-540, 197);
	glVertex2f(-500, 197);
	glVertex2f(-480, 187);
	glVertex2f(-460, 197);
	glEnd();

	glColor3f(0, 0, 0);
	vprint(-475, 210, GLUT_BITMAP_8_BY_13, "SIEMENS");

	glColor3f(0.7 , 0 , 0);
	vprint(-575, 250, GLUT_BITMAP_8_BY_13, "THREAT LEVEL:");

	
	if (ratcount > 7)
		glColor3f(1, 0, 0); //red
	else if(ratcount > 4)
		glColor3f(0, 0, 1); //blue
	else
		glColor3f(0, 1, 0);//green (safe)
	
	circle(-457, 254, 9);

}

void cupboard()
{

	glColor3f(0.234, 0.324, 0.464); //dolap
	glRectf(199, -24, 551, -276);
	glColor3f(0.924, 0.950, 0.808); //dolap
	glRectf(200, -25, 550, -275);


	glBegin(GL_LINES); //
	glColor3f(0.234, 0.324, 0.464);
	glLineWidth(2);
	glVertex2f(200, -45);
	glVertex2f(550, -45);
	glVertex2f(200, -80);
	glVertex2f(550, -80);
	glEnd();

	glColor3f(0.714, 0.894, 0.742); //dolapicdetay
	glRectf(225, -100, 352, -250);
	glBegin(GL_LINES); //dolap ic detay
	glColor3f(0.714, 0.894, 0.742);
	glVertex2f(218, -95);
	glVertex2f(218, -255);
	glVertex2f(357, -95);
	glVertex2f(357, -255);
	//*
	glVertex2f(218, -95);
	glVertex2f(357, -95);
	glVertex2f(218, -255);
	glVertex2f(357, -255);
	glEnd();
	
	//*

	glColor3f(0.714, 0.894, 0.742); //dolapicdetay
	glRectf(410, -100, 525, -250);
	glBegin(GL_LINES); //dolap ic detay
	glColor3f(0.714, 0.894, 0.742);
	glVertex2f(405, -95);
	glVertex2f(405, -255);
	glVertex2f(530, -95);
	glVertex2f(530, -255);
	//**
	glVertex2f(405, -95);
	glVertex2f(530, -95);
	glVertex2f(405, -255);
	glVertex2f(530, -255);
	glEnd();

	//*

	glColor3f(0.671, 0.441, 0.613); //tut1
	circle(525, -62, 9);
	glColor3f(0.671, 0.441, 0.613); //tut1
	circle(225, -62, 9);
	glColor3f(0.671, 0.441, 0.613); //tut1
	circle(525, -100, 9);
	glColor3f(0.671, 0.441, 0.613); //tut1
	circle(350, -100, 9);

	glBegin(GL_LINES); //cizgi
	glColor3f(0.234, 0.324, 0.464);
	glVertex2f(380, -80);
	glVertex2f(380, -275);
	glEnd();

	

	glColor3f(0.234, 0.324, 0.464); //dolapdiscizgi
	glRectf(229, -35, 529, -38);


	


	//*

	

	glColor3f(0.722, 0.448, 0.593); //dolapdetay
	glRectf(200, 350, 550, 185);
	
	glColor3f(0.924, 0.950, 0.808); //dolapust
	glRectf(200, 350, 550, 195);
	
	glColor3f(0, 0, 0); //dolapdiscizgi
	glRectf(199, 350, 551, 194);
	
	glColor3f(0.924, 0.950, 0.808); //dolapust
	glRectf(200, 350, 550, 195);

	glColor3f(0,0,0); //dolapust
	glRectf(224, 350, 351, 224);
	glColor3f(0.714, 0.894, 0.742); //dolapust
	glRectf(225, 350, 350, 225);
	
	glColor3f(0, 0, 0); //dolapust
	glRectf(399, 350, 526, 224);
	glColor3f(0.714, 0.894, 0.742); //dolapust
	glRectf(400, 350, 525, 225);

	glColor3f(1, 1, 1);
	glRectf(210, 190, 520, 188);
	glColor3f(1, 1, 1);
	glRectf(526, 190, 538, 188);

	glColor3f(0.671, 0.441, 0.613); //tut
	circle(525, 230, 9);
	
	glColor3f(0.671, 0.441, 0.613); //tut1
	circle(350, 230, 9);

	glBegin(GL_LINES); //cizgi
	glColor3f(0, 0, 0);
	glVertex2f(375, 350);
	glVertex2f(375, 195);
	glEnd();



}

void oven()
{
	glColor3f(0, 0, 0); //f�r�ndisdetay
	glRectf(-202, 32, 102, -273);
	glColor3f(0.718, 0.890, 0.75); //f�r�n
	glRectf(-200, 30, 100, -275);

	glColor3f(0.402, 0.585, 0.761);
	glRectf(-180, 10, 80, 20);

	glBegin(GL_POLYGON);
	glColor3ub(0, 0, 0);  //tut1
	glVertex2f(-190, -17);
	glVertex2f(-155, -12);
	glVertex2f(-140, -14);
	glVertex2f(-145, -3);
	glEnd();
	glBegin(GL_POLYGON);
	glColor3ub(0, 0, 0);  //tut2
	glVertex2f(90, -17);
	glVertex2f(55, -12);
	glVertex2f(40, -14);
	glVertex2f(45, -3);
	glEnd();

	glBegin(GL_LINES); //detaylar
	glColor3f(0, 0, 0);
	glVertex2f(-200, -20);
	glVertex2f(100, -20);
	
	glVertex2f(-200, -70);
	glVertex2f(100, -70);
	
	glVertex2f(-200, -250);
	glVertex2f(100, -250);
	glEnd();

	//*
	glColor3f(0.714, 0.894, 0.742);
	glRectf(-185, -60, -105, -30);
	glColor3f(0.714, 0.894, 0.742);
	glRectf(85, -60, 5, -30);

	glColor3f(0,0,0); //tut1
	circle(-165, -45, 10);
	glColor3f(0,0,0); //tut1
	circle(-125, -45, 10);
	glColor3f(0,0,0); //tut1
	circle(65, -45, 10);
	glColor3f(0,0,0); //tut1
	circle(25, -45, 10);
	glColor3f(1,1,1);
	glRectf(-162, -45, -160, -43);
	glColor3f(1,1,1);
	glRectf(-122, -45, -120, -43);	
	glColor3f(1,1,1);
	glRectf(62, -45, 64, -43);	
	glColor3f(1,1,1);
	glRectf(22, -45, 24, -43);




	//*
	
	glColor3f(0,0,0);
	glRectf(-170, -95, -135, -105);
	
	glColor3f(0,0,0);
	glRectf(35, -95, 70, -105);
	
	glColor3f(1,1,1);
	glRectf(40, -100, 45, -103);
	glColor3f(1,1,1);
	glRectf(-145, -100, -140, -103);


	
	glColor3f(0,0,0); //havlushadow
	glRectf(16, -106, -31, -174);
	glColor3f(1,1,1); //havlu
	glRectf(15, -105, -30, -173);
	glColor3f(0, 0, 0); //havlushadow
	glRectf(16, -108, 19, -154);
	glColor3f(1,1,1); //havlu
	glRectf(15, -107, 18, -153);

	glLineWidth(2); //line width

	glBegin(GL_LINES); //havlucizgi
	glColor3f(0,0,0);
	glVertex2f(15, -166);
	glVertex2f(-30, -166);
	glEnd();

	glBegin(GL_LINES); //detaylar
	
	glColor3f(1, 0, 0);
	glVertex2f(-18, -150);
	glVertex2f(-30, -160);
	
	glVertex2f(-6, -160);
	glVertex2f(-18, -150);

	glVertex2f(6, -150);
	glVertex2f(-6, -160);

	glVertex2f(6, -150);
	glVertex2f(15, -160);


	glEnd();

	glBegin(GL_LINES); //detaylar
	glColor3f(0, 0, 0);
	glVertex2f(16, -105);
	glVertex2f(16, -155);
	glEnd();

	glBegin(GL_LINES); //detaylar
	glVertex2f(-170, -105); //kapak
	glVertex2f(70, -105);
	glEnd();

	glColor3f(1 , 0 , 0.3);
	vprint(-22, -120, GLUT_BITMAP_8_BY_13, "CTIS");
	vprint(-22, -135, GLUT_BITMAP_8_BY_13, " 164");


}

void background()
{

	glColor3f(0.414, 0.582, 0.765); //arka
	glRectf(-700, 700, 700, -700);

	glColor3f(0.871, 0.933, 0.757); //arka�st
	glRectf(-700, 50, 700, 700);

	for (int h = -600; h < 700; h += 55)
	{
		glColor3f(0.703, 0.906, 0.714); //benek
		circle(-h, 80, 15);
	}

	for (int h = -675; h < 700; h += 55)
	{
		glColor3f(0.703, 0.906, 0.714); //benek
		circle(-h, 160, 15);
	}

	for (int h = -600; h < 750; h += 55)
	{
		glColor3f(0.703, 0.906, 0.714); //benek
		circle(-h, 240, 15);
	}

	for (int h = -675; h < 750; h += 55)
	{
		glColor3f(0.703, 0.906, 0.714); //benek
		circle(-h, 320, 15);
	}

	glColor3f(0.75,0.42, 0.60); //serit
	glRectf(-700, 50, 700, -275);

	glBegin(GL_LINES); //serit0
	glColor3f(0.234, 0.324, 0.464);
	glVertex2f(-700, 25);
	glVertex2f(700, 25);
	glEnd();

	glBegin(GL_LINES); //serit1
	glColor3f(0.234, 0.324, 0.464);
	glVertex2f(-700, -250);
	glVertex2f(700, -250);
	glEnd();

	for (int b = -660; b < 675; b += 40)
	{
		glBegin(GL_LINES); //d�seme
		glColor3f(0.234, 0.324, 0.464);
		glVertex2f(b, 25);
		glVertex2f(b, -250);
		glEnd();
	}

}

void rat()
{
	for (o = 0; o < MAX; o++)
	{
		glColor3f(0.417, 0.488, 0.527); //arka g�vde
		glRectf(shape[o].xpos + 100, shape[o].ypos + 94, shape[o].xpos + 98, shape[o].ypos + 76);

		glColor3f(0.417, 0.488, 0.527); //arka g�vde
		glRectf(shape[o].xpos + 98, shape[o].ypos + 92, shape[o].xpos + 94, shape[o].ypos + 78);

		glColor3f(0.417, 0.488, 0.527); //arka g�vde
		glRectf(shape[o].xpos + 94, shape[o].ypos + 88, shape[o].xpos + 90, shape[o].ypos + 82);

		glColor3f(0.996, 0.582, 0.574); //kuyruk
		glRectf(shape[o].xpos + 90, shape[o].ypos + 86, shape[o].xpos + 84, shape[o].ypos + 82);

		glColor3f(0.996, 0.582, 0.574); //kuyruk
		glRectf(shape[o].xpos + 84, shape[o].ypos + 84, shape[o].xpos + 82, shape[o].ypos + 82);

		glColor3f(0.996, 0.582, 0.574); //kuyruk
		glRectf(shape[o].xpos + 82, shape[o].ypos + 82, shape[o].xpos + 80, shape[o].ypos + 80);

		glColor3f(0.996, 0.582, 0.574); //kuyruk
		glRectf(shape[o].xpos + 80, shape[o].ypos + 82, shape[o].xpos + 76, shape[o].ypos + 80);

		//*

		glColor3f(0.417, 0.488, 0.527); //g�vde
		glRectf(shape[o].xpos + 100, shape[o].ypos + 100, shape[o].xpos + 150, shape[o].ypos + 70);

		glColor3f(0.417, 0.488, 0.527); //g�vde
		glRectf(shape[o].xpos + 100, shape[o].ypos + 100, shape[o].xpos + 150, shape[o].ypos + 70);

		glColor3f(0.417, 0.488, 0.527); //g�vde
		glRectf(shape[o].xpos + 102, shape[o].ypos + 102, shape[o].xpos + 148, shape[o].ypos + 100);

		glColor3f(0.417, 0.488, 0.527); //g�vde
		glRectf(shape[o].xpos + 104, shape[o].ypos + 104, shape[o].xpos + 146, shape[o].ypos + 102);

		glColor3f(0.417, 0.488, 0.527); //g�vde
		glRectf(shape[o].xpos + 110, shape[o].ypos + 106, shape[o].xpos + 140, shape[o].ypos + 104);

		glColor3f(0.417, 0.488, 0.527); //g�vde
		glRectf(shape[o].xpos + 142, shape[o].ypos + 106, shape[o].xpos + 144, shape[o].ypos + 104);

		//*

		glColor3f(0.417, 0.488, 0.527); //�ng�vde
		glRectf(shape[o].xpos + 150, shape[o].ypos + 96, shape[o].xpos + 156, shape[o].ypos + 74);

		glColor3f(0.417, 0.488, 0.527); //�ng�vde
		glRectf(shape[o].xpos + 156, shape[o].ypos + 92, shape[o].xpos + 158, shape[o].ypos + 78);

		glColor3f(0.996, 0.582, 0.574); //burun
		glRectf(shape[o].xpos + 158, shape[o].ypos + 90, shape[o].xpos + 160, shape[o].ypos + 80);

		glColor3f(0.996, 0.582, 0.574); //burun
		glRectf(shape[o].xpos + 160, shape[o].ypos + 88, shape[o].xpos + 162, shape[o].ypos + 82);

		//*

		glColor3f(0.996, 0.582, 0.574); //1ayak
		glRectf(shape[o].xpos + 102, shape[o].ypos + 68, shape[o].xpos + 108, shape[o].ypos + 74);

		glColor3f(0.996, 0.582, 0.574); //1ayak
		glRectf(shape[o].xpos + 100, shape[o].ypos + 76, shape[o].xpos + 102, shape[o].ypos + 62);

		glColor3f(0.996, 0.582, 0.574); //1ayak
		glRectf(shape[o].xpos + 98, shape[o].ypos + 66, shape[o].xpos + 100, shape[o].ypos + 62);

		glColor3f(0.996, 0.582, 0.574); //1ayak
		glRectf(shape[o].xpos + 100, shape[o].ypos + 64, shape[o].xpos + 102, shape[o].ypos + 62);

		//*

		glColor3f(0.996, 0.582, 0.574); //2ayak
		glRectf(shape[o].xpos + 114, shape[o].ypos + 66, shape[o].xpos + 116, shape[o].ypos + 70);

		glColor3f(0.996, 0.582, 0.574); //2ayak
		glRectf(shape[o].xpos + 116, shape[o].ypos + 72, shape[o].xpos + 118, shape[o].ypos + 62);

		glColor3f(0.996, 0.582, 0.574); //2ayak
		glRectf(shape[o].xpos + 118, shape[o].ypos + 64, shape[o].xpos + 120, shape[o].ypos + 62);

		//*

		glColor3f(0.996, 0.582, 0.574); //3ayak
		glRectf(shape[o].xpos + 136, shape[o].ypos + 70, shape[o].xpos + 134, shape[o].ypos + 66);

		glColor3f(0.996, 0.582, 0.574); //3ayak
		glRectf(shape[o].xpos + 138, shape[o].ypos + 62, shape[o].xpos + 136, shape[o].ypos + 72);

		glColor3f(0.996, 0.582, 0.574); //3ayak
		glRectf(shape[o].xpos + 138, shape[o].ypos + 62, shape[o].xpos + 140, shape[o].ypos + 64);

		//*

		glColor3f(0.996, 0.582, 0.574); //4ayak
		glRectf(shape[o].xpos + 150, shape[o].ypos + 72, shape[o].xpos + 152, shape[o].ypos + 70);

		glColor3f(0.996, 0.582, 0.574); //4ayak
		glRectf(shape[o].xpos + 150, shape[o].ypos + 70, shape[o].xpos + 154, shape[o].ypos + 64);

		glColor3f(0.996, 0.582, 0.574); //4ayak
		glRectf(shape[o].xpos + 154, shape[o].ypos + 64, shape[o].xpos + 152, shape[o].ypos + 62);

		//*

		glColor3f(0, 0, 0); //eye
		glRectf(shape[o].xpos + 138, shape[o].ypos + 98, shape[o].xpos + 146, shape[o].ypos + 94);
		glColor3f(0.078, 0.101, 0.101); //eye
		glRectf(shape[o].xpos + 138, shape[o].ypos + 94, shape[o].xpos + 146, shape[o].ypos + 92);


	}
}

void ground()
{

	glBegin(GL_LINES); //dikd�seme
	glColor3f(0.234, 0.324, 0.464);
	glVertex2f(-700, -275);
	glVertex2f(700, -275);
	glEnd();

	for (int b = -660; b < 675; b += 40)
	{
		glBegin(GL_LINES); //d�seme
		glColor3f(0.234, 0.324, 0.464);
		glVertex2f(b, -275);
		glVertex2f(b, -350);
		glEnd();
	}



	glBegin(GL_LINES); //dikd�seme
	glColor3f(0.234, 0.324, 0.464);
	glVertex2f(-700, -312.5);
	glVertex2f(700, -312.5);
	glEnd();

}

void info()
{

	glColor3f(0.7, 0.2, 0.8);
	vprint(-130, 217, GLUT_BITMAP_8_BY_13, "CLICK LEFT TO CREATE");
	vprint(-145, 190, GLUT_BITMAP_8_BY_13, " RAZZLE DAZZLE [MAX 10]");



}

void info2()
{

	glColor3f(0 , 0.1 , 0.2);
	vprint(-460, 8, GLUT_BITMAP_8_BY_13, "F1");
	vprint(-460, -6, GLUT_BITMAP_8_BY_13, "WILL");
	vprint(-460, -18, GLUT_BITMAP_8_BY_13, "CLEAN");
	vprint(-460, -30, GLUT_BITMAP_8_BY_13, "ALL");
	vprint(-460, -42, GLUT_BITMAP_8_BY_13, "MESS");

}

void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	//kitchen items

	background();
	shadows();
	refri();
	cupboard();
	oven();
	ground();
		
	//kitchen items


	if (ratcount > 0 && !activeTimer) //showing F1 message
		info2();

	glColor3f(0.1, 0.1, 0.11); //fontback
	glRectf(-150, 300, 50, 180);

	if (!activeTimer) //showing deploy message
	info();

	glColor3f(0.7 , 0.2 , 0.8);
	vprint(-95, 275, GLUT_BITMAP_8_BY_13, "MERTER EKER");
	vprint(-83, 245, GLUT_BITMAP_8_BY_13, "21704343");
	glColor3f(0.4 , 0 , 0.3);
	vprint(-406, -44, GLUT_BITMAP_8_BY_13, "HOME");
	vprint(-411, -58, GLUT_BITMAP_8_BY_13, "SWEET");
	vprint(-406, -72, GLUT_BITMAP_8_BY_13, "HOME");

	rat(); //dynamic object

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
	}

	if (key == GLUT_KEY_F1) {

		activeTimer = !activeTimer;

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

	xeksen = x - winWidth / 2 - 125; //Rats are deployable only on the ground and table.CHECK.
	yeksen = winHeight / 2 - y - 85; //Rats are deployable only on the ground and table.CHECK.

	////*******MAX 10 object. User cannot deploy item while rats're moving. Rats are deployable only on the ground and table.********** 
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && ratcount < MAXRAT && !activeTimer && (yeksen<-350 || (xeksen > -410 && yeksen > -40 && yeksen < -10))) 
	{

		if (ratview)
		{
			shape[ratcount].xpos = x - winWidth / 2 - 125; //this code adjust the coordinates of rat
			shape[ratcount].ypos = winHeight / 2 - y - 85; //this code adjust the coordinates of rat
			ratcount++; //rat number
		}

		ratview = true; //bool function

	}


	if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN) {

		{


		}

		// to refresh the window it calls display() function
		glutPostRedisplay();
	}
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


	glutTimerFunc(1, onTimer, 0);


	if (activeTimer) {

		for (int l = 0; l < ratcount; l++)
		{
			(shape+l)->xpos++; //position changer 
			if ((shape+l)->xpos >= 2000) //position check
			{
				activeTimer = false; //freezes the timer
				ratview = true; //activetes the deploy function
				ratcount = 0; //decreases the rat number after all of them gone

			}
		}
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

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Rats in the Kitchen! by Merter Eker");

	shape = (kitchen_t*)malloc(MAX * sizeof(kitchen_t)); //dynamically created array for rat array

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
	srand(time(0));


#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif


	Init();

	glutMainLoop();
}