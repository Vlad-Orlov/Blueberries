/*
 * L_PrimaryGeneratorAction.cpp
 *
 *  Created on: Feb 25, 2020
 *      Author: vorlov
 */

#include "L_PrimaryGeneratorAction.h"

L_PrimaryGeneratorAction::L_PrimaryGeneratorAction() {}

L_PrimaryGeneratorAction::~L_PrimaryGeneratorAction() {}

void L_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    G4String particleName = "gamma";
    G4ThreeVector position(5*cm, 30*cm, 0*cm);
    G4ThreeVector direction(0,-1,0);

    // Default particle kinematics
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);


     _particleGun->SetParticleDefinition(particle);
     _particleGun->SetParticleMomentumDirection(direction);
     _particleGun->SetParticleEnergy(0.5*MeV);
     _particleGun->SetParticlePosition(position);
     _particleGun->GeneratePrimaryVertex(anEvent);

}

