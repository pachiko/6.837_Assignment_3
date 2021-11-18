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
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, int idx) {
		  this->a = a;
		  this->e1 = b - a;
		  this->e2 = c - a;
          this->idx = idx;
	}

	bool intersect(const Ray& ray, PickInfo& info, float tmin) {
		//  Moller-Trumbore

		Vector3f s = ray.getOrigin() - a;
		Vector3f d = ray.getDirection();
		Vector3f e1d = Vector3f::cross(e1, d);
		
		float det = Vector3f::dot(e1d, e2);
		if (fabs(det) > 1e-3) {
			float invDet = 1.f / det;
			Vector3f se2 = Vector3f::cross(s, e2);
			float u = -Vector3f::dot(se2, d) * invDet;
			float v = Vector3f::dot(e1d, s) * invDet;
			float t = -Vector3f::dot(se2, e1) * invDet;

			if (t > tmin && t < info.t && u >= 0.f && v >= 0.f && (u + v) <= 1.f) {
                // Update PickInfo
                info.t = t;
                info.barycentric = Vector3f(u, v, 1.f - u - v);
                info.pos = ray.pointAtParameter(t);
                info.idx = idx;
				return true;
			}
		}

		return false;
	}

protected:
	Vector3f a, e1, e2;
    int idx;
};

#endif //TRIANGLE_H