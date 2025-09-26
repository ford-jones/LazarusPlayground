#include "game.h"

Game::Game()
{
    sound_manager = std::make_unique<Lazarus::AudioManager>();
    globals = Lazarus::GlobalsManager();
    window = nullptr;
    text_manager = nullptr;
    spring_waltz = {};

    turnX = 0.0f;
    turnY = 0.0f;
    moveX = 0.0f;
    moveZ = 0.0f;
    scale = 1.0f;

    skybox = {};
    fog = {};

    this->morpheus_font = 0;
    this->ubuntu_font = 0;
};

void Game::init()
{
    globals.setLaunchInFullscreen(true);
    globals.setVsyncDisabled(true);
    
    window = std::make_unique<Lazarus::WindowManager>("Lazarus Engine", 1000, 800);
    
    window->createWindow();
    window->eventsInit();
    sound_manager->initialise();

    //  Create cursor prior to enforced image sanitisation
    window->createCursor(32, 32, 0, 0, "assets/images/crosshair.png");

    globals.setEnforceImageSanity(true);
    globals.setMaxImageSize(500, 500);
    
    window->loadConfig();

    shader_manager.compileShaders(default_shader);
    shader_manager.compileShaders(caustics_shader, "assets/shaders/water.glsl");

    shader_manager.setActiveShader(default_shader);

    world_builder        = std::make_unique<Lazarus::WorldFX>(default_shader);
    mesh_builder         = std::make_unique<Lazarus::MeshManager>(default_shader);
    water_builder        = std::make_unique<Lazarus::MeshManager>(caustics_shader);
    text_manager         = std::make_unique<Lazarus::TextManager>(default_shader);
    light_builder        = std::make_unique<Lazarus::LightManager>(default_shader);
    camera_builder       = std::make_unique<Lazarus::CameraManager>(default_shader);

    Lazarus::LightManager::LightConfig lightConfig = {};
    lightConfig.position    = glm::vec3(10.0f, 10.0f, 1.0f);
    lightConfig.color       = glm::vec3(1.0f, 0.0f, 0.0f);
    lightConfig.brightness  = 10.0f;
    light_builder->createLightSource(red_light, lightConfig);

    lightConfig = {};
    lightConfig.position    = glm::vec3(-10.0f, 10.0f, -1.0f);
    lightConfig.color       = glm::vec3(0.0f, 0.0f, 1.0f);
    lightConfig.brightness  = 10.0f;
    light_builder->createLightSource(blue_light, lightConfig);

    Lazarus::CameraManager::CameraConfig cameraConfig = {};
    cameraConfig.clippingDistance = 500.0f;
    camera_builder->createPerspectiveCam(camera, cameraConfig);

    this->loadAssets();
    this->setupAudio();
    this->loadText();
};

void Game::loadAssets()
{
    auto start = std::chrono::system_clock::now();

    world_builder->createSkyBox(skybox, "assets/images/skybox/pos_x.png", "assets/images/skybox/neg_x.png", "assets/images/skybox/neg_y.png", "assets/images/skybox/pos_y.png", "assets/images/skybox/pos_z.png", "assets/images/skybox/neg_z.png");
    world_builder->createFog(fog, 5.0, 90.0f, 0.3f, glm::vec3(0.5f, 0.5f, 0.5f));

    shader_manager.setActiveShader(default_shader);

    Lazarus::MeshManager::AssetConfig assetConfig = {};
    assetConfig.meshPath = "assets/mesh/earth.glb";
    mesh_builder->create3DAsset(earth, assetConfig);
    transform.translateMeshAsset(earth, 0.0f, 13.0f, 0.0f);

    assetConfig = {};
    assetConfig.meshPath = "assets/mesh/town_square.glb";
    mesh_builder->create3DAsset(terrain, assetConfig);
    
    shader_manager.setActiveShader(caustics_shader);

    assetConfig = {};
    assetConfig.meshPath = "assets/mesh/river.glb";
    water_builder->create3DAsset(river, assetConfig);

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_ms = (end - start);
    std::cout << "\n" << std::endl;
    std::cout << "Loaded Assets: " << elapsed_ms.count() << "ms" << std::endl;
};

void Game::loadText()
{
    shader_manager.setActiveShader(default_shader);
    text_manager->initialise();
    
    text_manager->extendFontStack(this->ubuntu_font, "assets/fonts/Ubuntu-R.ttf", 150);
    text_manager->extendFontStack(this->morpheus_font, "assets/fonts/MORPHEUS.TTF", 50);

    Lazarus::TextManager::TextConfig textConfig = {};
    textConfig.targetString = "Lazarus Engine";
    textConfig.fontIndex = this->morpheus_font;
    textConfig.location = glm::vec2(((globals.getDisplayWidth() / 2) - 350), (globals.getDisplayHeight() - 80));
    textConfig.color = glm::vec3(0.6f, 0.0f, 0.0f);
    textConfig.letterSpacing = 10;
    text_manager->createText(this->title, textConfig);

    textConfig = {};
    textConfig.targetString = "FPS: ";
    textConfig.fontIndex = this->ubuntu_font;
    textConfig.location = glm::vec2(50, globals.getDisplayHeight() - 80);
    textConfig.color = glm::vec3(1.0f, 1.0f, 0.8f);
    textConfig.letterSpacing = 5;
    text_manager->createText(this->frame_counter, textConfig);
};

void Game::setupAudio()
{
    Lazarus::AudioManager::AudioConfig audioConfig = {};
    audioConfig.filepath = "assets/sound/spring_waltz.mp3";
    audioConfig.is3D = true;
    audioConfig.loopCount = 0;

    sound_manager->createAudio(spring_waltz, audioConfig);
    sound_manager->loadAudio(spring_waltz);
    sound_manager->playAudio(spring_waltz); 
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

        shader_manager.setActiveShader(default_shader);

        /*Light*/
        light_builder->loadLightSource(red_light);
        light_builder->loadLightSource(blue_light);

		/*Camera*/
        camera_builder->loadCamera(camera);
        transform.rotateCameraAsset(camera, turnX, turnY, 0.0f);
        transform.translateCameraAsset(camera, moveX, 0.0f, moveZ);
        sound_manager->updateListenerLocation(camera.position);
        
        /*sky*/
        fog.viewpoint = camera.position;
        world_builder->loadFog(fog);
        world_builder->drawSkyBox(skybox, camera);

        /*earth*/
        mesh_builder->loadMesh(earth);
        mesh_builder->drawMesh(earth);

        /*terrain*/
        mesh_builder->loadMesh(terrain);
        mesh_builder->drawMesh(terrain);

        shader_manager.setActiveShader(caustics_shader);
        shader_manager.uploadUniform("time", &window->elapsedTime);
        
        camera_builder->loadCamera(camera);
        world_builder->loadFog(fog, caustics_shader);

        /*river*/
        water_builder->loadMesh(river);
        water_builder->drawMesh(river);
        
        shader_manager.setActiveShader(default_shader);
        transform.rotateMeshAsset(earth, 0.0f, -0.7f, 0.0f);
        
        /*text*/
        text_manager->loadText(this->title);
        text_manager->drawText(this->title);
        
        uint8_t occupant = 0;
        camera_builder->getPixelOccupant(window->mousePositionX, window->mousePositionY, occupant);
        frame_counter.config.targetString     = std::string("FPS: ").append(std::to_string(static_cast<int>(window->framesPerSecond)));
        
        window->presentNextFrame();
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
            if(previous_key != 70) window->toggleFullscreen();
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

    previous_key = key;
};