#include "EnemyController.h"
#include "Collisions.h"
#include "bat.h"
#include "Walker.h"
#include "Scene.h"
#include "Map.h"

#include "Log.h"
EnemyController::EnemyController()
{
	name.Create("Enemy");
}

EnemyController::~EnemyController()
{
}

bool EnemyController::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (Enemies[i] != nullptr && Enemies[i]->pendingToDelete == true)
		{
			delete Enemies[i];
			Enemies[i] = nullptr;
		}
	}

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		// skip empty colliders
		if (Enemies[i] == nullptr)
			continue;

		Enemies[i]->PreUpdate();
		
	}

	return true;
}


bool EnemyController::Update(float dt)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		// skip empty colliders
		if (Enemies[i] == nullptr)
			continue;

		Enemies[i]->Update(dt);
	}

	return true;
}

bool EnemyController::PostUpdate()
{

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		// skip empty colliders
		if (Enemies[i] == nullptr)
			continue;

		Enemies[i]->PostUpdate();

	}
	return true;
}

bool EnemyController::LoadState(pugi::xml_node& data)
{
	CleanUp();

	pugi::xml_node enem = data.find_child_by_attribute("enemy", "name", "0");
	
	while (enem != NULL)
	{
		app->enemy->AddEnemy(data.child("enemy").attribute("x").as_int(), data.child("enemy").attribute("y").as_int(), data.child("enemy").attribute("name").as_int(), true);
		LOG("%i", enem.attribute("name").as_int());

		enem = enem.next_sibling();
	}
	return true;
}

bool EnemyController::SaveState(pugi::xml_node& data) const
{

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (Enemies[i] != nullptr)
		{
			pugi::xml_node enemy_ = data.append_child("enemy");
			int name_ = Enemies[i]->GetName();
			iPoint pos = Enemies[i]->GetPosition();

			enemy_.append_attribute("name") = name_;
			enemy_.append_attribute("x") = pos.x;
			enemy_.append_attribute("y") = pos.y;
		}
	}

	return true;
}


bool EnemyController::CleanUp()
{
	app->coll->RemoveColliderType(Collider::Type::ENEMY);

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (Enemies[i] != nullptr)
		{
			delete Enemies[i];
			Enemies[i] = nullptr;
		}
	}

	return true;
}

void EnemyController::StartLvl()
{
	CleanUp();

	app->map->LoadenemiesLvl();
}

Enemy* EnemyController::AddEnemy(int x, int y, int enemy, bool solve)
{
	Enemy* ret = nullptr;


	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (Enemies[i] == nullptr)
		{
			if (enemy == 0) ret = Enemies[i] = new Bat(x, y, i, solve);
			else if (enemy == 1) ret = Enemies[i] = new Walker(x, y, i, solve);

			break;
		}
	}

	return ret;
}

void EnemyController::DebugDraw()
{
}
