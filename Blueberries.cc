#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#include "QGSP_BERT.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#ifdef G4UI_USE_TCSH
#include "G4UItcsh.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif


#include "B_DetectorConstruction.h"
#include "B_PrimaryGeneratorAction.h"

#include "B_RunAction.h"
#include "B_EventAction.h"
#include "B_SteppingAction.h"
#include "B_PhysicsList.h"

#include "Randomize.hh"

int main(int argc, char** argv)
{

	G4UIExecutive* ui = 0;
	if ( argc == 1 ) {
		ui = new G4UIExecutive(argc, argv);
	}
    G4long myseed = G4long(argv[3]);


	G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheSeed(myseed);

    G4cout << "Seed: " << G4Random::getTheSeed() << G4endl;
	// Run manager initialization
	G4RunManager* runManager = new G4RunManager;

    G4VUserDetectorConstruction* detector = new B_DetectorConstruction;
	runManager->SetUserInitialization(detector);


    G4VModularPhysicsList* physicsList = new B_PhysicsList();
    physicsList->SetVerboseLevel(0);
    runManager->SetUserInitialization(physicsList);



    B_RunAction* runAction = new B_RunAction;
    if (argc == 4) runAction->SetOutputFileName(G4String(argv[2]));
	runManager->SetUserAction(runAction);

    B_PrimaryGeneratorAction* genAction = new B_PrimaryGeneratorAction();
	runManager->SetUserAction(genAction);

    B_SteppingAction* stepAction = new B_SteppingAction(genAction);
	runManager->SetUserAction(stepAction);

    B_EventAction* eventAction = new B_EventAction(runAction, stepAction);
	runManager->SetUserAction(eventAction);



	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();

	G4UImanager* UImanager = G4UImanager::GetUIpointer();


	if ( ! ui ) {
		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
	else {
		// interactive mode
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		if (ui->IsGUI()) {
			UImanager->ApplyCommand("/control/execute gui.mac");
		}
		ui->SessionStart();
		delete ui;
	}







	return 0;
}
