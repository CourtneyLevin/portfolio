#include "BoundingSphere.h"

using namespace NCL;
using namespace CSC3222;

BoundingSphere::BoundingSphere(Vector2* p, float r, RigidBody* b) : CollisionVolume(p, b)  {
	radius = r;
}

BoundingSphere::~BoundingSphere(){
}

float BoundingSphere::getRadius() {
	return radius;
}