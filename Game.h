#pragma once
#include <vector>
#include <algorithm>
#include <string>
#include <FreeImage.h>

#include "Light.h"
#include "Cannon.h"
#include "Timebar.h"
#include "Remove.h"
#include "Drop.h"
#include "CollisionHandling.h"
#include "SamePosition.h"

void draw_characters_white(void* font, const char* c, float x, float y);
void draw_characters_purple(void* font, const char* c, float x, float y);
void ColorHandling(SolidSphere& sph);
void DropHandling2(SolidSphere& sph);
void DropHandling();
bool EndDetection(const SolidSphere& sph);
FIBITMAP* createBitMap(char const* filename);
void init();
void idle();
void drawQuadWithTexturing();
void display(void);
void renderScene();
void processSpecialKeys(int key, int x, int y);
void processNormalKeys(unsigned char key, int x, int y);