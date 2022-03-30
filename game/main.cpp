#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
namespace rj = rapidjson;

#include <AL/al.h>
#include <AL/alc.h>

#include "InputManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Components.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
uint32_t windowSizeX = 800, windowSizeY = 600;

int main(void)
{    
    ALCdevice *device;
    device = alcOpenDevice(NULL);
    if(!device)
    {
        return -7;
    }


    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    rj::Document d;
    d.Parse(json);

    rj::Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    rj::StringBuffer buffer;
    rj::Writer<rj::StringBuffer> writer(buffer);
    d.Accept(writer);

    std::cout << buffer.GetString() << std::endl;
	

    glfwInit();
    const char* glsl_version = "#version 450";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(windowSizeX, windowSizeY, "OpenGL", NULL, NULL);
    if(window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(false);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    InputManager inputManager(window);
    ResourceManager* resMan = new ResourceManager();

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -2;
    }

    glEnable(GL_DEPTH_TEST);
	
	FT_Library ft;
	if(FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -3;
	}

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ///***

    ModelInstancesComponent* mic = new ModelInstancesComponent();
    mic->Create(9, 
            resMan->GetMesh("Resources/models/Crate/Crate.obj"),
            resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    ShaderComponent *shader = new ShaderComponent();
    shader->Create("Resources/shaders/inst.vert", "Resources/shaders/inst.frag");
    for(int x=-4, y=-4, i=0; i<9; i++)
    {
        mic->SetTransformation(i, TransformComponent::Transform(glm::vec3(x, -4, y), glm::vec3(0, 0, 0), 1));
        x += 4;
        if((x + 1) % 3 == 0)
        {
            x = -4;
            y += 4;
        }
    }
    mic->UpdateTransformations();

    ///***

    ModelComponent* mc = new ModelComponent();
    mc->Create(
        resMan->GetMesh("Resources/models/Crate/Crate.obj"),
        resMan->GetMaterial("Resources/models/Crate/Crate.mtl")
    );
    ShaderComponent *shader_d = new ShaderComponent();
    shader_d->Create("Resources/shaders/default.vert", "Resources/shaders/default.frag");
    TransformComponent* tc = new TransformComponent();


    GameObject go;
    go.AddComponent(shader_d);
    go.AddComponent(mc);
    go.AddComponent(tc);

    ///***

    float radius = 10.0f;
    float camX = 0, camZ = 0;
    glm::mat4 projection, view;
    glm::mat4 transform;

    while(!glfwWindowShouldClose(window))
    {
        if(inputManager.Keyboard()->OnPressed(KeyboardKey::Escape_KB))
        {
            glfwSetWindowShouldClose(window, true);
        }

        camX = sin(glfwGetTime()) * radius;
        camZ = cos(glfwGetTime()) * radius;
        projection = glm::perspective(glm::radians(45.0f), (float)windowSizeX / (float)windowSizeY, 0.1f, 1000.0f);
        view = glm::lookAt(glm::vec3(camX, 5.f, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        transform = projection * view * glm::mat4(1.f);

        glClearColor(0.08f, 0.2f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->Use();
        shader->SetMat4("transform", transform);
        mic->Draw(shader);

        go.GetComponent<cmp::Transform>()->Rotate(0, 1, 0);
        go.GetComponent<cmp::Shader>()->Use();
        go.GetComponent<cmp::Shader>()->SetMat4("transform", 
            transform * go.GetComponent<cmp::Transform>()->GetModelMatrix());
        go.GetComponent<cmp::Model>()->Draw(go.GetComponent<cmp::Shader>());

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete resMan;

    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    windowSizeX = width;
    windowSizeY = height;
    glViewport(0, 0, windowSizeX, windowSizeY);
}
