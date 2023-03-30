#include "RigidBody.h"

using namespace NCL;
using namespace CSC3222;

RigidBody::RigidBody()	{
	inverseMass = 1.0f + (((double)rand() / (RAND_MAX)) * 2 - 1);
	elasticity  = 0.5f;
}


RigidBody::~RigidBody()	{

}
