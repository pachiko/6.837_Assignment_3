#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include <vecmath.h>
#include <vector>
#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

#include "pendulumSystem.h"

class ClothSystem: public PendulumSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int i, int j);

	void draw() const;

	bool sweepMovement = false;

	bool wireMesh = false;

	bool visNormals = false;

	// Move fixed points
	virtual void sweepFixedPoints(float h);

	const vector<Vector3f>& getTriIndices() const { return indices; }

protected:
	int w, h;

	// Indices of particles forming each triangle (CCW)
	// Mainly for picking
	vector<Vector3f> indices;

	// CCW rotation?
	bool ccw = false;
	float angle = 0.f;

	Vector3f initPos() const;

	// update initial pos and velocity of each particle during initState
	void updatePos(int idx, Vector3f& pos) const;
	void updateVelocity(int idx, Vector3f& velocity) const;

	void setFixedPoints();

	void addParticleInfo(int idx);

	void addSprings(int idx);

	// add structural spring
	void addStructuralSpring(int idx);

	void addFlexSpring(int idx);
	void addShearSpring(int idx);

	// draw lines for springs
	void drawLines() const;

	// draw triangle mesh
	void drawTriangles() const;

	// Calculate normal
	Vector3f calcNormal(int r, int c, const vector<Vector3f>& st, vector<Vector3f>& normals) const;
};


#endif
