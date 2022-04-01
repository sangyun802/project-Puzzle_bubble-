#include "Remove.h"

bool Remove::operator()(const SolidSphere& sph) {
	if (sph.getRemove() == true)
		return true;
	else
		return false;
}