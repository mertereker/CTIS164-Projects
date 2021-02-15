/*********
   CTIS164 - Template Source Program
----------
STUDENT : Merter Eker
SECTION : 3
HOMEWORK: HW3 -- *Shooting Circles with Rainbow*
----------
PROBLEMS: ALL OF THE FUNCTIONS ARE WORKING PROPERLY. REQUIREMENTS DONE.
I'm using old version of Visual Studio so I don't get any security error on my computer.
----------
ADDITIONAL FEATURES:
Circle/s function finds shooting rate to be competitive (less is bad), putting a point bar was non-sense because there are 3 object to shoot as said in requirement.
There is a accuracy finder depending on the CLICK count, not for SPACE because there is no accuracy need for burst firing.
There is a mod setting that adjust firing hip. If easy mod open, user can fire multiple times with SPACE.
There is a record function that calculates record time and informs gamer.
Start button check added.
Speed function added.
Timer added.
**colors**
Circles changing color with click and also deploy in different color each lap as said.
Complex rainbow bullet added.
Board designed colorfully.
Rainbow arrow added.
**colors**
-*Thanks for the extra time*-
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1100
#define WINDOW_HEIGHT 800
#define PI 3.1415
#define TIMER_PERIOD	5	// Period for the timer.
#define TIMER_ON		1	// 0:disable timer, 1:enable timer
#define CircleCNT 3
#define D2R	0.0174532
#define  START 0 //game state check
#define  RUN   1 //game state check

enum speed_t { FAST = 1, SLOW = 2 }; //enum type speed definition to be user-friendly
//substructs
typedef struct {
	int r, g, b;
}colors_t;

typedef struct {
	int red, green, blue;
}arrowcolor_t;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
	double xeksen;
	double yeksen;
}target_t;

typedef struct c{
	target_t pos;
	arrowcolor_t acolor[CircleCNT];
	arrowcolor_t bcolor[CircleCNT];
	arrowcolor_t ccolor[CircleCNT];
	colors_t color;
	speed_t gamespeed = FAST;
	int dir; //clockwise/counterclockwise
	float angle; //for sin-cos
	float dist; //distance from center
	float radius; //random size
	float speed; //random speed
	bool hitcheck;
}circle_t; //mainstruct

///
//callstruct
target_t target = {0}, gun, cannon;
circle_t circles[CircleCNT];
//variables
float angletarget;
float arrowroute = 0, firingroute = 0;
int hitobj = 0, max, gamecnt = 0;
int game[50], i, a, b, c, g, e;
int timecounter = 0, clickcounter = 0, showclick, sec, showtimer;
int record, gamecur, sangle, recordcheck;
int gamestate = START;
//boolchecks
bool playcheck = false;
bool firecheck = false;
bool startcheck = false;
bool easy = true;
///

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

//programming side
bool checkCircle(float px, float py, float cx, float cy, float R);
double dist(target_t pos1, target_t pos2);
float showangle(float angle);
float AngleTarget(target_t p);
void shape(target_t posx, target_t arr, double angle);
void endofgame(int timecounter, int recordcur, int gamecur);
void colortimer(int v);
void timer(int v);
void startbutton();
void shoot();
void obj();
//programming side

//design part
void drawcircles() {

	for (i = 0; i < CircleCNT; i++) {

		if (circles[i].hitcheck == false)
		{

			glColor3ub(circles[i].color.r, circles[i].color.g, circles[i].color.b); //random color
			circle(circles[i].pos.xeksen, circles[i].pos.yeksen, circles[i].radius); //drawing circle by looking randomly generated numbers

			glColor3f(0, 0.1, 0);
			vprint2(circles[i].pos.xeksen - 8, circles[i].pos.yeksen - 4, 0.11, "%.0f", circles[i].angle); //showing the dynamic position of object

		}

	}
}

void gamearea() {

	if (playcheck)
		drawcircles(); //drawing the objects if the game started

}

void arrow(float angle) {

	glColor3f(0, 1, 0.9);
	vprint2(cos(arrowroute) * 400, sin(arrowroute) * 400, 0.16, "%.0f", showangle(arrowroute)); //showing the dynamic position of arrow

	glColor3f(0.2, 0.8, 0.9); //0point
	circle(0, 0, 12);

	if (firecheck) { //cannon

		glColor3ub(circles[2].acolor->red, circles[2].acolor->green, circles[2].acolor->blue); //random color
		glBegin(GL_QUADS);
		glVertex2f(cannon.xeksen + 0, cannon.yeksen + 8);
		glVertex2f(cannon.xeksen + -8, cannon.yeksen + 0);
		glVertex2f(cannon.xeksen + -4, cannon.yeksen + 0);
		glVertex2f(cannon.xeksen + 0, cannon.yeksen + 4);
		glVertex2f(cannon.xeksen + 0, cannon.yeksen + 8);
		glEnd();

		glColor3ub(circles[1].acolor->red, circles[1].acolor->green, circles[1].acolor->blue); //random color
		glBegin(GL_QUADS);
		glVertex2f(cannon.xeksen + 8, cannon.yeksen + 0);
		glVertex2f(cannon.xeksen + 4, cannon.yeksen + 0);
		glVertex2f(cannon.xeksen + 0, cannon.yeksen + 4);
		glVertex2f(cannon.xeksen + -4, cannon.yeksen + 0);
		glVertex2f(cannon.xeksen + -8, cannon.yeksen + -4);
		glEnd();

	}
	
	for (c = 0; c < CircleCNT; c++)
	{

		glColor3ub(circles[c].acolor->red, circles[c].acolor->green, circles[c].acolor->blue); //random color1
		glBegin(GL_TRIANGLES);
		shape({ 12,12 }, { 0,0 }, angle);
		shape({ 70,0 }, { 0,0 }, angle);
		shape({ 12,-12 }, { 0,0 }, angle);

		glColor3ub(circles[c].bcolor->red, circles[c].bcolor->green, circles[c].bcolor->blue); //random color2
		glBegin(GL_TRIANGLES);
		shape({ 45,12 }, { 0,0 }, angle);
		shape({ 80,0 }, { 0,0 }, angle);
		shape({ 45,-12 }, { 0,0 }, angle);

		glColor3ub(circles[c].ccolor->red, circles[c].ccolor->green, circles[c].ccolor->blue); //random color3
		glBegin(GL_TRIANGLES);
		shape({ 70,12 }, { 0,0 }, angle);
		shape({ 100,0 }, { 0,0 }, angle);
		shape({ 70,-12 }, { 0,0 }, angle);
		glEnd();

	}

}

void board()
{
	//target board
	glColor3f(0, 0, 0.1);
	circle(0, 0, 360);
	glColor3f(0.9, 0.7, 0);
	circle(0, 0, 260);
	glColor3f(0, 0, 0);
	circle_wire(0, 0, 160);
	glColor3f(0, 0, 0.1);
	circle(0, 0, 160);
	glColor3f(0, 0, 0);
	circle_wire(0, 0, 260);
	glColor3f(0, 0, 0);
	circle_wire(0, 0, 360);

	glColor3f(0, 1, 0.9);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(-550, 0);
	glVertex2f(550, 0);
	glVertex2f(0, 550);
	glVertex2f(0, -550);
	glEnd();

}

void menu()
{

	if (gamestate == START)
	{
		startbutton(); //showing the start point

		//informations for the gamer

		glColor3f(0.9, 0, 0.3);
		vprint2(-500, +330, 0.15, "F1: Change Speed");
		vprint2(-500, +300, 0.15, "F2: Change Mod");

		if (easy)
		{
			glColor3f(0.9, 0, 0.3);
			vprint2(-500, 240, 0.15, "Mod : Easy");
			vprint2(-500, 210, 0.15, "SPACE : BURST");
			vprint2(-393, 180, 0.15, "FIRE");
			vprint2(-500, 360, 0.15, "CLICK : SINGLE FIRE");
		}

		else
		{
			glColor3f(0.7, 0, 0.5);
			vprint2(-500, 240, 0.15, "Mod : Hard");
			vprint2(-500, 210, 0.15, "CLICK : SINGLE");
			vprint2(-400, 180, 0.15, "FIRE");
		}

		if (circles->gamespeed == FAST)
		{
			glColor3f(0.9, 0, 0.3);
			vprint2(-500, 270, 0.15, "Speed : FAST");
		}
		else if (circles->gamespeed == SLOW)
		{
			glColor3f(0.7, 0, 0.5);
			vprint2(-500, 270, 0.15, "Speed : SLOW");
		}


		glColor3f(0.9, 0.5, 0);
		vprint2(-500, -300, 0.15, "Merter Eker");
		vprint2(-491, -330, 0.15, "21704343");
		vprint2(-480, -360, 0.15, "HW #3");


		if (gamecnt > 1)
		{
			glColor3f(0.9, 0.9, 0);
			vprint2(350, 330, 0.15, "Accuracy: %.0f%%", 100 * (3. / (showclick)));
			vprint2(350, 300, 0.15, "Fire Rate: %.2f/s", (3. / showtimer / 1000) * 1000000);
			vprint2(350, 270, 0.15, "Last Time: %.2d:%.2d", showtimer / 1000, showtimer % 100);

			if (gamecnt <= 3)
				vprint2(350, 240, 0.15, "Record: %.2d:%.2d", showtimer / 1000, showtimer % 100);

			else
				vprint2(350, 240, 0.15, "Record: %.2d:%.2d", max / 1000, max % 100);
		}

		if (recordcheck)
		{
			glColor3f(0.9, 0.3, 1);
			vprint2(-93, -310, 0.15, "New Record! : %.2d:%.2d", max / 1000, max % 100);
		}

	}

	if (gamestate == RUN)
	{
		arrow(arrowroute);
		glColor3f(0.9, 0.9, 0);
		vprint2(350, 300, 0.15, "Fire Rate: %.2f/s", (3. / timecounter / 1000) * 1000000);
		vprint2(350, 330, 0.15, "Fire Count: %d", clickcounter - 1);
		glColor3f(1, 0.3, 0.3);
		vprint2(375, -300, 0.35, "%.2d:%.2d", timecounter / 1000, timecounter % 100);
	}

}
//design part

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0.1, 0, 0.1, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	board(); // design and circles
	gamearea(); // rotating shapes
	menu(); //arrow and menu

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

	if (easy)
		if (key == ' ')
		{
			firingroute = arrowroute; //cannon route becomes arrow route
			firecheck = true; //fires
		}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (easy) //hip fire check
		if (key == ' ')
			firecheck = false; //stops
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
	// 

	if(!playcheck){
	
		if (key == GLUT_KEY_F2) { //changing mod

		if (easy)
			easy = false;

		else if (!easy)
			easy = true;

		}

	//changes game speed
	if (key == GLUT_KEY_F1) { //changing speed

		if (circles->gamespeed == FAST) 
			circles->gamespeed = SLOW;

		else if (circles->gamespeed == SLOW)
			circles->gamespeed = FAST;

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

	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {

		clickcounter++; //to calculate accuracy

		if (playcheck && !firecheck) {

			firingroute = arrowroute; //cannon route becomes arrow route
			firecheck = true; //fires

		}

		else if (gamestate == START && !playcheck && startcheck == true) {

			playcheck = true; //changing game state
			gamestate = RUN; //changing game state

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

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.

	target.xeksen = x - winWidth / 2; //x cursor
	target.yeksen = winHeight / 2 - y; //y cursor

	startcheck = checkCircle(target.xeksen, target.yeksen, 0, 0, 70); //start button check
	arrowroute = AngleTarget(target); // finding target direction

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

bool checkCircle(float px, float py, float cx, float cy, float R) { //to check whether cursor above button or not.

	float dx = px - cx;
	float dy = py - cy;
	float d = sqrt(dx*dx + dy * dy);
	return d <= R;

}

void startbutton() { //start button

	glColor3ub(255, 100, 140);
	circle(0, 0, 70);

	if (startcheck)
		glColor3ub(255, 80, 120);

	else
		glColor3ub(0, 0.3, 0.4);
	circle(0, 0, 67);

	if (startcheck)
		glColor3ub(0, 0, 120);

	else
		glColor3ub(255, 100, 0);
	vprint2(-47, -12, 0.25, "START");

}

float showangle(float angle) {

	sangle = angle / D2R;

	if (sangle < 0) //to do not let being a negative number
	{
		sangle = 360 + sangle;
	}

	return sangle;
}

double dist(target_t pos1, target_t pos2) {

	return sqrt((pos1.xeksen - pos2.xeksen)*(pos1.xeksen - pos2.xeksen) + (pos1.yeksen - pos2.yeksen)*(pos1.yeksen - pos2.yeksen)); //distance from the center

}

void shape(target_t posx, target_t arr, double angle) {

	double xpos = (posx.xeksen * cos(angle) - posx.yeksen * sin(angle)) + arr.xeksen; //arrow' constant x position
	double ypos = (posx.xeksen * sin(angle) + posx.yeksen * cos(angle)) + arr.yeksen; //arrow' constant y position

	glVertex2d(xpos, ypos); //drawing arrow by looking cursor pos and initial point

}

float AngleTarget(target_t p) {

	angletarget = atan2(p.yeksen, p.xeksen); // finding the angle by looking cursor position

	return angletarget;

}

void obj() {

	hitobj = 0; //counter set by default after game

	firecheck = false; //firing set by default
	playcheck = false; //playing set by default
	
	for (e = 0; e < CircleCNT; e++) { //random processes for shapes coordinates, speed, size

		circles[e].hitcheck = false; //initial hitcheck for shapes each lap
		circles[e].speed = (rand() % 4 + 4) / 8.; //define a speed for each shape
		circles[e].dist = e * 100 + 160; //circle wire adaption
		circles[e].radius = rand() % 10 + 25; //size of circles
		circles[e].dir = rand() % 4 > 2 ? -1 : 1; //to make shapes dynamically rotating clockwise/counterclockwise by changing the number positive or negatively.
		circles[e].angle = rand() % 360; //deploy point

	}

	gamecnt++; //gamecount for record system
	endofgame(timecounter, record, gamecur); //calculation of record
	showtimer = timecounter; //displays last time
	showclick = clickcounter - 1; //clickcounter
	timecounter = 0; //set by default after end game
	clickcounter = 0; //set by default after end game
	gamestate = START; //set by default after end game
	shoot(); //gameend

}

void shoot() {

	for (b = 0; b < CircleCNT; b++) { //random circle color

		circles[b].color.r = rand() % 255;
		circles[b].color.g = rand() % 255;
		circles[b].color.b = rand() % 255;

	}

	firecheck = false; //prevents shooting if 3 object hitted
	firingroute = 0; //route set by default

	cannon.xeksen = 0; //route set by default
	cannon.yeksen = 0; //route set by default

}


void endofgame(int timecounter, int recordcur, int gamecur)
{
	game[0] = timecounter; //record check

	if (gamecnt > -1)
	{
		game[gamecnt] = timecounter; //record check


		if (game[gamecnt - 1] > game[gamecnt])
		{
			max = game[gamecnt];
			recordcheck = 1; //whether record break or not
		}

		else
			recordcheck = 0; //whether record break or not

	}

}

//gametimer
void timer(int v)
{
	glutTimerFunc(1, timer, 0); //timer as split second

	if (playcheck) {
		timecounter++; //split-second increasing
		sec = timecounter / 1000; //finding sec
	}

}

//colortimer
void colortimer(int v)
{

	glutTimerFunc(75, colortimer, 0); //timer for arrow colors

	for (g = 0; g < CircleCNT; g++) //generating random colors
	{
		circles[g].acolor->red = rand() % 255;
		circles[g].acolor->green = rand() % 255;
		circles[g].acolor->blue = rand() % 255;
		circles[g].bcolor->red = rand() % 255;
		circles[g].bcolor->green = rand() % 255;
		circles[g].bcolor->blue = rand() % 255;
		circles[g].ccolor->red = rand() % 255;
		circles[g].ccolor->green = rand() % 255;
		circles[g].ccolor->blue = rand() % 255;
	}

}

//generaltimer
#if TIMER_ON == 1
void onTimer(int v) {

	if (circles->gamespeed == FAST)
		glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	else if (circles->gamespeed == SLOW)
		glutTimerFunc(TIMER_PERIOD * SLOW, onTimer, 0);

	// Write your codes here.

	if (playcheck) {

		if (firecheck) {

			cannon.xeksen += cos(firingroute) * 12; //cannon moves regarding to cos pos
			cannon.yeksen += sin(firingroute) * 12; //cannon moves regarding to sin pos

			if ((cannon.yeksen < -400 || cannon.yeksen >400) || (cannon.xeksen < -400 || cannon.xeksen > 400)) {
				shoot(); //if existing cannon dissappear from the screen, user can fire a new one
			}

		}

		for (a = 0; a < CircleCNT; a++) {

			if (firecheck) {

				if (!circles[a].hitcheck){

					if (dist(circles[a].pos, cannon) < circles[a].radius) {

						shoot(); //if existing cannon hits the shape, user can fire a new one
						hitobj++; //hit counter
						circles[a].hitcheck = true; //if user hits, specific object disappear

						if (hitobj == CircleCNT) {
							obj(); //finishing the game
						}

					}
				}

			}
			
			circles[a].angle += (circles[a].speed * circles[a].dir); //circle moves regarding to the speed, (-/+) direction that generated randomly
			circles[a].pos.xeksen = cos(circles[a].angle * D2R) * circles[a].dist; //circle moves
			circles[a].pos.yeksen = sin(circles[a].angle * D2R) * circles[a].dist; //circle moves
			
			if (circles[a].angle > 360) // %360 applied here to identify program that shape exceed the 360 degree
				circles[a].angle = 0;

			if (circles[a].angle < 0) // %360 applied here to identify program that shape exceed the 0 degree
				circles[a].angle = 360;

			}

	}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void main(int argc, char *argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Shooting Circles with Rainbow by Merter Eker");

	obj(); //calling objects

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
	glutTimerFunc(TIMER_PERIOD, timer, 0);
	glutTimerFunc(TIMER_PERIOD, colortimer, 0);

#endif

	glutMainLoop();

}