#include "game.h"

Game::Game()
{
    // soundManager = std::make_unique<Lazarus::AudioManager>();
    globals = Lazarus::GlobalsManager();
    window = nullptr;
    textManager = nullptr;
    springWaltz = {};

    turnX = 0.0f;
    turnY = 0.0f;
    moveX = 0.0f;
    moveZ = 0.0f;

    skyBox = {};
    fog = {};

    this->morpheusFont = 0;
    this->ubuntuFont = 0;

    word1 = {};
    word2 = {};
    word3 = {};
    word4 = {};
    word5 = {};
    word6 = {};
};

void Game::init()
{
    globals.setLaunchInFullscreen(true);
    globals.setVsyncDisabled(true);
    
    window = std::make_unique<Lazarus::WindowManager>("Lazarus Engine");
    
    window->createWindow();
    window->eventsInit();
    // soundManager->initialise();

    //  Create cursor prior to enforced image sanitisation
    window->createCursor(32, 32, 0, 0, "assets/images/crosshair.png");

    globals.setEnforceImageSanity(true);
    globals.setMaxImageSize(500, 500);
    
    window->loadConfig();

    shaderProgram = shader.compileShaders();
    shader.setActiveShader(shaderProgram);

    worldBuilder        = std::make_unique<Lazarus::WorldFX>(shaderProgram);
    meshBuilder         = std::make_unique<Lazarus::MeshManager>(shaderProgram);
    textManager         = std::make_unique<Lazarus::TextManager>(shaderProgram);
    lightBuilder        = std::make_unique<Lazarus::LightManager>(shaderProgram);
    cameraBuilder       = std::make_unique<Lazarus::CameraManager>(shaderProgram);

    light1              = lightBuilder->createLightSource(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    light2              = lightBuilder->createLightSource(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 2.0f);
    camera              = cameraBuilder->createPerspectiveCam();

    // this->setupAudio();
    this->loadScene();
    this->layoutScene();
    this->loadText();
};

void Game::loadScene()
{
    skyBox              = worldBuilder->createSkyBox("assets/images/skybox/posx.png", "assets/images/skybox/negx.png", "assets/images/skybox/negy.png", "assets/images/skybox/posy.png", "assets/images/skybox/posz.png", "assets/images/skybox/negz.png");
    fog                 = worldBuilder->createFog(5.0, 20.0f, 0.3f, 0.5f, 0.5f, 0.5f);
    
    earth               = meshBuilder->create3DAsset("assets/mesh/earth.glb");
    spiderweb           = meshBuilder->createQuad(4.0f, 4.0f, "assets/images/spiderweb.png");
    skull               = meshBuilder->create3DAsset("assets/mesh/skull.obj", "assets/material/skull.mtl", "assets/images/skull.png", true);
    floors              = meshBuilder->create3DAsset("assets/mesh/floors.obj", "assets/material/floors.mtl", "assets/images/floors.png");
    walls               = meshBuilder->create3DAsset("assets/mesh/walls.obj", "assets/material/walls.mtl", "assets/images/walls.png", true);
    sword               = meshBuilder->create3DAsset("assets/mesh/sword.obj", "assets/material/sword.mtl");
    metaball            = meshBuilder->create3DAsset("assets/mesh/metaball.glb");
};

void Game::loadText()
{
    this->ubuntuFont = textManager->extendFontStack("assets/fonts/Ubuntu-R.ttf", 150);
    this->morpheusFont = textManager->extendFontStack("assets/fonts/MORPHEUS.TTF", 50);

    this->word1 = textManager->loadText("Lazarus Engine", this->morpheusFont, ((globals.getDisplayWidth() / 2) - 350), (globals.getDisplayHeight() - 80), 10, 0.6f, 0.0f, 0.0f);
    this->word2 = textManager->loadText(" ", this->morpheusFont, 50, 50, 5, 1.0f, 1.0f, 0.9f);
    this->word3 = textManager->loadText(" ", this->morpheusFont, 50, 100, 5, 1.0f, 1.0f, 0.9f);
    this->word4 = textManager->loadText(" ", this->morpheusFont, 50, 150, 5, 1.0f, 1.0f, 0.9f);
    this->word5 = textManager->loadText(" ", this->ubuntuFont, 50, globals.getDisplayHeight() - 80, 5, 1.0f, 1.0f, 0.9f);
    this->word6 = textManager->loadText(" ", this->ubuntuFont, 50, globals.getDisplayHeight() - 120, 5, 1.0f, 1.0f, 0.9f);
};

void Game::layoutScene()
{
    transformer.rotateMeshAsset(spiderweb, -25.0f, -40.0f, 0.0f);

    transformer.translateMeshAsset(spiderweb, 1.5f, -1.0f, -3.0f);
    transformer.translateMeshAsset(sword, 0.0f, 1.0f, 3.0f);
    transformer.translateMeshAsset(earth, 0.0f, 3.0f, 0.0f);
    transformer.translateMeshAsset(metaball, 20.0f, 0.0f, 0.0f);

    transformer.scaleMeshAsset(metaball, 6.0f, 6.0f, 6.0f);
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
        transformer.rotateCameraAsset(camera, turnX, turnY, 0.0f);
        transformer.translateCameraAsset(camera, moveX, 0.0f, moveZ);
        // soundManager->updateListenerLocation(camera.position.x, camera.position.y, camera.position.z);
        
        /*sky*/
        fog.viewpoint = glm::vec3(camera.position.x, camera.position.y, camera.position.z);
        worldBuilder->loadFog(fog);
        worldBuilder->drawSkyBox(skyBox, camera);
        
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
        /*earth*/
        meshBuilder->loadMesh(earth);
        meshBuilder->drawMesh(earth);
        /*sword*/
        meshBuilder->loadMesh(sword);
        meshBuilder->drawMesh(sword);
        /*metaball*/
        meshBuilder->loadMesh(metaball);
        meshBuilder->drawMesh(metaball);
        
        transformer.translateMeshAsset(sword, (0.5f / 10), 0.0f, 0.0f);
        transformer.rotateMeshAsset(sword, 0.0f, 1.0f, 0.0f);
        transformer.rotateMeshAsset(earth, 0.0f, -0.7f, 0.0f);
        
        /*text*/
        textManager->loadText("Lazarus Engine", this->morpheusFont, ((globals.getDisplayWidth() / 2) - 350), (globals.getDisplayHeight() - 80), 10, 0.6f, 0.0f, 0.0f, this->word1);
        textManager->drawText(word1);
        
        std::string cameraX     = std::string("Camera-X: ").append(std::to_string(camera.position.x));
        std::string cameraY     = std::string("Camera-Y: ").append(std::to_string(camera.position.y));
        std::string cameraZ     = std::string("Camera-Z: ").append(std::to_string(camera.position.z));
        std::string fps         = std::string("FPS: ").append(std::to_string(static_cast<int>(window->framesPerSecond)));
        std::string occupant    = std::string("Select ID: ").append(std::to_string(cameraBuilder->getPixelOccupant(window->mousePositionX, window->mousePositionY)));
        
        textManager->loadText(cameraX, this->morpheusFont, 50, 150, 5, 1.0f, 1.0f, 0.9f, word2);
        textManager->drawText(word2);
        
        textManager->loadText(cameraY, this->morpheusFont, 50, 100, 5, 1.0f, 1.0f, 0.9f, word3);
        textManager->drawText(word3);
        
        textManager->loadText(cameraZ, this->morpheusFont, 50, 50, 5, 1.0f, 1.0f, 0.9f, word4);
        textManager->drawText(word4);
        
        textManager->loadText(fps, this->ubuntuFont, 50, globals.getDisplayHeight() - 80, 5, 1.0f, 1.0f, 0.9f, word5);
        textManager->drawText(word5);

        textManager->loadText(occupant, this->ubuntuFont, 50, globals.getDisplayHeight() - 120, 5, 1.0f, 1.0f, 0.9f, word6);
        textManager->drawText(word6);
        
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
			moveZ = 0.5f;
		}
		else if(key == "down")
		{
			moveZ = -0.5f;
		}
		else if(key == "left")
		{
			moveX = -0.5f;
		}
		else if(key == "right")
		{
			moveX = 0.5f;
		}
		else if(key == "w")
		{
			turnX += -1.0f;
		}
		else if(key == "s")
		{
			turnX += 2.0f;
		}
		else if(key == "a")
		{
			turnY += -2.0f;
		}
		else if(key == "d")
		{
			turnY += 2.0f;
		}
		else 
		{
			moveX = 0.0f;
			moveZ = 0.0f;
		};

        if(turnX > 360.0f || turnX < -360)
        {
            turnX = 0;
        };
};