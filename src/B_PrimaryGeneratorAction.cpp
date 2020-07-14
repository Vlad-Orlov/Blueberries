/*
 * B_PrimaryGeneratorAction.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: vorlov
 */

#include "B_PrimaryGeneratorAction.h"

B_PrimaryGeneratorAction::B_PrimaryGeneratorAction() {}

B_PrimaryGeneratorAction::~B_PrimaryGeneratorAction() {}

void B_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    G4String particleName = "pi+";
    G4ThreeVector position(0*cm, 30*cm, 0.*cm); //ya tut
    G4ThreeVector direction(0,-1,0);

    // Default particle kinematics
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);


     _particleGun->SetParticleDefinition(particle);
     _particleGun->SetParticleMomentumDirection(direction);
     _particleGun->SetParticleMomentum(2*GeV);
     _particleGun->SetParticlePosition(position);
     _particleGun->GeneratePrimaryVertex(anEvent);

}

