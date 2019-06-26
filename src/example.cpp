#include <iostream>

#include "entity.h" 
#include "component.h"
#include "system.h"

using namespace mnes; 

struct Position : Component {
    float x, y, z = 0.0f;
    Position(float x, float y, float z) : x(x), y(y), z(z) { }
};

struct Velocity : Component {
    float v1, v2, v3 = 0.0f;
    Velocity(float v1, float v2, float v3) : v1(v1), v2(v2), v3(v3) { }
};

class MovementSystem : public System {
    void Init() {
        Require<Position>();
        Require<Velocity>();
    }

    void Update() {
        for (auto e : GetEntities()) {
            Position* pos = e.GetComponent<Position>();
            Velocity* vel = e.GetComponent<Velocity>();

            pos->x += vel->v1;
            pos->y += vel->v2;
            pos->z += vel->v3;

        }
    }
};

int main(int argc, char** argv)  {
    EntityManager entity_manager;
    SystemManager system_manager(&entity_manager);

    system_manager.Register<MovementSystem>();
    system_manager.Init();

    Entity e = entity_manager.Create();
    e.AddComponent<Position>(10.0f, 12.0f, 4.0f)
     .AddComponent<Velocity>(1.0f, 0.0f, 0.5f)
     .Bind();
    
    for (int i = 0; i < 10; i++) {
        system_manager.Update();
        Position pos = *e.GetComponent<Position>();
        std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
    }
    return 0;
}