#include "GameApplication.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "Scene.h"
#include "Menu.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
namespace rj = rapidjson;

#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>


//#define FULLSCREEN_MODE

#define MS_PER_UPDATE (1.0 / 60.0)

#define DEFAULT_FOV 83.0f
#define DEFAULT_BRIGHTNESS 1.0f
#define DEFAULT_GAMMA 1.0f
#define DEFAULT_CONTRAST -1.0f
#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600
#define DEFAULT_NEAR_PLANE 0.1f
#define DEFAULT_FAR_PLANE 1000.f

GLFWwindow* GameApplication::s_Window = nullptr;

float GameApplication::s_Fov = DEFAULT_FOV;
float GameApplication::brightness = DEFAULT_BRIGHTNESS;
float GameApplication::gamma = DEFAULT_GAMMA;
float GameApplication::contrast = DEFAULT_CONTRAST;
float GameApplication::contrastMod = 0.0f;
float GameApplication::s_AspectRatio = (float)DEFAULT_SCREEN_WIDTH/DEFAULT_SCREEN_HEIGHT;
int GameApplication::s_ScreenWidth = DEFAULT_SCREEN_WIDTH;
int GameApplication::s_ScreenHeight = DEFAULT_SCREEN_HEIGHT;
float GameApplication::s_NearPlane = DEFAULT_NEAR_PLANE;
float GameApplication::s_FarPlane = DEFAULT_FAR_PLANE;
glm::mat4 GameApplication::s_ProjectionMatrix = glm::perspective(
    DEFAULT_FOV, 
    s_AspectRatio,
    DEFAULT_NEAR_PLANE, 
    DEFAULT_FAR_PLANE
);
glm::mat4 GameApplication::s_OrthographicMatrix = glm::ortho(
    0.0f, s_AspectRatio,
    0.0f, 1.0f
);

Scene* GameApplication::s_Scene = nullptr;
MenuScene* GameApplication::s_Menu = nullptr;
bool GameApplication::inGame = false;
InputManager* GameApplication::s_InputManager = nullptr;
ResourceManager* GameApplication::s_ResourceManager = nullptr;

float GameApplication::s_TotalElapsedTime = 0.0f;


struct MemoryStatistics
{
    size_t TotalAllocated = 0;
    size_t TotalFreed = 0;
    size_t CurrentUsage() { return TotalAllocated - TotalFreed; }
    float CurrentUsageMB() { return CurrentUsage() * 0.000001; }
    float TotalAllocatedMB() { return TotalAllocated * 0.000001; }
    float TotalFreedMB() { return TotalFreed * 0.000001; }
};

static MemoryStatistics s_MemoryStatistics;

void* operator new(size_t bytes)
{
    s_MemoryStatistics.TotalAllocated += bytes;
    return malloc(bytes);
}

void operator delete(void* memory, size_t bytes)
{
    s_MemoryStatistics.TotalFreed += bytes;
    free(memory);
}

int GameApplication::Init()
{
    //Load libraries

    glfwInit();
    const char* glsl_version = "#version 450";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWmonitor* monitor = nullptr;
    
#ifdef FULLSCREEN_MODE
    monitor = glfwGetPrimaryMonitor();
    s_ScreenWidth  = glfwGetVideoMode(monitor)->width;
    s_ScreenHeight = glfwGetVideoMode(monitor)->height;

    s_AspectRatio = (float)s_ScreenWidth / s_ScreenHeight;

    s_ProjectionMatrix = glm::perspective(
        DEFAULT_FOV,
        s_AspectRatio,
        DEFAULT_NEAR_PLANE,
        DEFAULT_FAR_PLANE
    );

   s_OrthographicMatrix = glm::ortho(
        0.0f, s_AspectRatio,
        0.0f, 1.0f
    );
#endif // FULLSCREEN_MODE

    s_Window = glfwCreateWindow(s_ScreenWidth, s_ScreenHeight, "Enlite Game Engine", monitor, NULL);
    if(s_Window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    else 
    {
        std::cout << "Successfully created GLFW window" << std::endl;
    }
    
    glfwSwapInterval(false);
    glfwMakeContextCurrent(s_Window);
    glfwSetFramebufferSizeCallback(s_Window, FramebufferResizeCallback);


    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -2;
    }
    else 
    {
        std::cout << "Successfully initialized GLAD" << std::endl;
    }
    
    
    glEnable(GL_MULTISAMPLE); 

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    //FT_Library ft;
	//if(FT_Init_FreeType(&ft))
	//{
	//	std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	//	return -3;
	//}
    //else 
    //{
    //    std::cout << "Successfully initialized FreeType" << std::endl;
    //}


    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    rj::Document d;
    d.Parse(json);

    rj::Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    rj::StringBuffer buffer;
    rj::Writer<rj::StringBuffer> writer(buffer);
    if (d.Accept(writer))
    {
        std::cout << "Successfully initialized RapidJSON" << std::endl;
    }
	

    ALCdevice *device;
    device = alcOpenDevice(NULL);
    if(!device)
    {
        return -7;
    }
    else
    {
        std::cout << "Successfully created AL device" << std::endl;
    }

    //Create managers / systems

    s_InputManager = new InputManager(s_Window);
    s_ResourceManager = new ResourceManager();

    //Load scene
    //Scene OnCreate
    s_Scene = new Scene();


    s_Menu = new MainMenu();

    return 0;
}

void GameApplication::Run()
{
    glfwMaximizeWindow(GameApplication::GetWindow());
        glfwRestoreWindow(GameApplication::GetWindow());
    double t1, t2 = glfwGetTime();
    double dt, lag = 0.0;
    double fpsMeasureTimer = 1.0f;
    int framesCountLastSecond = 0;
    int fps = 0;
    while (!glfwWindowShouldClose(s_Window))
    {
        if(s_InputManager->Keyboard()->OnPressed(KeyboardKey::F10))
        {
            printf("=== FPS: %i === Current memory usage: %.2f (Total allocated: %.2f, total freed: %.2f) ===\n", fps,
            s_MemoryStatistics.CurrentUsageMB(), s_MemoryStatistics.TotalAllocatedMB(), s_MemoryStatistics.TotalFreedMB());
        }
        //if(s_InputManager->Keyboard()->OnPressed(KeyboardKey::Escape_KB))
        //{
        //    glfwSetWindowShouldClose(s_Window, true);
        //}

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //get time step
        t1 = glfwGetTime();
        dt = t1 - t2;
        t2 = t1;
        lag = std::min(lag + dt, 1.618);

        if (fpsMeasureTimer > 0.0f)
        {
            fpsMeasureTimer -= dt;
            framesCountLastSecond++;
        }
        else
        {
            fps = framesCountLastSecond;
            framesCountLastSecond = 0;
            fpsMeasureTimer = 1.0f;
        }

        s_TotalElapsedTime += dt;

        if(inGame)
        {
            while (lag > MS_PER_UPDATE)
            {
                //update logic
                s_Scene->Update(MS_PER_UPDATE);
                lag -= MS_PER_UPDATE;
            }

            //show scene
            s_Scene->Render();
        }
        else
        {
            lag = 0.0;
            s_Menu->Update();
            s_Menu->Draw();
        }

        AudioManager::Update(dt);

        glfwPollEvents();
        glfwSwapBuffers(s_Window);
    }

    OnStop();
}

void GameApplication::OnStop()
{
    //Clean up
    delete s_InputManager;
    delete s_ResourceManager;

    glfwTerminate();
}


void GameApplication::FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    
    GameApplication::s_ScreenWidth = width;
    GameApplication::s_ScreenHeight = height;

    s_AspectRatio = (float)width/height;
    s_ProjectionMatrix = glm::perspective(s_Fov, s_AspectRatio, s_NearPlane, s_FarPlane);
    s_OrthographicMatrix = glm::ortho(0.0f, s_AspectRatio, 0.0f, 1.0f);
}

GLFWwindow* const GameApplication::GetWindow()
{
    return s_Window;
}

InputManager* const GameApplication::GetInputManager()
{
    return s_InputManager;
}

ResourceManager* const GameApplication::GetResourceManager()
{
    return s_ResourceManager;
}

const glm::mat4& GameApplication::GetProjection()
{
    return s_ProjectionMatrix;
}

const glm::mat4& GameApplication::GetOrthoProjection()
{
    return s_OrthographicMatrix;
}

glm::uvec2 GameApplication::GetWindowSize()
{
    return glm::uvec2(s_ScreenWidth, s_ScreenHeight);
}
 
glm::vec2 GameApplication::GetProjectionRange()
{
    return glm::vec2(s_NearPlane, s_FarPlane);
}

void GameApplication::SetProjectionRange(float nearPlane, float farPlane)
{
    s_NearPlane = nearPlane;
    s_FarPlane = farPlane;
    s_ProjectionMatrix = glm::perspective(s_Fov, (float)s_ScreenWidth/s_ScreenHeight, s_NearPlane, s_FarPlane);
}

float GameApplication::GetFov()
{
    return s_Fov;
}

void GameApplication::SetFov(float fov)
{
    s_Fov = fov;
    s_ProjectionMatrix = glm::perspective(s_Fov, (float)s_ScreenWidth/s_ScreenHeight, s_NearPlane, s_FarPlane);
}

float GameApplication::GetAspectRatio()
{
    return s_AspectRatio;
}

float GameApplication::GetTotalElapsedTime()
{
    return s_TotalElapsedTime;
}

float GameApplication::GetBright()
{
    return brightness;
}

float GameApplication::GetGamma()
{
    return gamma;
}

float GameApplication::GetContrast()
{
    return contrast;
}

float GameApplication::GetContrastWithMod()
{
    return contrast + contrastMod;
}

void GameApplication::SetBright(float b)
{
    brightness = b;
}

void GameApplication::SetGamma(float g)
{
    gamma = g;
}

void GameApplication::SetContrast(float c)
{
    contrast = c;
}

void GameApplication::SetContrastMod(float cm)
{
    contrastMod = cm;
}

void GameApplication::LoadSceneLevel(int newLevelIndex)
{
    s_Scene->SafeSwitchLevel(newLevelIndex);
}