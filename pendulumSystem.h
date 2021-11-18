#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include <vecmath.h>
#include <vector>
#include <set>

#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "particleSystem.h"

// static gravitational acceleration
static const Vector3f g = Vector3f(0.f, -9.8f, 0.f);

struct Spring {
	int p1;
	int p2;
	float restLength;
	float stiffness;
	bool isFlex;

	Spring(int p1, int p2, float restLength, float stiffness, bool isFlex=false) :
	p1(p1), p2(p2), restLength(restLength), stiffness(stiffness), isFlex(isFlex) {
	}
};

struct ParticleInfo {
	float mass;
	float dragConstant;
	Vector3f externalForce;

	// mass of a tennis ball around 60g
	ParticleInfo(float mass=0.1f, float dragConstant=0.1f, Vector3f force=Vector3f::ZERO) :
	mass(mass), dragConstant(dragConstant), externalForce(force) {
	}
};

class PendulumSystem: public ParticleSystem
{
public:
	using ParticleSystem::ParticleSystem; // inherit constructors; C++ 11

	vector<Vector3f> evalF(vector<Vector3f> state);

	void initState();    // overrides ParticleSystem::initState
	
	virtual void draw();

	// make sure the receiver is reference if you want to edit
	vector<ParticleInfo>& getParticleInfos(){ return particleInfos; }; 
	
protected:
	// NB: Whatever methods that need to be implemented in derived classes should be virtual
	// because main.cpp uses a pointer to a ParticleSystem (the base class) to hold the current system.

	// Initial position and velocity
	virtual Vector3f initPos();
	virtual Vector3f initVelocity();

	// Set fixed points of the system
	virtual void setFixedPoints();

	// add particle info per particle
	virtual void addParticleInfo(int idx);

	// add springs (could be structural, shear and flex etc.)
	virtual void addSprings(int idx);

	// add structural spring
	virtual void addStructuralSpring(int idx);

	// generic add Spring
	void genericAddSpring(int p1, int p2, float restLength, float stiffness, bool isFlex=false);

	// update initial pos and velocity of each particle during initState
	virtual void updatePos(int idx, Vector3f& pos);
	virtual void updateVelocity(int idx, Vector3f& velocity);

	// mass and drag constant for each particle. Not used if particle idx in fixedPoint
	vector<ParticleInfo> particleInfos;

	// spring indices for each particle. empty if no spring for a given particle
	vector<vector<int>> connectedSprings;

	// vector of springs
	vector<Spring> springCollection;

	// particles whose net forces are 0
	set<int> fixedPoints; 
};

#endif
