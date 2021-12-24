#include "camera.h"
#include "ClothSystem.h"
#include "pickInfo.h"
#include "obstacle.h"


// Cloth Picker.
class ClothPicker {
public:
    ClothPicker(float str=1.f) : strength(str) {
        // picked = false; // ONLY GLOBALS DEFAULT TO FALSE
    }

    // getter
    bool hasPicked() const { return picked; }

    // Try to pick.
    bool tryPick(const Camera& cam, const ClothSystem& cloth, const Obstacle& obstacle, int x, int y);

    // Update drag force on cloth
    void update(const Camera& cam, ClothSystem& cloth, int x, int y);

    // Reset forces and info when done picking.
    void resetPicking(ClothSystem& cloth);

    void drawInfo() const;

private:
    // Picked anything?
    bool picked = false;

    // Pick information
    PickInfo info;

    // Dragging strength. Multiplied with drag displacement vector
    float strength;

};