#pragma once
#include<GL/glut.h>

class Timebar {
public:
	Timebar();
	void setTimebar(float l);
	void draw();
	void setVelocity(float v);
	float getTimebar();

private:
	float length, velocity;
};