#ifndef WORLDMANAGER
#define WORLDMANAGER
#include <box2d/box2d.h> 

class WorldManager {
private:
	b2WorldDef worldDef;
	b2WorldId worldId;
	WorldManager();

public:
	WorldManager(const WorldManager&) = delete;
	void operator=(const WorldManager&) = delete;

	static WorldManager& GetInstance() {
		static WorldManager instance;
		return instance;
	}
	void step();
	b2WorldDef* GetWorld();
	b2WorldId GetWorldId();
};

#endif