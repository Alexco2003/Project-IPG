#include "lab/tema/tema.h"

#include <vector>
#include <string>
#include <iostream>
#include <stb/stb_image.h>
using namespace gfxc;

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema::Tema()
{
}


Tema::~Tema()
{
}


void Tema::Init()
{

    // Create a simple quad
    {
        vector<glm::vec2> textureCoords
        {
            // TODO(student): Specify the texture coordinates for the square
            glm::vec2(1.0f, 0.0f),  // top right
            glm::vec2(1.0f, 1.0f),  // bottom right
            glm::vec2(0.0f, 1.0f),  // bottom left
            glm::vec2(0.0f, 0.0f)   // top left
        };

        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(0.5f,   0.5f, 0.0f), glm::vec3(0), glm::vec3(0), textureCoords[0]),
            VertexFormat(glm::vec3(0.5f,   -0.5f, 0.0f), glm::vec3(0), glm::vec3(0), textureCoords[1]),
            VertexFormat(glm::vec3(-0.5f,   -0.5f, 0.0f), glm::vec3(0), glm::vec3(0), textureCoords[2]),
            VertexFormat(glm::vec3(-0.5f,   0.5f, 0.0f), glm::vec3(0), glm::vec3(0), textureCoords[3]),
        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        Mesh* mesh = new Mesh("square");
        mesh->InitFromData(vertices, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    CreateCarMeshes();

    // TODO(student): Load other shaders
    LoadShader("TemaShader");

}

Texture2D* Tema::LoadTexture(const char* imagePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);

    return CreateTexture(width, height, channels, imageData);
}

Texture2D* Tema::CreateTexture(unsigned int width, unsigned int height,
    unsigned int channels, unsigned char* data)
{
    GLuint textureID = 0;
    unsigned int size = width * height * channels;

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER and MAG_FILTER) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();


    // TODO(student): Use the "glTexImage2D" directive to load the information
    // into the graphics processor's memory. Use the correct format based on
    // the number of components:
    //   - 1 color channel - GL_RED
    //   - 2 color channels - GL_RG
    //   - 3 color channels - GL_RGB
    //   - 4 color channels - GL_RGBA
    if (channels == 1)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    else if (channels == 2)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
    else if (channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

Texture2D* Tema::CreateStripedTexture()
{
    unsigned int width = 1024;
    unsigned int height = 256;
    unsigned int channels = 3;

    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];
    // TODO(student): Generate the information for a striped image,
    // where all the pixels on the same line have the same color.
    // The color of the pixels of a line is chosen randomly.
    for (unsigned int y = 0; y < height; y++) {
        unsigned char r = rand() % 256;
        unsigned char g = rand() % 256;
        unsigned char b = rand() % 256;
        for (unsigned int x = 0; x < width; x++) {
            unsigned int index = (y * width + x) * channels;
            data[index] = r;
            data[index + 1] = g;
            data[index + 2] = b;
        }
    }

    return CreateTexture(width, height, channels, data);
}

void Tema::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema::Update(float deltaTimeSeconds)
{

	// car forward/backward movement
    float forward = (inputForward ? 1.4f : 1.0f);
    if (inputBack) 
        forward = 0.5f;

    // always move forward (world feels like it is coming toward you)
    carPosition.z -= forward * forwardSpeed * deltaTimeSeconds;

    // left/right steering
    if (inputLeft) carPosition.x -= lateralSpeed * deltaTimeSeconds;
    if (inputRight) carPosition.x += lateralSpeed * deltaTimeSeconds;
    // clamp lateral
    carPosition.x = glm::clamp(carPosition.x, -maxLateral, maxLateral);

    // wheel rotation based on forward travel
    float wheelCircumference = 2.0f * 3.14159265f * wheelRadius;
    float distanceTraveled = forward * forwardSpeed * deltaTimeSeconds;
    wheelAngle += distanceTraveled / wheelCircumference * 2.0f * 3.14159265f; // radians

	// camera follow logic
    Camera* cam = GetSceneCamera();
    if (cam) {
 
        float dist = 8.0f;
        float height = 4.5f;

        glm::vec3 forwardDir = glm::vec3(sin(carYaw), 0.0f, -cos(carYaw));
        glm::vec3 relativeCameraPosition = -forwardDir * dist + glm::vec3(0.0f, height, 0.0f);

        glm::vec3 finalCamPos = carPosition + relativeCameraPosition;
        cam->m_transform->SetWorldPosition(finalCamPos);

        glm::quat rotation = glm::quatLookAt(
            glm::normalize(carPosition - finalCamPos), 
            glm::vec3(0, 1, 0)                         
        );


        glm::vec3 eulerRot = glm::degrees(glm::eulerAngles(rotation));
        cam->m_transform->SetWorldRotation(eulerRot);
        cam->Update();
    }

    // render the car
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glm::mat4 projMatrix = GetSceneCamera()->GetProjectionMatrix();
    RenderCar(viewMatrix, projMatrix);
}


void Tema::FrameEnd()
{
    DrawCoordinateSystem();
}


void Tema::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need

    int location = glGetUniformLocation(shader->program, "time");
    glUniform1f(location, Engine::GetElapsedTime());

    Camera* cam = GetSceneCamera();
    if (cam) {
        glm::vec3 eye = cam->m_transform->GetWorldPosition();
        GLint locEye = glGetUniformLocation(shader->program, "eyePos");
        if (locEye >= 0)
            glUniform3f(locEye, eye.x, eye.y, eye.z);
    }


    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);


    }

    if (texture2)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);

    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema::LoadShader(const std::string& name)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders");

    // Create a shader program
    {
        Shader* shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".FS.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_A) inputLeft = true;
    if (key == GLFW_KEY_D) inputRight = true;
    if (key == GLFW_KEY_W) inputForward = true;
    if (key == GLFW_KEY_S) inputBack = true;
}


void Tema::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if (key == GLFW_KEY_A) inputLeft = false;
    if (key == GLFW_KEY_D) inputRight = false;
    if (key == GLFW_KEY_W) inputForward = false;
    if (key == GLFW_KEY_S) inputBack = false;
}


void Tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema::OnWindowResize(int width, int height)
{
}

void Tema::CreateBoxMesh(const std::string& meshName, float sx, float sy, float sz, const glm::vec3& color)
{
    // 6 faces, 2 triangles per face -> 36 vertices
    std::vector<VertexFormat> vertices;
    vertices.reserve(36);

    // helper lambda to push a triangle (3 positions + normal)
    auto pushFace = [&](const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const glm::vec3& normal) {
        // triangle 1: a, b, c
        vertices.push_back(VertexFormat(a, color, normal, glm::vec2(0.0f, 0.0f)));
        vertices.push_back(VertexFormat(b, color, normal, glm::vec2(1.0f, 0.0f)));
        vertices.push_back(VertexFormat(c, color, normal, glm::vec2(0.0f, 1.0f)));
        // triangle 2: a, c, d
        vertices.push_back(VertexFormat(a, color, normal, glm::vec2(1.0f, 0.0f)));
        vertices.push_back(VertexFormat(c, color, normal, glm::vec2(1.0f, 1.0f)));
        vertices.push_back(VertexFormat(d, color, normal, glm::vec2(0.0f, 1.0f)));
        };

    // half extents
    float hx = sx * 0.5f;
    float hy = sy * 0.5f;
    float hz = sz * 0.5f;

    glm::vec3 p000(-hx, -hy, -hz);
    glm::vec3 p001(-hx, -hy, hz);
    glm::vec3 p010(-hx, hy, -hz);
    glm::vec3 p011(-hx, hy, hz);
    glm::vec3 p100(hx, -hy, -hz);
    glm::vec3 p101(hx, -hy, hz);
    glm::vec3 p110(hx, hy, -hz);
    glm::vec3 p111(hx, hy, hz);

    // +X face (x = +hx)  - order CCW when viewed from +X
    pushFace(p110, p111, p101, p100, glm::vec3(1.0f, 0.0f, 0.0f));
    // -X face (x = -hx) - order CCW when viewed from -X
    pushFace(p011, p010, p000, p001, glm::vec3(-1.0f, 0.0f, 0.0f));
    // +Y face (y = +hy) - top
    pushFace(p011, p111, p110, p010, glm::vec3(0.0f, 1.0f, 0.0f));
    // -Y face (y = -hy) - bottom
    pushFace(p000, p100, p101, p001, glm::vec3(0.0f, -1.0f, 0.0f));
    // +Z face (z = +hz) - front
    pushFace(p001, p101, p111, p011, glm::vec3(0.0f, 0.0f, 1.0f));
    // -Z face (z = -hz) - back
    pushFace(p100, p000, p010, p110, glm::vec3(0.0f, 0.0f, -1.0f));

    // indices are sequential
    vector<unsigned int> indices(vertices.size());
    for (unsigned int i = 0; i < indices.size(); ++i) 
        indices[i] = i;

    Mesh* mesh = new Mesh(meshName);
    mesh->InitFromData(vertices, indices);
    meshes[mesh->GetMeshID()] = mesh;
}

// create car parts meshes
void Tema::CreateCarMeshes()
{
    // car body: wide rectangle
    CreateBoxMesh("car_body", 1.6f, 0.5f, 3.0f, glm::vec3(0.8f, 0.1f, 0.1f));
    // cabin / roof
    CreateBoxMesh("car_roof", 1.0f, 0.4f, 1.4f, glm::vec3(0.9f, 0.3f, 0.3f));
    // wheels (flattened boxes)
    CreateBoxMesh("car_wheel", wheelThickness, wheelRadius * 2.0f, wheelRadius * 2.0f, glm::vec3(0.05f, 0.05f, 0.05f));
}

// render the assembled car 
void Tema::RenderCar(const glm::mat4& view, const glm::mat4& proj)
{
    Shader* shader = shaders["TemaShader"];
    if (!shader) return;

    // ensure program is active so uniform locations are valid
    glUseProgram(shader->program);
    GLint locUseColor = glGetUniformLocation(shader->program, "u_UseObjectColor");
    GLint locColor = glGetUniformLocation(shader->program, "u_ObjectColor");
    GLint locUseEmission = glGetUniformLocation(shader->program, "u_UseEmission");
    GLint locEmissionColor = glGetUniformLocation(shader->program, "u_EmissionColor");
    GLint locEmissionIntensity = glGetUniformLocation(shader->program, "u_EmissionIntensity");
    GLint locEmissionSpeed = glGetUniformLocation(shader->program, "u_EmissionSpeed");

    // base model (car position & orientation)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, carPosition);
    model = glm::rotate(model, carYaw, glm::vec3(0, 1, 0));

    // main body
    glm::mat4 bodyModel = glm::translate(model, glm::vec3(0.0f, 0.25f, 0.0f));
    if (locUseColor >= 0) 
        glUniform1i(locUseColor, 1);
    if (locColor >= 0) 
        glUniform3f(locColor, 1.0f, 0.0f, 0.0f);
    // ensure emission disabled for body
    if (locUseEmission >= 0) 
        glUniform1i(locUseEmission, 0);
    RenderSimpleMesh(meshes["car_body"], shader, bodyModel);

    // roof
    glm::mat4 roofModel = glm::translate(model, glm::vec3(0.0f, 0.6f, -0.25f));
    if (locColor >= 0) 
        glUniform3f(locColor, 0.0f, 0.0f, 1.0f);
    RenderSimpleMesh(meshes["car_roof"], shader, roofModel);

    // wheels (enable pulsing emission)
    float halfWidth = 0.8f;
    float halfZ = 1.0f;
    glm::vec3 wheelOffsets[4] = {
        glm::vec3(halfWidth, 0.12f,  halfZ),
        glm::vec3(-halfWidth, 0.12f,  halfZ),
        glm::vec3(halfWidth, 0.12f, -halfZ),
        glm::vec3(-halfWidth, 0.12f, -halfZ)
    };

    // configure emission
    if (locUseEmission >= 0) 
        glUniform1i(locUseEmission, 1);
    if (locEmissionColor >= 0) 
        glUniform3f(locEmissionColor, 1.0f, 0.85f, 0.35f); // warm pulse color
    if (locEmissionIntensity >= 0) 
        glUniform1f(locEmissionIntensity, 0.9f);      // max emission multiplier
    if (locEmissionSpeed >= 0) 
        glUniform1f(locEmissionSpeed, 6.0f);             // pulse speed

    // also set wheel base color (dark)
    if (locColor >= 0) glUniform3f(locColor, 0.03f, 0.03f, 0.03f);

    for (int i = 0; i < 4; ++i) {
        glm::mat4 wm = glm::translate(model, wheelOffsets[i]);
        if (i < 2) {
            float steer = 0.0f;
            if (inputLeft) steer = 0.25f;
            if (inputRight) steer = -0.25f;
            wm = glm::rotate(wm, steer, glm::vec3(0, 1, 0));
        }
        wm = glm::rotate(wm, wheelAngle, glm::vec3(1, 0, 0));
        RenderSimpleMesh(meshes["car_wheel"], shader, wm);
    }

    // disable emission after wheels
    if (locUseEmission >= 0) glUniform1i(locUseEmission, 0);
    if (locUseColor >= 0) glUniform1i(locUseColor, 0);
}