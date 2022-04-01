#include "Drop.h"

bool Drop::operator()(const SolidSphere& sph) {
	if (sph.getDrop() == false)
		return true;
	else
		return false;
}