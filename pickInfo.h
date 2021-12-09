#ifndef PICKINFO_H_
#define PICKINFO_H_

// Include guards (above) are crucial to prevent 
// redefinition of struct PickInfo

#include "vecmath.h"
#include "ray.h"

struct PickInfo {
    PickInfo() = default;

    // When using member initializer syntax, order of fields matters!
    PickInfo(float t, Vector3f pos, int idx) : t(t), pos(pos), idx(idx) { }

    // Ray-triangle intersect time
    float t = __FLT_MAX__;

    // Initial position of picked-up triangle
    // Used to calculate drag displacement vector
    Vector3f pos;

    // Triangle index
    int idx = 0;

    // Barycentric coordinates of intersection (x,y,z) = (u,v,w)
    // Used to scale the drag force acting on each vertex of the triangle.
    Vector3f barycentric;

    // Ray for debug.
    Vector3f o;
    
    Vector3f dir;
};

#endif