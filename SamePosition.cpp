#include "SamePosition.h"

void SamePosition::setpos(Vector3 center) {
	pos = center;
}
bool SamePosition::operator()(const SolidSphere& sph) {
	//좌표 계산 중 오차를 고려
	if (abs(pos[0] - sph.getCenter()[0]) <= 0.001 && abs(pos[1] - sph.getCenter()[1]) <= 0.001)
		return true;
	else
		return false;
}