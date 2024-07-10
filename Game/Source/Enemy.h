#pragma once

#include "Module.h"
#include "App.h"
#include "Animation.h"

#include "Point.h"
#include "SDL/include/SDL.h"



class Enemy 
{
public:
	

	bool Start();
	bool Awake(pugi::xml_node&);

	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}


	void OnCollision(Collider* c1, Collider* c2);

	// Load / Save
	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&) const
	{
		return true;
	}

	bool CleanUp()
	{
		return true;
	}

	virtual void SolveColl(SDL_Rect rect)
	{

	}

	virtual void SolveCollHard(SDL_Rect rect)
	{

	}
	
	virtual bool GetSolveHard() 
	{
		return bool();
	}

	SDL_Rect player;

	bool pendingToDelete;

	virtual iPoint GetPosition()
	{
		return iPoint();
	}

	virtual int GetName()
	{
		return int();
	}

	//Collider* hit_bat;
private:

	
	int playerframescounter;

}; 
