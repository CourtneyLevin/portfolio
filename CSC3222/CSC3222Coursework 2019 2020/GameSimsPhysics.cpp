#include "GameSimsPhysics.h"
#include "RigidBody.h"
#include "SimObject.h"
#include "CollisionVolume.h"
#include "../../Common/Vector2.h"
#include "Laser.h"
#include <algorithm>

using namespace NCL;
using namespace CSC3222;
using namespace std;

GameSimsPhysics::GameSimsPhysics()	{

}

GameSimsPhysics::~GameSimsPhysics()	{

}

void GameSimsPhysics::Update(float dt) {
	
	CollisionDetection(dt);
	Integration(dt);
	for (int i = 0; i < allBodies.size(); i++) {
		allBodies.at(i)->force = Vector2{ 0,0 };
	};
}

void GameSimsPhysics::AddRigidBody(RigidBody* b) {
	allBodies.emplace_back(b);
}

void GameSimsPhysics::RemoveRigidBody(RigidBody* b) {
	auto at = std::find(allBodies.begin(), allBodies.end(), b);

	if (at != allBodies.end()) {
		//maybe delete too?
		allBodies.erase(at);
	}
}

void GameSimsPhysics::AddCollider(CollisionVolume* c) {
	allColliders.emplace_back(c);
}

void GameSimsPhysics::RemoveCollider(CollisionVolume* c) {
	auto at = std::find(allColliders.begin(), allColliders.end(), c);

	if (at != allColliders.end()) {
		//maybe delete too?
		allColliders.erase(at);
	}
}

void GameSimsPhysics::Integration(float dt) {

	Vector2 acceleration;

	for (int i = 0; i < allBodies.size(); i++) {
		acceleration = allBodies.at(i)->force * allBodies.at(i)->inverseMass;
		allBodies.at(i)->SetVelocity(allBodies.at(i)->GetVelocity() + acceleration * dt);
	};

	//future courtney: are two for loops needed?

	for (int i = 0; i < allBodies.size(); i++) {
		allBodies.at(i)->SetPosition(allBodies.at(i)->GetPosition() + allBodies.at(i)->GetVelocity() * dt);
		allBodies.at(i)->SetVelocity(allBodies.at(i)->GetVelocity() * 0.97f);
	};
};

void GameSimsPhysics::CollisionDetection(float dt) {
	for (int i = 0; i < allColliders.size(); ++i) {
		for (int j = i + 1; j < allColliders.size(); ++j) {
			if (CheckCollision(allColliders.at(i), allColliders.at(j))) {
				CollisionResolutionCheck(allColliders.at(i), allColliders.at(j));
			}
		}
	}
}

void GameSimsPhysics::CollisionResolutionCheck(CollisionVolume* a, CollisionVolume* b) {

	if (a->getBody()->GetVelocity() == Vector2(0, 0) && b->getBody()->GetVelocity() == Vector2(0, 0)) return;

	if (dynamic_cast<BoundingBox*>(a) != nullptr && dynamic_cast<BoundingBox*>(b) != nullptr) {
		CollisionResolution(dynamic_cast<BoundingBox*>(a), dynamic_cast<BoundingBox*>(b));
	}
	else if (dynamic_cast<BoundingSphere*>(a) != nullptr && dynamic_cast<BoundingSphere*>(b) != nullptr) {
		CollisionResolution(dynamic_cast<BoundingSphere*>(a), dynamic_cast<BoundingSphere*>(b));
	}
	else if (dynamic_cast<BoundingBox*>(a) != nullptr && dynamic_cast<BoundingSphere*>(b) != nullptr) {
		CollisionResolution(dynamic_cast<BoundingBox*>(a), dynamic_cast<BoundingSphere*>(b));
	}
	else {
		CollisionResolution(dynamic_cast<BoundingBox*>(b), dynamic_cast<BoundingSphere*>(a));
	}
}


bool GameSimsPhysics::CheckCollision(CollisionVolume* a, CollisionVolume* b){

	if (dynamic_cast<BoundingBox*>(a) != nullptr && dynamic_cast<BoundingBox*>(b) != nullptr) {
		return Collision(dynamic_cast<BoundingBox*>(a), dynamic_cast<BoundingBox*>(b));
	}
	else if (dynamic_cast<BoundingSphere*>(a) != nullptr && dynamic_cast<BoundingSphere*>(b) != nullptr) {
		return Collision(dynamic_cast<BoundingSphere*>(a), dynamic_cast<BoundingSphere*>(b));
	}
	else if (dynamic_cast<BoundingBox*>(a) != nullptr && dynamic_cast<BoundingSphere*>(b) != nullptr) {
		return Collision(dynamic_cast<BoundingBox*>(a), dynamic_cast<BoundingSphere*>(b));
	}
	else {
		return Collision(dynamic_cast<BoundingBox*>(b), dynamic_cast<BoundingSphere*>(a));
	}

}

bool GameSimsPhysics::Collision(BoundingBox* a, BoundingBox* b) {
	Vector2 bca = { a->getPosition().x - a->getSize().x / 2, a->getPosition().y - a->getSize().y / 2 };
	Vector2 tca = { a->getPosition().x + a->getSize().x / 2, a->getPosition().y + a->getSize().y / 2 };
	Vector2 bcb = { b->getPosition().x - b->getSize().x / 2, b->getPosition().y - b->getSize().y / 2 };
	Vector2 tcb = { b->getPosition().x + b->getSize().x / 2, b->getPosition().y + b->getSize().y / 2 };

	if (((bcb.x <= bca.x) && (bca.x <= tcb.x)) || ((bcb.x <= tca.x) && (tca.x <= tcb.x))) {
		if (((bcb.y <= bca.y) && (bca.y <= tcb.y)) || ((bcb.y <= tca.y) && (tca.y <= tcb.y))) {
			return true;
		}
	}

	if (((bca.x <= bcb.x) && (bcb.x <= tca.x)) || ((bca.x <= tcb.x) && (tcb.x <= tca.x))) {
		if (((bca.y <= bcb.y) && (bcb.y <= tca.y)) || ((bca.y <= tcb.y) && (tcb.y <= tca.y))) {
			return true;
		}
	}
	return false;
}

float GameSimsPhysics::Clamp(float value, float maxValue, float minValue) {
	value = std::min(value, maxValue);
	value = std::max(value, minValue);

	return value;
}

bool GameSimsPhysics::Collision(BoundingBox* b, BoundingSphere* s) {
	Vector2 clampVector = {(Clamp(s->getPosition().x, b->getPosition().x + b->getSize().x / 2,b->getPosition().x - b->getSize().x / 2)), 
		(Clamp(s->getPosition().y, b->getPosition().y + b->getSize().y / 2,b->getPosition().y - b->getSize().y / 2)) };

	clampVector = { clampVector.x - s->getPosition().x, clampVector.y - s->getPosition().y };

	if (((clampVector.x *clampVector.x) + (clampVector.y * clampVector.y)) < (s->getRadius() * s->getRadius())) return true;
	return false;
}

bool GameSimsPhysics::Collision(BoundingSphere* a, BoundingSphere* b) {
	double diffX = a->getPosition().x - b->getPosition().x;
	double diffY = a->getPosition().y - b->getPosition().y;
	if (((diffX * diffX) + (diffY * diffY)) < ((a->getRadius() * 2) * (b->getRadius() * 2))) return true;
	return false;
}

void GameSimsPhysics::CollisionResolution(BoundingSphere* a, BoundingSphere* b) {
	double diffX = a->getPosition().x - b->getPosition().x;
	double diffY = a->getPosition().y - b->getPosition().y;
	double centreDistance = sqrt((diffX * diffX) + (diffY * diffY));
	double penetration = ((a->getRadius() + b->getRadius()) - centreDistance);
	Vector2 normal = Vector2(diffX, diffY);
	normal.Normalise();

	Vector2 aPos = normal * penetration * (a->getBody()->getIMass() / (a->getBody()->getIMass() + b->getBody()->getIMass()));
	Vector2 bPos = normal * penetration * (b->getBody()->getIMass() / (a->getBody()->getIMass() + b->getBody()->getIMass()));

	a->getBody()->SetPosition(a->getPosition() + aPos);
	b->getBody()->SetPosition(b->getPosition() - bPos);

	float velAlongNorm = Vector2::Dot((a->getBody()->GetVelocity() - b->getBody()->GetVelocity()), normal);

	if (velAlongNorm == 0) {
		return;
	}
	
	float e = std::min(a->getBody()->getElast(), b->getBody()->getElast());

	float j = -(1.0f + e) * velAlongNorm;
	j /= a->getBody()->getIMass() + b->getBody()->getIMass();


	Vector2 impulse = normal * j;

	std::cout << b->getBody()->GetVelocity();
	a->getBody()->SetVelocity(a->getBody()->GetVelocity() + (impulse * a->getBody()->getIMass()));
	b->getBody()->SetVelocity(b->getBody()->GetVelocity() - (impulse * b->getBody()->getIMass()));
	std::cout << b->getBody()->GetVelocity();
}

void GameSimsPhysics::CollisionResolution(BoundingBox* b, BoundingSphere* s) {
	Vector2 clampVector = { (Clamp(s->getPosition().x, b->getPosition().x + b->getSize().x / 2,b->getPosition().x - b->getSize().x / 2)),
		(Clamp(s->getPosition().y, b->getPosition().y + b->getSize().y / 2,b->getPosition().y - b->getSize().y / 2)) };

	clampVector = { clampVector.x - s->getPosition().x, clampVector.y - s->getPosition().y };

	double distance = clampVector.Length();

	double penetration = s->getRadius() - distance;

	Vector2 normalVector = Vector2(clampVector - s->getPosition());
	normalVector.Normalise();



	Vector2 sPos = normalVector * penetration * (s->getBody()->getIMass() / (s->getBody()->getIMass() + b->getBody()->getIMass()));
	Vector2 bPos = normalVector * penetration * (b->getBody()->getIMass() / (s->getBody()->getIMass() + b->getBody()->getIMass()));

	s->getBody()->SetPosition(s->getPosition() - sPos);
	b->getBody()->SetPosition(b->getPosition() + bPos);

	float velAlongNorm = Vector2::Dot((b->getBody()->GetVelocity() - s->getBody()->GetVelocity()), normalVector);

	if (velAlongNorm == 0) {
		return;
	}

	float e = std::min(s->getBody()->getElast(), b->getBody()->getElast());

	float j = -(1.0f + e) * velAlongNorm;
	j /= s->getBody()->getIMass() + b->getBody()->getIMass();


	Vector2 impulse = normalVector * j;

	std::cout << b->getBody()->GetVelocity();
	s->getBody()->SetVelocity(s->getBody()->GetVelocity() - (impulse * s->getBody()->getIMass()));
	b->getBody()->SetVelocity(b->getBody()->GetVelocity() + (impulse * b->getBody()->getIMass()));
	std::cout << b->getBody()->GetVelocity();



	
}

void GameSimsPhysics::CollisionResolution(BoundingBox* a, BoundingBox* b) {
	Vector2 bca = { a->getPosition().x - a->getSize().x / 2, a->getPosition().y - a->getSize().y / 2 };
	Vector2 tca = { a->getPosition().x + a->getSize().x / 2, a->getPosition().y + a->getSize().y / 2 };
	Vector2 bcb = { b->getPosition().x - b->getSize().x / 2, b->getPosition().y - b->getSize().y / 2 };
	Vector2 tcb = { b->getPosition().x + b->getSize().x / 2, b->getPosition().y + b->getSize().y / 2 };


	Vector2 normal;

	if (abs(tca.x - tcb.x) > abs(bca.x - bcb.x)) {
		normal = Vector2(tca.x - tcb.x, 0);
	}
	else {
		normal = Vector2(bca.x - bcb.x, 0);
	}

	if (abs(tca.y - tcb.y) < abs(bca.y - bcb.y)) {
		normal = Vector2(normal.x,tca.y - tcb.y);
	}
	else {
		normal = Vector2(normal.x, bca.y - bcb.y);
	}



	double xOverlap = std::fmax(0,std::min(tca.x, tcb.x) - std::max(bca.x, bcb.x));
	double yOverlap = std::fmax(0,(bca.y, bcb.y) - std::max(tca.y, tcb.y));

	double overlapArea = xOverlap * yOverlap;

	double diffX = a->getSize().x / 2 + b->getSize().y / 2;
	double diffY = a->getSize().y / 2 + b->getSize().y / 2;
	
	double centreDiff = sqrt((diffX * diffX) + (diffY * diffY));

	double penetration = overlapArea - centreDiff;

	normal.Normalise();

	Vector2 aPos = normal * penetration * (a->getBody()->getIMass() / (a->getBody()->getIMass() + b->getBody()->getIMass()));
	Vector2 bPos = normal * penetration * (b->getBody()->getIMass() / (a->getBody()->getIMass() + b->getBody()->getIMass()));

	a->getBody()->SetPosition(a->getPosition() - aPos);
	b->getBody()->SetPosition(b->getPosition() + bPos);

	float velAlongNorm = Vector2::Dot((a->getBody()->GetVelocity() - b->getBody()->GetVelocity()), normal);


	float e = std::min(a->getBody()->getElast(), b->getBody()->getElast());

	float j = -(1.0f + e) * velAlongNorm;
	j /= a->getBody()->getIMass() + b->getBody()->getIMass();

	Vector2 impulse = normal * j;

	std::cout << b->getBody()->GetVelocity();
	a->getBody()->SetVelocity(a->getBody()->GetVelocity() - (impulse * a->getBody()->getIMass()));
	b->getBody()->SetVelocity(b->getBody()->GetVelocity() + (impulse * b->getBody()->getIMass()));
	std::cout << b->getBody()->GetVelocity();


	

	
}