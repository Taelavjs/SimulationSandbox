// WorldManager.cpp

#include "WorldManager.hpp"
#include <box2d/box2d.h> // Full definition of b2World, b2BodyDef, b2Vec2, etc.
#include "cppFallingSands-master/src/Utility/GlobalVariables.hpp"

WorldManager::WorldManager() : worldDef(b2DefaultWorldDef())
{
	worldDef.gravity = { 0.0f, 10.0f };
	worldId = b2CreateWorld(&worldDef);


}

void WorldManager::step()
{
	b2World_Step(worldId, GlobalVariables::getDeltaTime(), 4);
	b2Counters totalCount = b2World_GetCounters(worldId);
}

b2WorldDef* WorldManager::GetWorld() {
	return &worldDef;
}
b2WorldId WorldManager::GetWorldId() {
	return worldId;
}
//b2BodyDef groundBodyDef = b2DefaultBodyDef();
//groundBodyDef.position = { 0.0f, -10.0f };
//b2BodyId groundId = b2CreateBody(worldId, &groundBodyDef);
//b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);
//b2ShapeDef groundShapeDef = b2DefaultShapeDef();
//b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);