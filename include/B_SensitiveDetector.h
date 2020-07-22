/*
 * B_SensitiveDetector.h
 *
 *  Created on: July, 2020
 *      Author: orlov
 */

#ifndef INCLUDE_B_SensitiveDetector_H_
#define INCLUDE_B_SensitiveDetector_H_

#include <G4VSensitiveDetector.hh>
#include "B_Hit.h"
#include "HitDataStructure.hh"
#include "B_EventAction.h"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class B_SensitiveDetector: public G4VSensitiveDetector {
public:
    B_SensitiveDetector(G4String name);
    virtual ~B_SensitiveDetector();


	void Initialize(G4HCofThisEvent*);

	G4bool ProcessHits(G4Step*, G4TouchableHistory*);

    G4bool ProcessHitsL(G4Step*, G4TouchableHistory*);

	void EndOfEvent(G4HCofThisEvent*);
private:

    B_HitsCollection* _Collection;
	G4String SDname;

};

#endif /* INCLUDE_B_SensitiveDetector_H_ */
