#include "simulation/factory/minkowski_simulation_factory.h"

#include <simulation/minkowski_simulation.h>

MinkowskiSimulationFactory::MinkowskiSimulationFactory() {}

MinkowskiSimulationFactory::~MinkowskiSimulationFactory(){}

std::shared_ptr<ifx::PhysicsSimulation> MinkowskiSimulationFactory::Create(){
    auto minkowski_simulation = std::make_shared<MinkowskiSimulation>();
    return minkowski_simulation;
}