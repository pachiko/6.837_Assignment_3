#ifndef SIMPLESYSTEM_H
#define SIMPLESYSTEM_H

#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include <vecmath.h>
#include <vector>

#include "particleSystem.h"

using namespace std;

class SimpleSystem: public ParticleSystem
{
public:
	using ParticleSystem::ParticleSystem; // inherit constructors

	void initState();    // overrides ParticleSystem::initState

	vector<Vector3f> evalF(const vector<Vector3f>& state) const;
	
	void draw() const;
	
};

#endif
