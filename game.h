#include <lazarus.h>
#include <string>
#include <memory>

using std::unique_ptr;
using std::shared_ptr;

class Game
{
    public:
        Game();
        void init();
        void loadScene();
        void layoutScene();
        void loadText();
        void start();

    public:
        void setupAudio();
        void keyCapture(std::string key);

        int shaderProgram;
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
        
        Lazarus::WorldFX::SkyBox skyBox;
        Lazarus::WorldFX::Fog fog;

        Lazarus::AudioManager::Audio springWaltz;

        Lazarus::CameraManager::Camera camera;
        Lazarus::LightManager::Light light1;
        Lazarus::LightManager::Light light2;

        Lazarus::MeshManager::Mesh skull;
        Lazarus::MeshManager::Mesh walls;
        Lazarus::MeshManager::Mesh floors;
        Lazarus::MeshManager::Mesh spiderweb;
        Lazarus::MeshManager::Mesh sword;


        Lazarus::TextManager::Text word1;
        Lazarus::TextManager::Text word2;
        Lazarus::TextManager::Text word3;
        Lazarus::TextManager::Text word4;
        Lazarus::TextManager::Text word5;
};