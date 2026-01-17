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
        void keyCapture(int32_t key);

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

        Lazarus::WindowManager::Event event;

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

        Lazarus::MeshManager::Mesh *skull;
        // Lazarus::MeshManager::Mesh terrain;
        Lazarus::MeshManager::Mesh river;
        Lazarus::MeshManager::Mesh *sword;
        Lazarus::MeshManager::Mesh *earth;
        Lazarus::MeshManager::Mesh *metaball;

        std::map<std::string, Lazarus::MeshManager::Mesh> assets = {};
        std::vector<Lazarus::TextManager::Text> layout = {};
        uint32_t morpheusFont;
        uint32_t ubuntuFont;

        Lazarus::TextManager::Text *title;
        Lazarus::TextManager::Text *status_x;
        Lazarus::TextManager::Text *status_y;
        Lazarus::TextManager::Text *status_z;
        Lazarus::TextManager::Text *frame_counter;
        Lazarus::TextManager::Text *selection;

        uint32_t previousKey;
};