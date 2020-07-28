/*
 * B_EventAction.cpp
 *
 *  Created on: July, 2020
 *      Author: orlov
 */

#include "B_EventAction.h"


#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

//G4
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "globals.hh"

B_EventAction::B_EventAction(B_RunAction* runact, B_SteppingAction* steppingAction) :
        runAction(runact), _steppingAction(steppingAction), printModulo(50)
{
	//  theCollectionID = -1;
}

B_EventAction::~B_EventAction() {
}

void B_EventAction::BeginOfEventAction(const G4Event* event)
{
    G4int eventNum = event->GetEventID();

	if (eventNum%printModulo == 0) {
		G4cout << "\n---> Begin of Event: " << eventNum << G4endl;
	}

	//	if (theCollectionID < 0) {
	G4String colName;
	theCollectionID =
			G4SDManager::GetSDMpointer()->GetCollectionID("Collection");
	//	}

    // Setting the number of photons to 0 for further counting
    runAction->_nPart = 0;

	_steppingAction->Reset();
	_steppingAction->ResetPerEvent();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B_EventAction::EndOfEventAction(const G4Event* event)
{

	// Print info about end of the event
	G4int eventNum = event->GetEventID();

	//	  if (theCollectionID < 0) return;

	// Get the Hit Collection
	G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    B_HitsCollection * THC = 0;

	G4int nHit = 0;

	if (HCE){
        THC = (B_HitsCollection*)(HCE->GetHC(theCollectionID));
	}

	if (0 == THC) return;

	nHit = THC->entries();

	for (G4int i = 0; i < nHit; i++) {
		runAction->_TrackID[i] = (*THC)[i]->myData.TrackID;
		runAction->_ParentID[i] = (*THC)[i]->myData.ParentID;
        runAction->_Energy[i] = (*THC)[i]->myData.Energy / MeV;
        runAction->_Time[i] = (*THC)[i]->myData.Time / ps;
		runAction->_PdgID[i] = (*THC)[i]->myData.PdgID;
		runAction->_StationID[i] = (*THC)[i]->myData.StationID;
        runAction->_X[i] = (*THC)[i]->myData.X / mm;
        runAction->_Y[i] = (*THC)[i]->myData.Y / mm;
        runAction->_Z[i] = (*THC)[i]->myData.Z / mm;
        runAction->_Px[i] = (*THC)[i]->myData.Px / MeV;
        runAction->_Py[i] = (*THC)[i]->myData.Py / MeV;
        runAction->_Pz[i] = (*THC)[i]->myData.Pz / MeV;
        runAction->_Momentum[i] = (*THC)[i]->myData.Momentum / MeV;
        runAction->_Wavelength[i] = (*THC)[i]->myData.Wavelength /nm;

	}

	runAction->_EventID = eventNum;
	runAction->_nPart = nHit;

	runAction->tree->Fill();

	//	G4cout << "End of event" << G4endl;
}
