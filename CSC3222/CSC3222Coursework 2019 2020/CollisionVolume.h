#pragma once
#include "../../Common/Vector2.h"
#include "RigidBody.h"

namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		class CollisionVolume {
		public:
			CollisionVolume(Vector2* p, RigidBody* b);
			virtual ~CollisionVolume()=0;
			Vector2 getPosition();
			RigidBody* getBody();
		protected:
			Vector2* position;
			RigidBody* body;
		};
	}
}

