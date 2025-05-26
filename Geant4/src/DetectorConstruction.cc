#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "ScintillatorSD.hh"

G4VPhysicalVolume* DetectorConstruction::Construct() {
// Definición de materiales
    auto nist = G4NistManager::Instance();
    G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    G4Material* lead = nist->FindOrBuildMaterial("G4_Pb");
    G4Material* uranium = nist->FindOrBuildMaterial("G4_U");
    G4Material* scint = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

// Se define el mundo como un cubo de 15m de lado.
    G4double worldSize = 15*m;
    auto solidWorld = new G4Box("World", worldSize/2, worldSize/2, worldSize/2);
    auto logicWorld = new G4LogicalVolume(solidWorld, vacuum, "World");
    auto physWorld = new G4PVPlacement(nullptr, {}, logicWorld, "World", nullptr, false, 0);

// Se definen las tapas de la caja a simular:
//  top = una caja de 10m*2m*10m de plomo
//  sides = una caja de 2m*6m*10m de plomo
	G4Box* top = new G4Box("top", 10.0 * m /2, 2.0 * m  / 2, 10.0 * m  / 2.0);
	G4Box* sides = new G4Box("bottom", 2.0 * m /2, 6.0 * m  / 2, 10.0 * m  / 2.0);
	
	G4LogicalVolume* logicTop = new  G4LogicalVolume(top, lead, "Logic_top");
	G4LogicalVolume* logicSides = new  G4LogicalVolume(sides, lead, "Logic_sides");

	new G4PVPlacement(0,G4ThreeVector(0,4.*m,0), logicTop, "Top", logicWorld, false, 0, true);
	new G4PVPlacement(0,G4ThreeVector(0,-4.*m,0), logicTop, "Bottom", logicWorld, false, 0, true);
	new G4PVPlacement(0,G4ThreeVector(-4.*m,0,0), logicSides, "Left", logicWorld, false, 0, true);
	new G4PVPlacement(0,G4ThreeVector(4.*m,0,0), logicSides, "Right", logicWorld, false, 0, true);

//Se define un cubo sólido de lado 10m de plomo
	// G4Box* Box = new G4Box("Box", 10.0 * m /2.0, 10.0 * m  / 2.0, 10.0 * m  / 2.0);
	// G4LogicalVolume* logicBox = new  G4LogicalVolume(Box, lead, "Logic_Box");
	// new G4PVPlacement(0,G4ThreeVector(0,0,0), logicBox, "Box", logicWorld, false, 0, true);

    G4double sx = (1*m), sy = (1*m), sz = 0.1*m;
    auto solidScint = new G4Box("Scint", sx/2, sy/2, sz/2);
    auto logicScint = new G4LogicalVolume(solidScint, scint, "Scintillator");

    G4double z = 5.5*m + sz/2;
    int idx = 0;
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            G4double x = -6*m + sx/2 + i*sx;
            G4double y = -6*m + sy/2 + j*sy;
            new G4PVPlacement(nullptr, G4ThreeVector(x,y,z), logicScint, "Scintillator", logicWorld, false, idx++);
        }
    }
    return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
    auto sd = new ScintillatorSD("ScintillatorSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    SetSensitiveDetector("Scintillator", sd);
}
