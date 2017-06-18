#ifndef PROJECT_EXAMPLE_GUI_H
#define PROJECT_EXAMPLE_GUI_H

#include "gui/gui.h"

#include <memory>

class MinkowskiSimulation;

namespace ifx{
class EngineGUI;
class SceneContainer;
}

class MinkowskiGUI : public ifx::GUI{
public:

    MinkowskiGUI(GLFWwindow* window,
                std::shared_ptr<ifx::SceneContainer> scene,
                std::shared_ptr<MinkowskiSimulation> simulation);
    ~MinkowskiGUI();

    virtual void Render() override;
    virtual void RenderSceneObjects() override;
private:
    void RenderMinkowskiGUI();

    std::shared_ptr<ifx::EngineGUI> engine_gui_;

    std::shared_ptr<MinkowskiSimulation> simulation_;

};


#endif //PROJECT_EXAMPLE_GUI_H
