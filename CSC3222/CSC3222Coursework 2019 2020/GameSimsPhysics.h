#pragma once
#include <vector>
#include "../../Common/Vector2.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include <algorithm> 


namespace NCL {
	namespace CSC3222 {
		class RigidBody;
		class CollisionVolume;

		class GameSimsPhysics	{
		public:
			GameSimsPhysics();
			~GameSimsPhysics();

			void Update(float dt);

			void AddRigidBody(RigidBody* b);
			void RemoveRigidBody(RigidBody* b);

			void AddCollider(CollisionVolume* c);
			void RemoveCollider(CollisionVolume* c);

		protected:
			void Integration(float dt);
			void CollisionDetection(float dt);
			void CollisionResolutionCheck(CollisionVolume* a, CollisionVolume* b);
			float Clamp(float value, float maxValue, float minValue);
			bool CheckCollision(CollisionVolume* a, CollisionVolume* b);
			bool Collision(BoundingBox* a, BoundingBox* b);
			bool Collision(BoundingBox* b, BoundingSphere* s);
			bool Collision(BoundingSphere* a, BoundingSphere* b);
			void CollisionResolution(BoundingBox* a, BoundingBox* b);
			void CollisionResolution(BoundingBox* b, BoundingSphere* s);
			void CollisionResolution(BoundingSphere* a, BoundingSphere* b);

			std::vector<RigidBody*>			allBodies;
			std::vector<CollisionVolume*>	allColliders;
		};
	}
}

