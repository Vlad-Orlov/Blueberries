#include "B_PhysicsList.h"


// Physics list constructor inherited from QGSP_BERT
B_PhysicsList::B_PhysicsList() : QGSP_BERT()
{
    theCerenkovProcess = 0;
    theScintProcess = 0;
    defaultCutValue = 1.0*mm;
    SetVerboseLevel(0);

    G4cout << "Physics list constructor" << G4endl;
}

B_PhysicsList::~B_PhysicsList()
{
    delete theCerenkovProcess;
    delete theScintProcess;
}

void B_PhysicsList::ConstructParticle()
{
    //  Constructing HEP particles from base physics list
    //  and adding OpticalPhoton to them

    QGSP_BERT::ConstructParticle();
    G4OpticalPhoton::OpticalPhotonDefinition();
}


void B_PhysicsList::ConstructProcess()
{
    //  Constructing processes from base physics list and adding
    //  Cherenkov process and optical processes

    QGSP_BERT::ConstructProcess();
    ConstructOp();
}
//void SetCuts();



void B_PhysicsList::ConstructOp()
{


    G4ParticleTable::G4PTblDicIterator *theParticleIterator = GetParticleIterator();

    G4cout<<" 000 "<<G4endl;

    // Optical Photon Processes
    theCerenkovProcess = new G4Cerenkov("Cerenkov");
    theScintProcess = new G4Scintillation("Scintillation");
    theWlsProcess = new G4OpWLS();
    G4cout<<" 111 "<<G4endl;

    SetVerbose(0);

    theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
    theCerenkovProcess->SetTrackSecondariesFirst(true);

    theScintProcess->SetTrackSecondariesFirst(true);
    theScintProcess->SetScintillationYieldFactor(1.);

    G4cout<<" 111 "<<G4endl;

    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        if (theCerenkovProcess->IsApplicable(*particle)) {
            G4cout << "Add Cerenkov process to " << particleName << G4endl;
            pmanager->AddProcess(theCerenkovProcess);
            pmanager->SetProcessOrdering(theCerenkovProcess, idxPostStep);
        }

        if (theScintProcess->IsApplicable(*particle)) {
            G4cout << "Add Scintillation process to " << particleName << G4endl;
            pmanager->AddProcess(theScintProcess);
            pmanager->SetProcessOrderingToLast(theScintProcess, idxAtRest);
            pmanager->SetProcessOrderingToLast(theScintProcess, idxPostStep);
        }



        if (particleName == "opticalphoton") {
            G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
            pmanager->AddDiscreteProcess(new G4OpAbsorption());
            pmanager->AddDiscreteProcess(new G4OpRayleigh());
            pmanager->AddDiscreteProcess(new G4OpBoundaryProcess());
            pmanager->AddDiscreteProcess(theWlsProcess);

        }
    }


    G4cout << "Optics constructed" << G4endl;
}


void B_PhysicsList::SetVerbose(G4int verbose)
{
    theCerenkovProcess->SetVerboseLevel(verbose);
    theScintProcess->SetVerboseLevel(verbose);

}

