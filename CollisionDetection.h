#pragma once
#include "SolidSphere.h"

class CollisionDetection {
public:
	bool operator()(const SolidSphere& sph1, const SolidSphere& sph2);
};

