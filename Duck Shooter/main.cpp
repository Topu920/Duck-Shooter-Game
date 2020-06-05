#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#define PI 3.1415926535898
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<string.h>

#define PI 3.14159
#define GAME_SCREEN 0			//Constant to identify background color
#define MENU_SCREEN 4
#define MAX_STONES  100
#define MAX_STONE_TYPES 5
#define stoneRotationSpeed 20
#define SPACESHIP_SPEED 20
int stoneTranslationSpeed=1;

GLint m_viewport[4];
GLint CI=0;
int x,y;
int i;
int randomStoneIndices[100];
int index;
int Score=0;
int alienLife=100;
int GameLvl= 1;
float mouseX ,mouseY ;				//Cursor coordinates;
float LaserAngle=0 ,stoneAngle =0,lineWidth = 1;
float xOne=0,yOne=0;				//Spaceship coordinates
float xStone[MAX_STONES] ,yStone[MAX_STONES];//coordinates of stones
float xHealthBarStart = 1200;				//Health bar starting coodinate
GLint stoneAlive[MAX_STONES];		//check to see if stone is killed

bool mButtonPressed= false,startGame=false,gameOver=false;		//boolean values to check state of the game
bool startScreen = true ,nextScreen=false,previousScreen=false;
bool gameQuit = false,instructionsGame = false, optionsGame = false;

GLfloat a[][2]={0,-50, 70,-50, 70,70, -70,70};
GLfloat LightColor[][3]={1,1,0,   0,1,1,   0,1,0};
GLfloat HumanBody[][2]={{-4,9}, {-6,0}, {0,0}, {0.5,9}, {0.15,12}, {-14,18}, {-19,10}, {-20,0},{-6,0}};

GLfloat HumanFace[][2]={{-6,11}, {-4.5,18}, {0.5,20}, {0.,20.5}, {0.1,19.5}, {1.8,19}, {5,20}, {7,23}, {9,29},
						{6,29.5}, {5,28}, {7,30}, {10,38},{11,38}, {11,40}, {11.5,48}, {10,50.5},{8.5,51}, {6,52},
						{1,51}, {-3,50},{-1,51}, {-3,52}, {-5,52.5}, {-6,52}, {-9,51}, {-10.5,50}, {-12,49}, {-12.5,47},
						{-12,43}, {-13,40}, {-12,38.5}, {-13.5,33},{-15,38},{-14.5,32},  {-14,28}, {-13.5,33}, {-14,28},
						{-13.8,24}, {-13,20}, {-11,19}, {-10.5,12}, {-6,11} } ;
GLfloat HumanBeak[][2]={{-4,22.5},   {-11,22.5} };

void display();
void StoneGenerate();
void displayRasterText(float x ,float y ,float z ,char *stringToDisplay) {
	int length;
	glRasterPos3f(x, y, z);
		length = strlen(stringToDisplay);

	for(int i=0 ;i<length ;i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 ,stringToDisplay[i]);
	}
}
void SetDisplayMode(int modeToDisplay) {
		switch(modeToDisplay){
		case GAME_SCREEN: glClearColor(0, 0, .5, 1);break;
		case MENU_SCREEN : glClearColor(1, 0 , 0, 1);break;
	}
}
void initializeStoneArray() {
	//random stones index

	for(int i = 0;i < MAX_STONES ;i++) {
		randomStoneIndices[i]=rand()%MAX_STONE_TYPES;
		stoneAlive[i]=true;
	}

	xStone[0] = -(200*MAX_STONES)-600;             //START LINE for stone appearance

	for(int i = 0;i<MAX_STONES ;i++) {				//ramdom appearance yIndex for each stone
		yStone[i]=rand()%600;
		if(int(yStone[i])%2)
			yStone[i]*=-1;
		xStone[i+1] = xStone[i] + 200;				//xIndex of stone aligned with 200 units gap
	}
}
void DrawHumanBody()
{
	glColor3f(0,1,0);				//BODY color
	glBegin(GL_POLYGON);
	for(i=0;i<=8;i++)
		glVertex2fv(HumanBody[i]);
	glEnd();

	glColor3f(0,0,0);			//BODY Outline
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=8;i++)
		glVertex2fv(HumanBody[i]);
	glEnd();

	glBegin(GL_LINES);                //BODY effect
		glVertex2f(-13,11);
		glVertex2f(-15,9);
	glEnd();
}

void DrawHumanFace()
{
	//glColor3f(0.6,0.0,0.286);				//FACE
	//glColor3f(0.8,0.2,0.1);
	//glColor3f(0,0.5,1);
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);
	for(i=0;i<=42 ;i++)
		glVertex2fv(HumanFace[i]);
	glEnd();

	glColor3f(0,0,0);				//FACE outline
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=42 ;i++)
		glVertex2fv(HumanFace[i]);
	glEnd();

	glBegin(GL_LINE_STRIP);      //EAR effect
		glVertex2f(3.3,22);
		glVertex2f(4.4,23.5);
		glVertex2f(6.3,26);
	glEnd();
}
void DrawHumanBeak()
{


	glColor3f(0,0,0);				//BEAK outline
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=14 ;i++)
		glVertex2fv(HumanBeak[i]);
	glEnd();
}
void DrawHumanEyes()
{

	glColor3f(0,0,0);

	glPushMatrix();
	glRotated(-10,0,0,1);
	glTranslated(-6,32.5,0);      //Left eye
	glScalef(2.5,4,0);
	glutSolidSphere(1,20,30);
	glPopMatrix();

	glPushMatrix();
	glRotated(-1,0,0,1);
	glTranslated(-8,36,0);							//Right eye
	glScalef(2.5,4,0);
	glutSolidSphere(1,100,100);
	glPopMatrix();
}
void DrawAlien()
{
	DrawHumanBody();
//	DrawAlienCollar();
	DrawHumanFace();
	DrawHumanBeak();
	DrawHumanEyes();
}
void DrawSpaceshipBody()
{
	glColor3f(1,1,1);				//BASE

	/*glPushMatrix();
	glScalef(70,20,1);
	glutSolidSphere(1,50,50);
	glPopMatrix();*/
	glBegin(GL_POLYGON);


		glVertex2f(50,20);
glVertex2f(30,0);
		glVertex2f(-60,0);

glVertex2f(-80,20);

	glEnd();


}


void DrawSpaceShipLazer() {

	glColor3f(1, 0, 0);
	glPushMatrix();
	glBegin(GL_POLYGON);           //Lazer stem
		glVertex2f(-55+20 ,10);
		glVertex2f(-55+20 ,30);
		glVertex2f(-50+20, 30);
		glVertex2f(-50+20 ,10);
	glEnd();

	float xMid =0,yMid =0;
	//Mid point of the lazer horizontal
	xMid = (55+50)/2.0;
	yMid = (25+35)/2.0;

	//Rotating about the point ,20
	glTranslated(-xMid+20, yMid, 0);
	glRotated(LaserAngle, 0, 0 ,1);
	glTranslated(xMid , -yMid ,0);

	//find mid point of top of lazer stem
	float midPoint = -(55+50)/2.0;

	glBegin(GL_POLYGON);           //Lazer horizontal stem
		glVertex2f(midPoint + 10 ,25);
		glVertex2f(midPoint + 10 ,35);
		glVertex2f(midPoint - 10 ,35);
		glVertex2f(midPoint - 10 ,25);
	glEnd();

	glPopMatrix();
}
void Drawgun() {

	float xMid = -(55+50+20)/2.0;
	float yMid = (25+35)/2.0;

	float mouseXEnd = -((- mouseX) + xOne);
	float mouseYEnd = -((- mouseY) + yOne);
	glLineWidth(5);   //----Laser beam width

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(xMid ,yMid);
		glVertex2f(mouseXEnd ,mouseYEnd);
	glEnd();
	glLineWidth(1);
}
void DrawStone(int StoneIndex)
{
	glPushMatrix();
	glLoadIdentity();
	switch(StoneIndex)                           //CHANGE INDEX VALUE FOR DIFFERENT STONE VARIETY;
	{


	case 1:

			glColor3f(1.0f, 1.0f, 0.0f);

		glTranslated(xStone[index] , yStone[index] ,0);
		//glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(70,40,1);
		glutSolidSphere(1,9,50);
		glLoadIdentity();
glColor3f(1.f, 0.0f, 0.0f);
		glTranslated(xStone[index]+45 , yStone[index]+20 ,0);
		//glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(30,30,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glColor3f(1.f, 1.0f, 1.0f);
		glTranslated(xStone[index]+45 , yStone[index]+30 ,0);
		//glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(5,5,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glColor3f(1.f, 1.0f, 1.0f);
		glTranslated(xStone[index]+80 , yStone[index]+25 ,0);
		//glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,4,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();

		break;


	case 4:
		glColor3f(1.0f, 1.0f, 0.0f);

		glTranslated(xStone[index] , yStone[index] ,0);
		//glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(70,40,1);
		glutSolidSphere(1,9,50);
		glLoadIdentity();
glColor3f(1.f, 0.0f, 0.0f);
		glTranslated(xStone[index]+45 , yStone[index]+20 ,0);
		//glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(30,30,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glColor3f(1.f, 1.0f, 1.0f);
		glTranslated(xStone[index]+45 , yStone[index]+30 ,0);
		//glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(5,5,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glColor3f(1.f, 1.0f, 1.0f);
		glTranslated(xStone[index]+80 , yStone[index]+25 ,0);
		//glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,4,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();



		break;
	}
	glPopMatrix();
}
bool checkIfSpaceShipIsSafe() {
		for(int i =0 ;i<MAX_STONES ;i++) {
		if(stoneAlive[i]&((xOne >= (xStone[i]/2 -70) && xOne <= (xStone[i]/2 + 70) && yOne >= (yStone[i]/2 -18 ) && yOne <= (yStone[i]/2 + 53)) || (yOne <= (yStone[i]/2 - 20) && yOne >= (yStone[i]/2 - 90) && xOne >= (xStone[i]/2 - 40) && xOne <= (xStone[i]/2 + 40))))
		{
			stoneAlive[i]=0;
			return false;
		}
	}
	return true;
}
void SpaceshipCreate(){
	glPushMatrix();
	glTranslated(xOne,yOne,0);
	if(!checkIfSpaceShipIsSafe() && alienLife ){
		alienLife-=10;
		xHealthBarStart -= 230;
	}
//	DrawSpaceshipDoom();
	glPushMatrix();
	glTranslated(4,19,0);
	DrawAlien();
	glPopMatrix();
//	DrawSteeringWheel();
	DrawSpaceshipBody();
	DrawSpaceShipLazer();
	if(mButtonPressed) {
		Drawgun();
	}
	glEnd();
	glPopMatrix();
}
void DisplayHealthBar() {

	glColor3f(1 ,0 ,0);
	glBegin(GL_POLYGON);
		glVertex2f(-xHealthBarStart ,700);
		glVertex2f(1200 ,700);
		glVertex2f(1200 ,670);
		glVertex2f(-xHealthBarStart, 670);
	glEnd();
	char temp[40];
	glColor3f(0 ,0 ,1);
	sprintf(temp,"SCORE = %d",Score);
	displayRasterText(-1100 ,600 ,0.4 ,temp);//<---display variable score ?
	sprintf(temp,"  LIFE = %d",alienLife);
	displayRasterText(800 ,600 ,0.4 ,temp);
	sprintf(temp,"  LEVEL : %d",GameLvl);
	displayRasterText(-100 ,600 ,0.4 ,temp);
	glColor3f(1 ,0 ,0);
}

void GameScreenDisplay()
{
	SetDisplayMode(GAME_SCREEN);
	DisplayHealthBar();
	glScalef(2, 2 ,0);
	if(alienLife){
		SpaceshipCreate();
	}
	else {
		gameOver=true;

		startScreen = false;
	}								//<----------------------gameover screen

	StoneGenerate();

}


void GameOverScreen()
{
	exit(0);
}
void StoneGenerate(){



	for(int i=0; i<MAX_STONES ;i++){
		index = i;

		if(mouseX <= (xStone[i]/2+20) && mouseX >=(xStone[i]/2-20) && mouseY >= (yStone[i]/2-20) && mouseY <= (yStone[i]/2+20) && mButtonPressed){
			if(stoneAlive[i]){   // IF ALIVE KILL STONE
				stoneAlive[i]=0;
				Score++;
				if(Score%1==0) {
					stoneTranslationSpeed+=0;			//<--------------Rate of increase of game speed
				}
			}
		}
		xStone[i] += stoneTranslationSpeed;
		if(stoneAlive[i] )             //stone alive
			DrawStone(randomStoneIndices[i]);
	}

}

void display() {

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,1200,700);

	if(!gameOver)
		GameScreenDisplay();



	else if(gameOver)
		GameOverScreen();

	//Make spaceship bigger


	//Reset Scaling values
	glScalef(1/2 ,1/2 ,0);
	glFlush();
	glLoadIdentity();
	glutSwapBuffers();
}
void somethingMovedRecalculateLaserAngle() {

	float mouseXForTan = (-50 - mouseX) + xOne;
	float mouseYForTan = (35 - mouseY) + yOne;
	float LaserAngleInRadian = atan(mouseYForTan/mouseXForTan);
	LaserAngle = (180/PI) * LaserAngleInRadian;
}
void keys(unsigned char key, int x, int y)
{
	//if(key=='w' && key=='d' ){xOne+=0.5;yOne+=0.5;}
	if(key == 'd') xOne+=SPACESHIP_SPEED;
	if(key == 'a') xOne-=SPACESHIP_SPEED;
	if(key == 'w') {yOne+=SPACESHIP_SPEED;}
	if(key == 's') {yOne-=SPACESHIP_SPEED;}
	if(key == 'd' || key == 'a' || key == 'w' || key == 's')
		somethingMovedRecalculateLaserAngle();

	display();

}
void myinit()
{
	glClearColor(0.5,0.5,0.5,0);
	glColor3f(1.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluOrtho2D(-1200,1200,-700,700);                   //<-----CHANGE THIS TO GET EXTRA SPACE
//  gluOrtho2D(-200,200,-200,200);
	glMatrixMode(GL_MODELVIEW);
}
void passiveMotionFunc(int x,int y) {

	//when mouse not clicked
	mouseX = float(x)/(m_viewport[2]/1200.0)-600.0;  //converting screen resolution to ortho 2d spec
	mouseY = -(float(y)/(m_viewport[3]/700.0)-350.0);

	//Do calculations to find value of LaserAngle
	somethingMovedRecalculateLaserAngle();
	display();
}
 void mouseClick(int buttonPressed ,int state ,int x, int y) {

	if(buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mButtonPressed = true;
	else
		mButtonPressed = false;
	display();
}

 void idleCallBack() {			//when no mouse or keybord pressed
	 display();
 }
int main(int argc, char** argv) {


	glutInit(&argc, argv);
	glutInitWindowSize(1200,700);
	glutInitWindowPosition(90 ,0);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
//	glutTimerFunc(50,UpdateColorIndexForSpaceshipLights,0);
	glutCreateWindow("THE SPACESHIP SHOOTING GAME");
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutPassiveMotionFunc(passiveMotionFunc);
	glBlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);
	glutIdleFunc(idleCallBack);
	glutMouseFunc(mouseClick);
	glGetIntegerv(GL_VIEWPORT ,m_viewport);
	myinit();
	SetDisplayMode(GAME_SCREEN);
	initializeStoneArray();
	glutMainLoop();
 }
