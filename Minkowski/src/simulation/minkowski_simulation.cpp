#include "simulation/minkowski_simulation.h"

#include <game/scene_container.h>
#include <simulation/factory/scene_object_factory.h>
#include <game/game_object.h>
#include <game/components/render/render_component.h>
#include <iostream>

MinkowskiSimulation::MinkowskiSimulation() :
        scene_(nullptr),
        velocity_(0.5),
        old_velocity_(velocity_){}

MinkowskiSimulation::~MinkowskiSimulation(){}

void MinkowskiSimulation::Update(float time_delta) {
    if(!is_running_)
        return;
    for(auto& event : events_){
        UpdateTimePosition(event, time_delta);
    }
    UpdateWorldLine();
}

void MinkowskiSimulation::SetVelocity(float velocity){
    old_velocity_ = velocity_;
    velocity_ = velocity;
    for(auto& event : events_){
        CalculateObservedEvent(event, false);
    }
}

float MinkowskiSimulation::GetVelocity(){
    return velocity_;
}

void MinkowskiSimulation::SetGravity(const glm::vec3& g){

}

glm::vec3 MinkowskiSimulation::GetGravity(){

}

void MinkowskiSimulation::AddImpulse(){

}

void MinkowskiSimulation::Init(std::shared_ptr<ifx::SceneContainer> scene){
    scene_ = scene;
    scene_objects_.observer_axis = scene->CreateAndAddEmptyGameObject();
    auto ct_axis
            = SceneObjectFactory().CreateLine(glm::vec3(0, 0, 0),
                                              glm::vec3(0, 10, 0),
                                              glm::vec3(255, 0, 0));
    auto d_axis
            = SceneObjectFactory().CreateLine(glm::vec3(0, 0, 0),
                                              glm::vec3(10, 0, 0),
                                              glm::vec3(255, 0, 0));
    auto ct_prime_axis
            = SceneObjectFactory().CreateLine(glm::vec3(0, 0, 0),
                                              glm::vec3(0, 4, 0),
                                              glm::vec3(0, 255, 0));
    scene_objects_.observer_axis->Add(ct_prime_axis);
    scene_objects_.observer_axis->Add(ct_axis);
    scene_objects_.observer_axis->Add(d_axis);

    scene_objects_.worldline_light_lines = scene->CreateAndAddEmptyGameObject();
    float a = 2.0f / sqrt(2.0f) * 10;
    auto worldline_light1
            = SceneObjectFactory().CreateLine(glm::vec3(-a, -a, 0),
                                              glm::vec3(a, a, 0),
                                              glm::vec3(255, 255, 0));
    auto worldline_light2
            = SceneObjectFactory().CreateLine(glm::vec3(-a, a, 0),
                                              glm::vec3(a, -a, 0),
                                              glm::vec3(255, 255, 0));

    scene_objects_.worldline_light_lines->Add(worldline_light1);
    scene_objects_.worldline_light_lines->Add(worldline_light2);
}

void MinkowskiSimulation::AddEventRelativeToOtherObserver(Event& event){
    event.game_object = scene_->CreateAndAddEmptyGameObject();
    event.observer_game_object = scene_->CreateAndAddEmptyGameObject();

    event.game_object->Add(SceneObjectFactory().
            CreateCircle(0.1, glm::vec3(255,255,255)));

    CalculateObservedEvent(event);

    events_.push_back(event);
}

void MinkowskiSimulation::Reset(){
    for(auto& event : events_){
        scene_->Remove(event.observer_game_object);
        scene_->Remove(event.game_object);
    }
    events_.clear();
}

void MinkowskiSimulation::CalculateObservedEvent(Event& event, bool first){
    float relative_velocity = (velocity_);
    if(relative_velocity == 0)
        relative_velocity = 0.01f;

    float relative_velocity_sqr = relative_velocity * relative_velocity;
    float sqrt_inverse = sqrt(1 - relative_velocity_sqr);

    // Inverse Lorentz
    if(!first){
        float old_relative_velocity = (old_velocity_);
        float ct_tmp = event.ct;
        event.ct = (event.ct - (old_relative_velocity * event.d)) / sqrt_inverse;
        event.d = (event.d - (ct_tmp * old_relative_velocity)) / sqrt_inverse;
    }

    // Lorentz
    event.init_ct = ((relative_velocity * event.d) + event.ct) / sqrt_inverse;
    event.init_d = (event.d + (relative_velocity * event.ct)) / sqrt_inverse;

    event.ct = event.init_ct;
    event.d = event.init_d;
}

void MinkowskiSimulation::UpdateTimePosition(Event& event, float time_delta){
    float relative_velocity = (velocity_);
    if(relative_velocity == 0)
        relative_velocity = 0.01f;

    event.ct = event.ct - time_delta;
    //auto delta = (event.init_ct - event.ct) / event.init_ct;
    //event.d = event.init_d - (delta * event.init_d);

    event.game_object->moveTo(glm::vec3(event.d, event.ct, 0));

}

void MinkowskiSimulation::UpdateWorldLine(){
    auto ct_prime = std::static_pointer_cast<ifx::RenderComponent>(
            scene_objects_.observer_axis->GetComponents()[0]);
    ct_prime->rotateTo(glm::vec3(0,0, velocity_*(-45.0f)));
}