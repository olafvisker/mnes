# mnes
A minimal c++11 entity component system focused on efficiency and readable code.

# example
**includes**
```c++
#include  "entity.h"
#include  "component.h"
#include  "system.h"

using  namespace  mnes;
```

**components**
```c++
struct Position : Component {
	float x, y, z = 0.0f;
	Position(float x, float y, float z) : x(x), y(y), z(z) { }
};

struct Velocity : Component {
	float v1, v2, v3 = 0.0f;
	Velocity(float v1, float v2, float v3) : v1(v1), v2(v2), v3(v3) { }
};
```

**system**
```c++
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
```

**main**

```c++
EntityManager entity_manager;
SystemManager system_manager(&entity_manager);

system_manager.Register<MovementSystem>();  	//register a new system to the system manager
system_manager.Init();                      	//initialize all systems

Entity e = entity_manager.Create();         	//create a new entity
e.AddComponent<Position>(10.0f, 12.0f, 4.0f)	//add a position components
 .AddComponent<Velocity>(1.0f, 0.0f, 0.5f)		//add a velocity component using chaining
 .Bind();										//bind entity to relevant systems next update

for (int i = 0; i < 10; i++) {
	system_manager.Update();					//update all systems
	Position pos = *e.GetComponent<Position>();
	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
}
```

# system sets
Sometimes systems require access to multiple sets of components. In order to solve this problem **mnes** makes use of so called system sets. This allows a single system to work with multiple combinations (entities) of components. You can find an example below.
```c++
class RenderSystem : public System {
	enum Sets {                                       //define your entity sets
		CAMERAS,
		OBJECTS,
	};

	void Init() {
		Require<Viewport>(Sets::CAMERAS);             //define camera entity
		Require<Camera>(Sets::CAMERAS);

		Require<Transform>(Sets::OBJECTS);            //define object entity
		Require<MeshRenderer>(Sets::OBJECTS);
	}

	void Update() {
		for (auto c : GetEntities(Sets::CAMERAS)) {     //get all cameras      
			...  		        
			for (auto e : GetEntities(Sets::OBJECTS)) { //get all objects
                ...
			}
		}
	}
};
```
**enjoy!**
