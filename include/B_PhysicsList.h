#pragma once

#include "G4VModularPhysicsList.hh"
#include "QGSP_BERT.hh"

// Cerenkov, scintillation and optical physics processes
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ParticleTableIterator.hh"

#include "G4ProcessManager.hh"


class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpBoundaryProcess;

class B_PhysicsList : public QGSP_BERT
{
public:
    B_PhysicsList();
    ~B_PhysicsList();

protected:
  // Construct phyisics processes and register them
  void ConstructOp();
  // Construct particle and physics process
  void ConstructParticle();
  void ConstructProcess();


protected:
  void SetNbOfPhotonsCerenkov(G4int);
  void SetVerbose(G4int verbose);

private:
  G4Cerenkov* theCerenkovProcess;
  G4Scintillation* theScintProcess;


};
