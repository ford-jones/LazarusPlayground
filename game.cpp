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

    scale = 1.0f;

    skyBox = {};
    fog = {};

    word1 = {};
    word2 = {};
    word3 = {};
    word4 = {};
    word5 = {};
};

void Game::init()
{
    globals.setLaunchInFullscreen(true);
    // globals.setVsyncDisabled(true);
    
    window = std::make_unique<Lazarus::WindowManager>("Lazarus Engine");
    
    window->createWindow();
    window->eventsInit();
    soundManager->initialise();

    window->createCursor(32, 32, 0, 0, "assets/images/crosshair.png");

    globals.setEnforceImageSanity(true);
    globals.setMaxImageSize(500, 500);

    shaderProgram = shader.compileShaders();
    window->loadConfig(shaderProgram);

    worldBuilder        = std::make_unique<Lazarus::WorldFX>(shaderProgram);
    meshBuilder         = std::make_unique<Lazarus::MeshManager>(shaderProgram);
    textManager         = std::make_unique<Lazarus::TextManager>(shaderProgram);
    lightBuilder        = std::make_unique<Lazarus::LightManager>(shaderProgram);
    cameraBuilder       = std::make_unique<Lazarus::CameraManager>(shaderProgram);

    light1              = lightBuilder->createLightSource(1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0);
    light2              = lightBuilder->createLightSource(-1.0, 1.0, -1.0, 0.0, 0.0, 1.0, 2.0);
    camera              = cameraBuilder->createPerspectiveCam();

    this->setupAudio();
    this->loadScene();
    this->layoutScene();
    this->loadText();
};

void Game::loadScene()
{
    skyBox              = worldBuilder->createSkyBox("assets/images/skybox/posx.png", "assets/images/skybox/negx.png", "assets/images/skybox/negy.png", "assets/images/skybox/posy.png", "assets/images/skybox/posz.png", "assets/images/skybox/negz.png");
    fog                 = worldBuilder->createFog(1.0, 10.0, 0.3, 0.5, 0.5, 0.5);
    
    spiderweb           = meshBuilder->createQuad(4.0, 4.0, "assets/images/spiderweb.png");
    skull               = meshBuilder->create3DAsset("assets/mesh/skull.obj", "assets/material/skull.mtl", "assets/images/skull.png", true);
    floors              = meshBuilder->create3DAsset("assets/mesh/floors.obj", "assets/material/floors.mtl", "assets/images/floors.png");
    walls               = meshBuilder->create3DAsset("assets/mesh/walls.obj", "assets/material/walls.mtl", "assets/images/walls.png", true);
    sword               = meshBuilder->create3DAsset("assets/mesh/sword.obj", "assets/material/sword.mtl");
};

void Game::loadText()
{
    textManager->extendFontStack("assets/fonts/MORPHEUS.TTF", 50);

    this->word1 = textManager->loadText("Lazarus Engine", ((globals.getDisplayWidth() / 2) - 350), (globals.getDisplayHeight() - 80), 10, 0.6f, 0.0f, 0.0f);
    this->word2 = textManager->loadText(" ", 50, 50, 5, 1.0f, 1.0f, 0.9f);
    this->word3 = textManager->loadText(" ", 50, 100, 5, 1.0f, 1.0f, 0.9f);
    this->word4 = textManager->loadText(" ", 50, 150, 5, 1.0f, 1.0f, 0.9f);
    this->word5 = textManager->loadText(" ", 50, globals.getDisplayHeight() - 80, 5, 1.0f, 1.0f, 0.9f);
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
        window->monitorFPS();
        window->monitorEvents();
        window->monitorPixelOccupants();
        
        this->keyCapture(window->keyEventString);

        /*Light*/
        lightBuilder->loadLightSource(light1);
        lightBuilder->loadLightSource(light2);

		/*Camera*/
        cameraBuilder->loadCamera(camera);
        transformer.rotateCameraAsset(camera, turnX, turnY, 0.0);
        transformer.translateCameraAsset(camera, moveX, 0.0, moveZ);
        soundManager->updateListenerLocation(camera.position.x, camera.position.y, camera.position.z);
        
        /*sky*/
            worldBuilder->drawSkyBox(skyBox, camera);
            fog.viewpoint = glm::vec3(camera.position.x, camera.position.y, camera.position.z);
            worldBuilder->loadFog(fog);
            
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
            textManager->loadText("Lazarus Engine", ((globals.getDisplayWidth() / 2) - 350), (globals.getDisplayHeight() - 80), 10, 0.6f, 0.0f, 0.0f, this->word1);
            textManager->drawText(word1);
            
            std::string cameraX = std::string("Camera-X: ").append(std::to_string(camera.position.x));
            std::string cameraY = std::string("Camera-Y: ").append(std::to_string(camera.position.y));
            std::string cameraZ = std::string("Camera-Z: ").append(std::to_string(camera.position.z));
            
            std::string fps = std::string("FPS: ").append(std::to_string(static_cast<int>(window->framesPerSecond)));
            
            int32_t occupant = cameraBuilder->getPixelOccupant(window->mousePositionX, window->mousePositionY);
            std::cout << "Mesh / occupant-id under cursor focus: " << occupant << std::endl;

            textManager->loadText(cameraX, 50, 150, 5, 1.0f, 1.0f, 0.9f, word2);
            textManager->drawText(word2);
            
            textManager->loadText(cameraY, 50, 100, 5, 1.0f, 1.0f, 0.9f, word3);
            textManager->drawText(word3);
            
            textManager->loadText(cameraZ, 50, 50, 5, 1.0f, 1.0f, 0.9f, word4);
            textManager->drawText(word4);
            
            textManager->loadText(fps, 50, globals.getDisplayHeight() - 80, 5, 1.0f, 1.0f, 0.9f, word5);
            textManager->drawText(word5);
            
            window->presentNextFrame();
            
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
			moveZ = 0.2;
		}
		else if(key == "down")
		{
			moveZ = -0.2;
		}
		else if(key == "left")
		{
			moveX = -0.2;
		}
		else if(key == "right")
		{
			moveX = 0.2;
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
        else if(key == "z")
        {
            scale += 0.01;
        }
        else if(key == "x")
        {
            scale += -0.01;
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