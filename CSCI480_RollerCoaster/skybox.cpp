#include "skybox.h"
#include <pic.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>


Pic *g_skyfront, *g_skyback, *g_skyleft, *g_skyright, *g_skyup, *g_skybottom;
GLuint skyTexture[6];

void loadSkyTexture(){
	//Load texture file	
	g_skyfront = jpeg_read("texture/front.jpg", NULL);
	g_skyback = jpeg_read("texture/back.jpg", NULL);
	g_skyleft = jpeg_read("texture/left.jpg", NULL);
	g_skyright = jpeg_read("texture/right.jpg", NULL);
	g_skyup = jpeg_read("texture/up.jpg", NULL);
	g_skybottom = jpeg_read("texture/down.jpg", NULL);

	if (!g_skyfront || !g_skyback || !g_skyleft ||!g_skyright
		|| !g_skyup|| !g_skybottom)
	{
		printf ("error reading texture file!\n" );
		exit(1);
	}
}

void createTextureObj(){

	glGenTextures(6, skyTexture);

	//bind front texture of skybox
	glBindTexture(GL_TEXTURE_2D, skyTexture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_skyfront->nx, g_skyfront->ny, 0, GL_RGB, GL_UNSIGNED_BYTE, g_skyfront->pix);


	//bind front texture of skybox
	glBindTexture(GL_TEXTURE_2D, skyTexture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_skyback->nx, g_skyback->ny, 0, GL_RGB, GL_UNSIGNED_BYTE, g_skyback->pix);

	//bind left texture of skybox
	glBindTexture(GL_TEXTURE_2D, skyTexture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_skyleft->nx, g_skyleft->ny, 0, GL_RGB, GL_UNSIGNED_BYTE, g_skyleft->pix);

	//bind right texture of skybox
	glBindTexture(GL_TEXTURE_2D, skyTexture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_skyright->nx, g_skyright->ny, 0, GL_RGB, GL_UNSIGNED_BYTE, g_skyright->pix);

	//bind up texture of skybox
	glBindTexture(GL_TEXTURE_2D, skyTexture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_skyup->nx, g_skyup->ny, 0, GL_RGB, GL_UNSIGNED_BYTE, g_skyup->pix);

	//bind bottom texture of skybox
	glBindTexture(GL_TEXTURE_2D, skyTexture[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_skybottom->nx, g_skybottom->ny, 0, GL_RGB, GL_UNSIGNED_BYTE, g_skybottom->pix);


}


//Draw a cubic skybox centered at point (cx,cy,cz)
void drawSkybox(float cx, float cy, float cz, float size){
	//(lx,ly,lz) is the left bottom back point
	float lx = cx - size  / 2; 
	float ly = cy - size / 2;
	float lz = cz - size / 2;

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	// front side
	glBindTexture(GL_TEXTURE_2D, skyTexture[0]);
	glBegin(GL_QUADS);	
	glTexCoord2f(0.0, 1.0); glVertex3f(lx,      ly,      lz+size);
	glTexCoord2f(0.0, 0.0); glVertex3f(lx,      ly+size, lz+size);
	glTexCoord2f(1.0, 0.0); glVertex3f(lx+size, ly+size, lz+size); 
	glTexCoord2f(1.0, 1.0); glVertex3f(lx+size, ly,      lz+size);
	glEnd();


	// back side
	glBindTexture(GL_TEXTURE_2D, skyTexture[1]);
	glBegin(GL_QUADS);		
	glTexCoord2f(0.0, 1.0); glVertex3f(lx+size, ly,      lz);
	glTexCoord2f(0.0, 0.0); glVertex3f(lx+size, ly+size, lz); 
	glTexCoord2f(1.0, 0.0); glVertex3f(lx,      ly+size, lz);
	glTexCoord2f(1.0, 1.0); glVertex3f(lx,      ly,      lz);
	glEnd();

	// left side
	glBindTexture(GL_TEXTURE_2D, skyTexture[2]);
	glBegin(GL_QUADS);		
	glTexCoord2f(0.0, 0.0); glVertex3f(lx, ly+size,	lz);	
	glTexCoord2f(1.0, 0.0); glVertex3f(lx, ly+size,	lz+size); 
	glTexCoord2f(1.0, 1.0); glVertex3f(lx, ly,		lz+size);
	glTexCoord2f(0.0, 1.0); glVertex3f(lx, ly,		lz);		
	glEnd();

	// right side
	glBindTexture(GL_TEXTURE_2D, skyTexture[3]);
	glBegin(GL_QUADS);		
	glTexCoord2f(1.0, 1.0); glVertex3f(lx+size, ly,	     lz);
	glTexCoord2f(0.0, 1.0); glVertex3f(lx+size, ly,	     lz+size);
	glTexCoord2f(0.0, 0.0); glVertex3f(lx+size, ly+size, lz+size); 
	glTexCoord2f(1.0, 0.0); glVertex3f(lx+size, ly+size, lz);
	glEnd();

	// up side
	glBindTexture(GL_TEXTURE_2D, skyTexture[4]);
	glBegin(GL_QUADS);		
	glTexCoord2f(1.0, 0.0); glVertex3f(lx+size, ly+size, lz);
	glTexCoord2f(1.0, 1.0); glVertex3f(lx+size, ly+size, lz+size); 
	glTexCoord2f(0.0, 1.0); glVertex3f(lx,	    ly+size, lz+size);
	glTexCoord2f(0.0, 0.0); glVertex3f(lx,	    ly+size, lz);
	glEnd();

	// bottom side
	glBindTexture(GL_TEXTURE_2D, skyTexture[5]);
	glBegin(GL_QUADS);		
	glTexCoord2f(0.0, 1.0); glVertex3f(lx,	    ly, lz);
	glTexCoord2f(0.0, 0.0); glVertex3f(lx,	    ly, lz+size);
	glTexCoord2f(1.0, 0.0); glVertex3f(lx+size, ly, lz+size); 
	glTexCoord2f(1.0, 1.0); glVertex3f(lx+size, ly, lz);

	glEnd();

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}