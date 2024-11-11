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
        void start();

    public:
        void setupAudio();
        void keyCapture(std::string key);

        int shaderProgram;
        int errorCode;
        int engineStatus;

        float moveX, moveZ;
        float turnX, turnY;
        
        const char** errorMessage;

        Lazarus::GlobalsManager globals;
        Lazarus::EventManager eventManager;
        Lazarus::Shader shader;
        Lazarus::Transform transformer;
        Lazarus::FpsCounter fpsCounter;

        unique_ptr<Lazarus::WindowManager> window;
        unique_ptr<Lazarus::SoundManager> soundManager;
        unique_ptr<Lazarus::TextManager> textManager;
        unique_ptr<Lazarus::Camera> cameraBuilder;
        unique_ptr<Lazarus::Light> lightBuilder;

        unique_ptr<Lazarus::Mesh> spiderwebBuilder;
        unique_ptr<Lazarus::Mesh> wallsBuilder;
        unique_ptr<Lazarus::Mesh> skullBuilder;
        unique_ptr<Lazarus::Mesh> floorsBuilder;
        unique_ptr<Lazarus::Mesh> swordBuilder;

        shared_ptr<Lazarus::SoundManager::Audio> springWaltz;
        shared_ptr<Lazarus::SoundManager::Audio> footstep;

        shared_ptr<Lazarus::Camera::FixedCamera> camera;
        shared_ptr<Lazarus::Light::AmbientLight> light;

        Lazarus::Mesh::TriangulatedMesh spiderweb;
        Lazarus::Mesh::TriangulatedMesh skull;
        Lazarus::Mesh::TriangulatedMesh walls;
        Lazarus::Mesh::TriangulatedMesh floors;
        Lazarus::Mesh::TriangulatedMesh sword;
};