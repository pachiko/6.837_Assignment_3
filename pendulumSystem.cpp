#include "pendulumSystem.h"
#include <iostream>


Vector3f PendulumSystem::initPos() {
	return Vector3f::RIGHT + m_numParticles*Vector3f::UP;
}

Vector3f PendulumSystem::initVelocity() {
	return Vector3f();
}

void PendulumSystem::setFixedPoints() {
	fixedPoints.clear();
	fixedPoints.insert(0);
}

void PendulumSystem::addParticleInfo(int idx) {
	particleInfos.push_back(ParticleInfo{0.2f});
}

void PendulumSystem::addSprings(int idx) {
	addStructuralSpring(idx);
}

void PendulumSystem::addStructuralSpring(int idx) {
	if (idx > 0) {
		genericAddSpring(idx - 1, idx, 1.f, 7.f);
	}
}

void PendulumSystem::genericAddSpring(int p1, int p2, float restLength, float stiffness, bool isFlex) {
	Spring sp{p1, p2, restLength, stiffness, isFlex};
	int springIdx = springCollection.size();
	connectedSprings[p1].push_back(springIdx);
	connectedSprings[p2].push_back(springIdx);
	springCollection.push_back(sp);
}

void PendulumSystem::updatePos(int idx, Vector3f& pos) {
	pos += -Vector3f::RIGHT;
}

void PendulumSystem::updateVelocity(int idx, Vector3f& velocity) {
	if (idx == 0) {
		velocity = 2.f*Vector3f::RIGHT - *Vector3f::UP;
		return;
	}
	velocity *= 1.2f;
}

void PendulumSystem::initState()
{
	springCollection.clear();
	connectedSprings = vector<vector<int>>(m_numParticles);

	// fill in code for initializing the state based on the number of particles
	vector<Vector3f> initialState;

	Vector3f pos = initPos();
	Vector3f vel = initVelocity();

	setFixedPoints();

	for (int i = 0; i < m_numParticles; i++) {
		// for this system, we care about the position and the velocity
		initialState.push_back(pos);
		initialState.push_back(vel);
		addParticleInfo(i);
		addSprings(i);
		updatePos(i, pos);
		updateVelocity(i, vel);
	}

	setState(initialState);
}


// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(const vector<Vector3f>& state)
{
	vector<Vector3f> f;

	// YOUR CODE HERE
	for (int i = 0; i < m_numParticles; i++) {
		if (fixedPoints.find(i) == fixedPoints.end()) {
			const ParticleInfo& pInfo = particleInfos[i];
			Vector3f gravity = pInfo.mass*g; // m[i]*g
			Vector3f drag = -pInfo.dragConstant*state[2*i + 1]; // -k[i]*v[i]

			Vector3f springForces;
			const vector<int>& springs = connectedSprings[i];
			for (auto j : springs) {
				const Spring& sp = springCollection[j];
				bool isP1 = (sp.p1 == i);

				Vector3f d = isP1? state[2*(sp.p1)] - state[2*(sp.p2)] :
					state[2*(sp.p2)] - state[2*(sp.p1)];

				float norm = d.abs();

				springForces += -(sp.stiffness)*(norm - sp.restLength)*d.normalized();
			}

			Vector3f accel = (springForces + gravity + drag + pInfo.externalForce)/pInfo.mass;
			f.push_back(state[2*i + 1]);
			f.push_back(accel);
		} else {
			// Fixed point
			f.push_back(Vector3f()); // no velocity
			f.push_back(Vector3f()); // no accel
		}	
	}

	return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
    GLfloat testParticleColor[] = {0.f, 1.f, 0.f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, testParticleColor);

	const vector<Vector3f> states = getState();
	for (int i = 0; i < m_numParticles; i++) {
		const Vector3f& pos = states[2*i]; // position of particle i. YOUR CODE HERE
		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glutSolidSphere(0.075f, 10.0f, 10.0f);
		glPopMatrix();
	}
}
