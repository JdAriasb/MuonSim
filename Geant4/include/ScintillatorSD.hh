#pragma once

#include "G4VSensitiveDetector.hh"

class ScintillatorSD : public G4VSensitiveDetector {
public:
    ScintillatorSD(const G4String& name);
    ~ScintillatorSD() override = default;
    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step*, G4TouchableHistory*) override;
    void EndOfEvent(G4HCofThisEvent*) override;
};
