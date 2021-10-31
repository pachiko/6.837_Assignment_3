#include "obstacle.h"

// Prevent z-fighting
const float eps = 0.2f;

Sphere::Sphere(Vector3f pos, float r) : pos(pos), r(r) {
	r_sqr = (r + eps)*(r + eps);
}

void Sphere::draw() {
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glutSolidSphere(r, r*10.0f, r*10.0f);
	glPopMatrix();
}

bool Sphere::collides(ParticleSystem* particleSystem) {
	bool res = false;
	vector<Vector3f> states = particleSystem->getState();

	for (int i = 0; i < particleSystem->m_numParticles; i++) {
		int idx = 2*i;
		Vector3f particlePos = states[idx];
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