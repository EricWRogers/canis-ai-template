#include "App.hpp"

App::App()
{
	
}
App::~App()
{
	
}

void App::Run()
{
	if (appState == AppState::ON)
		Canis::FatalError("App already running.");

	Canis::Init();

	unsigned int windowFlags = 0;

	window.Create("Canis", 1280, 720, windowFlags);

	time.init(30);

	camera.override_camera = false;


	seed = std::time(NULL);
	srand(seed);
	Canis::Log("seed : " + std::to_string(seed));

	// Add more scenes here
	sceneManager.Add(new SpriteDemoScene("SpriteDemoScene"));

	sceneManager.PreLoad(
		&window,
		&inputManager,
		&time,
		&camera
	);

	Canis::Log("Q App 0");

	Load();

	appState = AppState::ON;

	Loop();
}
void App::Load()
{
	// Choose the first scene
	sceneManager.Load("SpriteDemoScene");

	// start timer
	previousTime = high_resolution_clock::now();
}
void App::Loop()
{
	while (appState == AppState::ON)
	{
		deltaTime = time.startFrame();
		sceneManager.SetDeltaTime(deltaTime);

		Update();
		Draw();
		// Get SDL to swap our buffer
		window.SwapBuffer();
		LateUpdate();
		InputUpdate();

		window.fps = time.endFrame(); 
	}
}
void App::Update()
{
	sceneManager.Update();
}
void App::Draw()
{
	sceneManager.Draw();
}
void App::LateUpdate()
{
	sceneManager.LateUpdate();
}
void App::InputUpdate()
{
	inputManager.swapMaps();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			appState = AppState::OFF;
			break;
		case SDL_MOUSEMOTION:
				inputManager.mouse.x = event.motion.x;
				inputManager.mouse.y = event.motion.y;
				camera.ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
			break;
		case SDL_KEYUP:
			inputManager.releasedKey(event.key.keysym.sym);
			//Canis::Log("UP" + std::to_string(event.key.keysym.sym));
			break;
		case SDL_KEYDOWN:
			inputManager.pressKey(event.key.keysym.sym);
			//Canis::Log("DOWN");
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT)
				inputManager.leftClick = true;
			if(event.button.button == SDL_BUTTON_RIGHT)
				inputManager.rightClick = true;
			break;
		}
	}

	sceneManager.InputUpdate();
}