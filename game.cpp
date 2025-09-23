#include "game.h"

Game::Game()
{
    soundManager = std::make_unique<Lazarus::AudioManager>();
    globals = Lazarus::GlobalsManager();
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
    
    window = std::make_unique<Lazarus::WindowManager>("Lazarus Engine", 1000, 800);
    
    window->createWindow();
    window->eventsInit();
    soundManager->initialise();

    //  Create cursor prior to enforced image sanitisation
    window->createCursor(32, 32, 0, 0, "assets/images/crosshair.png");

    globals.setEnforceImageSanity(true);
    globals.setMaxImageSize(500, 500);
    
    window->loadConfig();

    default_shader = shader.compileShaders();
    caustics_shader = shader.compileShaders("assets/shaders/water.glsl");

    shader.setActiveShader(default_shader);

    worldBuilder        = std::make_unique<Lazarus::WorldFX>(default_shader);
    meshBuilder         = std::make_unique<Lazarus::MeshManager>(default_shader);
    waterBuilder        = std::make_unique<Lazarus::MeshManager>(caustics_shader);
    textManager         = std::make_unique<Lazarus::TextManager>(default_shader);
    lightBuilder        = std::make_unique<Lazarus::LightManager>(default_shader);
    cameraBuilder       = std::make_unique<Lazarus::CameraManager>(default_shader);

    light1              = lightBuilder->createLightSource(glm::vec3(10.0f, 10.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 10.0f);
    light2              = lightBuilder->createLightSource(glm::vec3(-10.0f, 10.0f, -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), 10.0f);
    camera              = cameraBuilder->createPerspectiveCam(500.0f);

    this->loadAssets();
    this->layoutScene();
    this->setupAudio();
    this->loadText();
};

void Game::loadAssets()
{
    auto start = std::chrono::system_clock::now();

    skyBox              = worldBuilder->createSkyBox("assets/images/skybox/pos_x.png", "assets/images/skybox/neg_x.png", "assets/images/skybox/neg_y.png", "assets/images/skybox/pos_y.png", "assets/images/skybox/pos_z.png", "assets/images/skybox/neg_z.png");
    fog                 = worldBuilder->createFog(5.0, 90.0f, 0.3f, glm::vec3(0.5f, 0.5f, 0.5f));

    shader.setActiveShader(default_shader);
    earth               = meshBuilder->create3DAsset("assets/mesh/earth.glb");
    skull               = meshBuilder->create3DAsset("assets/mesh/skull.obj", "assets/material/skull.mtl", true);
    terrain             = meshBuilder->create3DAsset("assets/mesh/town_square.glb");
    sword               = meshBuilder->create3DAsset("assets/mesh/sword.obj", "assets/material/sword.mtl");
    metaball            = meshBuilder->create3DAsset("assets/mesh/metaball.glb");
    
    shader.setActiveShader(caustics_shader);
    river               = waterBuilder->create3DAsset("assets/mesh/river.glb");

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_ms = (end - start);
    std::cout << "\n" << std::endl;
    std::cout << "Loaded Assets: " << elapsed_ms.count() << "ms" << std::endl;
};

void Game::loadText()
{
    shader.setActiveShader(default_shader);
    textManager->initialise();
    
    this->ubuntuFont = textManager->extendFontStack("assets/fonts/Ubuntu-R.ttf", 150);
    this->morpheusFont = textManager->extendFontStack("assets/fonts/MORPHEUS.TTF", 50);

    this->word1 = textManager->createText("Lazarus Engine", this->morpheusFont, glm::vec2(((globals.getDisplayWidth() / 2) - 350), (globals.getDisplayHeight() - 80)), glm::vec3(0.6f, 0.0f, 0.0f), 10);
    this->word2 = textManager->createText(" ", this->morpheusFont, glm::vec2(50, 50), glm::vec3(1.0f, 1.0f, 0.9f), 5);
    this->word3 = textManager->createText(" ", this->morpheusFont, glm::vec2(50, 100), glm::vec3(1.0f, 1.0f, 0.9f), 5);
    this->word4 = textManager->createText(" ", this->morpheusFont, glm::vec2(50, 150), glm::vec3(1.0f, 1.0f, 0.9f), 5);
    this->word5 = textManager->createText(" ", this->ubuntuFont, glm::vec2(50, globals.getDisplayHeight() - 80), glm::vec3(1.0f, 1.0f, 0.9f));
    this->word6 = textManager->createText(" ", this->ubuntuFont, glm::vec2(50, globals.getDisplayHeight() - 120), glm::vec3(1.0f, 1.0f, 0.9f));
};

void Game::layoutScene()
{
    transformer.translateCameraAsset(camera, 0.0f, 20.0f, 0.0f);

    transformer.translateMeshAsset(sword, 0.0f, 11.0f, 3.0f);
    transformer.translateMeshAsset(earth, 0.0f, 13.0f, 0.0f);
    transformer.translateMeshAsset(skull, 0.0f, 10.0f, 0.0f);
    transformer.translateMeshAsset(metaball, 20.0f, 30.0f, 0.0f);

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
        
        this->keyCapture(window->keyCode);

        shader.setActiveShader(default_shader);

        /*Light*/
        lightBuilder->loadLightSource(light1);
        lightBuilder->loadLightSource(light2);

		/*Camera*/
        cameraBuilder->loadCamera(camera);
        transformer.rotateCameraAsset(camera, turnX, turnY, 0.0f);
        transformer.translateCameraAsset(camera, moveX, 0.0f, moveZ);
        soundManager->updateListenerLocation(camera.position);
        
        /*sky*/
        fog.viewpoint = camera.position;
        worldBuilder->loadFog(fog);
        worldBuilder->drawSkyBox(skyBox, camera);
    
        /*skull*/
        meshBuilder->loadMesh(skull);
        meshBuilder->drawMesh(skull);
        /*terrain*/
        meshBuilder->loadMesh(terrain);
        meshBuilder->drawMesh(terrain);
        /*earth*/
        meshBuilder->loadMesh(earth);
        meshBuilder->drawMesh(earth);
        /*sword*/
        meshBuilder->loadMesh(sword);
        meshBuilder->drawMesh(sword);
        /*metaball*/
        meshBuilder->loadMesh(metaball);
        meshBuilder->drawMesh(metaball);

        shader.setActiveShader(caustics_shader);
        shader.uploadUniform("time", &window->elapsedTime);
        
        cameraBuilder->loadCamera(camera);
        worldBuilder->loadFog(fog, caustics_shader);

        /*river*/
        waterBuilder->loadMesh(river);
        waterBuilder->drawMesh(river);
        
        shader.setActiveShader(default_shader);
        transformer.translateMeshAsset(sword, (0.5f / 10), 0.0f, 0.0f);
        transformer.rotateMeshAsset(sword, 0.0f, 1.0f, 0.0f);
        transformer.rotateMeshAsset(earth, 0.0f, -0.7f, 0.0f);
        transformer.scaleMeshAsset(skull, scale, scale, scale);
        
        /*text*/
        textManager->loadText(this->word1);
        textManager->drawText(word1);
        
        word2.targetString     = std::string("Camera-X: ").append(std::to_string(camera.direction.x));
        word3.targetString     = std::string("Camera-Y: ").append(std::to_string(camera.direction.y));
        word4.targetString     = std::string("Camera-Z: ").append(std::to_string(camera.direction.z));
        word5.targetString     = std::string("FPS: ").append(std::to_string(static_cast<int>(window->framesPerSecond)));
        word6.targetString     = std::string("Select ID: ").append(std::to_string(cameraBuilder->getPixelOccupant(window->mousePositionX, window->mousePositionY)));
        
        textManager->loadText(word2);
        textManager->drawText(word2);
        
        textManager->loadText(word3);
        textManager->drawText(word3);
        
        textManager->loadText(word4);
        textManager->drawText(word4);
        
        textManager->loadText(word5);
        textManager->drawText(word5);

        textManager->loadText(word6);
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

void Game::keyCapture(uint32_t key)
{
    //  Key-bindings
    switch(key)
    {
        //  Arrow up / move forward
        case 265:
            moveZ = 0.5f;
            break;
        //  Arrow down / move back
        case 264:
            moveZ = -0.5f;
            break;
        //  Arrow left / move left
        case 263:
            moveX = -0.5f;
            break;
        //  Arrow right / move right
        case 262:
            moveX = 0.5f;
            break;
        //  W / Look up
        case 87:
            turnX += -1.0f;
            break;
        //  A / Look left
        case 65:
            turnY += -2.0f;
            break;
        //  S / Look down
        case 83:
            turnX += 2.0f;
            break;
        //  D / Look right
        case 68:
            turnY += 2.0f;
            break;
        //  F / toggle fullscreen
        case 70:
            if(previousKey != 70) window->toggleFullscreen();
            break;
        //  Z / shrink skull
        case 90:
            scale -= 0.2f;
            break;
        //  X / grow skull
        case 88:
            scale += 0.2f;
            break;
        //  Reset
        default:
            moveX = 0.0f;
			moveZ = 0.0f;
            break;
    }

    if(turnX > 360.0f || turnX < -360)
    {
        turnX = 0;
    };

    previousKey = key;
};