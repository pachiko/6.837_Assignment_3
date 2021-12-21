#include "picker.h"
#include "ray.h"
#include "triangle.h"


bool ClothPicker::tryPick(Camera& cam, ClothSystem& cloth, Obstacle& obstacle, int x, int y) {
    // x is positive to the right, y is positive downwards
    Matrix4f invView = cam.viewMatrix().inverse();

    // Vector3f o = cam.GetCenter(); // DONT USE, doesnt get moved by rots and zooms
    Vector3f o = (invView * Vector4f(Vector3f(), 1.f)).xyz();
    Vector3f dir = cam.pixelDirection(x, y);
	dir = (invView * Vector4f(dir, 0.f)).xyz().normalized();

    Ray r(o, dir);
    info.o = o;
    info.dir = dir;

    // Intersect with sphere first.
    // All subsequent triangles need to be in front to be valid.
    try {
        // Cannot check if null-reference; throws an error if cant dynamic cast
        Sphere& sphere{dynamic_cast<Sphere&>(obstacle)};
        sphere.intersect(r, info, 0.f);
    } catch(...) {}

    // Return value and index counter
    bool res = false;
    int i = 0;

    // Contains particle pos.
    const vector<Vector3f>& st = cloth.getState();

    for (auto& idx : cloth.getTriIndices()) {
        // Particle pos
        const Vector3f& a = st[2*idx.x()];
        const Vector3f& b = st[2*idx.y()];
        const Vector3f& c = st[2*idx.z()];

        // Triangle with index
        Triangle tri(a, b, c, i);

        if (tri.intersect(r, info, 0.f)) {
            if (!res) res = true;
            // DON'T update particle force, this may NOT be the nearest triangle!
        }
        i++;
    }

    // std::cout << x << ", " << y << ", " << res << std::endl;
    // std::cout << r << std::endl;
    return picked = res;
}

void ClothPicker::drawInfo() const { // Rotate to see it! But only rotatable if not picked...
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
    Matrix4f view = cam.viewMatrix();
    Vector3f pickPos = (view * Vector4f(info.pos, 1.f)).xyz();

    Vector3f newDir = cam.pixelDirection(x, y).normalized();
    
    float t = (pickPos.z())/(newDir.z());
    Vector3f newPos = t*newDir;

    Vector3f delta = (newPos - pickPos);
    delta = (view.inverse() * Vector4f(delta, 0.f)).xyz();
    delta *= strength;
    // delta *= exp(delta.abs()); // unstable (explode)

    Vector3f idx = cloth.getTriIndices()[info.idx];
    vector<ParticleInfo>& pInfos = cloth.getParticleInfos();
    for (int i = 0; i < 3; i++) {
        Vector3f force = info.barycentric[i]*delta;
        pInfos[int(idx[i])].externalForce = force;
    }
}
    
void ClothPicker::resetPicking(ClothSystem& cloth) {
    picked = false;
    Vector3f idx = cloth.getTriIndices()[info.idx];
    vector<ParticleInfo>& pInfos = cloth.getParticleInfos();
    for (int i = 0; i < 3; i++) {
        pInfos[int(idx[i])].externalForce = Vector3f::ZERO;
    }
    info = PickInfo();
}
