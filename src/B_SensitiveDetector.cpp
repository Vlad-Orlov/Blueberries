/*
 * B_SensitiveDetector.cpp
 *
 *  Created on: Oct 3, 2018
 *      Author: vsevolod
 */

#include "B_SensitiveDetector.h"

#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

B_SensitiveDetector::B_SensitiveDetector(G4String name) : // @suppress("Class members should be properly initialized")
G4VSensitiveDetector(name){
	//  G4RunManager* runManager = G4RunManager::GetRunManager();
	G4String HCname = "Collection";
	collectionName.insert(HCname);
}

B_SensitiveDetector::~B_SensitiveDetector() {
}

void B_SensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
	_Collection =
            new B_HitsCollection(SensitiveDetectorName, collectionName[0]);
	static G4int HCID = -1;
	if (HCID < 0) {
		HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	}
	HCE->AddHitsCollection(HCID, _Collection);

	//  G4cout << "___________________________ Detector initialized" << G4endl;
}


G4bool B_SensitiveDetector::ProcessHitsL(G4Step* aStep, G4TouchableHistory* hist) {
	return ProcessHits(aStep, hist);
}

G4bool B_SensitiveDetector::ProcessHits(G4Step* aStep,
		G4TouchableHistory*)
{

	G4Track* aTrack = aStep->GetTrack();
	G4ThreeVector globalPosition = aStep->GetPostStepPoint()->GetPosition();
//	newHit->myData;

	G4StepPoint *aPostPoint = aStep->GetPostStepPoint();
	G4StepPoint *aPrevPoint = aStep->GetPreStepPoint();
    if (!aPostPoint->GetPhysicalVolume()) return false;

    G4LogicalVolume *PostVolume = aPostPoint->GetPhysicalVolume()->GetLogicalVolume();
	G4LogicalVolume *PrevVolume = aPrevPoint->GetPhysicalVolume()->GetLogicalVolume();


    G4String PreName = PrevVolume->GetName();
    G4String PostName = PostVolume->GetName();

    const G4DynamicParticle *aParticle = aTrack->GetDynamicParticle();


//    if (aParticle->GetCharge() == 0) return false;

    B_Hit* newHit = new B_Hit();

	newHit->myData.TrackID = aTrack->GetTrackID();
	newHit->myData.ParentID = aTrack->GetParentID();
	newHit->myData.Energy = aTrack->GetKineticEnergy();
	newHit->myData.PdgID = aTrack->GetParticleDefinition()->GetPDGEncoding();
	newHit->myData.Time = aTrack->GetGlobalTime();
    newHit->myData.X = globalPosition.x();
	newHit->myData.Y = globalPosition.y();
	newHit->myData.Z = globalPosition.z();
    newHit->myData.Momentum= aTrack->GetMomentum().mag();
	newHit->myData.Px = aTrack->GetMomentum().x();
	newHit->myData.Py = aTrack->GetMomentum().y();
	newHit->myData.Pz = aTrack->GetMomentum().z();

         if (PreName == "wlstube" && PostName == "North") newHit->myData.StationID = 1;
    else if (PreName == "wlstube" && PostName == "South") newHit->myData.StationID = 2;
    else if (PreName == "wlstube" && PostName == "West") newHit->myData.StationID = 3;
    else if (PreName == "wlstube" && PostName == "East") newHit->myData.StationID = 4;
    else return false;


	// Insert this hit
	_Collection->insert(newHit);
	newHit->Draw();
	//  }


	return true;
}

void B_SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

	G4int NbHits = _Collection->entries();

}
