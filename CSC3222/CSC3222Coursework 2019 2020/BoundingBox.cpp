#include "BoundingBox.h"

using namespace NCL;
using namespace CSC3222;

BoundingBox::BoundingBox(Vector2* p, Vector2 xy, RigidBody* b) : CollisionVolume(p, b) 
{
	boundVector = xy;
}

BoundingBox::~BoundingBox() {
}

Vector2 BoundingBox::getSize() {
	return boundVector;
}