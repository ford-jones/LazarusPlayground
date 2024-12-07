#include "game.h"

Game::Game()
{
    soundManager = std::make_unique<Lazarus::AudioManager>();
    window = nullptr;
    textManager = nullptr;
    springWaltz = {};

    turnX = 0.0f;
    turnY = 0.0f;
    moveX = 0.0f;
    moveZ = 0.0f;

    word1 = 0;
    word2 = 0;
};

void Game::init()
{
    // globals.setLaunchInFullscreen(true);
    
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
    lightBuilder        = std::make_unique<Lazarus::LightManager>(shaderProgram);
    cameraBuilder       = std::make_unique<Lazarus::CameraManager>(shaderProgram);

    light               = lightBuilder->createLightSource(1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
    camera              = cameraBuilder->createPerspectiveCam(1.0, 1.0, 1.0, 0.0, 0.0, 0.0);

    // this->setupAudio();
    this->loadScene();
    this->layoutScene();


    textManager->extendFontStack("assets/fonts/MORPHEUS.TTF", 50);
};

void Game::loadScene()
{
    meshBuilder         = std::make_unique<Lazarus::MeshManager>(shaderProgram);

    skull               = meshBuilder->create3DAsset("assets/mesh/skull.obj", "assets/material/skull.mtl", "assets/images/skull.png");
    floors              = meshBuilder->create3DAsset("assets/mesh/floors.obj", "assets/material/floors.mtl", "assets/images/floors.png");
    walls               = meshBuilder->create3DAsset("assets/mesh/walls.obj", "assets/material/walls.mtl", "assets/images/walls.png");
    spiderweb           = meshBuilder->createQuad(4.0, 4.0, "assets/images/spiderweb.png");
    sword               = meshBuilder->create3DAsset("assets/mesh/sword.obj", "assets/material/sword.mtl");
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

            // soundManager->updateListenerLocation(camera.locationX, camera.locationY, camera.locationZ);

        /*skull*/
            meshBuilder->loadMesh(skull);
            meshBuilder->drawMesh(skull);

        /*floors*/
            meshBuilder->loadMesh(floors);
            meshBuilder->drawMesh(floors);

        /*walls*/
            meshBuilder->loadMesh(walls);
            meshBuilder->drawMesh(walls);

        /*spiderweb*/
            meshBuilder->loadMesh(spiderweb);
            meshBuilder->drawMesh(spiderweb);

        /*sword*/
            meshBuilder->loadMesh(sword);
            meshBuilder->drawMesh(sword);

            transformer.translateMeshAsset(sword, (0.5 / 10), 0.0, 0.0);
            transformer.rotateMeshAsset(sword, 0.0, 1.0, 0.0);
            
        /*text*/
            textManager->loadText("Lazarus Engine", ((globals.getDisplayWidth() / 2) - 350), (globals.getDisplayHeight() - 80), 10, 0.6f, 0.0f, 0.0f, word1);
            textManager->drawText(word1);

            std::string fps = std::string("FPS: ").append(std::to_string(static_cast<int>(fpsCounter.framesPerSecond)));
            textManager->loadText(fps, 50, 50, 5, 1.0f, 1.0f, 0.9f, word2);
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