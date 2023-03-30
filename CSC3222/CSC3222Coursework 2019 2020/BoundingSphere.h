#pragma once
#include "CollisionVolume.h"

namespace NCL {
	namespace CSC3222 {
		class BoundingSphere : public CollisionVolume
		{
		public:

			BoundingSphere(Vector2* p, float r, RigidBody* b);
			~BoundingSphere();
			float getRadius();

		protected:
			float radius;
		};

	}
}