#include "SolidSphere.h"

SolidSphere::SolidSphere(float r, int sl, int st) : SolidShape3D() {
	radius = r;
	segmentation[0] = sl;
	segmentation[1] = st;
	
	remove = drop = false;
}

SolidSphere::SolidSphere(const SolidSphere& sph) : SolidShape3D(sph) {
	radius = sph.radius;
	segmentation[0] = sph.segmentation[0];
	segmentation[1] = sph.segmentation[1];
	
	for (int i = 0; i < 6; i++) {
		surround[i] = sph.surround[i];
	}
	remove = sph.remove;
	drop = sph.drop;
}


float SolidSphere::getRadius() const {
	return radius;
}

void SolidSphere::setSurround(Vector3 pos) {
	surround[0].setXYZ(pos[0] - radius, pos[1] + sqrt(3) * radius, 0);
	surround[1].setXYZ(pos[0] + radius, pos[1] + sqrt(3) * radius, 0);
	surround[2].setXYZ(pos[0] + 2 * radius, pos[1], 0);
	surround[3].setXYZ(pos[0] + radius, pos[1] - sqrt(3) * radius, 0);
	surround[4].setXYZ(pos[0] - radius, pos[1] - sqrt(3) * radius, 0);
	surround[5].setXYZ(pos[0] - 2 * radius, pos[1], 0);
}

void SolidSphere::draw() const {
	glPushMatrix();
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glTranslatef(center[0], center[1], center[2]);
	glutSolidSphere(radius, segmentation[0], segmentation[1]);
	glPopMatrix();
}

const Vector3* SolidSphere::getSurround()const {
	return surround;
}
void SolidSphere::setRemove(bool r) {
	remove = r;
}
void SolidSphere::setDrop(bool d) {
	drop = d;
}
bool SolidSphere::getDrop()const { return drop; }
bool SolidSphere::getRemove()const { return remove; }