#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"


namespace lab
{
    class Tema : public gfxc::SimpleScene
    {
    public:
        Tema();
        ~Tema();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        Texture2D* CreateTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);
        Texture2D* CreateStripedTexture();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        Texture2D* LoadTexture(const char* imagePath);
        void LoadShader(const std::string& name);

        std::unordered_map<std::string, Texture2D*> mapTextures;

		// car related methods
        void CreateBoxMesh(const std::string& meshName, float sx, float sy, float sz, const glm::vec3& color);
        void CreateCarMeshes();
        void RenderCar(const glm::mat4& view, const glm::mat4& proj);

		// ground mesh
        void CreateMeshGrid(const char* name, int cols, int rows);

        // car state
        glm::vec3 carPosition = glm::vec3(0.0f, 0.5f, 0.0f);
        float carYaw = 0.0f;         // rotation around Y (radians)
        float wheelAngle = 0.0f;     // rotation for wheels
        float forwardSpeed = 6.0f;   // units per second
        float lateralSpeed = 4.0f;
        float maxLateral = 4.0f;

        bool inputLeft = false;
        bool inputRight = false;
        bool inputForward = false;
        bool inputBack = false;

        float wheelRadius = 0.3f;
        float wheelThickness = 0.18f;
    };
}   // namespace lab
