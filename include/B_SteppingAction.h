/*
 * B_SteppingAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#ifndef SRC_B_SteppingAction_H_
#define SRC_B_SteppingAction_H_

#include <G4UserSteppingAction.hh>
#include "B_PrimaryGeneratorAction.h"

class B_PrimaryGeneratorAction;

class B_SteppingAction: public G4UserSteppingAction {
public:
    B_SteppingAction(B_PrimaryGeneratorAction*);
    virtual ~B_SteppingAction();
	void UserSteppingAction(const G4Step*);

	void Reset();
	void ResetPerEvent();

private:
    G4double _probOfReflection;
    G4double _particleID;
    B_PrimaryGeneratorAction* _genAction;
    void InternalReflectionProbability(G4double energy,
                                       G4double& probability);
};

#endif /* SRC_B_SteppingAction_H_ */
