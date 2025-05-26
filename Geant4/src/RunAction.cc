
#include "RunAction.hh"
#include "G4AnalysisManager.hh"

RunAction::RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
    auto man = G4AnalysisManager::Instance();
    man->OpenFile("output.root");
    man->CreateNtuple("ScintHits", "Hits in Scintillators");
    man->CreateNtupleIColumn("ScintID");
    man->CreateNtupleDColumn("Edep");
    man->CreateNtupleDColumn("X");
    man->CreateNtupleDColumn("Y");
    man->CreateNtupleDColumn("Z");
    man->FinishNtuple();
}

void RunAction::EndOfRunAction(const G4Run*) {
    auto man = G4AnalysisManager::Instance();
    man->Write();
    man->CloseFile();
}
