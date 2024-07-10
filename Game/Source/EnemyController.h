#ifndef __ENEMYCONTROLLER_H__
#define __ENEMYCONTROLLER_H__

#define MAX_ENEMIES 20

#include "Module.h"
#include "Collider.h"
#include "Enemy.h"

#include "SDL/include/SDL.h"

class EnemyController : public Module
{
public:
	// Constructor

	EnemyController();

	// Destructor
	virtual ~EnemyController();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	bool CleanUp();

	void StartLvl();

	Enemy* AddEnemy(int x, int y, int enemy, bool solve);

	void DebugDraw();

	// All existing colliders in the scene
	Enemy* Enemies[MAX_ENEMIES] = { nullptr };

};

#endif // __MODULE_ENEMYCONTROLLER_H__

