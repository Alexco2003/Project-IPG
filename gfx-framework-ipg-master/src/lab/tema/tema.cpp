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

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\horse.jpg");
		mapTextures["horse"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\sapca.jpg");
        mapTextures["sapca"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\camasa.jpg");
        mapTextures["camasa"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\blugi.jpg");
        mapTextures["blugi"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\sack.jpg");
        mapTextures["face"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\lamp.jpg");
        mapTextures["lamp"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\door.jpg");
        mapTextures["door"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\brickLight.jpg");
        mapTextures["brickLight"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\window.jpg");
        mapTextures["window"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\brick.jpeg");
        mapTextures["brick"] = texture;
    }


    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\trunchiTexture.jpg");
        mapTextures["trunchiTexture"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\crateX.jpg");
        mapTextures["crateX"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\planeTexture.jpg");
        mapTextures["planeTexture"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\grass.jpg");
        mapTextures["grass"] = texture;
    }


    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\frunze.jpg");
        mapTextures["frunze"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\apple.jpg");
        mapTextures["apple"] = texture;
    }

    {

        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\dirt.jpg");
        mapTextures["dirt"] = texture;
    }

    {

        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\wheel.jpg");
        mapTextures["wheel"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\truckBody.jpeg");
        mapTextures["truckBody"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\truckCabin.jpg");
        mapTextures["truckCabin"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\images\\fan.jpg");
        mapTextures["fan"] = texture;
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

    score = 0;
    distanceTraveled = 0.0f;
	isGameOver = false;
	isGameStarted = false;

    // TODO(student): Load other shaders
    LoadShader("TemaShader");
	LoadShader("ShadowShader");
	InitShadowMap();


    for (int i = 0; i < 100; i++) {
        std::cout << std::endl;
    }

    std::cout << "=======================================================" << std::endl;
    std::cout << "         W E L C O M E   T O   F A R M   R U S H       " << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "                                                       " << std::endl;
    std::cout << "   [ CONTROLS ]                                        " << std::endl;
    std::cout << "     [W] / [S]    :   Accelerate / Brake               " << std::endl;
    std::cout << "     [A] / [D]    :   Steer Left / Right               " << std::endl;
    std::cout << "     [F]          :   Toggle Headlights (Low Beams)    " << std::endl;
    std::cout << "     [G]          :   Toggle High Beams                " << std::endl;
    std::cout << "     [R]          :   Restart Game (On Game Over)      " << std::endl;
    std::cout << "                                                       " << std::endl;
    std::cout << "   [ MISSION ]                                         " << std::endl;
    std::cout << "     > Dodge obstacles!                                " << std::endl;
    std::cout << "     > Drive as far as possible to increase Score!     " << std::endl;
    std::cout << "     > Good luck, driver!                              " << std::endl;
    std::cout << "                                                       " << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << "      Waiting for start... PRESS [SPACE] TO PLAY!      " << std::endl;
    std::cout << "=======================================================" << std::endl;
    std::cout << std::endl;

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
    //glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema::Update(float deltaTimeSeconds)
{

    // randare umbre
    glm::vec3 fwd = glm::vec3(sin(carYaw), 0.0f, -cos(carYaw));
    glm::vec3 rgt = glm::normalize(glm::cross(fwd, glm::vec3(0, 1, 0)));
    float bOffset = 7.5f;
    glm::vec3 visualCarPos = carPosition - (fwd * bOffset);

    float height = 4.5f;
    float spread = 0.2f;
    float fwdOffsetLights = -5.5f;

    glm::vec3 sPos[3];
    glm::vec3 sDir[3];

    sPos[0] = visualCarPos + (fwd * fwdOffsetLights) + glm::vec3(0, height, 0);
    sPos[1] = visualCarPos + (fwd * fwdOffsetLights) - (rgt * spread) + glm::vec3(0, height, 0);
    sPos[2] = visualCarPos + (fwd * fwdOffsetLights) + (rgt * spread) + glm::vec3(0, height, 0);

    sDir[0] = glm::normalize((fwd * 1.0f) + glm::vec3(0.0f, -0.25f, 0.0f));
    sDir[1] = glm::normalize((fwd * 0.9f) - (rgt * 0.4f) + glm::vec3(0.0f, -0.35f, 0.0f));
    sDir[2] = glm::normalize((fwd * 0.9f) + (rgt * 0.4f) + glm::vec3(0.0f, -0.35f, 0.0f));

    glm::mat4 shadowProj = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 50.0f);

    Shader* shadowShader = shaders["ShadowShader"];
    glUseProgram(shadowShader->program);
    glUniform3fv(glGetUniformLocation(shadowShader->program, "playerPos"), 1, glm::value_ptr(carPosition));
    glUniform1f(glGetUniformLocation(shadowShader->program, "curvatureFactor"), 0.002f);

    glViewport(0, 0, 2048, 2048);

    if (overheadLightsState == 1) {
        for (int i = 0; i < 3; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO[i]);
            glClear(GL_DEPTH_BUFFER_BIT);

            glm::mat4 shadowView = glm::lookAt(sPos[i], sPos[i] + sDir[i], glm::vec3(0, 1, 0));
            lightSpaceMatrices[i] = shadowProj * shadowView;

            glUniformMatrix4fv(glGetUniformLocation(shadowShader->program, "LightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrices[i]));

            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            for (int i = 0; i < obstacles.size(); i++)
            {

                if (obstacles[i].position.z > carPosition.z + 20.0f)
                {

                    obstacles[i].position.z = carPosition.z - 300.0f;
                    obstacles[i].position.x = (rand() % 80 / 10.0f) - 4.0f;
                    //obstacles[i].type = rand() % 4;


                    int type = obstacles[i].type;
                    if (type == 0) obstacles[i].position.y = 0.0f;
                    else if (type == 1) obstacles[i].position.y = 0.0f;
                    else if (type == 2) obstacles[i].position.y = 0.0f;
                    else if (type == 3) obstacles[i].position.y = 0.0f;
                    /*       else if (type == 4) obstacles[i].position.y = 0.05f;*/
                }
            }
            RenderObstacles(shaders["ShadowShader"], 30.0f);
            glCullFace(GL_BACK);
            glDisable(GL_CULL_FACE);
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);


    // gameplay logic
    if (!isGameStarted) {
        return;
	}
	
    if (isGameOver) {
       
        return;
    }

    float currentSpeed = forwardSpeed;
    if (inputForward) currentSpeed *= 1.5f;
    if (inputBack) currentSpeed *= 0.5f;

    distanceTraveled += currentSpeed * deltaTimeSeconds;

    int newScore = (int)(distanceTraveled / 10.0f);

    if (newScore > score) {
        score = newScore;
        std::cout << ">>> SCORE: " << score << " <<<" << std::endl;
        forwardSpeed += 0.1f; 
    }


    glm::vec3 forwardDir = glm::vec3(sin(carYaw), 0.0f, -cos(carYaw));
    float backOffset = 7.5f;
    glm::vec3 tractorCenter = carPosition - (forwardDir * backOffset);

    float collisionRadius = 2.0f;

    for (int i = 0; i < obstacles.size(); i++)
    {

        float dx = tractorCenter.x - obstacles[i].position.x;
        float dz = tractorCenter.z - obstacles[i].position.z;
        float d = sqrt(dx * dx + dz * dz);

        if (d < collisionRadius)
        {
            isGameOver = true;

			std::cout << std::endl;
            std::cout << "===========================" << std::endl;
            std::cout << "       GAME OVER!          " << std::endl;
            std::cout << "   Final Score: " << score << std::endl;
            std::cout << "   Press R to Restart      " << std::endl;
            std::cout << "===========================" << std::endl;
			std::cout << std::endl;
        }
    }

	// lumini active
    activeStreetLights.clear();
    activeWindmillLights.clear();

    // lumini stalpi
    {
        float lampSpacing = 12.0f;
        float fenceX = maxLateral + 1.7f;

        int carIndex = (int)round(carPosition.z / lampSpacing);

        for (int i = carIndex - 3; i <= carIndex + 5; i++)
        {
            float z = i * lampSpacing;

            activeStreetLights.push_back(glm::vec3(-fenceX + 1.4f, 4.4f, z));
            activeStreetLights.push_back(glm::vec3(fenceX - 1.4f, 4.4f, z));
        }
    }

	// morile de vant lumini
    {
        float segmentZ = 4.5f;
        int centerSeg = (int)(carPosition.z / segmentZ);

        for (int i = centerSeg - 10; i <= centerSeg + 20; i++)
        {
            float z = i * segmentZ;

            float startX = maxLateral + 3.5f;
            float endX = startX + 30.0f;
     
            for (float x = startX; x < startX + 10.0f; x += 3.5f)
            {
                float sides[2] = { x, -x };
                for (float currentX : sides)
                {
                   
                    int seed = (int)(abs(currentX) * 12.9898f + abs(z) * 78.233f);
                    int objectType = seed % 15;

                    if (objectType == 0) { 
                        glm::vec3 winPos = glm::vec3(currentX, 6.0f, z + 1.0f);
                        activeWindmillLights.push_back(winPos);

                    }
                }
            }
        }
   

        float time = Engine::GetElapsedTime();
        float pulse = (sin(time * 3.0f) * 0.5f + 0.5f);
        currentWindmillColor = glm::mix(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), pulse);
    }


    // grass plane
    {
        Shader* shader = shaders["TemaShader"];
        glUseProgram(shader->program);

        glUniform3fv(glGetUniformLocation(shader->program, "playerPos"), 1, glm::value_ptr(carPosition));
        glUniform1f(glGetUniformLocation(shader->program, "curvatureFactor"), 0.002f);

        glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 0);

        glm::mat4 model = glm::mat4(1.0f);
        float groundOffset = 80.0f;

        model = glm::translate(model, glm::vec3(0.0f, -0.02f, carPosition.z - groundOffset));
        model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));

        RenderSimpleMesh(meshes["ground"], shader, model, mapTextures["grass"]);
    }

	// dirt road plane
    {
        Shader* shader = shaders["TemaShader"];

        glm::mat4 model = glm::mat4(1.0f);
        float groundOffset = 80.0f;

        model = glm::translate(model, glm::vec3(0.0f, 0.01f, carPosition.z - groundOffset));
        model = glm::scale(model, glm::vec3(0.24f, 1.0f, 1.0f));

        RenderSimpleMesh(meshes["ground"], shader, model, mapTextures["dirt"]);
    }

	// fence and lamp posts
    {
        Shader* shader = shaders["TemaShader"];
        glUseProgram(shader->program);

        glUniform3fv(glGetUniformLocation(shader->program, "playerPos"), 1, glm::value_ptr(carPosition));
        glUniform1f(glGetUniformLocation(shader->program, "curvatureFactor"), 0.002f);
        glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 0);

        float fenceX = maxLateral + 1.7f;
        Texture2D* fenceTex = mapTextures["crate"];

        
        {
            float segmentLength = 4.0f;
            int numSegmentsBehind = 2;
            int numSegmentsFront = 12;

            int currentGridIndex = (int)round(carPosition.z / segmentLength);
            float startZ = (currentGridIndex + numSegmentsBehind) * segmentLength;

            for (int i = 0; i < numSegmentsBehind + numSegmentsFront; i++)
            {
                float currentZ = startZ - (i * segmentLength);

                // STANGA
                glm::mat4 modelL = glm::translate(glm::mat4(1.0f), glm::vec3(-fenceX, 0.6f, currentZ));
                RenderSimpleMesh(meshes["fence_post"], shader, modelL, fenceTex);
                RenderSimpleMesh(meshes["fence_board"], shader, glm::translate(modelL, glm::vec3(0, 0.3f, 0)), fenceTex);
                RenderSimpleMesh(meshes["fence_board"], shader, glm::translate(modelL, glm::vec3(0, -0.3f, 0)), fenceTex);

                // DREAPTA
                glm::mat4 modelR = glm::translate(glm::mat4(1.0f), glm::vec3(fenceX, 0.6f, currentZ));
                RenderSimpleMesh(meshes["fence_post"], shader, modelR, fenceTex);
                RenderSimpleMesh(meshes["fence_board"], shader, glm::translate(modelR, glm::vec3(0, 0.3f, 0)), fenceTex);
                RenderSimpleMesh(meshes["fence_board"], shader, glm::translate(modelR, glm::vec3(0, -0.3f, 0)), fenceTex);
            }
        }

        {
            float lampSpacing = 12.0f;

  
            float manualOffsetZ = 0.0f;

            int lampsBehind = 1;
            int lampsFront = 6; 

          
            int lampGridIndex = (int)round(carPosition.z / lampSpacing);
            float startLampZ = (lampGridIndex + lampsBehind) * lampSpacing;

            for (int i = 0; i < lampsBehind + lampsFront; i++)
            {
                float z = startLampZ - (i * lampSpacing) + manualOffsetZ;


                float postY = 3.2f;

                // STANGA (-X)
                {
       
                    glm::mat4 post = glm::translate(glm::mat4(1.0f), glm::vec3(-fenceX, postY, z));
                    RenderSimpleMesh(meshes["farm_post_body"], shader, post, fenceTex);

     
                    glm::mat4 arm = glm::translate(glm::mat4(1.0f), glm::vec3(-fenceX + 0.75f, postY + 1.5f, z));
                    RenderSimpleMesh(meshes["farm_post_arm"], shader, arm, fenceTex);

                
                    glm::mat4 lantern = glm::translate(glm::mat4(1.0f), glm::vec3(-fenceX + 1.4f, postY + 1.22f, z));
                    RenderSimpleMesh(meshes["farm_lantern"], shader, lantern, mapTextures["lamp"]); 
                }

                // DREAPTA (+X)
                {
           
                    glm::mat4 post = glm::translate(glm::mat4(1.0f), glm::vec3(fenceX, postY, z));
                    RenderSimpleMesh(meshes["farm_post_body"], shader, post, fenceTex);

                    glm::mat4 arm = glm::translate(glm::mat4(1.0f), glm::vec3(fenceX - 0.75f, postY + 1.5f, z));
                    RenderSimpleMesh(meshes["farm_post_arm"], shader, arm, fenceTex);

                    glm::mat4 lantern = glm::translate(glm::mat4(1.0f), glm::vec3(fenceX - 1.4f, postY + 1.22f, z));
                    RenderSimpleMesh(meshes["farm_lantern"], shader, lantern, mapTextures["lamp"]);
                }
            }
        }
    }

    {
        Shader* shader = shaders["TemaShader"];
        glUseProgram(shader->program);

        glUniform3fv(glGetUniformLocation(shader->program, "playerPos"), 1, glm::value_ptr(carPosition));
        glUniform1f(glGetUniformLocation(shader->program, "curvatureFactor"), 0.002f);

        float time = Engine::GetElapsedTime();

        float startX = maxLateral + 3.5f;
        float endX = startX + 50.0f;
        float stepX = 3.5f;

        float segmentZ = 4.5f;
        int rowsBehind = 3;
        int rowsFront = 8;
        float startZ = ((int)(carPosition.z / segmentZ) * segmentZ) + (rowsBehind * segmentZ);

        for (int i = 0; i < rowsBehind + rowsFront; i++)
        {
            float z = startZ - (i * segmentZ);

            for (float x = startX; x < endX; x += stepX)
            {
                float sides[2] = { x, -x };

                for (float currentX : sides)
                {
                    int seed = (int)(abs(currentX) * 12.9898f + abs(z) * 78.233f);
                    int objectType = seed % 15;

                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(currentX, 0, z));

                    // moara de vant
                    if (objectType == 0)
                    {
                       
                        glm::mat4 body = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
                        body = glm::translate(body, glm::vec3(0, 2.5f, 0));

                        glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 0);
                        RenderSimpleMesh(meshes["windmill_body"], shader, body, mapTextures["brick"]);

                     
                        glm::mat4 doorMat = glm::translate(model, glm::vec3(0, 1.25f, 0.8f));

                        
                        RenderSimpleMesh(meshes["windmill_door"], shader, doorMat, mapTextures["door"]);

                     
                        glm::mat4 winMat = glm::translate(model, glm::vec3(0, 6.0f, 0.8f));

                        
                        RenderSimpleMesh(meshes["windmill_window"], shader, winMat, mapTextures["window"]);

                        
                        glm::mat4 roof = glm::translate(model, glm::vec3(0, 10.0f, 0));
                        RenderSimpleMesh(meshes["windmill_roof"], shader, roof, mapTextures["brickLight"]);

                        
                        float speed = 2.0f;
                        float dir = (seed % 2 == 0) ? 1.0f : -1.0f;
                        float rotAngle = time * speed * dir;

                        glm::mat4 blades = glm::translate(model, glm::vec3(0, 9.5f, 1.1f));
                        blades = glm::rotate(blades, rotAngle, glm::vec3(0, 0, 1));

                        RenderSimpleMesh(meshes["windmill_blade"], shader, blades, mapTextures["planeTexture"]);
                        glm::mat4 blade2 = glm::rotate(blades, glm::radians(90.0f), glm::vec3(0, 0, 1));
                        RenderSimpleMesh(meshes["windmill_blade"], shader, blade2, mapTextures["planeTexture"]);
                    }
                    // copac
                    else
                    {
                        bool hasApples = (objectType == 1 || objectType == 2);

                        
                        glm::mat4 trunk = glm::scale(model, glm::vec3(1.2f, 2.5f, 1.2f));
                        glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 0);
                        RenderSimpleMesh(meshes["tree_trunk"], shader, trunk, mapTextures["trunchiTexture"]);

                     
                        glm::mat4 crown1 = glm::translate(model, glm::vec3(0, 2.8f, 0));
                        glm::mat4 crown1Scale = glm::scale(crown1, glm::vec3(1.2f, 1.2f, 1.2f)); 
                        RenderSimpleMesh(meshes["tree_crown"], shader, crown1Scale, mapTextures["frunze"]);

                       
                        glm::mat4 crown2 = glm::translate(model, glm::vec3(0, 4.0f, 0));
                        crown2 = glm::scale(crown2, glm::vec3(0.9f, 1.0f, 0.9f));
                        RenderSimpleMesh(meshes["tree_crown"], shader, crown2, mapTextures["frunze"]);

                        if (hasApples)
                        {
                            
                            glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 0);
                          

                            glm::vec3 appleOffsets[] = {
                                
                                glm::vec3(-1.3f, 2.5f, 1.3f),   
                                
                                glm::vec3(1.4f, 3.0f, 0.0f), 

                                glm::vec3(0.9f, 4.0f, -0.9f),

                                glm::vec3(0.0f, 4.8f, 0.0f)
                            };

                            for (auto off : appleOffsets) {
                                 glm::mat4 appleM = glm::translate(model, off);
                                 
                                 appleM = glm::rotate(appleM, (float)seed, glm::vec3(1,1,1));
                                
                                 RenderSimpleMesh(meshes["apple_mesh"], shader, appleM, mapTextures["apple"]);
                            }
                        }
                    }
                }
            }
        }
    }

	// car forward/backward movement
    float forward = (inputForward ? 1.4f : 1.0f);
    if (inputBack) 
        forward = 0.5f;

    // always move forward (world feels like it is coming toward you)
    carPosition.z -= forward * forwardSpeed * deltaTimeSeconds;

    // smooth steering
    float targetSteer = 0.0f;
    if (inputLeft) targetSteer = 0.5f;        
    else if (inputRight) targetSteer = -0.5f; 

    float steerSpeed = 3.0f;

    if (currentSteerAngle < targetSteer) {
        currentSteerAngle += steerSpeed * deltaTimeSeconds;
        if (currentSteerAngle > targetSteer) currentSteerAngle = targetSteer;
    }
    else if (currentSteerAngle > targetSteer) {
        currentSteerAngle -= steerSpeed * deltaTimeSeconds;
        if (currentSteerAngle < targetSteer) currentSteerAngle = targetSteer;
    }

    carPosition.x -= currentSteerAngle * lateralSpeed * deltaTimeSeconds;
    carPosition.x = glm::clamp(carPosition.x, -maxLateral, maxLateral);

    // rotation of wheels
    float wheelCircumference = 2.0f * 3.14159f * 0.7f;
    float distanceTraveled = forward * forwardSpeed * deltaTimeSeconds;
    wheelAngle += distanceTraveled / wheelCircumference * 2.0f * 3.14159f;

	// smoke particles
    UpdateSmokeParticles(deltaTimeSeconds);
    RenderSmokeParticles();

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

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glm::mat4 projMatrix = GetSceneCamera()->GetProjectionMatrix();
    RenderCar(viewMatrix, projMatrix);

    for (int i = 0; i < obstacles.size(); i++)
    {
        
        if (obstacles[i].position.z > carPosition.z + 20.0f)
        {
     
            obstacles[i].position.z = carPosition.z - 300.0f;
            obstacles[i].position.x = (rand() % 80 / 10.0f) - 4.0f;
            //obstacles[i].type = rand() % 4;


            int type = obstacles[i].type;
            if (type == 0) obstacles[i].position.y = 0.0f;
            else if (type == 1) obstacles[i].position.y = 0.0f;
            else if (type == 2) obstacles[i].position.y = 0.0f;
            else if (type == 3) obstacles[i].position.y = 0.0f;
     /*       else if (type == 4) obstacles[i].position.y = 0.05f;*/
        }
    }
    RenderObstacles(shaders["TemaShader"], 70.0f);
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
   
	// headlights
    glm::vec3 forwardDir = glm::vec3(sin(carYaw), 0.0f, -cos(carYaw));
    glm::vec3 rightDir = glm::normalize(glm::cross(forwardDir, glm::vec3(0, 1, 0)));
    glm::vec3 lightDir = glm::normalize(forwardDir + glm::vec3(0, -0.10f, 0)); 


    float backOffset = 7.5f;
    glm::vec3 visualCarPos = carPosition - (forwardDir * backOffset);

    float distInFata = 1.3f;   
    float inaltime = 0.5f;   
    float latime = 0.5f; 

    glm::vec3 headlightOffsetL = visualCarPos + (forwardDir * distInFata) + glm::vec3(0, inaltime, 0) - (rightDir * latime);
    glm::vec3 headlightOffsetR = visualCarPos + (forwardDir * distInFata) + glm::vec3(0, inaltime, 0) + (rightDir * latime);

    GLint locHeadlightPos = glGetUniformLocation(shader->program, "headlightPos");
    if (locHeadlightPos >= 0) {
        float positions[6] = {
            headlightOffsetL.x, headlightOffsetL.y, headlightOffsetL.z,
            headlightOffsetR.x, headlightOffsetR.y, headlightOffsetR.z
        };
        glUniform3fv(locHeadlightPos, 2, positions);
    }

    GLint locHeadlightDir = glGetUniformLocation(shader->program, "headlightDir");
    if (locHeadlightDir >= 0) {
        glUniform3fv(locHeadlightDir, 1, glm::value_ptr(lightDir));
    }

    glUniform1i(glGetUniformLocation(shader->program, "headlightsOn"), headlightsState);


    // stalpi lumini
    int lightsToSend = (int)activeStreetLights.size();
    if (lightsToSend > 20) lightsToSend = 20;
    glUniform1i(glGetUniformLocation(shader->program, "streetLightCount"), lightsToSend);

    if (lightsToSend > 0) {
        glUniform3fv(glGetUniformLocation(shader->program, "streetLightPos"), lightsToSend, glm::value_ptr(activeStreetLights[0]));
    }

    glUniform3f(glGetUniformLocation(shader->program, "streetLightColor"), 1.0f, 0.8f, 0.4f);


    // mori de vant lumini
    int millsToSend = (int)activeWindmillLights.size();
    if (millsToSend > 20) millsToSend = 20;
    glUniform1i(glGetUniformLocation(shader->program, "windmillLightCount"), millsToSend);
    glUniform3fv(glGetUniformLocation(shader->program, "windmillLightColor"), 1, glm::value_ptr(currentWindmillColor));

    if (millsToSend > 0) {
        glUniform3fv(glGetUniformLocation(shader->program, "windmillLightPos"), millsToSend, glm::value_ptr(activeWindmillLights[0]));
    }


    // reflectoare
    glm::vec3 fwd = glm::vec3(sin(carYaw), 0.0f, -cos(carYaw));
    glm::vec3 rgt = glm::normalize(glm::cross(fwd, glm::vec3(0, 1, 0)));

    float bOffset = 7.5f;

    float height = 4.5f;
    float spread = 0.2f;

    glm::vec3 overPos[3];

    float forwardOffset = -3.5f;

    overPos[0] = visualCarPos + (fwd * forwardOffset) + glm::vec3(0, height, 0);                 
    overPos[1] = visualCarPos + (fwd * forwardOffset) - (rgt * spread) + glm::vec3(0, height, 0); 
    overPos[2] = visualCarPos + (fwd * forwardOffset) + (rgt * spread) + glm::vec3(0, height, 0); 

    glm::vec3 overDir[3];

    overDir[0] = glm::normalize((fwd * 1.0f) + glm::vec3(0.0f, -0.25f, 0.0f));
    overDir[1] = glm::normalize((fwd * 0.9f) - (rgt * 0.4f) + glm::vec3(0.0f, -0.35f, 0.0f));
    overDir[2] = glm::normalize((fwd * 0.9f) + (rgt * 0.4f) + glm::vec3(0.0f, -0.35f, 0.0f));


    GLint locOverPos = glGetUniformLocation(shader->program, "u_OverheadPos");
    GLint locOverDir = glGetUniformLocation(shader->program, "u_OverheadDir");
    GLint locOverActive = glGetUniformLocation(shader->program, "u_OverheadActive");

    if (locOverPos >= 0) glUniform3fv(locOverPos, 3, glm::value_ptr(overPos[0]));
    if (locOverDir >= 0) glUniform3fv(locOverDir, 3, glm::value_ptr(overDir[0]));

    glUniform1i(locOverActive, overheadLightsState);


    for (int i = 0; i < 3; i++) {
        std::string name = "LightSpaceMatrix[" + std::to_string(i) + "]";
        GLint loc = glGetUniformLocation(shader->program, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrices[i]));

 
        std::string texName = "shadowMap[" + std::to_string(i) + "]";
        glActiveTexture(GL_TEXTURE2 + i);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture[i]);
        glUniform1i(glGetUniformLocation(shader->program, texName.c_str()), 2 + i);
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
        newObs.type = rand() % 4;

		if (newObs.type == 0) // capita de fan
            newObs.position.y = 0.0f;
		else if (newObs.type == 1) // sperietoare de ciori
            newObs.position.y = 0.0f;
        else if (newObs.type == 2) // cal se uita la dreapta
            newObs.position.y = 0.0f;
        else if (newObs.type == 3) // cal se uita la stanga
            newObs.position.y = 0.0f;
        //else if (newObs.type == 4) 
        //    newObs.position.y = 0.05f; // con

        newObs.scale = 1.0f;
        obstacles.push_back(newObs);
    }
}

void Tema::CreateObstacleMeshes()
{

    // cal (obstacol 2 & 3)
    CreateBoxMesh("horse_torso", 2.0f, 0.8f, 0.9f, glm::vec3(0.45f, 0.25f, 0.1f));
    CreateBoxMesh("horse_neck", 0.5f, 1.2f, 0.45f, glm::vec3(0.45f, 0.25f, 0.1f));
    CreateBoxMesh("horse_head", 0.5f, 0.5f, 1.0f, glm::vec3(0.45f, 0.25f, 0.1f));
    CreateBoxMesh("horse_leg", 0.35f, 1.4f, 0.35f, glm::vec3(0.45f, 0.25f, 0.1f));
    CreateBoxMesh("horse_hoof", 0.4f, 0.2f, 0.4f, glm::vec3(0.1f, 0.1f, 0.1f));
    CreateSphereMesh("horse_eye", 0.08f, 10, 10, glm::vec3(0.0f, 0.0f, 0.0f));


	// sperietoare de ciori (obstacol 1)
    CreateBoxMesh("sc_leg", 0.25f, 0.9f, 0.3f, glm::vec3(0.2f, 0.2f, 0.6f));
    CreateBoxMesh("sc_body", 0.7f, 0.8f, 0.4f, glm::vec3(0.6f, 0.2f, 0.2f)); 
    CreateBoxMesh("sc_arm", 0.8f, 0.25f, 0.25f, glm::vec3(0.6f, 0.2f, 0.2f));
    CreateSphereMesh("sc_head", 0.25f, 20, 20, glm::vec3(0.9f, 0.8f, 0.6f)); 
    CreateConeMesh("sc_hat", 0.4f, 0.5f, 20, glm::vec3(0.3f, 0.2f, 0.1f)); 

	// baloti de fan (obstacol 0)
    CreateBoxMesh("hay_bale", 1.0f, 0.7f, 1.7f, glm::vec3(0.9f, 0.8f, 0.2f));

	// stalp de pe gard (environment)
    CreateBoxMesh("farm_post_body", 0.2f, 4.0f, 0.2f, glm::vec3(0.4f, 0.25f, 0.1f));
    CreateBoxMesh("farm_post_arm", 1.5f, 0.15f, 0.15f, glm::vec3(0.4f, 0.25f, 0.1f));
    CreateBoxMesh("farm_lantern", 0.4f, 0.5f, 0.4f, glm::vec3(0.8f, 0.8f, 0.6f));

	// moara de vant (environment)
    CreateBoxMesh("windmill_body", 1.5f, 5.0f, 1.5f, glm::vec3(0.8f, 0.8f, 0.8f));
    CreateConeMesh("windmill_roof", 1.2f, 1.5f, 16, glm::vec3(0.6f, 0.1f, 0.1f));
    CreateBoxMesh("windmill_blade", 6.0f, 0.4f, 0.1f, glm::vec3(0.9f, 0.9f, 0.9f));
    CreateBoxMesh("windmill_door", 1.0f, 2.5f, 0.1f, glm::vec3(0.4f, 0.2f, 0.1f));
    CreateBoxMesh("windmill_window", 1.2f, 1.2f, 0.1f, glm::vec3(0.2f, 0.2f, 0.5f));

	// apples for tree (environment)
    CreateBoxMesh("apple_mesh", 0.25f, 0.25f, 0.25f, glm::vec3(1.0f, 0.0f, 0.0f));

	// fence (environment)
    CreateBoxMesh("fence_post", 0.2f, 1.2f, 0.2f, glm::vec3(0.4f, 0.2f, 0.1f));
    CreateBoxMesh("fence_board", 0.1f, 0.2f, 4.1f, glm::vec3(0.4f, 0.2f, 0.1f));

	// tree pieces (environment)
    CreateBoxMesh("tree_trunk", 0.6f, 2.0f, 0.6f, glm::vec3(0.5f, 0.35f, 0.05f));
    CreateBoxMesh("tree_crown", 2.0f, 1.5f, 2.0f, glm::vec3(0.0f, 0.5f, 0.0f));



	// crate with X bars
    CreateBoxMesh("crate_core", 2.5f, 2.5f, 2.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    CreateBoxMesh("crate_x_bar", 3.6f, 0.3f, 0.05f, glm::vec3(0.2f, 0.2f, 0.2f));


	// pole pieces
    CreateBoxMesh("pole_body", 0.3f, 5.0f, 0.3f, glm::vec3(0.4f, 0.4f, 0.4f));
    CreateBoxMesh("pole_arm", 1.5f, 0.25f, 0.25f, glm::vec3(0.4f, 0.4f, 0.4f));
    CreateBoxMesh("pole_lamp", 0.4f, 0.4f, 0.4f, glm::vec3(1.0f, 1.0f, 0.6f));

	// barrier pieces
    CreateBoxMesh("barrier_post", 0.4f, 1.5f, 0.4f, glm::vec3(0.8f, 0.1f, 0.1f));
    CreateBoxMesh("barrier_bar", 4.0f, 0.3f, 0.3f, glm::vec3(0.9f, 0.9f, 0.9f));

	// traffic cone pieces
    CreateBoxMesh("cone_rubber_base", 1.6f, 0.1f, 1.6f, glm::vec3(0.1f, 0.1f, 0.1f));
    CreateBoxMesh("cone_plastic_base", 1.4f, 0.1f, 1.4f, glm::vec3(1.0f, 0.5f, 0.0f));
    CreateConeMesh("cone_body_smooth", 0.6f, 2.5f, 30, glm::vec3(1.0f, 0.5f, 0.0f));
}

void Tema::RenderObstacles(Shader* shader, float drawDistance)
{
    if (!shader) return;

    glUseProgram(shader->program);
    glUniform3fv(glGetUniformLocation(shader->program, "playerPos"), 1, glm::value_ptr(carPosition));
    glUniform1f(glGetUniformLocation(shader->program, "curvatureFactor"), 0.002f);

    glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 1);

    for (const auto& obs : obstacles)
    {
        float dist = abs(obs.position.z - carPosition.z);

        if (dist > drawDistance) continue;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, obs.position);
        
        model = glm::scale(model, glm::vec3(obs.scale));

        // capita de fan
        if (obs.type == 0)
        {
            glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 0);
            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.9f, 0.8f, 0.2f);

            RenderSimpleMesh(meshes["hay_bale"], shader, glm::translate(model, glm::vec3(0, 0.35f, 0)), mapTextures["fan"]);
            RenderSimpleMesh(meshes["hay_bale"], shader, glm::translate(model, glm::vec3(0.85f, 0.35f, 0.2f)), mapTextures["fan"]);  
            RenderSimpleMesh(meshes["hay_bale"], shader, glm::translate(model, glm::vec3(-0.8f, 0.35f, -0.1f)), mapTextures["fan"]); 

            RenderSimpleMesh(meshes["hay_bale"], shader, glm::translate(model, glm::vec3(0.4f, 1.05f, 0.0f)), mapTextures["fan"]);   
            RenderSimpleMesh(meshes["hay_bale"], shader, glm::translate(model, glm::vec3(-0.45f, 1.05f, 0.1f)), mapTextures["fan"]); 

            glm::mat4 top = glm::translate(model, glm::vec3(0, 1.75f, 0));
            top = glm::rotate(top, glm::radians(15.0f), glm::vec3(0, 1, 0));
            RenderSimpleMesh(meshes["hay_bale"], shader, top, mapTextures["fan"]);
        }
		else if (obs.type == 1) // sperietoare de ciori
        {
 
            glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 0);
            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.2f, 0.3f, 0.7f); 

            
            RenderSimpleMesh(meshes["sc_leg"], shader, glm::translate(model, glm::vec3(-0.2f, 0.45f, 0)), mapTextures["blugi"]);
            RenderSimpleMesh(meshes["sc_leg"], shader, glm::translate(model, glm::vec3(0.2f, 0.45f, 0)), mapTextures["blugi"]);

            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.7f, 0.2f, 0.2f);
            RenderSimpleMesh(meshes["sc_body"], shader, glm::translate(model, glm::vec3(0, 1.3f, 0)), mapTextures["camasa"]);

            glm::mat4 leftArm = glm::translate(model, glm::vec3(-0.7f, 1.5f, 0));
            leftArm = glm::rotate(leftArm, glm::radians(15.0f), glm::vec3(0, 0, 1)); 
            RenderSimpleMesh(meshes["sc_arm"], shader, leftArm, mapTextures["camasa"]);

            glm::mat4 rightArm = glm::translate(model, glm::vec3(0.7f, 1.5f, 0));
            rightArm = glm::rotate(rightArm, glm::radians(-15.0f), glm::vec3(0, 0, 1));
            RenderSimpleMesh(meshes["sc_arm"], shader, rightArm, mapTextures["camasa"]);

            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.95f, 0.85f, 0.6f); 
            RenderSimpleMesh(meshes["sc_head"], shader, glm::translate(model, glm::vec3(0, 1.95f, 0)), mapTextures["face"]);

            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.4f, 0.25f, 0.1f);
            RenderSimpleMesh(meshes["sc_hat"], shader, glm::translate(model, glm::vec3(0, 2.15f, 0)), mapTextures["sapca"]);
        }

        else if (obs.type == 2 || obs.type == 3)
        {
     
            glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 0);
            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.45f, 0.25f, 0.1f);

            float rotAngle = (obs.type == 2) ? 0.0f : 180.0f;

            glm::mat4 horseBase = glm::rotate(model, glm::radians(rotAngle), glm::vec3(0, 1, 0));

            RenderSimpleMesh(meshes["horse_torso"], shader, glm::translate(horseBase, glm::vec3(0, 1.4f, 0)), mapTextures["horse"]);

            float legX = 0.7f;
            float legZ = 0.25f;
            float legY = 0.7f;

            RenderSimpleMesh(meshes["horse_leg"], shader, glm::translate(horseBase, glm::vec3(-legX, legY, legZ)), mapTextures["horse"]);
            RenderSimpleMesh(meshes["horse_leg"], shader, glm::translate(horseBase, glm::vec3(-legX, legY, -legZ)), mapTextures["horse"]);
            RenderSimpleMesh(meshes["horse_leg"], shader, glm::translate(horseBase, glm::vec3(legX, legY, legZ)), mapTextures["horse"]);
            RenderSimpleMesh(meshes["horse_leg"], shader, glm::translate(horseBase, glm::vec3(legX, legY, -legZ)), mapTextures["horse"]);

            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.1f, 0.1f, 0.1f);
            RenderSimpleMesh(meshes["horse_hoof"], shader, glm::translate(horseBase, glm::vec3(-legX, 0.1f, legZ)), mapTextures["planeTexture"]);
            RenderSimpleMesh(meshes["horse_hoof"], shader, glm::translate(horseBase, glm::vec3(-legX, 0.1f, -legZ)), mapTextures["planeTexture"]);
            RenderSimpleMesh(meshes["horse_hoof"], shader, glm::translate(horseBase, glm::vec3(legX, 0.1f, legZ)), mapTextures["planeTexture"]);
            RenderSimpleMesh(meshes["horse_hoof"], shader, glm::translate(horseBase, glm::vec3(legX, 0.1f, -legZ)), mapTextures["planeTexture"]);

            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.45f, 0.25f, 0.1f);

       
            glm::mat4 neckM = glm::translate(horseBase, glm::vec3(0.8f, 2.0f, 0));
            neckM = glm::rotate(neckM, glm::radians(20.0f), glm::vec3(0, 0, 1));
            RenderSimpleMesh(meshes["horse_neck"], shader, neckM, mapTextures["horse"]);

         
            glm::mat4 headM = glm::translate(horseBase, glm::vec3(1.2f, 2.7f, 0));
            headM = glm::rotate(headM, glm::radians(20.0f), glm::vec3(0, 0, 1));
            headM = glm::rotate(headM, glm::radians(90.0f), glm::vec3(0, 1, 0));
            RenderSimpleMesh(meshes["horse_head"], shader, headM, mapTextures["horse"]);


            glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.0f, 0.0f, 0.0f);
            glm::mat4 eyeL = glm::translate(headM, glm::vec3(0.26f, 0.1f, 0.2f));
            RenderSimpleMesh(meshes["horse_eye"], shader, eyeL, mapTextures["planeTexture"]);
            glm::mat4 eyeR = glm::translate(headM, glm::vec3(-0.26f, 0.1f, 0.2f));
            RenderSimpleMesh(meshes["horse_eye"], shader, eyeR, mapTextures["planeTexture"]);
        }
        //else if (obs.type == 3) 
        //{
        //
        //    glm::mat4 leftPost = glm::translate(model, glm::vec3(-1.8f, 0.0f, 0.0f));
        //    glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.7f, 0.1f, 0.1f);
        //    RenderSimpleMesh(meshes["barrier_post"], shader, leftPost);
        //   
        //    glm::mat4 rightPost = glm::translate(model, glm::vec3(1.8f, 0.0f, 0.0f));
        //    RenderSimpleMesh(meshes["barrier_post"], shader, rightPost);
        //    
        //    glm::mat4 bar = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
        //    glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.9f, 0.9f, 0.9f);
        //    RenderSimpleMesh(meshes["barrier_bar"], shader, bar);
        //}
        //else if (obs.type == 4) 
        //{
        //    
        //    glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.1f, 0.1f, 0.1f);
        //    RenderSimpleMesh(meshes["cone_rubber_base"], shader, model);


        //    glm::mat4 base2 = glm::translate(model, glm::vec3(0.0f, 0.1f, 0.0f));
        //    glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 1.0f, 0.5f, 0.0f);
        //    RenderSimpleMesh(meshes["cone_plastic_base"], shader, base2);

        //    glm::mat4 body = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
        //    RenderSimpleMesh(meshes["cone_body_smooth"], shader, body);
        //}
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
            glm::vec2 texCoord = glm::vec2(u * 25.0f, v * 100.0f);

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

    if (key == GLFW_KEY_F) {
        headlightsState = 1 - headlightsState;
    }

    if (key == GLFW_KEY_G) {
        overheadLightsState = 1 - overheadLightsState;
    }

    if (key == GLFW_KEY_ESCAPE) {
        exit(0);
	}

    if (key == GLFW_KEY_SPACE && !isGameStarted) {
        isGameStarted = true;
        score = 0;
        distanceTraveled = 0.0f;
        forwardSpeed = 6.0f;
        carPosition = glm::vec3(0, 0.5f, 0);
        InitObstacles();
        std::cout << "Game Started!" << std::endl;

	}

    if (key == GLFW_KEY_R && isGameOver) {
        isGameOver = false;
        score = 0;
        distanceTraveled = 0.0f;
        forwardSpeed = 6.0f;
        carPosition = glm::vec3(0, 0.5f, 0);

        InitObstacles();

        std::cout << "Game Restarted!" << std::endl;
    }
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

// nice cylinder mesh
void Tema::CreateCylinderMesh(const std::string& name, float radius, float height, int slices, glm::vec3 color)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

	// centrul capetelor
    vertices.emplace_back(glm::vec3(0, -height / 2, 0), color, glm::vec3(0, -1, 0), glm::vec2(0.5f, 0.5f)); // 0
    vertices.emplace_back(glm::vec3(0, height / 2, 0), color, glm::vec3(0, 1, 0), glm::vec2(0.5f, 0.5f));  // 1

    for (int i = 0; i < slices; i++)
    {
        float angle = 2.0f * 3.14159f * i / slices;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        float u = (float)i / slices;

        // cerc jos
        vertices.emplace_back(glm::vec3(x, -height / 2, z), color, glm::vec3(x, 0, z), glm::vec2(u, 0.0f));
        // cerc sus
        vertices.emplace_back(glm::vec3(x, height / 2, z), color, glm::vec3(x, 0, z), glm::vec2(u, 1.0f));
    }

    // generare indici
    for (int i = 0; i < slices; i++)
    {
        int botCenter = 0;
        int topCenter = 1;
        int currentBot = 2 + 2 * i;
        int currentTop = 2 + 2 * i + 1;
        int nextBot = 2 + 2 * ((i + 1) % slices);
        int nextTop = 2 + 2 * ((i + 1) % slices) + 1;

        // capac jos
        indices.push_back(botCenter);
        indices.push_back(nextBot);
        indices.push_back(currentBot);

        // capac sus
        indices.push_back(topCenter);
        indices.push_back(currentTop);
        indices.push_back(nextTop);

        // lateral (2 triunghiuri)
        indices.push_back(currentBot);
        indices.push_back(nextBot);
        indices.push_back(nextTop);

        indices.push_back(currentBot);
        indices.push_back(nextTop);
        indices.push_back(currentTop);
    }

    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    meshes[mesh->GetMeshID()] = mesh;
}


void Tema::CreateCarMeshes()
{

	// corp tractor
    CreateBoxMesh("tractor_body", 1.2f, 0.8f, 2.5f, glm::vec3(0.2f, 0.6f, 0.2f));

	// cabina tractor
    CreateBoxMesh("tractor_cabin", 1.1f, 1.2f, 1.1f, glm::vec3(0.3f, 0.3f, 0.3f));

    // cos de fum tractor
    CreateCylinderMesh("tractor_chimney", 0.1f, 1.0f, 16, glm::vec3(0.1f, 0.1f, 0.1f));

    // roti spate - 32 pizza slices
    CreateCylinderMesh("wheel_big", 0.7f, 0.5f, 32, glm::vec3(0.1f, 0.1f, 0.1f));

    // roti fata
    CreateCylinderMesh("wheel_small", 0.4f, 0.3f, 32, glm::vec3(0.1f, 0.1f, 0.1f));

	// cub particula fum
    CreateBoxMesh("smoke_particle", 0.15f, 0.15f, 0.15f, glm::vec3(0.8f, 0.8f, 0.8f));
}

// render the assembled car 
void Tema::RenderCar(const glm::mat4& view, const glm::mat4& proj)
{
    Shader* shader = shaders["TemaShader"];
    if (!shader) return;
    glUseProgram(shader->program);


    GLint locUseColor = glGetUniformLocation(shader->program, "u_UseObjectColor");
    GLint locColor = glGetUniformLocation(shader->program, "u_ObjectColor");
    GLint locEmissionColor = glGetUniformLocation(shader->program, "u_EmissionColor");
    GLint locEmissionIntensity = glGetUniformLocation(shader->program, "u_EmissionIntensity");
    GLint locUseEmission = glGetUniformLocation(shader->program, "u_UseEmission");
    GLint locEmissionSpeed = glGetUniformLocation(shader->program, "u_EmissionSpeed");

    float backOffset = 7.5f;
    glm::vec3 forwardDir = glm::vec3(sin(carYaw), 0.0f, -cos(carYaw));
    glm::vec3 visualPosition = carPosition - (forwardDir * backOffset);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, visualPosition);
    model = glm::rotate(model, carYaw, glm::vec3(0, 1, 0));

	// vibratii tractor
    float time = Engine::GetElapsedTime();

    float frequency = 60.0f;
    float amplitude = 0.005f;

    if (inputForward) {
        frequency = 70.0f; 
        amplitude = 0.012f;  
    }
    else if (inputBack) {
        frequency = 30.0f;
        amplitude = 0.003f;
    }


    float vibration = sin(time * frequency) * amplitude;

    model = glm::rotate(model, vibration, glm::vec3(0, 0, 1));
    model = glm::translate(model, glm::vec3(0, vibration * 0.2f, 0));
    
    glm::mat4 bodyModel = glm::translate(model, glm::vec3(0.0f, 0.7f, 0.0f));
    if (locUseColor >= 0) glUniform1i(locUseColor, 0);
    if (locColor >= 0) glUniform3f(locColor, 1.0f, 1.0f, 1.0f);
    RenderSimpleMesh(meshes["tractor_body"], shader, bodyModel, mapTextures["truckBody"]);

    glm::mat4 cabinModel = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.6f));
    RenderSimpleMesh(meshes["tractor_cabin"], shader, cabinModel, mapTextures["truckCabin"]);



    glm::mat4 chimneyModel = glm::translate(model, glm::vec3(0.4f, 1.5f, -1.0f));
    RenderSimpleMesh(meshes["tractor_chimney"], shader, chimneyModel, mapTextures["planeTexture"]);

    // roti, efect de noroi 
    glUniform1i(locUseEmission, 1);
    glUniform3f(locEmissionColor, 0.4f, 0.25f, 0.1f);
    glUniform1f(locEmissionIntensity, 0.5f);
    if (locEmissionSpeed >= 0) 
        glUniform1f(locEmissionSpeed, 6.0f); // pulse speed

    // roti spate
    float bigWheelY = 0.7f;
    float bigWheelZ = 0.8f;
    float bigWheelX = 0.9f;

    // stanga spate
    glm::mat4 wBL = glm::translate(model, glm::vec3(-bigWheelX, bigWheelY, bigWheelZ));
    wBL = glm::rotate(wBL, wheelAngle, glm::vec3(1, 0, 0));
    wBL = glm::rotate(wBL, glm::radians(90.0f), glm::vec3(0, 0, 1));
    RenderSimpleMesh(meshes["wheel_big"], shader, wBL, mapTextures["wheel"]);

	// dreapta spate
    glm::mat4 wBR = glm::translate(model, glm::vec3(bigWheelX, bigWheelY, bigWheelZ));
    wBR = glm::rotate(wBR, wheelAngle, glm::vec3(1, 0, 0));
    wBR = glm::rotate(wBR, glm::radians(90.0f), glm::vec3(0, 0, 1));
    RenderSimpleMesh(meshes["wheel_big"], shader, wBR, mapTextures["wheel"]);

    // roti fata
    float smallWheelY = 0.4f;
    float smallWheelZ = -1.1f;
    float smallWheelX = 0.7f;

    // stanga fata
    glm::mat4 wFL = glm::translate(model, glm::vec3(-smallWheelX, smallWheelY, smallWheelZ));
    wFL = glm::rotate(wFL, currentSteerAngle, glm::vec3(0, 1, 0));
    wFL = glm::rotate(wFL, wheelAngle * 1.5f, glm::vec3(1, 0, 0));
    wFL = glm::rotate(wFL, glm::radians(90.0f), glm::vec3(0, 0, 1));
    RenderSimpleMesh(meshes["wheel_small"], shader, wFL, mapTextures["wheel"]);

    // dreapta fata
    glm::mat4 wFR = glm::translate(model, glm::vec3(smallWheelX, smallWheelY, smallWheelZ));
    wFR = glm::rotate(wFR, currentSteerAngle, glm::vec3(0, 1, 0));
    wFR = glm::rotate(wFR, wheelAngle * 1.5f, glm::vec3(1, 0, 0));
    wFR = glm::rotate(wFR, glm::radians(90.0f), glm::vec3(0, 0, 1));
    RenderSimpleMesh(meshes["wheel_small"], shader, wFR, mapTextures["wheel"]);

    glUniform1i(locUseEmission, 0);
}

void Tema::UpdateSmokeParticles(float deltaTimeSeconds)
{
	// spawn noi particule
    {
        Particle p;

        glm::vec3 forwardDir = glm::vec3(sin(carYaw), 0.0f, -cos(carYaw));
        float backOffset = 7.5f;

        glm::vec3 visualPosition = carPosition - (forwardDir * backOffset);

        glm::vec3 chimneyLocalOffset = glm::vec3(0.4f, 1.5f, -1.0f);

        glm::vec3 rotatedOffset = glm::rotate(glm::mat4(1.0f), carYaw, glm::vec3(0, 1, 0)) * glm::vec4(chimneyLocalOffset, 1.0f);

        p.position = visualPosition + rotatedOffset;
		p.position.y += 0.5f;


        // viteza: iese in sus (Y) si putin in spate/lateral random
        p.velocity = glm::vec3(
            (rand() % 100 / 100.0f - 0.5f) * 0.5f, // random X mic
            2.5f + (rand() % 100 / 100.0f) * 1.0f, // sus puternic (Y)
            (rand() % 100 / 100.0f - 0.5f) * 0.5f  // random Z mic
        );

        p.initialLife = 1.0f; 
        p.life = p.initialLife;

  
        if (smokeParticles.size() < 200) {
            smokeParticles.push_back(p);
        }
    }

    // update particule existente
    for (int i = 0; i < smokeParticles.size(); i++) {
        smokeParticles[i].position += smokeParticles[i].velocity * deltaTimeSeconds;
        smokeParticles[i].life -= deltaTimeSeconds;
    }

	// delete particule moarte
    for (int i = smokeParticles.size() - 1; i >= 0; i--) {
        if (smokeParticles[i].life <= 0) {
            smokeParticles.erase(smokeParticles.begin() + i);
        }
    }
}

void Tema::RenderSmokeParticles()
{
    Shader* shader = shaders["TemaShader"];
    glUseProgram(shader->program);


    glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 1);
    glUniform3f(glGetUniformLocation(shader->program, "u_ObjectColor"), 0.5f, 0.5f, 0.5f);
    glUniform1i(glGetUniformLocation(shader->program, "u_HasTexture"), 0);

    glUniform3fv(glGetUniformLocation(shader->program, "playerPos"), 1, glm::value_ptr(carPosition));
    glUniform1f(glGetUniformLocation(shader->program, "curvatureFactor"), 0.002f);

    for (const auto& p : smokeParticles) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, p.position);

        // scade marimea treptat
        float scale = 0.5f * (p.life / p.initialLife);
        model = glm::scale(model, glm::vec3(scale));
        model = glm::rotate(model, p.life * 2.0f, glm::vec3(1, 1, 1));

        RenderSimpleMesh(meshes["smoke_particle"], shader, model);
    }
   
    glUniform1i(glGetUniformLocation(shader->program, "u_UseObjectColor"), 0);
}

void Tema::CreateSphereMesh(const std::string& name, float radius, int rings, int sectors, glm::vec3 color)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float const R = 1.0f / (float)(rings - 1);
    float const S = 1.0f / (float)(sectors - 1);

    for (int r = 0; r < rings; r++)
    {
        for (int s = 0; s < sectors; s++)
        {
            float const y = sin(-1.570796f + 3.14159f * r * R);
            float const x = cos(2 * 3.14159f * s * S) * sin(3.14159f * r * R);
            float const z = sin(2 * 3.14159f * s * S) * sin(3.14159f * r * R);

            vertices.emplace_back(glm::vec3(x * radius, y * radius, z * radius), color, glm::vec3(x, y, z), glm::vec2(s * S, r * R));
        }
    }

    for (int r = 0; r < rings - 1; r++)
    {
        for (int s = 0; s < sectors - 1; s++)
        {
            indices.push_back(r * sectors + s);
            indices.push_back((r + 1) * sectors + s);
            indices.push_back((r + 1) * sectors + (s + 1));

            indices.push_back(r * sectors + s);
            indices.push_back((r + 1) * sectors + (s + 1));
            indices.push_back(r * sectors + (s + 1));
        }
    }

    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    meshes[mesh->GetMeshID()] = mesh;
}

void Tema::InitShadowMap()
{
    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

    for (int i = 0; i < 3; i++) {
        glGenFramebuffers(1, &shadowMapFBO[i]);

        glGenTextures(1, &shadowMapTexture[i]);
        glBindTexture(GL_TEXTURE_2D, shadowMapTexture[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
            SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMapTexture[i], 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}