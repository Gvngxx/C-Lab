#pragma once

#include <string>
#include <vector>

namespace microverse {

struct Vector3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

enum class EnergyType {
    Blessed,
    Cursed
};

enum class EyeId {
    Tenkaikai,
    MugenNoMe
};

struct EnergyCloud {
    std::string id;
    EnergyType type = EnergyType::Blessed;
    Vector3 position;
    Vector3 velocity;
    double orientation = 0.0;
    double quantity = 0.0;
    double density = 1.0;
    double cohesion = 1.0;
    double stability = 1.0;
    double interactionRadius = 1.0;
};

struct EyeState {
    bool active = true;
    double storedEnergy = 0.0;
    double capacity = 0.0;
    double transferRate = 0.0;
    double absorptionRate = 0.0;
};

struct OperationResult {
    bool success = false;
    double energyConsumed = 0.0;
    double energyTransferred = 0.0;
    double energyProduced = 0.0;
    double energyObserved = 0.0;
    std::vector<std::string> conditions;
    std::string reason;
    std::string stateChange;
};

struct SimulationConfig {
    double passiveAbsorptionRadius = 20.0;
    double directManipulationRange = 30.0;
    double cursedDetectionRange = 15.0;
    double mugenCapacity = 100.0;
    double mugenAbsorptionRate = 10.0;
    double mugenTransferRate = 20.0;
    double tenkaikaiCapacity = 100.0;
    double barrierCost = 10.0;
    double barrierMinimumCohesion = 0.7;
    double barrierMinimumStability = 0.8;
    double energyControlCost = 1.0;
    double blessedSourceCohesion = 0.85;
    double blessedSourceStability = 0.9;
    double blessedSourceQuantity = 200.0;
    double cursedObjectQuantity = 20.0;
    double curseUpkeepRate = 0.5;
    std::string curseFallback = "suspended";

    static SimulationConfig Load(const std::string& path);
};

struct CurseState {
    std::string id = "curse.test-sustain";
    std::string name = "Sustained condition";
    int severity = 1;
    double upkeepRate = 0.0;
    bool active = true;
    std::string fallback;
};

struct PhysicalTestObject {
    Vector3 position{3.0, 0.0, 0.0};
    Vector3 velocity;
    bool launched = false;
    bool blockedByBarrier = false;
};

class Simulation {
public:
    explicit Simulation(SimulationConfig config = {});

    void SetPlayerPosition(Vector3 position);
    std::vector<OperationResult> Step(double fixedDeltaTime);
    OperationResult SetEyeActive(EyeId eye, bool active);
    OperationResult TransferToTenkaikai(double simulatedSeconds);
    OperationResult ManipulateBlessedEnergy(const std::string& cloudId, Vector3 velocity);
    OperationResult FormBarrier();
    OperationResult LaunchTestObject();
    OperationResult InspectCursedObject();

    const SimulationConfig& Config() const;
    const EyeState& Tenkaikai() const;
    const EyeState& MugenNoMe() const;
    const EnergyCloud& BlessedSource() const;
    const EnergyCloud& CursedObject() const;
    const CurseState& TestCurse() const;
    const PhysicalTestObject& TestObject() const;
    bool BarrierFormed() const;
    double BarrierEnergy() const;
    double TotalAccountedEnergy() const;
    double InitialEnergy() const;
    const std::string& LastEvent() const;

private:
    OperationResult AbsorbBlessedEnergy(double simulatedSeconds);
    double DistanceTo(const Vector3& position) const;
    EyeState& Eye(EyeId eye);
    const EyeState& Eye(EyeId eye) const;
    OperationResult Failure(std::string reason, std::vector<std::string> conditions = {});

    SimulationConfig config_;
    Vector3 playerPosition_{0.0, 2.0, 8.0};
    EyeState tenkaikai_;
    EyeState mugen_;
    EnergyCloud blessedSource_;
    EnergyCloud cursedObject_;
    CurseState testCurse_;
    PhysicalTestObject testObject_;
    bool barrierFormed_ = false;
    double barrierEnergy_ = 0.0;
    double dissipatedEnergy_ = 0.0;
    double initialEnergy_ = 0.0;
    std::string lastEvent_ = "Simulation ready";
};

} // namespace microverse