#pragma once

#include "Enemy.h"
#include "App.h"
#include "Animation.h"

#include "Point.h"
#include "SDL/include/SDL.h"

struct vec2 {
	int x, y;
};

class Bat : public Enemy
{
public:
	Bat(int x, int y, int name_, bool hardSolve);

	virtual ~Bat();
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	void gravity();

	bool CleanUp();

	void SolveColl(SDL_Rect rect);

	void SolveCollHard(SDL_Rect rect);

	SDL_Rect player;

	iPoint GetPosition();

	int GetName();

	bool GetSolveHard();

	int name;

	Collider* hit_bat;

	bool hardColl;

private:

	SDL_Texture* bat_sprite;
	Animation formation;
	Animation fly;
	Animation death;


	Animation* current_animation;

	int bat_state;
	int direction;
	bool is_moving;

	iPoint position;
	iPoint momentum;
	iPoint max_momentum;

	int gravity_;


	


};