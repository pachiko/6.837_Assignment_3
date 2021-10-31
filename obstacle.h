#include <vecmath.h>
#include "particleSystem.h"

#ifdef _WIN32
#include "GL/freeglut.h"
#else
#include <GL/glut.h>
#endif

class Obstacle {
    public:
	    virtual bool collides(ParticleSystem* particleSystem)=0;
        virtual void draw() = 0;
};

class Sphere:public Obstacle
{
    public:
        Sphere(Vector3f pos, float r);

    bool collides(ParticleSystem* particleSystem);
    void draw();

    protected:
	    Vector3f pos;
        float r;
        
    private:
        float r_sqr;
};