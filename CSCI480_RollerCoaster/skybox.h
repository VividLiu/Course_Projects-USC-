#ifndef _SKYBOX_H
#define _SKYBOX_H

//load 6 image from file as texture
void loadSkyTexture();
//Create texture objects for six faces and set texture properties
void createTextureObj();

//Create a cubic skybox centered at point(x,y,x)
//Texture map all faces
void drawSkybox(float cx, float cy, float cz, float size);

#endif
