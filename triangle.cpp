#include "triangle.h"

bool Triangle::intersect(const Ray& ray, PickInfo& info, float tmin) const {
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