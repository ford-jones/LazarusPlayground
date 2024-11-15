#include "game.h"

Game::Game()
{
    soundManager = std::make_unique<Lazarus::SoundManager>();
    window = nullptr;
    springWaltz = nullptr;
    footstep = nullptr;
    textManager = nullptr;
};

void Game::init()
{
    globals.setLaunchInFullscreen(true);
    
    window = std::make_unique<Lazarus::WindowManager>("Lazarus Engine");
    
    window->initialise();
    eventManager.initialise();
    soundManager->initialise();

    window->createCursor(32, 32, 0, 0, "assets/images/crosshair.png");

    globals.setEnforceImageSanity(true);
    globals.setMaxImageSize(500, 500);

    shaderProgram = shader.initialiseShader();
    window->loadConfig(shaderProgram);

    textManager         = std::make_unique<Lazarus::TextManager>(shaderProgram);
    lightBuilder        = std::make_unique<Lazarus::Light>(shaderProgram);
    cameraBuilder       = std::make_unique<Lazarus::Camera>(shaderProgram);

    light               = lightBuilder->createAmbientLight(1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
    camera              = cameraBuilder->createPerspectiveCam(1.0, 1.0, 1.0, 0.0, 0.0, 0.0);

    this->setupAudio();
    this->loadScene();
    this->layoutScene();


    textManager->extendFontStack("assets/fonts/MORPHEUS.TTF", 50);
};

void Game::loadScene()
{
    spiderwebBuilder    = std::make_unique<Lazarus::Mesh>(shaderProgram);
    skullBuilder        = std::make_unique<Lazarus::Mesh>(shaderProgram);
    floorsBuilder       = std::make_unique<Lazarus::Mesh>(shaderProgram);
    wallsBuilder        = std::make_unique<Lazarus::Mesh>(shaderProgram);
    swordBuilder        = std::make_unique<Lazarus::Mesh>(shaderProgram);

    spiderweb           = spiderwebBuilder->createQuad(4.0, 4.0, "assets/images/spiderweb.png");
    skull               = skullBuilder->create3DAsset("assets/mesh/skull.obj", "assets/material/skull.mtl", "assets/images/skull.png");
    floors              = floorsBuilder->create3DAsset("assets/mesh/floors.obj", "assets/material/floors.mtl", "assets/images/floors.png");
    walls               = wallsBuilder->create3DAsset("assets/mesh/walls.obj", "assets/material/walls.mtl", "assets/images/walls.png");
    sword               = swordBuilder->create3DAsset("assets/mesh/sword.obj", "assets/material/sword.mtl");
};

void Game::layoutScene()
{
    transformer.translateMeshAsset(spiderweb, 1.5, -1.0, -3.0);
    transformer.rotateMeshAsset(spiderweb, -25.0, -40.0, 0.0);
    transformer.translateMeshAsset(sword, 0.0, 1.0, 3.0);
};

void Game::setupAudio()
{
    springWaltz = soundManager->createAudio("assets/sound/springWaltz.mp3", true, 0);
    soundManager->loadAudio(springWaltz);
    soundManager->playAudio(springWaltz);

    footstep = soundManager->createAudio("assets/sound/footsteps.mp3", true, 0);
    soundManager->loadAudio(footstep);
    soundManager->playAudio(footstep);
};

void Game::start()
{
    window->open();

    while(window->isOpen)
    {
        fpsCounter.calculateFramesPerSec();
        
        /*Events*/
        eventManager.listen();
        this->keyCapture(eventManager.keyString);

        /*Light*/
        lightBuilder->loadLightSource(light);
        transformer.translateLightAsset(light, (moveX / 10), 0.0, (moveZ / 10));

		/*Camera*/
            cameraBuilder->loadCamera(camera);

            transformer.rotateCameraAsset(camera, turnX, turnY, 0.0);
            transformer.translateCameraAsset(camera, (moveX / 10), 0.0, (moveZ / 10));

            soundManager->updateListenerLocation(camera.locationX, camera.locationY, camera.locationZ);

        /*spiderweb*/
            spiderwebBuilder->initialiseMesh(spiderweb);

            spiderwebBuilder->loadMesh(spiderweb);
            spiderwebBuilder->drawMesh(spiderweb);

        /*skull*/
            skullBuilder->initialiseMesh(skull);

            skullBuilder->loadMesh(skull);
            skullBuilder->drawMesh(skull);

        /*sword*/
            swordBuilder->initialiseMesh(sword);

            swordBuilder->loadMesh(sword);
            swordBuilder->drawMesh(sword);

            transformer.translateMeshAsset(sword, (0.5 / 10), 0.0, 0.0);
            transformer.rotateMeshAsset(sword, 0.0, 1.0, 0.0);

        /*floors*/
            floorsBuilder->initialiseMesh(floors);

            floorsBuilder->loadMesh(floors);
            floorsBuilder->drawMesh(floors);

        /*walls*/
            wallsBuilder->initialiseMesh(walls);

            wallsBuilder->loadMesh(walls);
            wallsBuilder->drawMesh(walls);

        /*text*/
            int word1 = textManager->loadText("Lazarus Engine", ((globals.getDisplayWidth() / 2) - 350), (globals.getDisplayHeight() - 80), 10, 0.6f, 0.0f, 0.0f);
            textManager->drawText(word1);

            std::string fps = std::string("FPS: ").append(std::to_string(static_cast<int>(fpsCounter.framesPerSecond)));

            int word2 = textManager->loadText(fps, 50, 50, 5, 1.0f, 1.0f, 0.9f);
            textManager->drawText(word2);

        window->handleBuffers();

        engineStatus = globals.getExecutionState();
        
        if(engineStatus != LAZARUS_OK)
        {
            std::cout << "Engine status: " << engineStatus << std::endl;
            break;
        }
    };
};

void Game::keyCapture(string key)
{
		if(key == "up")
		{
			moveZ = 0.5;
		}
		else if(key == "down")
		{
			moveZ = -0.5;
		}
		else if(key == "left")
		{
			moveX = -0.5;
		}
		else if(key == "right")
		{
			moveX = 0.5;
		}
		else if(key == "w")
		{
			turnX += -1.0;
		}
		else if(key == "s")
		{
			turnX += 1.0;
		}
		else if(key == "a")
		{
			turnY += -1.0;
		}
		else if(key == "d")
		{
			turnY += 1.0;
		}
		else 
		{
			moveX = 0.0;
			moveZ = 0.0;
		};

        if(turnX > 360.0f || turnX < -360)
        {
            turnX = 0;
        };
};