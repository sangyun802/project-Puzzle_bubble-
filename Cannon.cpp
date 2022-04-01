#include "Cannon.h"

Cannon::Cannon() {
	angle = 0;
	setMTL();
}
void Cannon::setangle(float ang) {
	angle = ang;
}
float Cannon::getangle()const {
	return angle;
}
void Cannon::setMTL() {
	mtl.setAmbient(0.3, 0.3, 0.3, 1);
	mtl.setDiffuse(0.5, 0.5, 0.5, 1);
	mtl.setSpecular(1.0, 1.0, 1.0, 1);
	mtl.setShininess(10);
	mtl.setEmission(0.1, 0.1, 0.1, 1);
}
void Cannon::draw()const {
	glPushMatrix();
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());
	glTranslatef(0, -350, 0);
	glRotatef(-angle, 0.0f, 0.0f, 1.0f);
	glRotatef(-90.0, 1.0f, 0.0f, 0.0f);
	glTranslatef(0, 0, 35);
	glutSolidCone(25, 30, 100, 100);
	glTranslatef(0, 0, -85);
	glBegin(GL_POLYGON);
	GLUquadricObj* obj = gluNewQuadric();
	gluCylinder(obj, 10, 10, 100, 100, 100);
	glEnd();
	glPopMatrix();
}