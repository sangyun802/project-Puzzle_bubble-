#pragma once
#include "SolidSphere.h"

class Drop {
public:
	bool operator()(const SolidSphere& sph);
};
