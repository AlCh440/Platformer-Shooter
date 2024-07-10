#include "Walker.h"
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

Walker::Walker(int x, int y, int name_, bool hardSolve)
{
    position.x = x;
    position.y = y;
    
    name = name_;

    Start();
}

Walker::~Walker()
{
}

bool Walker::Start()
{
    walker_sprite = app->tex->Load("Assets/Textures/walker/walker_sprite_sheet.png");

    walker_state = 0;
    current_animation = &formation;

    formation.PushBack({ 24, 16, 16, 16 });
    formation.PushBack({ 24, 64, 16, 16 });
    formation.PushBack({ 24, 112, 16, 16 });
    formation.PushBack({ 24, 160, 16, 16 });
    formation.PushBack({ 24, 208, 16, 16 });
    formation.PushBack({ 24, 256, 16, 16 });
    formation.PushBack({ 24, 304, 16, 16 });

    formation.loop = false;
    formation.speed = 0.1f;

    walking_rigth.PushBack({ 24, 353, 16, 16 });
    walking_rigth.PushBack({ 24, 400, 16, 16 });
    walking_rigth.PushBack({ 24, 448, 16, 16 });
    walking_rigth.PushBack({ 24, 496, 16, 16 });
    walking_rigth.PushBack({ 24, 544, 16, 16 });
    walking_rigth.PushBack({ 24, 592, 16, 16 });
    walking_rigth.PushBack({ 24, 640, 16, 16 });

    walking_rigth.loop = true;
    walking_rigth.speed = 0.15f;

    walking_left.PushBack({ 60, 353, 16, 16 });
    walking_left.PushBack({ 60, 400, 16, 16 });
    walking_left.PushBack({ 60, 448, 16, 16 });
    walking_left.PushBack({ 60, 496, 16, 16 });
    walking_left.PushBack({ 60, 544, 16, 16 });
    walking_left.PushBack({ 60, 592, 16, 16 });
    walking_left.PushBack({ 60, 640, 16, 16 });

    walking_left.loop = true;
    walking_left.speed = 0.15f;


    hit_bat = app->coll->AddCollider({ position.x, position.y, 16, 16 }, Collider::Type::ENEMY, this, app->scene);

    return true;
}

bool Walker::PreUpdate()
{
    return true;
}

bool Walker::Update(float dt)
{
    iPoint pos = app->map->WorldToMap(position.x, position.y);
    iPoint dest = app->map->WorldToMap(app->player->player.x, app->player->player.y);
    app->pathfinding->CreatePath(pos, dest);

    if (walker_state == 1 && !app->player->die)
    {
        const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
        if ((path->At(path->Count() - 1)->DistanceManhattan(pos) < 8))
        {
            if (path->At(1) != nullptr)
            {
                if (path->At(1)->x < pos.x)
                {
                    position.x--;
                    direction = 0;
                }
                else if (path->At(1)->x > pos.x)
                {
                    position.x++;
                    direction = 1;
                }
            }
        }
    }

    gravity();

    if (formation.HasFinished()) walker_state = 1;
    

    if (walker_state == 1)
    {
        if (direction == 0) current_animation = &walking_left;
        else if (direction == 1)current_animation = &walking_rigth;
    }

    current_animation->Update();

    hit_bat->SetPos(position.x, position.y);

    return true;
}

bool Walker::PostUpdate()
{
    app->render->DrawTexture(walker_sprite, position.x, position.y, &current_animation->GetCurrentFrame());
    return true;
}

void Walker::gravity()
{
    position.y++;
}


bool Walker::CleanUp()
{
    
    return true;
}

void Walker::SolveColl(SDL_Rect rect)
{
    if (rect.y < position.y + 16) position.y--;
    //else if (direction == 0) position.x += 2;
    //else if (direction == 1) position.x -= 2;
    else if (direction == 2) position.y += 2;
    else if (direction == 3) position.y -= 2;

}

void Walker::SolveCollHard(SDL_Rect rect)
{
    position.x = rect.x - 5 - 16;
    position.y = rect.y - 5 - 16;

    direction = 4;
    hardColl = false;
}

iPoint Walker::GetPosition()
{
    return position;
}

int  Walker::GetName()
{
    return name;
}

bool Walker::GetSolveHard()
{
    return hardColl;
}

