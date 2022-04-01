#pragma once
#include "SolidShape3D.h"

class Cannon :public SolidShape3D {
public:
	Cannon();
	void setangle(float ang);
	float getangle() const;
	void draw()const;
	void setMTL();
private:
	float angle;
};