#include "simpleSystem.h"
using namespace std;


void SimpleSystem::initState()
{
	vector<Vector3f> initialState;
	initialState.push_back(Vector3f(1.f, 0.f, 0.f));
	setState(initialState);
}

// TODO: implement evalF
// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	// YOUR CODE HERE
	f.push_back(Vector3f(-state[0].y(), state[0].x(), 0.f));

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{
	Vector3f pos = getState()[0]; // YOUR PARTICLE POSITION
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glutSolidSphere(0.075f, 10.0f, 10.0f);
	glPopMatrix();
}
