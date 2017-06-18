#ifndef PROJECT_MINKOWSKI_SIMULATION_FACTORY_H
#define PROJECT_MINKOWSKI_SIMULATION_FACTORY_H

#include <physics/factory/physics_simulation_factory.h>

namespace ifx{
class PhysicsSimulation;
}

class MinkowskiSimulationFactory : public ifx::PhysicsSimulationFactory {
public:

    MinkowskiSimulationFactory();

    ~MinkowskiSimulationFactory();

    virtual std::shared_ptr<ifx::PhysicsSimulation> Create() override;
private:
};


#endif //PROJECT_MINKOWSKI_SIMULATION_FACTORY_H
