#include <lazarus.h>
#include <string>
#include <memory>
#include <chrono>

using std::unique_ptr;
using std::shared_ptr;

class Game
{
    public:
        Game();
        void init();
        void loadAssets();
        void layoutScene();
        void loadText();
        void start();

    public:
        void setupAudio();
        void keyCapture(uint32_t key);

        uint32_t default_shader;
        uint32_t caustics_shader;

        int errorCode;
        int engineStatus;

        float moveX, moveZ;
        float turnX, turnY;
        float scale;
        
        const char** errorMessage;

        Lazarus::GlobalsManager globals;
        Lazarus::Shader shader;
        Lazarus::Transform transformer;

        unique_ptr<Lazarus::WindowManager> window;
        unique_ptr<Lazarus::AudioManager> soundManager;
        unique_ptr<Lazarus::TextManager> textManager;
        unique_ptr<Lazarus::CameraManager> cameraBuilder;
        unique_ptr<Lazarus::LightManager> lightBuilder;

        unique_ptr<Lazarus::WorldFX> worldBuilder;
        unique_ptr<Lazarus::MeshManager> meshBuilder;
        unique_ptr<Lazarus::MeshManager> waterBuilder;
        
        Lazarus::WorldFX::Skybox skyBox;
        Lazarus::WorldFX::Fog fog;

        Lazarus::AudioManager::Audio springWaltz;

        Lazarus::CameraManager::Camera camera;
        Lazarus::LightManager::Light light1;
        Lazarus::LightManager::Light light2;

        Lazarus::MeshManager::Mesh skull;
        Lazarus::MeshManager::Mesh terrain;
        Lazarus::MeshManager::Mesh river;
        Lazarus::MeshManager::Mesh sword;
        Lazarus::MeshManager::Mesh earth;
        Lazarus::MeshManager::Mesh metaball;

        std::vector<Lazarus::MeshManager::Mesh> assets;

        uint32_t morpheusFont;
        uint32_t ubuntuFont;

        Lazarus::TextManager::Text word1;
        Lazarus::TextManager::Text word2;
        Lazarus::TextManager::Text word3;
        Lazarus::TextManager::Text word4;
        Lazarus::TextManager::Text word5;
        Lazarus::TextManager::Text word6;

        uint32_t previousKey;
};