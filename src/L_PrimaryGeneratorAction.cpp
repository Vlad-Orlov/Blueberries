/*
 * L_PrimaryGeneratorAction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "L_PrimaryGeneratorAction.h"


L_PrimaryGeneratorAction::L_PrimaryGeneratorAction() {
//    ReadFile();
    iEv = 0;


    G4RunManager *runManager = G4RunManager::GetRunManager();
    G4int evToGen = runManager->GetNumberOfEventsToBeProcessed();
    pythia.readFile("PythiaSettings.cmnd");
    pythia.readString("Main:numberOfEvents = " + std::to_string(evToGen));
    pythia.init();
}

L_PrimaryGeneratorAction::~L_PrimaryGeneratorAction() {

}

void L_PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    G4LogicalVolume* worldLV
            = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

    if (!pythia.next()) GeneratePrimaries(anEvent);

    GetEvent(PythiaEvent);

    for (G4int pId = 0; pId < nParticles; ++pId){
        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition* particle = particleTable->FindParticle(pdgID[pId]);
        G4double m = particle->GetPDGMass();

        G4ThreeVector dir = G4ThreeVector(pX[pId],pY[pId],pZ[pId]);

        G4double momentum = TMath::Sqrt(pX[pId]*pX[pId] + pY[pId]*pY[pId] + pZ[pId]*pZ[pId]);
        G4double Ekin = (TMath::Sqrt(momentum*momentum + m*m) - m);

        _particleGun->SetParticleDefinition(particle);
        _particleGun->SetParticleMomentumDirection(dir);
        _particleGun->SetParticleEnergy(Ekin);
        _particleGun->SetParticlePosition(G4ThreeVector(X[pId], Y[pId], Z[pId]));
        _particleGun->GeneratePrimaryVertex(anEvent);

        //			G4cout << "Particle name = " << particle->GetParticleName() << G4endl;
    }
}



bool L_PrimaryGeneratorAction::GetEvent(Pythia8::Event event) {
    G4int particleID = 0;
    for (G4int i = 3; i < event.size(); ++i){
        if (!event[i].isFinal()) continue;

        pdgID[particleID] = event[i].id();
        X[particleID] = event[i].xProd();
        Y[particleID] = event[i].yProd();
        Z[particleID] = event[i].zProd();
        pX[particleID] = event[i].px();
        pY[particleID] = event[i].py();
        pZ[particleID] = event[i].pz();
        particleID ++;
    }
    nParticles = particleID;
}