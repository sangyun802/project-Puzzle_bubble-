#include "Timebar.h"

Timebar::Timebar() {
	velocity = 0.1;
	length = 250;
}
void Timebar::draw(){
	if (length >= 50)
	{
		glColor3f(1, 0, 0);
		glLineWidth(10.0f);
		glBegin(GL_LINES);

		//Time
		glVertex2f(50, 320);
		glVertex2f(length, 320);
		glEnd();
		length -= velocity;
	}
}
void Timebar::setTimebar(float l){
	length = l;
}

void Timebar::setVelocity(float v) {
	velocity = v;
}
float Timebar::getTimebar() {
	return length;
}