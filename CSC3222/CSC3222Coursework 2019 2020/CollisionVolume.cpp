#include "CollisionVolume.h"

using namespace NCL;
using namespace CSC3222;

CollisionVolume::CollisionVolume(Vector2* p, RigidBody* b)
{
	position = p;
	body = b;
}


CollisionVolume::~CollisionVolume()
{
}

Vector2 CollisionVolume::getPosition() {
	return *position;
}

RigidBody* CollisionVolume::getBody() {
	return body;
}