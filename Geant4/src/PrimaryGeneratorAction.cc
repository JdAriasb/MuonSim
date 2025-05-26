#include "PrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    fParticleGun = new G4ParticleGun(1);

    auto mu = G4ParticleTable::GetParticleTable()->FindParticle("mu-");

    fParticleGun->SetParticleDefinition(mu);
    // fParticleGun->SetParticleEnergy(100.*GeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
	// Valores aleatorios entre -7.5 m y +7.5 m
	G4double x = (G4UniformRand() - 0.5)* 12.0 *m;
	G4double y = (G4UniformRand() - 0.5)* 12.0 *m;
	G4double z = -5.5*m;

	// Generación de energías por medio de su función cumulativa de probabilidad.
	G4double E = std::pow(((G4UniformRand()*8.503863413817225325e-02) -0.00029)/(-0.0848)+std::pow(1,-1.7),-1/1.7);
    fParticleGun->SetParticleEnergy(E*GeV);

	G4ThreeVector pos(x,y,z);
	fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1));
		
    fParticleGun->GeneratePrimaryVertex(event);
}
