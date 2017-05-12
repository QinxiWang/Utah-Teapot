#include "App.h"
#include <iostream>

namespace basicgraphics{

using namespace std;
using namespace glm;

App::App(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight) : BaseApp(argc, argv, windowName, windowWidth, windowHeight) {

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    float w2 = _windowWidth / 2.0f;
    float h2 = _windowHeight / 2.0f;
    viewportMat = mat4(w2, 0.0f, 0.0f, 0.0f,
                          0.0f, h2, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          w2, h2, 0.0f, 1.0f);



    // This load shaders from disk, we do it once when the program starts
    // up, but since you don't need to recompile to reload shaders, you can
    // even do this inteactively as you debug your shaders!  Press the R
    // key to reload them while your program is running!
    reloadShaders();
    
    // This loads the model from a file and initializes an instance of the model class to store it
    modelMesh.reset(new Model("teapot.obj", 0.035f, vec4(1.0)));
    
    turntable.reset(new TurntableManipulator(6, 1.5, 0.23));
    turntable->setCenterPosition(vec3(0, 0 ,0));
    
    float radius = 20.0;
    lightPosition = vec4(-1.7*radius, 0.3*radius, 1.0*radius, 1.0);
    
}
    
void App::reloadShaders()
{
    
    shader.compileShader("BlinnPhong.vert", GLSLShader::VERTEX);
    shader.compileShader("BlinnPhong.geom", GLSLShader::GEOMETRY);
    shader.compileShader("BlinnPhong.frag", GLSLShader::FRAGMENT);
    shader.link();

    
    normalShader.compileShader("normals.vert", GLSLShader::VERTEX);
    normalShader.compileShader("normals.geom", GLSLShader::GEOMETRY);
    normalShader.compileShader("normals.frag", GLSLShader::FRAGMENT);
    normalShader.link();

}
    

void App::onEvent(shared_ptr<Event> event)
{
    turntable->onEvent(event);
}

void App::onRenderGraphics() {
    
    // Setup the camera with a good initial position and view direction to see the table
    glm::mat4 view = turntable->frame();
    glm:mat4 model(1.0);
    // Setup the projection matrix so that things are rendered in perspective
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)_windowWidth / (GLfloat)_windowHeight, 0.1f, 100.0f);
    shader.use(); // Tell opengl we want to use this specific shader.
    shader.setUniform("view_mat", view);
    shader.setUniform("projection_mat", projection);
    shader.setUniform("model_mat", model);
    shader.setUniform("normal_mat", mat3(transpose(inverse(model))));
    vec3 eyePosition = turntable->getPos();
    shader.setUniform("eye_world", eyePosition);
    
    
    vec3 ambientReflectionCoeff = vec3(0.4, 0.4, 0.4);
    vec3 diffuseReflectionCoeff = vec3(0.8, 0.8, 0.8);
    vec3 specularReflectionCoeff = vec3(1.0, 1.0, 1.0);
    float specularExponent = 40.9;
    
    vec3 ambientLightIntensity = vec3(0.4, 0.4, 0.4);
    vec3 diffuseLightIntensity = vec3(0.6, 0.6, 0.6);
    vec3 specularLightIntensity = vec3(1.0, 1.0, 1.0);
    
    // Set shader material properties (k coeffs)
    shader.setUniform("ambientReflectionCoeff", ambientReflectionCoeff);
    shader.setUniform("diffuseReflectionCoeff", diffuseReflectionCoeff);
    shader.setUniform("specularReflectionCoeff", specularReflectionCoeff);
    shader.setUniform("specularExponent", specularExponent);
    
    // Set shader light properties (intensity and position)
    shader.setUniform("lightPosition", lightPosition);
    shader.setUniform("ambientLightIntensity", ambientLightIntensity);
    shader.setUniform("diffuseLightIntensity", diffuseLightIntensity);
    shader.setUniform("specularLightIntensity", specularLightIntensity);

    shader.setUniform("viewportMatrix", viewportMat);
    
    shader.setUniform("lineWidth", 0.25f);
    shader.setUniform("lineColor", vec4(0.0f, 1.0f, 0.0f, 1.0f));

    // Draw the model
    modelMesh->draw(shader);
    
    // Draw normals
    normalShader.use();
    normalShader.setUniform("normalLength", 0.2f);
    normalShader.setUniform("view_mat", view);
    normalShader.setUniform("projection_mat", projection);
    normalShader.setUniform("model_mat", model);
    modelMesh->draw(normalShader);

}
}//namespace




