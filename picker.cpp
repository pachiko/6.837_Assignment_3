#include "picker.h"
#include "ray.h"
#include "triangle.h"


bool ClothPicker::tryPick(Camera& cam, ClothSystem& cloth, int x, int y) {
    // x is positive to the right, y is positive downwards

    // Vector3f o = cam.GetCenter(); // DONT USE, doesnt get moved by rots and zooms
    Vector3f o = ((cam.viewMatrix().inverse())*Vector4f(Vector3f(), 1.f)).xyz();

    const int* viewPort = cam.GetViewPort();

    // compute "distance" of image plane (wrt projection matrix)
    // IMAGE PLANE IS NOT NEAR PLANE! so d != nearZ
    // const float* pers = cam.GetPerspective();
    // float d = -float(viewPort[3])/2.0f / tan(pers[0]*M_PI / 180.0f / 2.0f);
    
    // viewPort is starting from (0, 0) in our case
    // int cx = x - viewPort[0];
    // int cy = y - viewPort[1];
    // float cr = cx - viewPort[2]/2.0f + 0.5f; // (x - w/2)
    // float cu = -(cy - viewPort[3]/2.0f) + 0.5f; // -(y - h/2)

    // Vector3f dir(cr, cu, d);
	// dir = ((cam.viewMatrix().inverse())*Vector4f(dir, 0.f)).xyz().normalized();
    // dir = (dir - o).normalized();

    float fx = (2.0f * x) / viewPort[2] - 1.0f;
    float fy = 1.0f - (2.0f * y) / viewPort[3];
    Vector2f ray_nds(fx, fy);
    Vector4f ray_clip(ray_nds, -1.0f, 1.0f);
    Vector4f ray_eye = cam.projectionMatrix().inverse() * ray_clip;
    ray_eye = Vector4f(ray_eye.xy(), -1.0, 0.0);
    Vector3f dir = ((cam.viewMatrix().inverse()) * ray_eye).xyz();
    dir.normalize();
    Ray r(o, dir);

    info.o = o;
    info.dir = dir;

    // Return value and index counter
    bool res = false;
    int i = 0;

    // Contains particle pos.
    vector<Vector3f> st = cloth.getState();

    for (auto idx : cloth.getTriIndices()) {
        // Particle pos
        Vector3f a = st[2*idx.x()];
        Vector3f b = st[2*idx.y()];
        Vector3f c = st[2*idx.z()];

        // Triangle with index
        Triangle tri(a, b, c, i); // i == 84, botLeft corner

        if (i == 84) {
            int j;
        }
        if (tri.intersect(r, info, 0.f)) { // should use near plane
            if (!res) res = true;
            // DON'T update particle force, this may NOT be the nearest triangle!
        }
        i++;
    }

    std::cout << x << ", " << y << ", " << res << std::endl;
    std::cout << r << std::endl;
    return picked = res;
}

void ClothPicker::drawInfo() { // Rotate to see it! But only rotatable if not picked...
    if (info.dir != Vector3f::ZERO && info.o != Vector3f::ZERO) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glDisable(GL_LIGHTING); 
        glColor4f(1, 1, 0, 1);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex3fv(info.o);
        glVertex3fv(info.o + 100.f*info.dir);
        glEnd();
        glPopAttrib();
    }
}

void ClothPicker::update(Camera& cam, ClothSystem& cloth, int x, int y) {

}

    
void ClothPicker::resetPicking(ClothSystem& cloth) {
    picked = false;
    Vector3f idx = cloth.getTriIndices()[info.idx];
    vector<ParticleInfo> pInfos = cloth.getParticleInfos();
    for (int i = 0; i < 3; i++) {
        pInfos[int(idx[i])].externalForce = Vector3f::ZERO;
    }
    info = PickInfo();
}