#include "gui/minkowski_gui.h"

#include <engine_gui/factory/engine_gui_factory.h>
#include <engine_gui/engine_gui.h>
#include <gui/imgui/imgui.h>
#include <simulation/minkowski_simulation.h>
#include <physics/physics_simulation.h>
#include <random>

MinkowskiGUI::MinkowskiGUI(GLFWwindow* window,
                 std::shared_ptr<ifx::SceneContainer> scene,
                 std::shared_ptr<MinkowskiSimulation> simulation) :
        ifx::GUI(window),
        simulation_(simulation){
    engine_gui_ = ifx::EngineGUIFactory().CreateEngineGUI(scene,
                                                          simulation);
}

MinkowskiGUI::~MinkowskiGUI(){}

void MinkowskiGUI::Render(){
    NewFrame();

    engine_gui_->Render();

    RenderMinkowskiGUI();

    ImGui::Render();
}

void MinkowskiGUI::RenderSceneObjects(){
    engine_gui_->RenderSceneObjects();
}

void MinkowskiGUI::RenderMinkowskiGUI(){
    ImGui::Begin("Minkowski");

    if(ImGui::CollapsingHeader("Minkowski"), 1){
        if(ImGui::Button("Reset")){
            simulation_->Reset();
        }

        if(ImGui::TreeNode("Center Observer")){
            float velocity = simulation_->GetVelocity();
            if(ImGui::SliderFloat("Relative Velocity * c", &velocity,
                                  -0.99, 0.99)){
                simulation_->SetVelocity(velocity);
            }
            ImGui::TreePop();
        }

        if(ImGui::TreeNode("New Event")) {
            static float ct = 3;
            static float d = 4;
            ImGui::SliderFloat("ct\'", &ct, -5, 5);
            ImGui::SliderFloat("d\'", &d, -5, 5);

            if (ImGui::Button("Add Event")) {
                Event event{ct, d};
                simulation_->AddEventRelativeToOtherObserver(event);
            }

            ImGui::TreePop();
        }


        if(ImGui::TreeNode("Random Events")) {
            static int count = 100;
            ImGui::InputInt("Count", &count, 1, 100);

            static float axis_min = -10;
            static float axis_max = 10;

            ImGui::SliderFloat("axis min", &axis_min, -10, 10);
            ImGui::SliderFloat("axis_max", &axis_max, -10, 10);

            std::uniform_real_distribution<> dist(axis_min, axis_max);
            std::random_device rd;
            std::mt19937 e2(rd());

            if (ImGui::Button("Generate")) {
                for(int i = 0; i < count; i++){
                    float ct = dist(e2);
                    float d = dist(e2);

                    Event event{ct, d};
                    simulation_->AddEventRelativeToOtherObserver(event);
                }
            }

            ImGui::TreePop();
        }
    }


    ImGui::End();
}