#ifndef App_h
#define App_h

#include "BaseApp.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "TurntableManipulator.h"

namespace basicgraphics {
class App : public BaseApp {
public:
  
    App(int argc, char** argv, std::string windowName, int windowWidth, int windowHeight);
    ~App(){};
  
    void onRenderGraphics() override;
    void onEvent(std::shared_ptr<Event> event) override;

  
protected:
    
    virtual void reloadShaders();
    
    glm::mat4 viewportMat;
    
    GLSLProgram shader;
    GLSLProgram normalShader;
    
    std::unique_ptr<Model> modelMesh;
    std::shared_ptr<TurntableManipulator> turntable;
    
    glm::vec4 lightPosition;
  
};
}
#endif
