/*
 * B_RunAction.cpp
 *
 *  Created on: July, 2020
 *      Author: orlov
 */

#include "B_RunAction.h"


B_RunAction::B_RunAction() { // @suppress("Class members should be properly initialized")
    // TODO Auto-generated constructor stub
    //	_outputFileName = "data.root";
    timer = new G4Timer();
}

B_RunAction::~B_RunAction() {
    // TODO Auto-generated destructor stub


    delete tree;
    delete hfile;
}

void B_RunAction::BeginOfRunAction(const G4Run* run)
{
    //inform the runManager to save random number seed
    //	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

    timer->Start();

    // Histogramming
    hfile = new TFile(_outputFileName, "RECREATE", "Blueberries", 1);
    if (hfile->IsZombie()) exit(-1);
    tree = new TTree("T", "Blueberries Data Tree");
    //	tree->SetAutoSave(1000000);

    // Create new event
    TTree::SetBranchStyle(0);
    //Event
    tree->Branch("EventID",  &_EventID,  "EventID/I");

    //Hits

    tree->Branch("nPart", &_nPart, "nPart/I");
    tree->Branch("TrackID", _TrackID, "TrackID[nPart]/I");
    tree->Branch("ParentID", _ParentID, "ParentID[nPart]/I");
    tree->Branch("Energy", _Energy, "Energy[nPart]/D");
    tree->Branch("Time", _Time, "Time[nPart]/D");
    tree->Branch("PdgID",_PdgID, "PdgID[nPart]/I");
    tree->Branch("StationID", _StationID, "StationID[nPart]/I");
    tree->Branch("X", _X, "X[nPart]/D");
    tree->Branch("Y", _Y, "Y[nPart]/D");
    tree->Branch("Z", _Z, "Z[nPart]/D");
    tree->Branch("Momentum", _Momentum, "Momentum[nPart]/D");
    tree->Branch("Wavelength", _Wavelength, "Wavelength[nPart]/D");
    tree->Branch("Px", _Px, "Px[nPart]/D");
    tree->Branch("Py", _Py, "Py[nPart]/D");
    tree->Branch("Pz", _Pz, "Pz[nPart]/D");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B_RunAction::EndOfRunAction(const G4Run* )
{
    //	hfile = tree->GetCurrentFile();
    tree->Write();
    hfile->Write();
    tree->Print();
    timer->Stop();
    hfile->Close();

    G4cout << "Time: " << *timer << G4endl;

}
