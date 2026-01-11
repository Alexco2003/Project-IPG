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
    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\crate.jpg");
        mapTextures["crate"] = texture;
    }

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
   
    CreateMeshGrid("ground", 50, 150);
	CreateObstacleMeshes();
	InitObstacles();

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

    {
        Shader* shader = shaders["TemaShader"];
        glUseProgram(shader->program);

        glm::mat4 model = glm::mat4(1.0f);

        //float groundOffset = 100.0f;
		float groundOffset = 80.0f;

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, carPosition.z - groundOffset));


        GLint locUseColor = glGetUniformLocation(shader->program, "u_UseObjectColor");
        glUniform1i(locUseColor, 1);
        GLint locColor = glGetUniformLocation(shader->program, "u_ObjectColor");
        glUniform3f(locColor, 0.0f, 0.6f, 0.1f);


        GLint locCurv = glGetUniformLocation(shader->program, "curvatureFactor");
        glUniform1f(locCurv, 0.02f);

        GLint locPlayer = glGetUniformLocation(shader->program, "playerPos");
        glUniform3fv(locPlayer, 1, glm::value_ptr(carPosition));

        RenderSimpleMesh(meshes["ground"], shader, model);

        glUniform1i(locUseColor, 0);
    }

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
 
        //float dist = 15.0f;
        //float height = 13.5f;

        float dist = 15.0f;
        float height = 5.5f;

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

    for (int i = 0; i < obstacles.size(); i++)
    {
        
        if (obstacles[i].position.z > carPosition.z + 20.0f)
        {
     
            obstacles[i].position.z = carPosition.z - 300.0f;
            obstacles[i].position.x = (rand() % 80 / 10.0f) - 4.0f;
            // obstacles[i].type = rand() % 5;


            int type = obstacles[i].type;
            if (type == 0) obstacles[i].position.y = 0.5f;
            else if (type == 1) obstacles[i].position.y = 2.5f;
            else if (type == 2) obstacles[i].position.y = 1.0f;
            else if (type == 3) obstacles[i].position.y = 0.75f;
            else if (type == 4) obstacles[i].position.y = 0.05f;
        }
    }
    RenderObstacles();
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

    GLint loc_player = glGetUniformLocation(shader->program, "playerPos");
    if (loc_player != -1) {
        glUniform3fv(loc_player, 1, glm::value_ptr(carPosition));
    }
    GLint loc_curv = glGetUniformLocation(shader->program, "curvatureFactor");
    if (loc_curv != -1) {
        glUniform1f(loc_curv, 0.02f);
    }

    Camera* cam = GetSceneCamera();
    if (cam) {
        glm::vec3 eye = cam->m_transform->GetWorldPosition();
        GLint locEye = glGetUniformLocation(shader->program, "eyePos");
        if (locEye >= 0)
            glUniform3f(locEye, eye.x, eye.y, eye.z);
    }

    GLint locHasTexture = glGetUniformLocation(shader->program, "u_HasTexture");

    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

        glUniform1i(locHasTexture, 1);


    }
    else
    {
        glUniform1i(locHasTexture, 0);
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

void Tema::InitObstacles()
{

    CreateBoxMesh("crate", 1.0f, 1.0f, 1.0f, glm::vec3(0.6f, 0.0f, 0.6f));

    obstacles.clear();

    for (int i = 0; i < 50; i++)
    {
        Obstacle newObs;

        newObs.position.z = -30.0f - (i * 15.0f); // 15 m intre obs
        // X: random intre benzile drumului (intre -4 si 4)
        newObs.position.x = (rand() % 80 / 10.0f) - 4.0f;
        newObs.type = rand() % 5;

        if (newObs.type == 0) // cutie
            newObs.position.y = 0.5f;
        else if (newObs.type == 1) // stalp (5m -> y=2.5)
            newObs.position.y = 2.5f;
        else if (newObs.type == 2) // copac (trunchi 2m -> y=1.0)
            newObs.position.y = 1.0f;
        else if (newObs.type == 3) // bariera (picior 1.5m -> y=0.75)
            newObs.position.y = 0.75f;
        else if (newObs.type == 4) 
            newObs.position.y = 0.05f; // con

        newObs.scale = 1.0f;
        obstacles.push_back(newObs);
    }
}

void Tema::CreateObstacleMeshes()
{
    // 1. piese pentru STALP (Type 1)
    CreateBoxMesh("pole_body", 0.3f, 5.0f, 0.3f, glm::vec3(0.4f, 0.4f, 0.4f));
    CreateBoxMesh("pole_arm", 1.5f, 0.25f, 0.25f, glm::vec3(0.4f, 0.4f, 0.4f));
    CreateBoxMesh("pole_lamp", 0.4f, 0.4f, 0.4f, glm::vec3(1.0f, 1.0f, 0.6f));

    // 2. Piese pentru COPAC (Type 2)
    CreateBoxMesh("tree_trunk", 0.6f, 2.0f, 0.6f, glm::vec3(0.5f, 0.35f, 0.05f));
    CreateBoxMesh("tree_crown", 2.0f, 1.5f, 2.0f, glm::vec3(0.0f, 0.5f, 0.0f));

    // 3. piese pentru BARIERA (Type 3)
    CreateBoxMesh("barrier_post", 0.4f, 1.5f, 0.4f, glm::vec3(0.8f, 0.1f, 0.1f));
    CreateBoxMesh("barrier_bar", 4.0f, 0.3f, 0.3f, glm::vec3(0.9f, 0.9f, 0.9f));

    // 4. CON DE TRAFIC 

    CreateBoxMesh("cone_base", 1.5f, 0.2f, 1.5f, glm::vec3(1.0f, 0.5f, 0.0f));
    CreateConeMesh("cone_body_smooth", 0.6f, 2.5f, 30, glm::vec3(1.0f, 0.5f, 0.0f));
    CreateConeMesh("cone_white_layer", 0.36f, 0.8f, 30, glm::vec3(1.0f, 1.0f, 1.0f));
}

void Tema::RenderObstacles()
{
    Shader* shader = shaders["TemaShader"];
    if (!shader) return;

    glUseProgram(shader->program);

    glUniform3fv(glGetUniformLocation(shader->program, "playerPos"), 1, glm::value_ptr(carPosition));
    glUniform1f(glGetUniformLocation(shader->program, "curvatureFactor"), 0.002f);
    glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 1);

    for (const auto& obs : obstacles)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obs.position);

        if (obs.type == 0)
        {
            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.6f, 0.2f, 0.6f);
            RenderSimpleMesh(meshes["crate"], shader, model, mapTextures["crate"]);
        }
        else if (obs.type == 1)
        {
  
            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.3f, 0.3f, 0.3f);
            RenderSimpleMesh(meshes["pole_body"], shader, model);


            glm::mat4 armModel = glm::translate(model, glm::vec3(0.6f, 2.2f, 0.0f));
            RenderSimpleMesh(meshes["pole_arm"], shader, armModel);

            glm::mat4 lampModel = glm::translate(model, glm::vec3(1.2f, 2.0f, 0.0f));
            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 1.0f, 1.0f, 0.8f);
            RenderSimpleMesh(meshes["pole_lamp"], shader, lampModel);
        }
        else if (obs.type == 2)
        {

            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.45f, 0.35f, 0.2f);
            RenderSimpleMesh(meshes["tree_trunk"], shader, model);

 
            glm::mat4 crown1 = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.0f, 0.4f, 0.0f);
            RenderSimpleMesh(meshes["tree_crown"], shader, crown1);

            glm::mat4 crown2 = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
            crown2 = glm::scale(crown2, glm::vec3(0.7f, 0.8f, 0.7f));
            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.2f, 0.6f, 0.2f);
            RenderSimpleMesh(meshes["tree_crown"], shader, crown2);
        }
        else if (obs.type == 3)
        {

            glm::mat4 leftPost = glm::translate(model, glm::vec3(-1.8f, 0.0f, 0.0f));
            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.7f, 0.1f, 0.1f);
            RenderSimpleMesh(meshes["barrier_post"], shader, leftPost);

            glm::mat4 rightPost = glm::translate(model, glm::vec3(1.8f, 0.0f, 0.0f));
            RenderSimpleMesh(meshes["barrier_post"], shader, rightPost);

            glm::mat4 bar = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.9f, 0.9f, 0.9f);
            RenderSimpleMesh(meshes["barrier_bar"], shader, bar);
        }
        else if (obs.type == 4) 
        {
         
    /*        glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 1.0f, 0.5f, 0.0f);
            RenderSimpleMesh(meshes["cone_base"], shader, model);*/

            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 1.0f, 0.5f, 0.0f);
            RenderSimpleMesh(meshes["cone_base"], shader, model);

            glm::mat4 bodyModel = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));
            RenderSimpleMesh(meshes["cone_body_smooth"], shader, bodyModel);

         /*   glm::mat4 bodyModel = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));
            RenderSimpleMesh(meshes["cone_body_smooth"], shader, bodyModel);*/

        }
    }

    glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 0);
}

void Tema::CreateMeshGrid(const char* name, int cols, int rows)
{
    float width = 50.0f;
    float length = 200.0f;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            float u = (float)c / (cols - 1);
            float v = (float)r / (rows - 1);

            float x = (u - 0.5f) * width;
            float z = (v - 0.5f) * length;
            float y = 0.0f;

            glm::vec3 color = glm::vec3(0.2f, 0.7f, 0.3f);
            glm::vec3 normal = glm::vec3(0, 1, 0);
            glm::vec2 texCoord = glm::vec2(u, v);

            vertices.push_back(VertexFormat(glm::vec3(x, y, z), color, normal, texCoord));
        }
    }

    for (int r = 0; r < rows - 1; r++)
    {
        for (int c = 0; c < cols - 1; c++)
        {
            int topLeft = r * cols + c;
            int topRight = topLeft + 1;
            int bottomLeft = (r + 1) * cols + c;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    meshes[mesh->GetMeshID()] = mesh;
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
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 p0 = glm::vec3(-sx / 2, -sy / 2, sz / 2);
    glm::vec3 p1 = glm::vec3(sx / 2, -sy / 2, sz / 2);
    glm::vec3 p2 = glm::vec3(-sx / 2, sy / 2, sz / 2);
    glm::vec3 p3 = glm::vec3(sx / 2, sy / 2, sz / 2);
    glm::vec3 p4 = glm::vec3(-sx / 2, -sy / 2, -sz / 2);
    glm::vec3 p5 = glm::vec3(sx / 2, -sy / 2, -sz / 2);
    glm::vec3 p6 = glm::vec3(-sx / 2, sy / 2, -sz / 2);
    glm::vec3 p7 = glm::vec3(sx / 2, sy / 2, -sz / 2);

    glm::vec3 n;

    auto addFace = [&](glm::vec3 bl, glm::vec3 br, glm::vec3 tr, glm::vec3 tl, glm::vec3 normal) {
        int i = vertices.size();
        vertices.emplace_back(bl, color, normal, glm::vec2(0.0f, 0.0f)); // Stanga-Jos
        vertices.emplace_back(br, color, normal, glm::vec2(1.0f, 0.0f)); // Dreapta-Jos
        vertices.emplace_back(tr, color, normal, glm::vec2(1.0f, 1.0f)); // Dreapta-Sus
        vertices.emplace_back(tl, color, normal, glm::vec2(0.0f, 1.0f)); // Stanga-Sus

        indices.push_back(i); indices.push_back(i + 1); indices.push_back(i + 2);
        indices.push_back(i); indices.push_back(i + 2); indices.push_back(i + 3);
        };


    addFace(p0, p1, p3, p2, glm::vec3(0, 0, 1));  // Fata
    addFace(p5, p4, p6, p7, glm::vec3(0, 0, -1)); // Spate
    addFace(p1, p5, p7, p3, glm::vec3(1, 0, 0));  // Dreapta
    addFace(p4, p0, p2, p6, glm::vec3(-1, 0, 0)); // Stanga
    addFace(p2, p3, p7, p6, glm::vec3(0, 1, 0));  // Sus
    addFace(p4, p5, p1, p0, glm::vec3(0, -1, 0)); // Jos

    Mesh* mesh = new Mesh(meshName);
    mesh->InitFromData(vertices, indices);
    meshes[mesh->GetMeshID()] = mesh;
}

void Tema::CreateConeMesh(const std::string& name, float radius, float height, int slices, glm::vec3 color)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    vertices.emplace_back(glm::vec3(0, height, 0), color, glm::vec3(0, 1, 0), glm::vec2(0.5f, 1.0f));

    for (int i = 0; i < slices; i++)
    {
        float angle = 2.0f * 3.14159f * i / slices;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, 0, z), color, glm::vec3(x, 0, z), glm::vec2(0.5f, 0.0f));
    }

    vertices.emplace_back(glm::vec3(0, 0, 0), color, glm::vec3(0, -1, 0), glm::vec2(0.5f, 0.5f));
    int centerIndex = slices + 1;

    for (int i = 0; i < slices; i++)
    {
        int next = (i + 1) % slices;

        int currentVertex = i + 1;
        int nextVertex = next + 1;

        indices.push_back(0);
        indices.push_back(currentVertex);
        indices.push_back(nextVertex);

        indices.push_back(centerIndex);
        indices.push_back(nextVertex);
        indices.push_back(currentVertex);
    }

    Mesh* mesh = new Mesh(name);
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

    float backOffset = 7.5f;
    glm::vec3 forwardDir = glm::vec3(sin(carYaw), 0.0f, -cos(carYaw));
    glm::vec3 visualPosition = carPosition - (forwardDir * backOffset);

    //// base model (car position & orientation)
    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, carPosition);
    //model = glm::rotate(model, carYaw, glm::vec3(0, 1, 0));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, visualPosition);
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
        if (i < 4) {
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