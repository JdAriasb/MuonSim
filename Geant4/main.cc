
#include "G4RunManagerFactory.hh"
#include "ActionInitialization.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"

int main(int argc, char** argv) {
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new QGSP_BERT);
	runManager->SetUserInitialization(new ActionInitialization());

    runManager->Initialize();

    // G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    if (ui) {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    } else {
        UImanager->ApplyCommand("/control/execute run.mac");
    }

    // G4UImanager::GetUIpointer()->ApplyCommand("/control/execute vis.mac");
    // ui->SessionStart();

    delete visManager;
    delete runManager;
    delete ui;
    return 0;
}
