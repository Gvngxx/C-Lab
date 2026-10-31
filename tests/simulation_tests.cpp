#include "simulation/Simulation.h"

#include <cassert>
#include <cmath>
#include <string>

using namespace microverse;

namespace {

bool Near(double left, double right) {
    return std::abs(left - right) < 1e-9;
}

void TestEnergyConservationAndAbsorptionRate() {
    Simulation simulation;
    const double initial = simulation.TotalAccountedEnergy();
    simulation.Step(0.25);
    assert(Near(simulation.MugenNoMe().storedEnergy, 2.5));
    assert(Near(simulation.TotalAccountedEnergy(), initial));
}

void TestStorageCapacityAndTransfer() {
    SimulationConfig config;
    config.mugenCapacity = 3.0;
    Simulation simulation(config);
    simulation.Step(1.0);
    assert(Near(simulation.MugenNoMe().storedEnergy, 3.0));

    const OperationResult transfer = simulation.TransferToTenkaikai(1.0);
    assert(transfer.success);
    assert(Near(transfer.energyTransferred, 3.0));
    assert(Near(simulation.MugenNoMe().storedEnergy, 0.0));
    assert(Near(simulation.Tenkaikai().storedEnergy, 3.0));
}

void TestFailedOperationsDoNotMutateState() {
    Simulation simulation;
    const double sourceBefore = simulation.BlessedSource().quantity;
    const OperationResult failed = simulation.FormBarrier();
    assert(!failed.success);
    assert(!failed.reason.empty());
    assert(Near(simulation.BlessedSource().quantity, sourceBefore));
    assert(!simulation.BarrierFormed());
}

void TestRangeValidation() {
    Simulation simulation;
    simulation.SetPlayerPosition({100.0, 0.0, 0.0});
    const auto results = simulation.Step(0.1);
    assert(!results.empty());
    assert(!results.front().success);
    assert(results.front().reason.find("outside") != std::string::npos);
    assert(!simulation.InspectCursedObject().success);
}

void TestExternalConfiguration() {
    const SimulationConfig config = SimulationConfig::Load("assets/config/microverse.json");
    assert(Near(config.passiveAbsorptionRadius, 20.0));
    assert(Near(config.mugenAbsorptionRate, 10.0));
    assert(config.curseFallback == "suspended");
}

void TestBarrierRequirementsAndCurseUpkeep() {
    Simulation simulation;
    simulation.SetEyeActive(EyeId::MugenNoMe, false);
    simulation.Step(10.0);
    simulation.SetEyeActive(EyeId::MugenNoMe, true);
    simulation.Step(1.0);
    simulation.TransferToTenkaikai(1.0);
    const OperationResult barrier = simulation.FormBarrier();
    assert(barrier.success);
    assert(simulation.BarrierFormed());
    assert(Near(simulation.BarrierEnergy(), simulation.Config().barrierCost));

    const double cursedBefore = simulation.CursedObject().quantity;
    simulation.Step(0.5);
    assert(Near(simulation.CursedObject().quantity, cursedBefore - 0.25));
    assert(simulation.TestCurse().active);
}

void TestBarrierStopsPhysicalObject() {
    Simulation simulation;
    simulation.Step(1.0);
    assert(simulation.TransferToTenkaikai(0.5).success);
    assert(simulation.FormBarrier().success);
    assert(simulation.LaunchTestObject().success);
    simulation.Step(1.0);
    assert(simulation.TestObject().blockedByBarrier);
    assert(Near(simulation.TestObject().position.x, 2.0));
}

void TestCurseFallbackOnEnergyDepletion() {
    SimulationConfig config;
    config.cursedObjectQuantity = 0.1;
    config.curseUpkeepRate = 0.5;
    config.blessedSourceQuantity = 0.0;
    Simulation simulation(config);
    const auto results = simulation.Step(1.0);
    assert(!simulation.TestCurse().active);
    assert(!results.empty());
    assert(results.back().reason.find("suspended") != std::string::npos);
    assert(Near(simulation.TotalAccountedEnergy(), simulation.InitialEnergy()));
}

void TestRepeatability() {
    Simulation first;
    Simulation second;
    for (int tick = 0; tick < 120; ++tick) {
        first.Step(1.0 / 60.0);
        second.Step(1.0 / 60.0);
    }
    assert(Near(first.MugenNoMe().storedEnergy, second.MugenNoMe().storedEnergy));
    assert(Near(first.CursedObject().quantity, second.CursedObject().quantity));
    assert(Near(first.TotalAccountedEnergy(), second.TotalAccountedEnergy()));
}

} // namespace

int main() {
    TestEnergyConservationAndAbsorptionRate();
    TestStorageCapacityAndTransfer();
    TestFailedOperationsDoNotMutateState();
    TestRangeValidation();
    TestExternalConfiguration();
    TestBarrierRequirementsAndCurseUpkeep();
    TestBarrierStopsPhysicalObject();
    TestCurseFallbackOnEnergyDepletion();
    TestRepeatability();
}