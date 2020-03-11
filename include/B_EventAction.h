/*
 * B_EventAction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#ifndef SRC_B_EventAction_H_
#define SRC_B_EventAction_H_

#include <G4UserEventAction.hh>
#include "globals.hh"
#include "B_RunAction.h"
#include "B_Hit.h"
#include "B_SteppingAction.h"

class G4Event;

class B_RunAction;
class B_SteppingAction;
class B_PrimaryGeneratorAction;

class B_EventAction: public G4UserEventAction {
public:
    B_EventAction(B_RunAction*, B_SteppingAction*);
    virtual ~B_EventAction();
public:
    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );

    void SetPrimGenerator(B_PrimaryGeneratorAction *gen){_primGenerator = gen;};
private:
    B_RunAction* runAction;
    B_SteppingAction* _steppingAction;
    G4int printModulo;
    G4int theCollectionID;

    B_PrimaryGeneratorAction* _primGenerator;
};

#endif /* SRC_B_EventAction_H_ */
