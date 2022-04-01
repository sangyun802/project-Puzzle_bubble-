#pragma once
#include "SolidSphere.h"

class Remove {
public:
	bool operator()(const SolidSphere& sph);
};