#include "game.h"

Game::Game()
{
    globals = Lazarus::GlobalsManager();
    soundManager = std::make_unique<Lazarus::AudioManager>();
    window          = nullptr;
    textManager     = nullptr;
    cameraBuilder   = nullptr;
    lightBuilder    = nullptr;
    worldBuilder    = nullptr;
    meshBuilder     = nullptr;
    status_x        = nullptr;
    status_y        = nullptr;
    status_z        = nullptr;
    frame_counter   = nullptr;
    selection       = nullptr;
    event = {};
    
    springWaltz = {};
    assets = {};

    skull    = nullptr;
    sword    = nullptr;
    earth    = nullptr;
    metaball = nullptr;
    warrior  = nullptr;

    turnX = 0.0f;
    turnY = 0.0f;
    moveX = 0.0f;
    moveZ = 0.0f;
    scale = 1.0f;

    skyBox = {};
    fog = {};

    this->morpheusFont = 0;
    this->ubuntuFont = 0;
};

void Game::init()
{
    globals.setLaunchInFullscreen(true);
    // globals.setVsyncDisabled(true);
    
    window = std::make_unique<Lazarus::WindowManager>("Lazarus Engine", 1000, 800);
    
    window->createWindow();
    window->eventsInit();
    soundManager->initialise();

    //  Create cursor prior to enforced image sanitisation
    window->createCursor(32, 32, 0, 0, "assets/images/crosshair.png");

    globals.setEnforceImageSanity(true);
    globals.setMaxImageSize(500, 500);
    
    window->loadConfig();

    shader.compileShaders(default_shader);
    shader.compileShaders(caustics_shader, "assets/shaders/water.glsl");

    shader.setActiveShader(default_shader);

    worldBuilder        = std::make_unique<Lazarus::WorldFX>(default_shader);
    meshBuilder         = std::make_unique<Lazarus::ModelManager>(default_shader);
    waterBuilder        = std::make_unique<Lazarus::ModelManager>(caustics_shader);
    textManager         = std::make_unique<Lazarus::TextManager>(default_shader);
    lightBuilder        = std::make_unique<Lazarus::LightManager>(default_shader);
    cameraBuilder       = std::make_unique<Lazarus::CameraManager>(default_shader);

    Lazarus::LightManager::LightConfig lightConfig = {};
    lightConfig.position    = glm::vec3(10.0f, 10.0f, 1.0f);
    lightConfig.color       = glm::vec3(1.0f, 0.0f, 0.0f);
    lightConfig.brightness  = 10.0f;
    lightConfig.type        = Lazarus::LightManager::LightType::POINT;
    lightBuilder->createLightSource(light1, lightConfig);

    lightConfig = {};
    lightConfig.position    = glm::vec3(-10.0f, 10.0f, -1.0f);
    lightConfig.color       = glm::vec3(1.0f, 1.0f, 1.0f);
    lightConfig.brightness  = 0.7f;
    lightBuilder->createLightSource(light2, lightConfig);

    Lazarus::CameraManager::CameraConfig cameraConfig = {};
    cameraConfig.clippingDistance = 500.0f;
    cameraBuilder->createPerspectiveCam(camera, cameraConfig);

    this->loadAssets();
    this->layoutScene();
    this->setupAudio();
    this->loadText();
};

void Game::loadAssets()
{
    auto start = std::chrono::system_clock::now();

    worldBuilder->createSkyBox(skyBox, "assets/images/skybox/pos_x.png", "assets/images/skybox/neg_x.png", "assets/images/skybox/neg_y.png", "assets/images/skybox/pos_y.png", "assets/images/skybox/pos_z.png", "assets/images/skybox/neg_z.png");
    worldBuilder->createFog(fog, 8.0, 90.0f, 0.3f, glm::vec3(0.5f, 0.5f, 0.5f));

    shader.setActiveShader(default_shader);

    std::vector<std::filesystem::directory_entry> files = {};
    assets = {};

    std::filesystem::path assetDirectory = std::filesystem::current_path().append("assets");
    std::filesystem::path materialDirectory = assetDirectory.append("material");
    std::filesystem::path meshDirectory = assetDirectory.replace_filename("mesh/");
    for(auto &filename : std::filesystem::directory_iterator(meshDirectory.generic_string()))
    {
        files.push_back(filename);
    };

    for(size_t i = 0; i < files.size(); i++)
    {
        std::filesystem::path p = files[i].path();
        
        Lazarus::ModelManager::Model m = {};
        Lazarus::ModelManager::AssetConfig assetConfig = {};

        std::string meshes = meshDirectory.string();
        std::string materials = materialDirectory.string();

        assetConfig.name = p.filename().replace_extension("").string();
        
        if(assetConfig.name == "river" || assetConfig.name == "shapes") continue;
        
        assetConfig.meshPath = meshes.append(p.filename().string());
        std::string ext = p.extension().string();
        if (strcmp(ext.c_str(), ".obj") == 0)
        {
            assetConfig.materialPath = materials.append("/" + p.filename().replace_extension("mtl").string());
        };
        meshBuilder->create3DAsset(m, assetConfig);
        assets.insert(std::pair<std::string, Lazarus::ModelManager::Model>(assetConfig.name, m));
    };

    Lazarus::ModelManager::AssetConfig assetConfig = {};
    assetConfig.name = "river";
    assetConfig.meshPath = "assets/mesh/river.glb";
    waterBuilder->create3DAsset(river, assetConfig);

    assetConfig = {};
    assetConfig.name = "shapes";
    assetConfig.meshPath = "assets/mesh/shapes.glb";
    assetConfig.instanceCount = 3;
    assetConfig.selectable = true;
    meshBuilder->create3DAsset(shapes, assetConfig);

    earth       = &assets["earth"];
    sword       = &assets["sword"];
    skull       = &assets["skull"];
    metaball    = &assets["metaball"];
    warrior     = &assets["warrior"];

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_ms = (end - start);
    std::cout << "\n" << std::endl;
    std::cout << "Loaded Assets: " << elapsed_ms.count() << "ms" << std::endl;
};

void Game::loadText()
{
    layout = {};

    shader.setActiveShader(default_shader);
    textManager->initialise();
    
    textManager->extendFontStack(this->ubuntuFont, "assets/fonts/times.ttf", 50);
    textManager->extendFontStack(this->morpheusFont, "assets/fonts/MORPHEUS.TTF", 80);

    std::vector<std::string> targetStrings = {
        "Lazarus Engine", "Camera-X: ", "Camera-Y: ", "Camera-Z: ", "FPS: ", "Select-ID: "
    };

    for(uint8_t i = 0; i < targetStrings.size(); i ++)
    {
        Lazarus::TextManager::Text text = {};
        Lazarus::TextManager::TextConfig textConfig = {};
        
        textConfig.targetString = targetStrings[i];
        textConfig.fontIndex = this->ubuntuFont;
        textConfig.location = glm::vec2(20.0f, (i * 50.0f));
        textConfig.color = glm::vec3(1.0f, 1.0f, 1.0f);
        textConfig.letterSpacing = 5;

        textManager->createText(text, textConfig);
        layout.push_back(text);
    };

    title           = &layout[0];
    status_x        = &layout[1];
    status_y        = &layout[2];
    status_z        = &layout[3];
    frame_counter   = &layout[4];
    selection       = &layout[5];

    title->config.fontIndex = this->morpheusFont;
    title->config.color = glm::vec3(0.6f, 0.0f, 0.0f);
    title->config.location = glm::vec2(((globals.getDisplayWidth() / 2) - 380), (globals.getDisplayHeight() - 80));
    title->config.letterSpacing = 10;
};

void Game::layoutScene()
{
    transformer.translateCamera(camera, 0.0f, 20.0f, 0.0f);

    transformer.translateModel(*sword, 0.0f, 11.0f, 3.0f);
    transformer.translateModel(*earth, 0.0f, 13.0f, 0.0f);
    transformer.translateModel(*skull, 0.0f, 10.0f, 0.0f);
    transformer.translateModel(*metaball, 20.0f, 30.0f, 0.0f);
    transformer.translateModel(*warrior, 0.0f, 11.5f, -10.0f);
    transformer.translateModel(shapes, 10.0f, 0.0f, 0.0f, 1);
    transformer.translateModel(shapes, -10.0f, 0.0f, 0.0f, 2);

    transformer.scaleModel(*metaball, 6.0f, 6.0f, 6.0f);
};

void Game::setupAudio()
{
    Lazarus::AudioManager::AudioConfig audioConfig = {};
    audioConfig.filepath = "assets/sound/springWaltz.mp3";
    audioConfig.is3D = true;
    audioConfig.loopCount = 0;

    soundManager->createAudio(springWaltz, audioConfig);
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

        for(size_t i = 0; i < window->eventQueue.size(); i++)
        {
            event = window->eventQueue[i];
            this->keyCapture(event.key);
        };

        shader.setActiveShader(default_shader);

        /*Light*/
        lightBuilder->loadLightSource(light1);
        lightBuilder->loadLightSource(light2);

		/*Camera*/
        cameraBuilder->loadCamera(camera);
        transformer.rotateCamera(camera, turnX, turnY, 0.0f);
        transformer.translateCamera(camera, moveX, 0.0f, moveZ);
        soundManager->updateListenerLocation(camera.position);
        
        /*sky*/
        fog.viewpoint = camera.position;
        worldBuilder->loadFog(fog);
        worldBuilder->drawSkyBox(skyBox, camera);

        for(auto &i : assets)
        {
            Lazarus::ModelManager::Model m = i.second;

            meshBuilder->loadModel(m);
            meshBuilder->drawModel(m);
        };

        meshBuilder->loadModel(shapes);
        meshBuilder->drawModel(shapes);

        shader.setActiveShader(caustics_shader);
        shader.uploadUniform("time", &window->elapsedTime);
        
        cameraBuilder->loadCamera(camera);
        worldBuilder->loadFog(fog, caustics_shader);

        /*river*/
        waterBuilder->loadModel(river);
        waterBuilder->drawModel(river);
        
        shader.setActiveShader(default_shader);
        transformer.translateModel(*sword, (0.5f / 10), 0.0f, 0.0f);
        transformer.rotateModel(*sword, 0.0f, 1.0f, 0.0f);
        // transformer.translateModel(*warrior, 0.0f, 0.0f, (0.5f / 10));
        transformer.rotateModel(*earth, 0.0f, -0.7f, 0.0f);
        transformer.scaleModel(*skull, scale, scale, scale);
        
        transformer.rotateModel(shapes, 0.0f, 1.0f, 0.0f, 1);
        
        /*text*/
        uint8_t occupant = 0;
        int32_t mouseX = 0;
        int32_t mouseY = 0;
        
        window->getLatestMouseMove(mouseX, mouseY);
        cameraBuilder->getPixelOccupant(mouseX, mouseY, occupant);
        
        status_x->config.targetString       = std::string("Camera-X: ").append(std::to_string(camera.direction.x));
        status_y->config.targetString       = std::string("Camera-Y: ").append(std::to_string(camera.direction.y));
        status_z->config.targetString       = std::string("Camera-Z: ").append(std::to_string(camera.direction.z));
        frame_counter->config.targetString  = std::string("FPS: ").append(std::to_string(static_cast<int>(window->framesPerSecond)));
        selection->config.targetString      = std::string("Select ID: ").append(std::to_string(occupant));

        // for(size_t i = 0; i < layout.size(); i++)
        // {
            // Lazarus::TextManager::Text t = layout[i];

            // textManager->loadText(t);
            // textManager->drawText(t);
        // };
        Lazarus::TextManager::Text t = layout[4];

        textManager->loadText(t);
        textManager->drawText(t);

        window->presentNextFrame();
    };
};

void Game::keyCapture(int32_t key)
{
    std::cout << key << std::endl;
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
            turnX = -1.0f;
            break;
        //  A / Look left
        case 65:
            turnY = -2.0f;
            break;
        //  S / Look down
        case 83:
            turnX = 2.0f;
            break;
        //  D / Look right
        case 68:
            turnY = 2.0f;
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
        case 49:
            meshBuilder->setActiveAnimation(*warrior, 0);
            break;
        case 50:
            meshBuilder->setActiveAnimation(*warrior, 1);
            break;
        case 51:
            meshBuilder->setToPosePosition(*warrior);
            break;
        //  Reset
        default:
            moveX = 0.0f;
			moveZ = 0.0f;
            turnX = 0.0f;
            turnY = 0.0f;
            break;
    }

    previousKey = key;
};