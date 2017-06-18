#ifndef PROJECT_MINKOWSKI_SIMULATION_H
#define PROJECT_MINKOWSKI_SIMULATION_H

#include <physics/physics_simulation.h>

#include <memory>

namespace ifx{
class GameObject;
class SceneContainer;
}

struct SceneObjects {
    std::shared_ptr<ifx::GameObject> observer_axis;
    std::shared_ptr<ifx::GameObject> worldline_light_lines;
};

struct Event{
    float ct;
    float d;
    float v;

    float init_ct;
    float init_d;

    std::shared_ptr<ifx::GameObject> game_object;
};

class MinkowskiSimulation : public ifx::PhysicsSimulation {
public:

    MinkowskiSimulation();

    ~MinkowskiSimulation();

    float* velocity(){return &velocity_;}

    void SetVelocity(float velocity);
    float GetVelocity();

    virtual void Update(float time_delta) override;

    virtual void SetGravity(const glm::vec3& g) override;
    virtual glm::vec3 GetGravity() override;
    virtual void AddImpulse() override;

    void Init(std::shared_ptr<ifx::SceneContainer> scene);

    /**
     * Adds event relative to other observer. Meaning that the Origin Lorentz
     * Transformation will be calculated to get the coordinates of this event
     * in the main (center) obersever coordinate system.
     * @param ct
     * @param d
     * @param v
     */
    void AddEventRelativeToOtherObserver(Event& event);

private:
    void CalculateObservedEvent(Event& event, bool first);
    void UpdateTimePosition(Event& event, float time_delta);

    float velocity_;
    float old_velocity_;

    std::shared_ptr<ifx::SceneContainer> scene_;

    std::vector<Event> events_;

    SceneObjects scene_objects_;
};


#endif //PROJECT_MINKOWSKI_SIMULATION_H
