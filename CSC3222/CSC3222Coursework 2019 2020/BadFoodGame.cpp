#include "BadFoodGame.h"
#include "SimObject.h"
#include "GameMap.h"
#include "TextureManager.h"
#include "GameSimsRenderer.h"
#include "GameSimsPhysics.h"
#include "PlayerCharacter.h"
#include "BadRobot.h"
#include "Laser.h"
#include "Food.h"
#include "Coin.h"
#include "Balloon.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

#include "../../Common/Window.h"
#include "../../Common/TextureLoader.h"

using namespace NCL;
using namespace CSC3222;

BadFoodGame::BadFoodGame()	{
	renderer	= new GameSimsRenderer();
	texManager	= new TextureManager();
	physics		= new GameSimsPhysics();
	SimObject::InitObjects(this, texManager);
	InitialiseGame();
}

BadFoodGame::~BadFoodGame()	{
	delete currentMap;
	delete texManager;
	delete renderer;
	delete physics;
}

void BadFoodGame::Update(float dt) {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
		Laser* laser;
		if (player->GetVelocity().x > 0) {
			laser = new Laser(Vector2(1, 0));
			laser->SetPosition(player->GetPosition()+Vector2(20,0));
		}
		else {
			laser = new Laser(Vector2(-1, 0));
			laser->SetPosition(player->GetPosition() - Vector2(20, 0));
		}
		
		laser->SetCollider(new BoundingSphere(laser->GetPositionPointer(), 5, laser));
		AddNewObject(laser);
	}



	for (auto i : newObjects) {
		gameObjects.emplace_back(i);
	}
	newObjects.clear();

	gameTime += dt;

	renderer->Update(dt);
	physics->Update(dt);
	currentMap->DrawMap(*renderer);

	srand((int)(gameTime * 1000.0f));

	for (auto i = gameObjects.begin(); i != gameObjects.end(); ) {
		if (!(*i)->UpdateObject(dt)) { //object has said its finished with
			delete (*i);
			i = gameObjects.erase(i);
		}
		else {
			(*i)->DrawObject(*renderer);
			++i;
		}
	}	
	renderer->DrawString("B4DF00D", Vector2(420, 700));

	renderer->DrawString("Score: " + std::to_string(currentScore), Vector2(10, 10));
	renderer->DrawString("Lives left: " + std::to_string(lives), Vector2(10, 30));
	renderer->DrawString("Food: " + std::to_string(foodCount), Vector2(10, 50));
	renderer->DrawString("Coins: " + std::to_string(coins), Vector2(10, 70));
	renderer->DrawString("Balloons: " + std::to_string(balloons), Vector2(10, 90));





	
	renderer->DrawBox(Vector2(16,16), Vector2(8, 8), Vector4(1, 0, 0, 1));
	renderer->DrawLine(Vector2(16, 16), Vector2(192, 192), Vector4(1, 1, 0, 1));
	renderer->DrawCircle(Vector2(100, 100), 10.0f, Vector4(1, 0, 1, 1));
	
	renderer->DrawBox(gameObjects.at(0)->GetCollider()->getPosition(), Vector2(12,15));
	renderer->DrawBox(gameObjects.at(1)->GetCollider()->getPosition(), Vector2(10,20));




	renderer->Render();
}

void BadFoodGame::InitialiseGame() {
	delete currentMap;
	for (auto o : gameObjects) {
		delete o;
	}
	gameObjects.clear();

	currentMap = new GameMap("BadFoodMap.txt", gameObjects, *texManager, physics);

	renderer->SetScreenProperties(16, currentMap->GetMapWidth(), currentMap->GetMapHeight());

	player = new PlayerCharacter();
	player->SetCharacterType(PlayerCharacter::CharacterType::TYPE_A);
	player->SetPosition(Vector2(100, 200));
	player->SetCollider(new BoundingBox(player->GetPositionPointer(), Vector2(24, 30), player));
	AddNewObject(player);

	BadRobot* testRobot = new BadRobot();
	testRobot->SetPosition(Vector2(150, 300));
	testRobot->SetCollider(new BoundingBox(testRobot->GetPositionPointer(), Vector2(20,20), testRobot));
	AddNewObject(testRobot);

	Food* testFood = new Food();
	testFood->SetPosition(Vector2(250, 200));
	AddNewObject(testFood);

	Coin* testCoin = new Coin();
	testCoin->SetPosition(Vector2(300, 200));
	testCoin->SetCollider(new BoundingSphere(testCoin->GetPositionPointer(), 5, testCoin));
	AddNewObject(testCoin);

	Balloon* testBalloon = new Balloon();
	testBalloon->SetPosition(Vector2(350, 200));
	AddNewObject(testBalloon);


	gameTime		= 0;
	currentScore	= 0;
	foodCount		= 0;
	coins			= 0;
	balloons		= 1;
	lives			= 3;
}

void BadFoodGame::AddNewObject(SimObject* object) {
	newObjects.emplace_back(object);
	physics->AddRigidBody(object);
	if (object->GetCollider()) {
		physics->AddCollider(object->GetCollider());
	}
}