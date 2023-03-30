#pragma once
#include <vector>

namespace NCL {
	namespace Maths {
		class Vector2;
	}
	namespace CSC3222 {
		class GameMap;
		class SimObject;
		class GameSimsRenderer;
		class GameSimsPhysics;
		class TextureBase;
		class TextureManager;
		class PlayerCharacter;

		class BadFoodGame {
		public:
			BadFoodGame();
			~BadFoodGame();

			void Update(float dt);

			void AddNewObject(SimObject* object);

		protected:
			void InitialiseGame();

			TextureManager*		texManager;
			GameSimsRenderer*	renderer;
			GameSimsPhysics*	physics;
			GameMap* currentMap;

			PlayerCharacter* player;

			float gameTime;

			int currentScore;
			int foodCount;
			int coins;
			int balloons;
			int lives;

			std::vector<SimObject*> gameObjects;
			std::vector<SimObject*> newObjects;
		};
	}
}

