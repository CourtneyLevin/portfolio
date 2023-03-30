#pragma once
#include "../../Common/Vector2.h"
#include "CollisionVolume.h"
namespace NCL {
	using namespace Maths;
	namespace CSC3222 {
		class BoundingBox : public CollisionVolume {
		public:
			BoundingBox(Vector2* p, Vector2 xy, RigidBody* b);
			~BoundingBox();
			Vector2 getSize();
		protected:
			Vector2 boundVector;
		};

	}
}