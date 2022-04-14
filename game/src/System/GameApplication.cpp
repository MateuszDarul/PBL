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





#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600
#define DEFAULT_FOV 83.0f

int GameApplication::s_ScreenWidth = DEFAULT_SCREEN_WIDTH;
int GameApplication::s_ScreenHeight = DEFAULT_SCREEN_HEIGHT;
GLFWwindow* GameApplication::s_Window = nullptr;
glm::mat4 GameApplication::s_ProjectionMatrix = glm::perspective(DEFAULT_FOV, (float)DEFAULT_SCREEN_WIDTH/DEFAULT_SCREEN_HEIGHT, 0.1f, 1000.0f);

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

        glClearColor(0.78f, 0.21f, 0.18f, 1.0f);
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

    s_ProjectionMatrix = glm::perspective(DEFAULT_FOV, (float)width/height, 0.1f, 1000.0f);
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