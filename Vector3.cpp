#include "Vector3.h"

Vector3::Vector3() {
	xyz[0] = 0; xyz[1] = 0; xyz[2] = 0;
}
Vector3::Vector3(const Vector3& vec3) {
	xyz[0] = vec3.xyz[0];
	xyz[1] = vec3.xyz[1];
	xyz[2] = vec3.xyz[2];
}
void Vector3::setXYZ(float x, float y, float z) {
	xyz[0] = x; xyz[1] = y; xyz[2] = z;
}
const float* Vector3::getXYZ() const {
	return xyz;
}

Vector3& Vector3::operator=(const Vector3& vec3) {
	for (int i = 0; i < 3; ++i)
		xyz[i] = vec3.xyz[i];
	return (*this);
}

float& Vector3::operator[](const int i) {
	return xyz[i];
}
float Vector3::operator[](const int i) const {
	return xyz[i];
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 vec;
	for (int i = 0; i < 3; ++i)
		vec[i] = v1[i] + v2[i];

	return vec;
}
Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 vec;
	for (int i = 0; i < 3; ++i)
		vec[i] = v1[i] - v2[i];

	return vec;
}
Vector3 operator-(const Vector3& v) {
	Vector3 vec;
	for (int i = 0; i < 3; ++i)
		vec[i] = -v[i];

	return vec;
}
Vector3 operator*(const float s, const Vector3& vec3) {
	Vector3 vec;
	for (int i = 0; i < 3; ++i)
		vec[i] = s * vec3[i];

	return vec;
}
float dotProduct(const Vector3& v1, const Vector3& v2) {
	float dot = 0;
	for (int i = 0; i < 3; ++i)
		dot += v1[i] * v2[i];

	return dot;
}