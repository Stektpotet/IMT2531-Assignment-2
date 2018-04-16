#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define TINYFILES_IMPLEMENTATION

#include <overkill/Config.hpp>
#include <overkill/Init.hpp>
#include <overkill/Input.hpp>
#include <overkill/gl_caller.hpp>
#include <overkill/Renderer.hpp>

#include <overkill/TextureSystem.hpp>
#include <overkill/ShaderSystem.hpp>
#include <overkill/MaterialSystem.hpp>
#include <overkill/ModelSystem.hpp>

#define DEBUG 1

using namespace overkill;


int main()
{

	auto window = Init::GLFW(
        C::VersionMajor, 
        C::VersionMinor, 
        C::WinWidth, 
        C::WinHeight,
        C::WinName);

    Init::GLEW();
    Init::OpenGL(C::ClearColor); //(0.05f, 0.06f, 0.075f, 1.0f) for sexy dark blue-grey
    Init::Watcher();

    TextureSystem::load();
    ShaderSystem::load();
    MaterialSystem::load();
    ModelSystem::load();

    auto model    = ModelSystem::getByTag("cube");
    ShaderProgram shader = model.m_meshes[0].m_shaderProgram;
    auto material = MaterialSystem::getById(model.m_meshes[0].m_materialID);


    auto renderer = EdgeRenderer();
    

    //SCALE -> ROTATE -> TRANSLATE
    glm::mat4 projection = glm::perspective(C::FOV, C::AspectRatio, C::NearClip, C::FarClip);
    glm::mat4 camera = glm::mat4(1); 
    glm::mat4 pivot = glm::translate(glm::mat4(1),glm::vec3(0, 0, C::CameraOffset));  //Camera pos in world.
    glm::mat4 view = glm::mat4(1);


    //GLCall(glSetUn)
    GLint uniformMVP, uniformTime;
    GLint uniformMVP2, uniformTime2;
    GLint uniformView;                                  //Will communicate camera orientation to shader.


    shader.bind();
    uniformMVP  = shader.getUniformLocation("projection");
    uniformTime = shader.getUniformLocation("time");
    uniformView = shader.getUniformLocation("view");

    GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
    shader.setMaterial(material);

    float twoSecondTick = 0.0f;
    auto everyTwoSeconds = [&twoSecondTick](float t){

        if (t - twoSecondTick > 2.0f) {
            //Watcher::scanFiles();
            MaterialSystem::reload();
            twoSecondTick += 2;
        }
    };

    for(;;)
    {
        float t = glfwGetTime();
        if ((glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window) != 0))
            break;

        renderer.clear();
        renderer.draw(model.m_vao, model.m_meshes[0].m_ebo, shader);

		//@TODO shader.bindDynamic()
        projection = glm::perspective(Input::m_fovy, C::AspectRatio, 0.1f, -100.0f);
        camera = glm::rotate(glm::mat4(1), (C::LookSensitivity * Input::m_camRotX / C::WinWidth), glm::vec3(0.0f, 1.0f, 0.0f));
        camera = glm::rotate(glm::mat4(1), (C::LookSensitivity * Input::m_camRotY / C::WinHeight), glm::vec3(1.0f, 0.0f, 0.0f)) * camera;
        pivot = glm::translate(glm::mat4(1),glm::vec3(Input::m_camPanX, Input::m_camPanY, C::CameraOffset));  //Camera pos in world.
        
        view = pivot * camera;
        glm::inverse(view); //To reverse both axis, so controls are not reverse.


        shader.bind();
        GLCall(glUniformMatrix4fv(uniformMVP, 1, GL_FALSE, glm::value_ptr(projection)));
        GLCall(glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view)));
        GLCall(glUniform1f(uniformTime, (float)glfwGetTime()));

        glfwSwapBuffers(window);
        glfwPollEvents();

        everyTwoSeconds(t);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
	return 0;
}
