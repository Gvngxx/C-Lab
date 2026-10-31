#include "Simulation.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <utility>

#include <json.hpp>

namespace microverse {
namespace {

double Distance(const Vector3& left, const Vector3& right) {
    const double x = left.x - right.x;
    const double y = left.y - right.y;
    const double z = left.z - right.z;
    return std::sqrt(x * x + y * y + z * z);
}

} // namespace

SimulationConfig SimulationConfig::Load(const std::string& path) {
    SimulationConfig config;
    std::ifstream file(path);
    if (!file) {
        return config;
    }

    const auto values = nlohmann::json::parse(file);
    config.passiveAbsorptionRadius = values.value("passive_absorption_radius", config.passiveAbsorptionRadius);
    config.directManipulationRange = values.value("direct_manipulation_range", config.directManipulationRange);
    config.cursedDetectionRange = values.value("cursed_detection_range", config.cursedDetectionRange);
    config.mugenCapacity = values.value("mugen_capacity", config.mugenCapacity);
    config.mugenAbsorptionRate = values.value("mugen_absorption_rate", config.mugenAbsorptionRate);
    config.mugenTransferRate = values.value("mugen_transfer_rate", config.mugenTransferRate);
    config.tenkaikaiCapacity = values.value("tenkaikai_capacity", config.tenkaikaiCapacity);
    config.barrierCost = values.value("barrier_cost", config.barrierCost);
    config.barrierMinimumCohesion = values.value("barrier_minimum_cohesion", config.barrierMinimumCohesion);
    config.barrierMinimumStability = values.value("barrier_minimum_stability", config.barrierMinimumStability);
    config.energyControlCost = values.value("energy_control_cost", config.energyControlCost);
    config.blessedSourceCohesion = values.value("blessed_source_cohesion", config.blessedSourceCohesion);
    config.blessedSourceStability = values.value("blessed_source_stability", config.blessedSourceStability);
    config.blessedSourceQuantity = values.value("blessed_source_quantity", config.blessedSourceQuantity);
    config.cursedObjectQuantity = values.value("cursed_object_quantity", config.cursedObjectQuantity);
    config.curseUpkeepRate = values.value("curse_upkeep_rate", config.curseUpkeepRate);
    config.curseFallback = values.value("curse_fallback", config.curseFallback);
    return config;
}

Simulation::Simulation(SimulationConfig config)
    : config_(std::move(config)) {
    mugen_.capacity = config_.mugenCapacity;
    mugen_.absorptionRate = config_.mugenAbsorptionRate;
    mugen_.transferRate = config_.mugenTransferRate;
    tenkaikai_.capacity = config_.tenkaikaiCapacity;
    tenkaikai_.transferRate = config_.mugenTransferRate;

    blessedSource_ = {"energy.blessed.source", EnergyType::Blessed, {0.0, 0.0, 0.0}, {}, 0.0,
        config_.blessedSourceQuantity, 1.0, config_.blessedSourceCohesion, config_.blessedSourceStability, 1.0};
    cursedObject_ = {"object.cursed.test", EnergyType::Cursed, {4.0, 0.0, 0.0}, {}, 0.0,
        config_.cursedObjectQuantity, 1.0, 0.6, 0.7, 1.0};
    testCurse_.upkeepRate = config_.curseUpkeepRate;
    testCurse_.fallback = config_.curseFallback;
    initialEnergy_ = config_.blessedSourceQuantity + config_.cursedObjectQuantity;
}

void Simulation::SetPlayerPosition(Vector3 position) {
    playerPosition_ = position;
}

std::vector<OperationResult> Simulation::Step(double fixedDeltaTime) {
    std::vector<OperationResult> results;
    if (fixedDeltaTime <= 0.0) {
        return results;
    }

    const double previousObjectX = testObject_.position.x;
    if (testObject_.launched && !testObject_.blockedByBarrier) {
        testObject_.position.x += testObject_.velocity.x * fixedDeltaTime;
        if (barrierFormed_ && previousObjectX > 2.0 && testObject_.position.x <= 2.0) {
            testObject_.position.x = 2.0;
            testObject_.velocity = {};
            testObject_.blockedByBarrier = true;
            OperationResult collision;
            collision.success = true;
            collision.conditions = {"Barrier exists", "Physical object crossed the barrier plane"};
            collision.stateChange = "Barrier stopped the physical test object";
            lastEvent_ = collision.stateChange;
            results.push_back(collision);
        }
    }

    blessedSource_.position.x += blessedSource_.velocity.x * fixedDeltaTime;
    blessedSource_.position.y += blessedSource_.velocity.y * fixedDeltaTime;
    blessedSource_.position.z += blessedSource_.velocity.z * fixedDeltaTime;

    if (mugen_.active) {
        OperationResult absorption = AbsorbBlessedEnergy(fixedDeltaTime);
        if (absorption.energyTransferred > 0.0 || !absorption.success) {
            results.push_back(absorption);
        }
    }

    if (testCurse_.active) {
        const double required = testCurse_.upkeepRate * fixedDeltaTime;
        if (cursedObject_.quantity + 1e-9 >= required) {
            cursedObject_.quantity -= required;
            dissipatedEnergy_ += required;
            OperationResult upkeep;
            upkeep.success = true;
            upkeep.energyConsumed = required;
            upkeep.conditions = {"Curse is active", "Cursed energy is available"};
            upkeep.stateChange = "Test curse sustained";
            results.push_back(upkeep);
        } else {
            const double available = cursedObject_.quantity;
            cursedObject_.quantity = 0.0;
            dissipatedEnergy_ += available;
            testCurse_.active = false;
            OperationResult fallback;
            fallback.success = false;
            fallback.energyConsumed = available;
            fallback.conditions = {"Curse upkeep due", "Insufficient cursed energy"};
            fallback.reason = "Cursed energy depleted; fallback applied: " + testCurse_.fallback;
            fallback.stateChange = "Curse deactivated";
            lastEvent_ = fallback.reason;
            results.push_back(fallback);
        }
    }
    return results;
}

OperationResult Simulation::SetEyeActive(EyeId eye, bool active) {
    EyeState& state = Eye(eye);
    state.active = active;
    OperationResult result;
    result.success = true;
    result.conditions = {"Eye state is independently configurable"};
    result.stateChange = active ? "Eye activated" : "Eye deactivated";
    lastEvent_ = result.stateChange;
    return result;
}

OperationResult Simulation::TransferToTenkaikai(double simulatedSeconds) {
    if (!tenkaikai_.active || !mugen_.active) {
        return Failure("Both eyes must be active", {"Tenkaikai active", "Mugen no Me active"});
    }
    if (simulatedSeconds <= 0.0) {
        return Failure("Transfer duration must be positive", {"Positive simulated duration"});
    }

    const double freeCapacity = tenkaikai_.capacity - tenkaikai_.storedEnergy;
    const double amount = std::min({mugen_.storedEnergy, tenkaikai_.transferRate * simulatedSeconds, freeCapacity});
    if (amount <= 0.0) {
        return Failure("No transferable energy or Tenkaikai storage is full", {"Stored Mugen energy", "Tenkaikai capacity"});
    }

    mugen_.storedEnergy -= amount;
    tenkaikai_.storedEnergy += amount;
    OperationResult result;
    result.success = true;
    result.energyTransferred = amount;
    result.conditions = {"Both eyes active", "Transfer rate respected", "Target capacity respected"};
    result.stateChange = "Blessed energy transferred to Tenkaikai";
    lastEvent_ = result.stateChange;
    return result;
}

OperationResult Simulation::ManipulateBlessedEnergy(const std::string& cloudId, Vector3 velocity) {
    if (!tenkaikai_.active) {
        return Failure("Tenkaikai is inactive", {"Tenkaikai active"});
    }
    if (cloudId != blessedSource_.id) {
        return Failure("Energy cloud was not found", {"Target identifier exists"});
    }
    if (DistanceTo(blessedSource_.position) > config_.directManipulationRange + blessedSource_.interactionRadius) {
        return Failure("Target is outside direct manipulation range", {"Target within configured range"});
    }
    const double controlCost = config_.energyControlCost;
    if (tenkaikai_.storedEnergy < controlCost) {
        return Failure("Insufficient blessed energy in Tenkaikai", {"Available energy >= control cost"});
    }

    tenkaikai_.storedEnergy -= controlCost;
    dissipatedEnergy_ += controlCost;
    blessedSource_.velocity = velocity;
    OperationResult result;
    result.success = true;
    result.energyConsumed = controlCost;
    result.conditions = {"Tenkaikai active", "Target identified", "Target within range", "Control cost available"};
    result.stateChange = "Blessed energy velocity changed";
    lastEvent_ = result.stateChange;
    return result;
}

OperationResult Simulation::FormBarrier() {
    if (!tenkaikai_.active) {
        return Failure("Tenkaikai is inactive", {"Tenkaikai active"});
    }
    if (barrierFormed_) {
        return Failure("A barrier already exists", {"No existing barrier"});
    }
    if (DistanceTo(blessedSource_.position) > config_.directManipulationRange + blessedSource_.interactionRadius) {
        return Failure("Energy source is outside direct manipulation range", {"Source within configured range"});
    }
    if (blessedSource_.cohesion < config_.barrierMinimumCohesion) {
        return Failure("Energy cohesion is below the barrier requirement", {"Configured cohesion threshold"});
    }
    if (blessedSource_.stability < config_.barrierMinimumStability) {
        return Failure("Energy stability is below the barrier requirement", {"Configured stability threshold"});
    }
    if (tenkaikai_.storedEnergy < config_.barrierCost) {
        return Failure("Insufficient blessed energy to form barrier", {"Available energy >= barrier cost"});
    }

    tenkaikai_.storedEnergy -= config_.barrierCost;
    barrierEnergy_ = config_.barrierCost;
    barrierFormed_ = true;
    OperationResult result;
    result.success = true;
    result.energyConsumed = config_.barrierCost;
    result.conditions = {"Tenkaikai active", "Source in range", "Cohesion sufficient", "Stability sufficient", "Energy available"};
    result.stateChange = "Stable barrier formed";
    lastEvent_ = result.stateChange;
    return result;
}

OperationResult Simulation::LaunchTestObject() {
    if (testObject_.launched) {
        return Failure("Test object has already been launched", {"Object has not been launched"});
    }
    testObject_.launched = true;
    testObject_.velocity.x = -2.0;
    OperationResult result;
    result.success = true;
    result.conditions = {"Test object is available"};
    result.stateChange = "Physical test object launched toward the barrier plane";
    lastEvent_ = result.stateChange;
    return result;
}

OperationResult Simulation::InspectCursedObject() {
    if (!tenkaikai_.active && !mugen_.active) {
        return Failure("Both supernatural eyes are inactive", {"At least one eye active"});
    }
    if (DistanceTo(cursedObject_.position) > config_.cursedDetectionRange + cursedObject_.interactionRadius) {
        return Failure("Cursed object is outside detection range", {"Object within configured detection range"});
    }
    OperationResult result;
    result.success = true;
    result.energyObserved = cursedObject_.quantity;
    result.conditions = {"Object within detection range", "Cursed energy remains separate from blessed energy"};
    result.stateChange = "Inspected " + cursedObject_.id;
    return result;
}

const SimulationConfig& Simulation::Config() const { return config_; }
const EyeState& Simulation::Tenkaikai() const { return tenkaikai_; }
const EyeState& Simulation::MugenNoMe() const { return mugen_; }
const EnergyCloud& Simulation::BlessedSource() const { return blessedSource_; }
const EnergyCloud& Simulation::CursedObject() const { return cursedObject_; }
const CurseState& Simulation::TestCurse() const { return testCurse_; }
const PhysicalTestObject& Simulation::TestObject() const { return testObject_; }
bool Simulation::BarrierFormed() const { return barrierFormed_; }
double Simulation::BarrierEnergy() const { return barrierEnergy_; }
double Simulation::TotalAccountedEnergy() const {
    return blessedSource_.quantity + cursedObject_.quantity + mugen_.storedEnergy +
        tenkaikai_.storedEnergy + barrierEnergy_ + dissipatedEnergy_;
}
double Simulation::InitialEnergy() const { return initialEnergy_; }
const std::string& Simulation::LastEvent() const { return lastEvent_; }

OperationResult Simulation::AbsorbBlessedEnergy(double simulatedSeconds) {
    const double range = config_.passiveAbsorptionRadius + blessedSource_.interactionRadius;
    if (DistanceTo(blessedSource_.position) > range) {
        return Failure("Blessed energy source is outside absorption radius", {"Mugen no Me active", "Source within absorption radius"});
    }
    const double freeCapacity = mugen_.capacity - mugen_.storedEnergy;
    const double amount = std::min({blessedSource_.quantity, mugen_.absorptionRate * simulatedSeconds, freeCapacity});
    if (amount <= 0.0) {
        return Failure("No absorbable energy or Mugen no Me storage is full", {"Source energy available", "Mugen capacity available"});
    }

    blessedSource_.quantity -= amount;
    mugen_.storedEnergy += amount;
    OperationResult result;
    result.success = true;
    result.energyTransferred = amount;
    result.conditions = {"Mugen no Me active", "Source within absorption radius", "Absorption rate respected", "Storage capacity respected"};
    result.stateChange = "Blessed energy absorbed into Mugen no Me";
    lastEvent_ = result.stateChange;
    return result;
}

double Simulation::DistanceTo(const Vector3& position) const {
    return Distance(playerPosition_, position);
}

EyeState& Simulation::Eye(EyeId eye) {
    return eye == EyeId::Tenkaikai ? tenkaikai_ : mugen_;
}

const EyeState& Simulation::Eye(EyeId eye) const {
    return eye == EyeId::Tenkaikai ? tenkaikai_ : mugen_;
}

OperationResult Simulation::Failure(std::string reason, std::vector<std::string> conditions) {
    OperationResult result;
    result.conditions = std::move(conditions);
    result.reason = std::move(reason);
    lastEvent_ = result.reason;
    return result;
}

} // namespace microverse