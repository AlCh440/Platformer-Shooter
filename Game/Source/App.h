#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class Player;
class Collisions;
class Map;
class EnemyController;
class PathFinding;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// L02: DONE 1: Create methods to request Load / Save
	void LoadGameRequest();
	void SaveGameRequest() const;

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();


	// Load / Save
	bool LoadGame();
	bool SaveGame() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	Player* player;
	Collisions* coll;
	Map* map;
	EnemyController* enemy;
	PathFinding* pathfinding;

	Uint32 start;
	bool start_preupdate;
	bool stop_update;
	bool cappFrames;

private:

	int argc;
	char** args;
	SString title;
	SString organization;


	List<Module *> modules;

	// TODO 2: Create new variables from pugui namespace:
	// a xml_document to store the config file and
	// two xml_node to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;



	PerfTimer* ptimer;
	PerfTimer* frameDuration;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	float dt = 0.0f;

	

	uint32 maxFrameRate = 0;

	// L02: DONE 1: Create variables to control when to execute the request load / save
	mutable bool saveGameRequested;
	bool loadGameRequested;
};
extern App* app;

#endif	// __APP_H__