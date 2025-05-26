
#include "ScintillatorSD.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4AnalysisManager.hh"

ScintillatorSD::ScintillatorSD(const G4String& name) : G4VSensitiveDetector(name) {}

void ScintillatorSD::Initialize(G4HCofThisEvent*) {}

G4bool ScintillatorSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return false;

    auto pre = step->GetPreStepPoint();
    auto pos = pre->GetPosition();
    auto copyNo = pre->GetTouchableHandle()->GetCopyNumber();

    auto man = G4AnalysisManager::Instance();
    man->FillNtupleIColumn(0, copyNo);
    man->FillNtupleDColumn(1, edep/MeV);
    man->FillNtupleDColumn(2, pos.x()/m);
    man->FillNtupleDColumn(3, pos.y()/m);
    man->FillNtupleDColumn(4, pos.z()/m);
    man->AddNtupleRow();

    return true;
}

void ScintillatorSD::EndOfEvent(G4HCofThisEvent*) {}
