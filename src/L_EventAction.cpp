/*
 * L_EventAction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "L_EventAction.h"


#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

//G4
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "globals.hh"

L_EventAction::L_EventAction(L_RunAction* runact, // @suppress("Class members should be properly initialized")
        L_SteppingAction* steppingAction) :
		runAction(runact), _steppingAction(steppingAction), printModulo(100)
{
	//  theCollectionID = -1;
}

L_EventAction::~L_EventAction() {
}

void L_EventAction::BeginOfEventAction(const G4Event* event)
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

	_steppingAction->Reset();
	_steppingAction->ResetPerEvent();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void L_EventAction::EndOfEventAction(const G4Event* event)
{

	// Print info about end of the event
	G4int eventNum = event->GetEventID();

	//	  if (theCollectionID < 0) return;

	// Get the Hit Collection
	G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    L_HitsCollection * THC = 0;

	G4int nHit = 0;

	if (HCE){
        THC = (L_HitsCollection*)(HCE->GetHC(theCollectionID));
	}

	if (0 == THC) return;

	nHit = THC->entries();

	for (G4int i = 0; i < nHit; i++) {
		runAction->_TrackID[i] = (*THC)[i]->myData.TrackID;
		runAction->_ParentID[i] = (*THC)[i]->myData.ParentID;
		runAction->_Energy[i] = (*THC)[i]->myData.Energy;
		runAction->_Time[i] = (*THC)[i]->myData.Time;
		runAction->_PdgID[i] = (*THC)[i]->myData.PdgID;
		runAction->_StationID[i] = (*THC)[i]->myData.StationID;
		runAction->_X[i] = (*THC)[i]->myData.X;
		runAction->_Y[i] = (*THC)[i]->myData.Y;
		runAction->_Z[i] = (*THC)[i]->myData.Z;
		runAction->_Px[i] = (*THC)[i]->myData.Px;
		runAction->_Py[i] = (*THC)[i]->myData.Py;
		runAction->_Pz[i] = (*THC)[i]->myData.Pz;
        runAction->_Momentum[i] = (*THC)[i]->myData.Momentum;
	}

	runAction->_EventID = eventNum;
	runAction->_nPart = nHit;

	runAction->tree->Fill();

	//	G4cout << "End of event" << G4endl;
}
