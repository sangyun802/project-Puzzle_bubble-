#include "CollisionDetection.h"

bool CollisionDetection::operator()(const SolidSphere& sph1, const SolidSphere& sph2) {
	float distance = sqrt(dotProduct(sph1.getCenter() - sph2.getCenter(), sph1.getCenter() - sph2.getCenter()));
	
	return distance <= sph1.getRadius() + sph2.getRadius();
}