/*
 * L_SensitiveDetector.h
 *
 *  Created on: Oct 3, 2018
 *      Author: vsevolod
 */

#ifndef INCLUDE_L_SensitiveDetector_H_
#define INCLUDE_L_SensitiveDetector_H_

#include <G4VSensitiveDetector.hh>
#include "L_Hit.h"
#include "HitDataStructure.hh"
#include "L_EventAction.h"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class L_SensitiveDetector: public G4VSensitiveDetector {
public:
    L_SensitiveDetector(G4String name);
    virtual ~L_SensitiveDetector();


	void Initialize(G4HCofThisEvent*);

	G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    G4bool ProcessHitsL(G4Step*, G4TouchableHistory*);

	void EndOfEvent(G4HCofThisEvent*);
private:

    L_HitsCollection* _Collection;
	G4String SDname;

};

#endif /* INCLUDE_L_SensitiveDetector_H_ */
