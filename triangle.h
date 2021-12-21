#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vecmath.h"
#include "ray.h"
#include "pickInfo.h"
#include <cmath>
#include <iostream>

using namespace std;

class Triangle {
public:
	Triangle();
        ///@param a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, int idx) :
	 a(a), e1(b - a), e2(c - a), idx(idx) {
	}

	bool intersect(const Ray& ray, PickInfo& info, float tmin) const;

protected:
	Vector3f a, e1, e2;
    int idx;
};

#endif //TRIANGLE_H