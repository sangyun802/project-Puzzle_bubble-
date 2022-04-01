#pragma once
#include "SolidSphere.h"

class SamePosition {
public:
	void setpos(Vector3 center);
	bool operator()(const SolidSphere& sph);
private:
	Vector3 pos;
};