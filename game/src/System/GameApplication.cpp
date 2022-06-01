#include "GameApplication.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Scene.h"

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


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include <iostream>





#define DEFAULT_FOV 83.0f
#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600
#define DEFAULT_NEAR_PLANE 0.1f
#define DEFAULT_FAR_PLANE 1000.f

GLFWwindow* GameApplication::s_Window = nullptr;

float GameApplication::s_Fov = DEFAULT_FOV;
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
InputManager* GameApplication::s_InputManager = nullptr;
ResourceManager* GameApplication::s_ResourceManager = nullptr;


int GameApplication::Init()
{
    //Load libraries

    glfwInit();
    const char* glsl_version = "#version 450";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    s_Window = glfwCreateWindow(s_ScreenWidth, s_ScreenHeight, "Enlite Game Engine", NULL, NULL);
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





    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(s_Window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
	



    //Create managers / systems

    s_InputManager = new InputManager(s_Window);
    s_ResourceManager = new ResourceManager();


    //Load scene
     //Scene OnCreate
    s_Scene = new Scene();





    return 0;
}

void GameApplication::Run()
{
    double t1, t2 = glfwGetTime();
    double dt;
    while (!glfwWindowShouldClose(s_Window))
    {
        if(s_InputManager->Keyboard()->OnPressed(KeyboardKey::Escape_KB))
        {
            glfwSetWindowShouldClose(s_Window, true);
        }

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //get time step
        t1 = glfwGetTime();
        dt = t1 - t2;
        t2 = t1;

        //update logic
        s_Scene->Update(dt);

        //show scene
        s_Scene->Render();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("GUI");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(s_Window);
    }

    OnStop();
}

void GameApplication::OnStop()
{
    //Clean up

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
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

glm::vec3 GameApplication::GetWindowSize()
{
    return glm::vec3(s_ScreenWidth, s_ScreenHeight);
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