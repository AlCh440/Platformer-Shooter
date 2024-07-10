#include "Bat.h"
#include "Render.h"
#include "Textures.h"
#include "Player.h"
#include "Pathfinding.h"
#include "Map.h"
#include "Collisions.h"
#include "Enemy.h"
#include "Scene.h"

#include "Point.h"
#include "Defs.h"
#include "Log.h"
#include "App.h"

Bat::Bat(int x, int y, int name_, bool hardSolve)
{
    position.x = x;
    position.y = y;

    name = name_;
    hardColl = hardSolve;
    Start();
}

Bat::~Bat()
{
}

bool Bat::Start()
{
   bat_sprite = app->tex->Load("Assets/Textures/bat/bat_sprite.png");

    bat_state = 0;
  
    direction = 4;
    formation.PushBack({ 6, 82, 30, 24 });
    formation.PushBack({ 48, 82, 30, 24 });
    formation.PushBack({ 92, 82, 30, 24 });

    formation.loop = false;
    formation.speed = 0.06f;

    fly.PushBack({ 0, 0, 30, 24 });
    fly.PushBack({ 44, 0, 30, 24 });
    fly.PushBack({ 87, 0, 30, 24 });
    fly.PushBack({ 131, 0, 30, 24 });
    fly.PushBack({ 175, 0, 30, 24 });
    fly.PushBack({ 219, 0, 30, 24 });
    fly.PushBack({ 263, 0, 30, 24 });

    fly.loop = true;
    fly.speed = 0.2f;
    if (hit_bat != NULL) app->coll->RemoveCollider(hit_bat);
    hit_bat = app->coll->AddCollider({ position.x, position.y, 24, 18 }, Collider::Type::ENEMY, this, app->scene);

    return true;
}

bool Bat::PreUpdate()
{
    return true;
}

bool Bat::Update(float dt)
{
    iPoint pos = app->map->WorldToMap(position.x, position.y);
    iPoint dest = app->map->WorldToMap(app->player->player.x, app->player->player.y);
    app->pathfinding->CreatePath(pos, dest);

    if (bat_state == 1 && !app->player->die)
    {
        const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
        if ((path->At(path->Count() - 1)->DistanceManhattan(pos) < 7))
        {
            
            if (path->At(1) != nullptr)
            {
                iPoint pat = app->map->WorldToMap(path->At(1)->x, path->At(1)->y);
                pat.x = path->At(1)->x;
                pat.y = path->At(1)->y;

                if ( pat.x < pos.x)
                {
                    position.x--;
                    direction = 0;
                }
                else if (pat.x > pos.x)
                {
                    position.x++;
                    direction = 1;
                }

                if (pat.y < pos.y)
                {
                    position.y--;
                    direction = 2;
                }
                else if (pat.y > pos.y)
                {
                    position.y++;
                    direction = 3;
                }
            }
        }
    }

    if (formation.HasFinished()) bat_state = 1;

    if (bat_state == 0) current_animation = &formation;
    else if (bat_state == 1) current_animation = &fly;

    current_animation->Update();

    hit_bat->SetPos(position.x + 3, position.y + 3);

    return true;
}

bool Bat::PostUpdate()
{
    app->render->DrawTexture(bat_sprite, position.x, position.y, &current_animation->GetCurrentFrame());
    return true;
}

void Bat::gravity()
{
}


bool Bat::CleanUp()
{
    return true;
}

void Bat::SolveColl(SDL_Rect rect)
{
    if (direction == 0) position.x++;
    else if (direction == 1) position.x--;
    else if (direction == 2) position.y++;
    else if (direction == 3) position.y--;
}

void Bat::SolveCollHard(SDL_Rect rect)
{
    position.x = rect.x - 5 - 16;
    position.y = rect.y - 5 - 16;

    direction = 4;
    hardColl = false;
}

iPoint Bat::GetPosition()
{
    return position;
}

int Bat::GetName()
{
    return name;
}

bool Bat::GetSolveHard()
{
    return hardColl;
}
