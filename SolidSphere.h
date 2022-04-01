#pragma once
#include <math.h>
#include "SolidShape3D.h"

class SolidSphere : public SolidShape3D {
public:
	friend class CollisionHandling;

	SolidSphere(float r, int sl, int st);
	SolidSphere(const SolidSphere& sph);

	float getRadius() const;
	void setSurround(Vector3 pos);
	void draw() const;
	const Vector3* getSurround() const;
	void setRemove(bool r);
	void setDrop(bool d);
	bool getDrop() const;
	bool getRemove()const;

private:
	float radius;
	int segmentation[2];
	Vector3 surround[6]; //coordinate of surrounded 6 spheres
	bool remove; //if remove is true, we must remove this shphere
	bool drop; //if drop is ture, we must drop this sphere
};

