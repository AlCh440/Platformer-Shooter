#include "App.h"
#include "Player.h"
#include "Module.h"
#include "Scene.h"
#include "Render.h"
#include "Textures.h"
#include "Input.h"
#include "Scene.h"
#include "Collisions.h"

#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");
	
	playerframescounter = 0;

	is_moving = false;

	right_idle.PushBack({ 113, 0, 22, 23 });
	right_idle.PushBack({ 90, 0, 22, 23 });
	right_idle.PushBack({ 67, 0, 22, 23 });
	right_idle.PushBack({ 44, 0, 22, 23 });
	right_idle.PushBack({ 23, 0, 22, 23 });
	right_idle.PushBack({ 1, 0, 22, 23 });
	
	right_idle.loop = false;
	right_idle.speed = 0.08f;

	left_idle.PushBack({ 135, 0, 22, 23 });
	left_idle.PushBack({ 158, 0, 22, 23 });
	left_idle.PushBack({ 181, 0, 22, 23 });
	left_idle.PushBack({ 204, 0, 22, 23 });
	left_idle.PushBack({ 226, 0, 22, 23 });
	left_idle.PushBack({ 225, 26, 22, 23 });

	left_idle.loop = false;
	left_idle.speed = 0.05f;

	right_running.PushBack({ 0, 26, 22, 23 });
	right_running.PushBack({ 23, 26, 22, 23 });
	right_running.PushBack({ 46, 26, 22, 23 });
	right_running.PushBack({ 69, 26, 22, 23 });
	right_running.PushBack({ 91, 26, 22, 23 }); 
	right_running.PushBack({ 114, 26, 22, 23 });
	right_running.PushBack({ 136, 26, 22, 23 });

	right_running.loop = true;
	right_running.speed = 0.1f;

	left_running.PushBack({ 0, 51, 22, 23 });
	left_running.PushBack({ 22, 51, 22, 23 });
	left_running.PushBack({ 47, 51, 22, 23 });
	left_running.PushBack({ 69, 51, 22, 23 });
	left_running.PushBack({ 90, 51, 22, 23 });
	left_running.PushBack({ 112, 51, 22, 23 });
	left_running.PushBack({ 136, 51, 22, 23 });

	left_running.loop = true;
	left_running.speed = 0.1f;

	dying.PushBack({ 5, 74, 23, 23 });
	dying.PushBack({ 28, 74, 27, 23 });
	dying.PushBack({ 56, 74, 27, 23 });
	dying.PushBack({ 84, 74, 27, 23 });
	dying.PushBack({ 113, 74, 27, 23 });

	dying.loop = false;
	dying.speed = 0.08f;

	player = { 32, 250, 16, 23 };

	can_move_left = true;
	can_move_right = true;
	can_move_down = true;
	can_jump = true;
	can_double_jump = true;
	die = false;
}

Player::~Player()
{}

bool Player::Awake(pugi::xml_node& config)
{
	bool ret = true;

	player.x = config.child("player_").attribute("x").as_float();
	player.y = config.child("player_").attribute("y").as_float();
	max_momentum.x = config.child("max_mom").attribute("x").as_float();
	max_momentum.y = config.child("max_mom").attribute("y").as_float();
	gravity_ = config.child("player_grav").attribute("gravity").as_float();
 
	return ret;
}

// Load assets
bool Player::Start()
{
	idle_player = app->tex->Load("Assets/Textures/player/player_sheet.png");

	hit_player = app->coll->AddCollider(player, Collider::Type::PLAYER, 0, app->player);
	near_right = app->coll->AddCollider({ player.x + player.w, player.y, 1, player.h - 1 }, Collider::Type::NEAR, 0, app->player);
	near_left = app->coll->AddCollider({ player.x - 1, player.y, 1, player.h - 1 }, Collider::Type::NEAR, 0, app->player);
	near_down = app->coll->AddCollider({ player.x, player.y + player.w, player.w, 4 }, Collider::Type::NEAR, 0, app->player);

	current_animation = &right_running;

	return true;
}

// Unload assets
bool Player::CleanUp()
{


	return true;
}

// Called each loop iteration
bool Player::PreUpdate()
{

	if (can_jump) can_double_jump = true;

	if (!(app->scene->stop_game ))
	{
		if (!die)
		{
			if ((app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && can_move_right)
			{
				momentum.x += 20;
				is_moving = true;
			}

			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && can_move_left)
			{
				momentum.x -= 10;
				is_moving = true;
			}

			if ((app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && (can_jump || can_double_jump))
			{
				momentum.y = -200;
				
 				if (can_jump) can_jump = false;
    			else if (can_double_jump) can_double_jump = false;
			}
		}
	}

	return true;
}

// Called each loop iteration
bool Player::Update(float dt)
{
	gravity(dt);

	if (momentum.x < -max_momentum.x) momentum.x = -max_momentum.x;
	if (momentum.x > max_momentum.x + 10) momentum.x = max_momentum.x + 10;

	if (player.x < 0) player.x = 0;

	if (momentum.y > max_momentum.y) momentum.y = max_momentum.y;

	if (momentum.x > 5 && momentum.x < 5) momentum.x = 0;

	LOG("%f", momentum.x);

	player.x += momentum.x / dt;
	player.y += momentum.y / dt;
	
	if (momentum.x < 0)
	{
		momentum.x += 2;
		direction = 2;
	}
	else if (momentum.x > 0)
	{
		momentum.x -= 2;
		direction = 1;
	}
	else if (momentum.x == 0 && momentum.y == 0)
	{
		direction = 0;
	}

	if (direction == 0)
	{
		if ((playerframescounter > 180) && !die)
		{
			if (current_animation == &right_running) current_animation = &right_idle;
			else if (current_animation == &left_running) current_animation = &left_idle;
		}
		else if (die)
		{
			current_animation = &dying;
		}
		else playerframescounter++;
	}
	else if (direction == 1)
	{
		current_animation = &right_running;
		playerframescounter = 0;
	}
	else if (direction == 2)
	{
		current_animation = &left_running;
		playerframescounter = 0;
	}


	current_animation->Update();

	is_moving = false;
	can_move_right = true;
	can_move_left = true;
	can_move_down = true;

	// MOVING COLLITIONS

	hit_player->SetPos(player.x, player.y);
	near_right->SetPos(player.x + player.w, player.y);
	near_left->SetPos(player.x - 1, player.y);
	near_down->SetPos(player.x, player.y + player.h);
	
	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{

	//Drawing player
	if (current_animation == &left_idle || current_animation == &left_running) app->render->DrawTexture(idle_player, player.x - 5, player.y, &current_animation->GetCurrentFrame());
	else app->render->DrawTexture(idle_player, player.x, player.y, &current_animation->GetCurrentFrame());
	
	return true;
}

void Player::gravity(float dt)
{
	if (can_move_down)
	{
		momentum.y += gravity_;
	}
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == Collider::Type::WALL)
	{
		if (c1 == near_down)
		{
			if (momentum.y > 0)
			{
				player.y = c2->rect.y - player.h;
				
				momentum.y = 0;
			}
			else 
			{
				can_jump = true;
				can_move_down = false;
			}
		}
		else if (c1 == near_right)
		{
			if (momentum.x > 0 && player.x < c2->rect.x)
			{
				player.x = c2->rect.x - player.w;
				momentum.x = 0;
			}
			can_move_right = false;
		}
		else if (c1 == near_left)
		{
			if (momentum.x < 0 && player.x > c2->rect.x)
			{
				//player.x = c2->rect.x + c2->rect.w;
				momentum.x = 0;
			}
			can_move_left = false;
		}
	}
	if (c1 == near_down && c2->type == Collider::Type::PLAT)
	{
		if (momentum.y > 0)
		{
			if (player.x + player.h > c2->rect.x) player.y = c2->rect.y - player.h;
			momentum.y = 0;
		}
		can_jump = true;
		can_move_down = false;
	}

	if (c1 == hit_player && c2->type == Collider::Type::ENEMY)
	{
		die = true;
		current_animation = &dying;
		dying.Reset();
	}
}

// Load / Save
bool Player::LoadState(pugi::xml_node& data)
{

	player.x = data.child("player").attribute("x").as_int();
	player.y = data.child("player").attribute("y").as_int();

	return true;
}

bool Player::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node player_ = data.append_child("player");

	player_.append_attribute("x") = player.x;
	player_.append_attribute("y") = player.y;

	return true;
}

void Player::StartLvl()
{
	pugi::xml_document configFile;
	pugi::xml_node config;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = app->LoadConfig(configFile);

	player.x = config.child("player").child("player_").attribute("x").as_int();
	player.y = config.child("player").child("player_").attribute("y").as_int();

	app->render->camera.x = config.child("renderer").child("camera").attribute("x").as_int();
	app->render->camera.y = config.child("renderer").child("camera").attribute("y").as_int();

	die = false;
	current_animation = &right_idle;
}