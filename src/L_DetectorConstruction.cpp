/*
 * L_DetectorConstruction.cpp
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#include "L_DetectorConstruction.h"


L_DetectorConstruction::L_DetectorConstruction() {
    DefineMateials();

}

L_DetectorConstruction::~L_DetectorConstruction() {

}

G4VPhysicalVolume* L_DetectorConstruction::Construct() {
    DefineMateials();
    return DefineVolumes();
}

void L_DetectorConstruction::ConstructSDandField() {
    LSD = new L_SensitiveDetector("LSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(LSD);

    L1PlaneLogInner->SetSensitiveDetector(LSD);
    L2PlaneLogInner->SetSensitiveDetector(LSD);
    L1PlaneLogOuter->SetSensitiveDetector(LSD);
    L2PlaneLogOuter->SetSensitiveDetector(LSD);


    //    G4cout << "_____________________________________________Detectors are made" << G4endl;
}

void L_DetectorConstruction::DefineMateials() {
    G4String symbol;
    G4double a, z, density;
    G4int ncomponents, natoms;
    G4double fractionmass;

    G4NistManager* man = G4NistManager::Instance();

    G4Element* C =
            new G4Element("Carbon", symbol = "C", z = 6., a = 12.01*g/mole);
    G4Element* N =
            new G4Element("Nitrogen", symbol = "N", z = 7., a = 14.01*g/mole);
    G4Element* O =
            new G4Element("Oxygen", symbol = "O", z = 8., a = 16.00*g/mole);

    G4Material* Air =
            new G4Material("Air", density = 0.000290*mg/cm3, ncomponents = 2);
    Air->AddElement(N, fractionmass = 0.7);
    Air->AddElement(O, fractionmass = 0.3);
    worldMaterial = Air;

    G4Material* Scint = man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    ScintMaterial = Scint;

    BPMaterial = man->FindOrBuildMaterial("G4_Be");
    INOX = man->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    Vacuum = new G4Material( "Galactic", z=1., a=1.01*g/mole, density= universe_mean_density,
                             kStateGas, 2.73*kelvin, 3.e-18*pascal );


}

G4VPhysicalVolume* L_DetectorConstruction::DefineVolumes(){

    G4VSolid *worldSolid = new G4Box("World",
                                     LConst::worldSizeX/2,
                                     LConst::worldSizeY/2,
                                     LConst::worldSizeZ/2);

    G4LogicalVolume *worldLogical = new G4LogicalVolume(worldSolid,
                                                        Vacuum, // worldMaterial,
                                                        "World");

    G4VPhysicalVolume *worldPhysical = new G4PVPlacement(0,
                                                         G4ThreeVector(),
                                                         worldLogical,
                                                         "World",
                                                         0,
                                                         false,
                                                         0);


    G4Transform3D Tr(G4RotationMatrix(), G4ThreeVector(0,0,0));


    ///////////////////////////////////////////////////////
    G4VSolid *L1SolidPlane = new G4Box("L1Plane",
                                      LConst::detector_thickness/2.,
                                      LConst::box_height/2.,
                                      LConst::box_width/2.);

    L1PlaneLogOuter = new G4LogicalVolume(L1SolidPlane,
                                                Vacuum,
                                       "L1PlaneOuter");

    G4RotationMatrix *Ra = new G4RotationMatrix();

    G4VPhysicalVolume *L1PlanePhysOuter =  new G4PVPlacement(
                Ra,
                G4ThreeVector((LConst::detector_thickness + LConst::box_width)/2.,0.,0.),
                L1PlaneLogOuter,
                "L1PlaneOuter",
                worldLogical,
                false,
                0);
    ///////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////

    L1PlaneLogInner = new G4LogicalVolume(L1SolidPlane,
                                     Vacuum,
                                     "L1PlaneInner");

    Ra = new G4RotationMatrix();
    Ra->rotateY(90*deg);
    G4VPhysicalVolume *L1PlanePhysInner =  new G4PVPlacement(
                Ra,
                G4ThreeVector(0.,0.,-(LConst::detector_thickness + LConst::box_width)/2.),
                L1PlaneLogInner,
                "L1PlaneInner",
                worldLogical,
                false,
                0);
    ////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////

    L2PlaneLogOuter = new G4LogicalVolume(L1SolidPlane,
                                     Vacuum,
                                     "L2PlaneOuter");

    G4VPhysicalVolume *L2PlanePhysOuter =  new G4PVPlacement(
                Ra,
                G4ThreeVector(0.,0.,(LConst::detector_thickness + LConst::box_width)/2.),
                L2PlaneLogOuter,
                "L2PlaneOuter",
                worldLogical,
                false,
                0);
    ////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////

    L2PlaneLogInner = new G4LogicalVolume(L1SolidPlane,
                                     Vacuum,
                                     "L2PlaneInner");

    G4VPhysicalVolume *L2PlanePhysInner =  new G4PVPlacement(
                new G4RotationMatrix(),
                G4ThreeVector(-(LConst::detector_thickness + LConst::box_width)/2.,0.,0.),
                L2PlaneLogInner,
                "L2PlaneInner",
                worldLogical,
                false,
                0);
    ////////////////////////////////////////////////////////




    G4cout << "_____________________________________________Volumes are made" << G4endl;
    return worldPhysical;
}

