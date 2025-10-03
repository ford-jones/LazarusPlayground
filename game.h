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
        void loadText();
        void start();

    private:
        void setupAudio();
        void keyCapture(uint32_t key);

        uint32_t default_shader;
        uint32_t caustics_shader;

        lazarus_result engine_status;

        float moveX, moveZ;
        float turnX, turnY;
        float scale;

        Lazarus::GlobalsManager globals;
        Lazarus::Shader shader_manager;
        Lazarus::Transform transform;

        unique_ptr<Lazarus::WindowManager> window;
        unique_ptr<Lazarus::AudioManager> sound_manager;
        unique_ptr<Lazarus::TextManager> text_manager;
        unique_ptr<Lazarus::CameraManager> camera_builder;
        unique_ptr<Lazarus::LightManager> light_builder;

        unique_ptr<Lazarus::WorldFX> world_builder;
        unique_ptr<Lazarus::MeshManager> mesh_builder;
        unique_ptr<Lazarus::MeshManager> water_builder;
        
        Lazarus::WorldFX::Skybox skybox;
        Lazarus::WorldFX::Fog fog;

        Lazarus::AudioManager::Audio spring_waltz;

        Lazarus::CameraManager::Camera camera;
        Lazarus::LightManager::Light red_light;
        Lazarus::LightManager::Light blue_light;

        Lazarus::MeshManager::Mesh terrain;
        Lazarus::MeshManager::Mesh river;
        Lazarus::MeshManager::Mesh earth;

        uint32_t morpheus_font;
        uint32_t ubuntu_font;

        Lazarus::TextManager::Text title;
        Lazarus::TextManager::Text frame_counter;

        uint32_t previous_key;
};