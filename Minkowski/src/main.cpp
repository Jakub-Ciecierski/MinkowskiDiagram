#include <game/factory/game_factory.h>
#include <game/game.h>
#include <game/game_object.h>
#include <game/game_loop.h>
#include <game/scene_container.h>
#include <game/components/cameras/camera_component.h>

#include <graphics/rendering/renderer.h>

#include <simulation/minkowski_simulation.h>
#include <simulation/factory/minkowski_simulation_factory.h>
#include <gui/minkowski_gui.h>
#include <game/components/cameras/factory/camera_factory.h>
#include <game/components/lights/factory/light_component_factory.h>
#include <game/components/lights/light_directional_component.h>

int main(){
    auto game_factory
            = std::shared_ptr<ifx::GameFactory>(new ifx::GameFactory());
    game_factory->SetPhysicsSimulationFactory
            (std::make_shared<MinkowskiSimulationFactory>());

    auto game = game_factory->Create();

    auto simulation = std::static_pointer_cast<MinkowskiSimulation>(
            game->game_loop()->physics_simulation());
    simulation->Init(game->scene());

    auto gui = std::shared_ptr<MinkowskiGUI>(
            new MinkowskiGUI(
                    game->game_loop()->renderer()->window()->getHandle(),
                    game->scene(),
                    simulation));

    auto camera = ifx::CameraFactory().CreateCamera(
            game->game_loop()->renderer()->window());
    camera->moveTo(glm::vec3(0,0,15));
    camera->rotateTo(glm::vec3(-90,0,0));

    game->game_loop()->renderer()->SetGUI(gui);
    auto game_object = game->scene()->CreateAndAddEmptyGameObject();
    game_object->Add(camera);

    // Light
    auto game_object_lights = game->scene()->CreateAndAddEmptyGameObject();
    game_object_lights->Add(
            ifx::LightComponentFactory().CreateDirectionalLight());

    game->Start();
}