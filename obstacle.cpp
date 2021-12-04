#include "obstacle.h"

// Prevent z-fighting
const float eps = 0.2f;

Sphere::Sphere(Vector3f pos, float r) : pos(pos), r(r) {
	r_sqr = (r + eps)*(r + eps);
}


bool Sphere::intersect(const Ray& ray, PickInfo& info, float tmin) {
	bool res = false;

	Vector3f o_c = ray.getOrigin() - pos; // o - c
	float dTo_c = Vector3f::dot(ray.getDirection(), o_c); // d.(o - c)
	float det = dTo_c*dTo_c - (Vector3f::dot(o_c, o_c) - r*r); // b^2 - 4ac

	if (det < 0.f) return res; // cant sqrt negative
	det = sqrt(det);

	float tNear = (-dTo_c - det); // dTd = 1
	float tFar = (-dTo_c + det); // dTd = 1
	if (tNear < info.t && tNear > tmin) {
		res = true;
		info.t = tNear;
		info.pos = ray.pointAtParameter(tNear);
	}

	return res;
}


void Sphere::draw() {
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glutSolidSphere(r, r*10.0f, r*10.0f);
	glPopMatrix();
}


bool Sphere::collides(ParticleSystem* particleSystem) {
	bool res = false;
	vector<Vector3f>& states = particleSystem->getState();

	for (int i = 0; i < particleSystem->m_numParticles; i++) {
		int idx = 2*i;
		const Vector3f& particlePos = states[idx];
		Vector3f diff = particlePos - pos;
		if (Vector3f::dot(diff, diff) < r_sqr) {
			if (!res) res = true;
			diff.normalize();
			diff *= (r + eps);
			states[idx] = pos + diff;
			states[idx + 1] = Vector3f::ZERO; // Stop!
		}
	}
	
	if (res) particleSystem->setState(states);
    return res;
}