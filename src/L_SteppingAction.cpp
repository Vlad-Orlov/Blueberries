/*
 * L_SteppingAction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "L_SteppingAction.h"
#include "L_SensitiveDetector.h"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4SDManager.hh"
#include "G4ProcessManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

L_SteppingAction::L_SteppingAction(L_PrimaryGeneratorAction *genAction) :
_genAction(genAction){
	Reset();
	ResetPerEvent();
}

L_SteppingAction::~L_SteppingAction() {
}

void L_SteppingAction::UserSteppingAction(const G4Step* aStep) {
//	G4cout << "_____________________________ step" << G4endl;
	const G4Track* aTrack = aStep->GetTrack();
	// G4Track *aTrack = aStep->GetTrack();
	G4int trackID = aTrack->GetTrackID();
	//G4cout<<"trackID = "<<trackID<<G4endl;


	G4StepPoint* aPrePoint = aStep->GetPreStepPoint();
	G4VPhysicalVolume* aPrePV = aPrePoint->GetPhysicalVolume();

	G4StepPoint* aPostPoint = aStep->GetPostStepPoint();
	G4VPhysicalVolume* aPostPV = aPostPoint->GetPhysicalVolume();

	//
    if (!aPostPV) return;
    if(!aPostPV->GetLogicalVolume()->GetSensitiveDetector()) return;

	return;
}
void L_SteppingAction::ResetPerEvent(){

}

void L_SteppingAction::Reset()
{

}
