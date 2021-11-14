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

	void draw();

	bool sweepMovement;

	bool wireMesh;

	bool visNormals;

	// Move fixed points
	virtual void sweepFixedPoints(float h);

	const vector<Vector3f> &getTriIndices() { return indices; }

protected:
	int w, h;

	// Indices of particles forming each triangle (CCW)
	// Mainly for picking
	vector<Vector3f> indices;

	// CCW rotation?
	bool ccw;
	float angle;

	Vector3f initPos();

	// update initial pos and velocity of each particle during initState
	void updatePos(int idx, Vector3f& pos);
	void updateVelocity(int idx, Vector3f& velocity);

	void setFixedPoints();

	void addParticleInfo(int idx);

	void addSprings(int idx);

	// add structural spring
	void addStructuralSpring(int idx);

	void addFlexSpring(int idx);
	void addShearSpring(int idx);

	// draw lines for springs
	void drawLines();

	// draw triangle mesh
	void drawTriangles();

	// Calculate normal
	Vector3f calcNormal(int r, int c, const vector<Vector3f>& st, vector<Vector3f>& normals);
};


#endif
