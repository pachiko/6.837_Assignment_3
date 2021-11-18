#include "camera.h"
#include "ClothSystem.h"
#include "pickInfo.h"
#include "obstacle.h"


// Cloth Picker.
class ClothPicker {
public:
    ClothPicker(float str=1.f) : strength(str) {
        picked = false; // ONLY GLOBALS DEFAULT TO FALSE
     }

    // getter
    bool hasPicked() { return picked; }

    // Try to pick. True if successful.
    bool tryPick(Camera& cam, ClothSystem& cloth, Obstacle& obstacle, int x, int y);

    // Update drag force. Done before stepping?
    void update(Camera& cam, ClothSystem& cloth, int x, int y);

    // Reset when done picking.
    void resetPicking(ClothSystem& cloth);

    void drawInfo();

private:
    // Picked anything?
    bool picked;

    // Pick information
    PickInfo info;

    // Dragging strength. Multiplied with drag displacement vector
    float strength;

};