#include "CollisionHandling.h"

#define radius 25

void CollisionHandling::operator()(SolidSphere& sph, SolidSphere& new_sph) {
	CollisionDetection collision;
	if (collision(sph, new_sph)) {
		//6자리 중에 가까운 자리 붙고 서로의 위치 저장
		if (sph.center[0] == new_sph.center[0]) {
			if (new_sph.center[1] >= sph.center[1]) {
				new_sph.setCenter(sph.center[0] + radius, sph.center[1] + sqrt(3) * radius, 0);
			}
			else {
				new_sph.setCenter(sph.center[0] - radius, sph.center[1] - sqrt(3) * radius, 0);
			}
		}
		else {
			float slop = (sph.center[1] - new_sph.center[1]) / (sph.center[0] - new_sph.center[0]);
			if (new_sph.center[0] <= sph.center[0]) {
				if (slop <= - sqrt(3) / 3) {
					new_sph.setCenter(sph.center[0] - radius, sph.center[1] + sqrt(3) * radius, 0);
				}
				else if (slop >= sqrt(3) / 3) {
					new_sph.setCenter(sph.center[0] - radius, sph.center[1] - sqrt(3) * radius, 0);
				}
				else {
					new_sph.setCenter(sph.center[0] - 2 * radius, sph.center[1], 0);
				}
			}
			else {
				if (slop <= - sqrt(3) / 3) {
					new_sph.setCenter(sph.center[0] + radius, sph.center[1] - sqrt(3) * radius, 0);
				}
				else if (slop >= sqrt(3) / 3) {
					new_sph.setCenter(sph.center[0] + radius, sph.center[1] + sqrt(3) * radius, 0);
					
				}
				else {
					new_sph.setCenter(sph.center[0] + 2 * radius, sph.center[1], 0);
				}
			}
		}
		
		new_sph.setVelocity(0, 0, 0);
		new_sph.setSurround(new_sph.center);
	}
}