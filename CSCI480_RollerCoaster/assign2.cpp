/*
	Homework2: Roller Coaster Simulation
	Author: Yanqing Liu
	Date:   2013-11-20
*/

#include "stdafx.h"
#include "CatmullRom_splines.h"
#include "skybox.h"
#include <math.h>
#include <pic.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define bool int
#define false -1
#define true 1

/////////////////////////////////////////////
//Global variables

//original width and height of application window
GLint g_width = 640;
GLint g_height = 480;

//size of skybox
GLuint skysize = 200;


GLint g_iMenuId;

GLint g_vMousePos[2] = {0, 0};
GLint g_iLeftMouseButton = 0;    // 1 if pressed, 0 if not 
GLint g_iMiddleMouseButton = 0;
GLint g_iRightMouseButton = 0;

typedef enum { ROTATE, TRANSLATE, SCALE } CONTROLSTATE;

CONTROLSTATE g_ControlState = ROTATE;//default value

// state of the world 
GLfloat g_vLandRotate[3] = {0.0, 0.0, 0.0};
GLfloat g_vLandTranslate[3] = {0.0, 0.0, 0.0};
GLfloat g_vLandScale[3] = {1.0, 1.0, 1.0};

//Camera state
GLfloat g_oriCamX = 30.0;
GLfloat g_oriCamY = 20.0;
GLfloat g_oriCamZ = 50.0;

GLfloat g_oriCenX = 30.0;
GLfloat g_oriCenY = 20.0;
GLfloat g_oriCenZ = 0.0;

GLfloat g_camPos[3] = {g_oriCamX, g_oriCamY, g_oriCamZ};
GLfloat g_camCenter[3] = {g_oriCenX, g_oriCenY, g_oriCenZ};
GLfloat g_camUp[3] = {0.0, 1.0, 0.0};

GLfloat g_camAngleX = 0.0;
GLfloat g_camAngleY = 0.0;

GLfloat theta = 0.0; //rotate angle variable for mouse controlling

//set light properties
GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_position[] = {300, 300, -300, 0.0}; 

//set material properties
//track material, simulating metal, low diffuse component and hight specular component
GLfloat track_ambient[] = {0.35, 0.35, 0.35, 1.0};
GLfloat track_diffuse[] = {0.0, 0.0, 0.0, 1.0};
GLfloat track_specular[] = {0.5, 0.5, 0.5, 1.0};
//support cylinder, simulating red wooden, hight diffuse component and low specular component
GLfloat sup_ambient[] = {0.7, 0.0, 0.0, 1.0};
GLfloat sup_diffuse[] = {0.7, 0.0, 0.0, 1.0};
GLfloat sup_specular[] = {0.1, 0.1, 0.1, 1.0};
//support cylinder, simulating brown coarse wooden, hight diffuse component and low specular component
GLfloat plank_ambient[] = {0.85, 0.58, 0.27, 1.0};
GLfloat plank_diffuse[] = {0.85, 0.58, 0.27, 1.0};
GLfloat plank_specular[] = {0.0, 0.0, 0.0, 0.0};


//control variable

bool g_takeShot = false; //take screen shot when g_takeShot is true

bool g_startRide = false;//start riding animation when g_startRide is true
int g_camMotion = 0;     //the moving control of camera

bool g_lighton = false;  //turn on light when g_lighton is true

// the spline array 
spline *g_Splines;
// total number of splines
GLuint g_iNumOfSplines;
//The struct storing all points in the spline
orient trackOri;
//Hold the track object into display list
GLuint tracklist; 
//Texture name for planks
GLuint plankTex;

//variables for recording images
char fileName[100];
int i = 0;
int g_frameRate = 0;

/////////////////////////////////////////////
//Screen shot function
// Write a screenshot to the specified filename 
void saveScreenshot (char *filename)
{
	int i, j;
	Pic *in = NULL;

	if (filename == NULL)
		return;

	/* Allocate a picture buffer */
	in = pic_alloc(640, 480, 3, NULL);

	printf("File to save to: %s\n", filename);

	for (i=479; i>=0; i--) {
		glReadPixels(0, 479-i, 640, 1, GL_RGB, GL_UNSIGNED_BYTE,
			&in->pix[i*in->nx*in->bpp]);
	}

	if (jpeg_write(filename, in))
		printf("File saved Successfully\n");
	else
		printf("Error in Saving\n");

	pic_free(in);
}
/////////////////////////////////////////////
int loadSplines(char *argv) {
	char *cName = (char *)malloc(128 * sizeof(char));
	FILE *fileList;
	FILE *fileSpline;
	int iType, i = 0, j, iLength;

	// load the track file
	fileList = fopen(argv, "r");
	if (fileList == NULL) {
		printf ("can't open file\n");
		exit(1);
	}

	// stores the number of splines in a global variable
	fscanf(fileList, "%d", &g_iNumOfSplines);

	g_Splines = (spline *)malloc(g_iNumOfSplines * sizeof(spline));

	// reads through the spline files 
	for (j = 0; j < g_iNumOfSplines; j++) {
		i = 0;
		fscanf(fileList, "%s", cName);
		fileSpline = fopen(cName, "r");

		if (fileSpline == NULL) {
			printf ("can't open file\n");
			exit(1);
		}

		// gets length for spline file
		fscanf(fileSpline, "%d %d", &iLength, &iType);

		// allocate memory for all the points
		g_Splines[j].points = (point *)malloc(iLength * sizeof( point));
		g_Splines[j].numControlPoints = iLength;

		// saves the data to the struct
		while (fscanf(fileSpline, "%lf %lf %lf", 
			&g_Splines[j].points[i].x, 
			&g_Splines[j].points[i].y, 
			&g_Splines[j].points[i].z) != EOF) {
				i++;
		}
	}
	free(cName);

	return 0;
}


/////////////////////////////////////////////
//Planks

//Load plank texture and create texture object, return texture object name
void loadPlankTex(){
	Pic* wooden = jpeg_read("texture/plank2.jpg", NULL);

	if (!wooden){
		printf ("error reading plank texture file!\n" );
		exit(1);
	}

	glGenTextures(1, &plankTex);

	glBindTexture(GL_TEXTURE_2D, plankTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wooden->nx, wooden->ny, 0, GL_RGB, GL_UNSIGNED_BYTE, wooden->pix);

}

void polygon(vector3f* vertlist, int a, int b, int c , int d)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, plank_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plank_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plank_specular);

	glEnable(GL_TEXTURE_2D);
	//set GL_MODULATE to enable lighting mix with texture or primitives color
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// draw a polygon via list of vertices
	glBindTexture(GL_TEXTURE_2D, plankTex);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0,0.0);
	glVertex3f(vertlist[a].x,vertlist[a].y,vertlist[a].z);
	glTexCoord2f(0.0,0.0);
	glVertex3f(vertlist[b].x,vertlist[b].y,vertlist[b].z);
	glTexCoord2f(0.0,1.0);
	glVertex3f(vertlist[c].x,vertlist[c].y,vertlist[c].z);
	glTexCoord2f(1.0,1.0);
	glVertex3f(vertlist[d].x,vertlist[d].y,vertlist[d].z);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

int cubeboy(vector3f* veclist){
	
	
	polygon(veclist,0,3,2,1);
	polygon(veclist,2,3,7,6);
	polygon(veclist,0,4,7,3);
	polygon(veclist,1,2,6,5);
	polygon(veclist,4,5,6,7);
	polygon(veclist,0,1,5,4);

	return 1;
}

//draw planks along track
int plank(orient r){//
	float s = 0.3;
	vector3f vecarr[8];

	for (int i = 0; i < r.length; i=i+10){
		vecarr[5] = r.vertice[i]; //front right bottom 
		vecarr[1] = vecarr[5] + r.tangent[i].scale(s);//back right bottom
		vecarr[2] = vecarr[1] + r.beside[i].scale(s); //back right up
		vecarr[6] = vecarr[5] + r.beside[i].scale(s); //front right up

		vecarr[4] = vecarr[5] + r.normal[i];//front left bottom;
		vecarr[0] = vecarr[1] + r.normal[i];//back left bottom;
		vecarr[3] = vecarr[2] + r.normal[i];//back right up
		vecarr[7] = vecarr[6] + r.normal[i];//front right up
		
		cubeboy(vecarr);
	}
	return 1;
}
///////////////////////////////////////////////////////////////////////
//draw cylinder between two points p1, p2
//This function is referenced from http://lifeofaprogrammergeek.blogspot.com/2008/07/rendering-cylinder-between-two-points.html
void renderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{
	float vx = x2-x1;
	float vy = y2-y1;
	float vz = z2-z1;

	float v = sqrt( vx*vx + vy*vy + vz*vz );
	double ax = 0.0;

	double zero = 1.0e-3;

	if (fabs(vz) < zero){
		ax = 57.2957795*acos( vx/v ); // rotation angle in x-y plane
		if ( vx <= 0.0 ) ax = -ax;
	}
	else{
		ax = 57.2957795*acos( vz/v ); // rotation angle
		if ( vz <= 0.0 ) ax = -ax;
	}
	float rx = -vy*vz;
	float ry = vx*vz;
	glPushMatrix();
	//draw the cylinder body
	glTranslatef( x1,y1,z1 );
	if (fabs(vz) < zero){
		glRotated(90.0, 0, 1, 0.0); // Rotate & align with x axis
		glRotated(ax, -1.0, 0.0, 0.0); // Rotate to point 2 in x-y plane
	}
	else{
		glRotated(ax, rx, ry, 0.0); // Rotate about rotation vector
	}
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluCylinder(quadric, radius, radius, v, subdivisions, 1);

	glPopMatrix();
}

void renderCylinder_convenient(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions)
{
	//the same quadric can be re-used for drawing many cylinders
	GLUquadricObj *quadric=gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	renderCylinder(x1,y1,z1,x2,y2,z2,radius,subdivisions,quadric);
	gluDeleteQuadric(quadric);
}

//////////////////////////////////////////////////////////
//Track
int track(){


	//create display list
	tracklist = glGenLists(1);
	glNewList(tracklist, GL_COMPILE);
	
	trackOri = createCRspline(g_Splines[0]);
	

	//set material
	//glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, track_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, track_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, track_specular);

	glColor3f(0.35, 0.35, 0.35);
	//Draw right track
	for (int i = 1; i < trackOri.length; i++)
	{
		vector3f rightTrack = trackOri.vertice[i];
		//glVertex3f(rightTrack.x, rightTrack.y, rightTrack.z);
		renderCylinder_convenient(trackOri.vertice[i-1].x,trackOri.vertice[i-1].y,trackOri.vertice[i-1].z,rightTrack.x, rightTrack.y, rightTrack.z,0.1,20);
	}

	//Draw left track
	for (int i = 0; i < trackOri.length; i++)
	{
		vector3f leftTrack = trackOri.vertice[i] + trackOri.normal[i];
		vector3f aleftTrack = trackOri.vertice[i-1] + trackOri.normal[i-1];
		renderCylinder_convenient(aleftTrack.x, aleftTrack.y, aleftTrack.z,leftTrack.x, leftTrack.y, leftTrack.z,0.1,20);
	}
	
	//set material
	//glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sup_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sup_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sup_specular);

	glColor3f(0.7,0.0,0.0);
	//Draw support cylinder for right track
	for (int i = 1; i < trackOri.length; i = i+50)
	{
		vector3f rightTrack = trackOri.vertice[i];
		renderCylinder_convenient(rightTrack.x, rightTrack.y, -100.0 ,rightTrack.x, rightTrack.y, rightTrack.z,0.1,20);
	}
	//Draw support cylinder for left track
	for (int i = 0; i < trackOri.length; i=i+50)
	{
		vector3f leftTrack = trackOri.vertice[i] + trackOri.normal[i];
		vector3f aleftTrack = trackOri.vertice[i-1] + trackOri.normal[i-1];
		renderCylinder_convenient(leftTrack.x, leftTrack.y, -100.0, leftTrack.x, leftTrack.y, leftTrack.z,0.1,20);
	}
	glColor3f(1.0,1.0,1.0);
	//draw planks
	plank(trackOri);

	glEndList();
	
	return 1;
}
//////////////////////////////////////////////////////////
//Animation 
void ride(){
	if(g_startRide == true){
		//restore the camera state when starting animation
		g_vLandRotate[0] = 0.0;g_vLandRotate[1] = 0.0;g_vLandRotate[2] = 0.0;

		g_vLandTranslate[0] = 0.0;g_vLandTranslate[1] = 0.0;g_vLandTranslate[2] = 0.0;

		g_vLandScale[0] = 1.0;g_vLandScale[1] = 1.0;g_vLandScale[2] = 1.0;

		g_camAngleX = 0.0;g_camAngleY = 0.0;

		theta = 0.0;

		//Moving camera along the track
		//calculating the camera position 
		vector3f offset = trackOri.normal[g_camMotion].scale(0.5) + trackOri.beside[g_camMotion];		

		g_camUp[0] = trackOri.beside[g_camMotion].x;
		g_camUp[1] = trackOri.beside[g_camMotion].y;
		g_camUp[2] = trackOri.beside[g_camMotion].z;

		g_camPos[0] = trackOri.vertice[g_camMotion].x + offset.x;
		g_camPos[1] = trackOri.vertice[g_camMotion].y + offset.y;
		g_camPos[2] = trackOri.vertice[g_camMotion].z + offset.z;

		g_camCenter[0] = trackOri.vertice[g_camMotion + 1].x + offset.x;
		g_camCenter[1] = trackOri.vertice[g_camMotion + 1].y + offset.y;
		g_camCenter[2] = trackOri.vertice[g_camMotion + 1].z + offset.z;
		

		
		if(g_camMotion < trackOri.length-1){//moving camera
			g_camMotion++;
		}else{//animation completed, return to original scene
			g_camMotion = 0;
			g_startRide = false;

			//restore original setting of camera
			g_camPos[0] = g_oriCamX;    g_camPos[1] = g_oriCamY;    g_camPos[2] = g_oriCamZ;
			g_camCenter[0] = g_oriCenX; g_camCenter[1] = g_oriCenX; g_camCenter[2] = g_oriCenX;
			g_camUp[0] = 0.0; g_camUp[1] = 1.0; g_camUp[2] = 0.0;
		}
	}
}


/////////////////////////////////////////////
void myinit()
{
	// setup gl view here
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0, g_width/g_height, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);

	gluLookAt(g_camPos[0], g_camPos[1], g_camPos[2], g_camCenter[0], g_camCenter[1], g_camCenter[2], g_camUp[0], g_camUp[1], g_camUp[2]);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	//set lighting properties
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	//Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glEnable(GL_COLOR_MATERIAL);

	//load texture
	loadSkyTexture();
	createTextureObj();
	loadPlankTex();
	//Draw track and put it into display list
	track();

}

void myReshape(int w, int h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w != 0 && h != 0)
		gluPerspective(120.0, w/h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

//main draw function
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
	//Enable or Disable lighting
	if(g_lighton == true){
  		glEnable(GL_LIGHT0);
	}else{
  		glDisable(GL_LIGHT0);
	}

  
	//Control camera to navigate the scene
	glLoadIdentity();	
	//gluLookAt(g_camPos[0], g_camPos[1], g_camPos[2], g_camCenter[0], g_camCenter[1], g_camCenter[2], g_camUp[0], g_camUp[1], g_camUp[2]);
	
	//glLoadIdentity();
	glTranslatef(g_vLandTranslate[0],g_vLandTranslate[1],g_vLandTranslate[2]);
	glRotatef(theta,g_vLandRotate[0],g_vLandRotate[1],g_vLandRotate[2]); 
	glScalef(g_vLandScale[0],g_vLandScale[1],g_vLandScale[2]);
  
	//moving camera
	glRotatef(g_camAngleX, 1.0, 0.0, 0.0);
	glRotatef(g_camAngleY, 0.0, 1.0, 0.0);
	
	gluLookAt(g_camPos[0], g_camPos[1], g_camPos[2], g_camCenter[0], g_camCenter[1], g_camCenter[2], g_camUp[0], g_camUp[1], g_camUp[2]);
	
	glCallList(tracklist);

	glRotatef(90,1.0,0.0,0.0);
	//glDisable(GL_LIGHTING);
	drawSkybox(30,20,0,skysize);


	
	glFlush();	
	glutSwapBuffers(); 
}

void menufunc(int value)
{
  switch (value)
  {
    case 0:
      exit(0);
      break;
  }
}

void doIdle(){
	ride();

	if(g_takeShot == true){
		sprintf(fileName,"record/record.%04d.jpg",i);
		saveScreenshot(fileName);
		g_takeShot = -g_takeShot;
		i++;
	}
	glutPostRedisplay();
}


void mousedrag(int x, int y)
{
 
  int vMouseDelta[2] = {x-g_vMousePos[0], y-g_vMousePos[1]};
  
  switch (g_ControlState)
  {
    case TRANSLATE:  
      if (g_iLeftMouseButton)
      {
        g_vLandTranslate[0] += vMouseDelta[0]*0.1;
        g_vLandTranslate[1] -= vMouseDelta[1]*0.1;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandTranslate[2] += vMouseDelta[1]*0.1;
      }
      break;
    case ROTATE:
    	theta += 2;
      if (g_iLeftMouseButton)
      {
        g_vLandRotate[0] += vMouseDelta[1];
        g_vLandRotate[1] += vMouseDelta[0];
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandRotate[2] += vMouseDelta[1];
      }
     
      break;
    case SCALE:
      if (g_iLeftMouseButton)
      {
        g_vLandScale[0] *= 1.0+vMouseDelta[0]*0.01;
        g_vLandScale[1] *= 1.0-vMouseDelta[1]*0.01;
      }
      if (g_iMiddleMouseButton)
      {
        g_vLandScale[2] *= 1.0-vMouseDelta[1]*0.01;
      }
      break;
  }
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
  
  glutPostRedisplay();
}


void mouseidle(int x, int y)
{
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void mousebutton(int button, int state, int x, int y)
{

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
      g_iLeftMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_MIDDLE_BUTTON:
      g_iMiddleMouseButton = (state==GLUT_DOWN);
      break;
    case GLUT_RIGHT_BUTTON:
      g_iRightMouseButton = (state==GLUT_DOWN);
      break;
  }
  	 
  switch(glutGetModifiers())
  {
    case GLUT_ACTIVE_CTRL:
      g_ControlState = 	TRANSLATE;
      break;
    case GLUT_ACTIVE_SHIFT:
      g_ControlState = SCALE;
      break;
    default:
      g_ControlState = ROTATE;
      break;
  }
  g_vMousePos[0] = x;
  g_vMousePos[1] = y;
}

void myKey(unsigned char key, int x, int y)
{
	switch(key){
		case 'w':
			g_camAngleX += 2.0;
			glutPostRedisplay();
			break;
		case 's':
			g_camAngleX -= 2.0;
			glutPostRedisplay();
			break;	
		default:
		glutPostRedisplay();		   
	}
}

void specialKey(int key, int x, int y){
	switch(key){
	case(GLUT_KEY_UP):
		//moving the camera towards the terrain	
		g_camPos[2] = g_camPos[2]/1.1;
		glutPostRedisplay();
		break;
	case(GLUT_KEY_DOWN):
		//moving the camera far from the terrain
		g_camPos[2] = g_camPos[2]/0.75;
		glutPostRedisplay();
		break;
	case(GLUT_KEY_LEFT):
		g_camAngleY -= 2.0;
		if(g_camAngleY > 360.0)
			g_camAngleY = g_camAngleY - 360.0;
		if(g_camAngleY < -360.0)
			g_camAngleY = g_camAngleY + 360.0;
		glutPostRedisplay();
		break;
	case(GLUT_KEY_RIGHT):
		g_camAngleY += 2.0;
		if(g_camAngleY > 360.0)
			g_camAngleY = g_camAngleY - 360.0;
		if(g_camAngleY < -360.0)
			g_camAngleY = g_camAngleY + 360.0;
		glutPostRedisplay();
		break;
	case (GLUT_KEY_F1):
		//stop or start the animation
		g_startRide = -g_startRide;
		break;
	case (GLUT_KEY_F2):
		//turn on or turn off lighting
		g_lighton = -g_lighton;
		glutPostRedisplay();
		break;
	case (GLUT_KEY_F3):
		//take screen shots
		g_takeShot = true;
		glutPostRedisplay();
		break;
	default: 	
		break;	  
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("Loading...");

	if (argc<2)
	{  
		printf ("usage: %s <trackfile>\n", argv[0]);
		exit(0);
	}


	loadSplines(argv[1]);

	glutInit(&argc,argv);
  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowSize(g_width,g_height);
	glutCreateWindow("Roller Coaster");
	// do initialization
	myinit();
	
	//Enable depth test
	glEnable(GL_DEPTH_TEST);
  
	// tells glut to use a particular display function to redraw
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
  
   // allow the user to quit using the right mouse button menu 
	g_iMenuId = glutCreateMenu(menufunc);
	//Set the current menu to the menu ID.
	glutSetMenu(g_iMenuId);
	glutAddMenuEntry("Quit",0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
  
  
	//Navigate the scene when no event is triggered
	glutIdleFunc(doIdle);
	// callback for mouse drags 
	glutMotionFunc(mousedrag);
	// callback for idle mouse movement
	glutPassiveMotionFunc(mouseidle);
	// callback for mouse button changes
	glutMouseFunc(mousebutton);

	//call back for keyboard changes
	glutKeyboardFunc(myKey);
	glutSpecialFunc(specialKey);

	glutMainLoop();
  
	return 0;
}