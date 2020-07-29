/*
 * B_DetectorConstruction.cpp
 *
 *  Created on: July, 2020
 *      Author: orlov
 */

#include "B_DetectorConstruction.h"


B_DetectorConstruction::B_DetectorConstruction() {
    DefineMateials();

}

B_DetectorConstruction::~B_DetectorConstruction() {

}

G4VPhysicalVolume* B_DetectorConstruction::Construct() {
    DefineMateials();
    return DefineVolumes();
}

void B_DetectorConstruction::ConstructSDandField() {
    LSD = new B_SensitiveDetector("LSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(LSD);

    WestLogical->SetSensitiveDetector(LSD);
    SouthLogical->SetSensitiveDetector(LSD);
    NorthLogical->SetSensitiveDetector(LSD);
    EastLogical->SetSensitiveDetector(LSD);
    layer1_Logical->SetSensitiveDetector(LSD);

    //    G4cout << "_____________________________________________Detectors are made" << G4endl;
}

void B_DetectorConstruction::DefineMateials() {
    G4String symbol;
    G4double a, z, density;
    G4int ncomponents, natoms;
    G4double fractionmass;

    G4NistManager* man = G4NistManager::Instance();

    G4Element* C =
            new G4Element("Carbon", symbol = "C", z = 6., a = 12.01*g/mole);
    G4Element* H =
            new G4Element("Hydrogen", symbol = "H", z = 1., a = 1.0*g/mole);
    G4Element* N =
            new G4Element("Nitrogen", symbol = "N", z = 7., a = 14.01*g/mole);
    G4Element* O =
            new G4Element("Oxygen", symbol = "O", z = 8., a = 16.00*g/mole);
    G4Element* Al =
            new G4Element("Aluminum", symbol = "Al", z = 13., a = 26.98*g/mole);
    G4Element* Si =
            new G4Element("Silicon", symbol = "Si", z = 14., a = 28.09*g/mole);
    G4Element* elementXe =
            new G4Element("Xenon","Xe",54.,131.29*g/mole);

    G4Material* Air =
            new G4Material("Air", density = 0.000290*mg/cm3, ncomponents = 2);
    Air->AddElement(N, fractionmass = 0.7);
    Air->AddElement(O, fractionmass = 0.3);
    worldMaterial = Air;

    AluminumMirr = new G4Material("mirrAluminum", density = 2.7*g/cm3, ncomponents = 1);
    AluminumMirr->AddElement(Al, fractionmass = 1.0);

    // Aerogel Material (SiO2)
    Aerogel = new G4Material("Aerogel", density = 2000*g/m3, ncomponents = 2);
    Aerogel->AddElement(Si, natoms = 1);
    Aerogel->AddElement(O , natoms = 2);

    const G4int num = 51;
    G4double WaveLength[num];
    G4double Absorption[num];      // Default value for absorption
    G4double AirAbsorption[num];
    G4double AirRefractiveIndex[num];
    G4double PhotonEnergy[num];
    G4double AerogelRefractiveIndex[num];

    for (int i=0; i<num; i++) {
        WaveLength[i] = (200 + i*10)*nanometer;
        Absorption[i] = 100*m;      // Fake number for no absorption
        AirAbsorption[i] = 100*m;   // Place small value to kill photons
        AirRefractiveIndex[i] = 1.; // Rough air refraction
        PhotonEnergy[num - (i+1)] = twopi*hbarc/WaveLength[i];

        /* Absorption is given per length and G4 needs mean free path
         length, calculate it here
         mean free path length - taken as probablility equal 1/e
         that the photon will be absorbed */
        AerogelRefractiveIndex[i] = 1.1; // change rindex
    }


    G4MaterialPropertiesTable* AirMPT = new G4MaterialPropertiesTable();
    AirMPT->AddProperty("RINDEX", PhotonEnergy, AirRefractiveIndex, num);
    AirMPT->AddProperty("ABSLENGTH", PhotonEnergy, AirAbsorption, num);

    // Aerogel
    G4MaterialPropertiesTable* AerogelMPT = new G4MaterialPropertiesTable();
    AerogelMPT->AddProperty("RINDEX", PhotonEnergy, AerogelRefractiveIndex, num);
    AerogelMPT->AddProperty("ABSLENGTH", PhotonEnergy, Absorption, num);

    Vacuum = new G4Material( "Galactic", z=1., a=1.01*g/mole, density= universe_mean_density,
                             kStateGas, 2.73*kelvin, 3.e-18*pascal );

    Vacuum->SetMaterialPropertiesTable(AirMPT);
    Aerogel->SetMaterialPropertiesTable(AerogelMPT);
}




G4VPhysicalVolume* B_DetectorConstruction::DefineVolumes(){

    G4VSolid *worldSolid = new G4Box("World",
                                     BConst::worldSizeX/2,
                                     BConst::worldSizeY/2,
                                     BConst::worldSizeZ/2);

    G4LogicalVolume *worldLogical = new G4LogicalVolume(worldSolid,
                                                        Vacuum,
                                                        "World");

    G4VPhysicalVolume *worldPhysical = new G4PVPlacement(0,
                                                         G4ThreeVector(),
                                                         worldLogical,
                                                         "World",
                                                         0,
                                                         false,
                                                         0);


    //////////////////////////////////////////////////////////////////////

    G4VSolid *layer1_Solid = new G4Box("layer1",
                                      BConst::box_width/2.,
                                      BConst::layer1_height/2.,
                                      BConst::box_width/2.);


    layer1_Logical = new G4LogicalVolume(layer1_Solid,
                                        Aerogel,
                                        "layer1");


    G4VPhysicalVolume *layer1_Physical = new G4PVPlacement(0,
                                                           G4ThreeVector(),
                                                           layer1_Logical,
                                                           "layer1",
                                                           worldLogical,
                                                           false,
                                                           0);

    /////////////////////////////////////////////////////////////////////

    G4VSolid *layer2_Solid = new G4Box("layer2",
                                      BConst::box_width/2.,
                                      BConst::layer2_height/2.,
                                      BConst::box_width/2.);


    layer2_Logical = new G4LogicalVolume(layer2_Solid,
                                        Aerogel,
                                        "layer2");


    G4VPhysicalVolume *layer2_Physical = new G4PVPlacement(0,
                                                           G4ThreeVector(0.,
                                                                         (BConst::layer1_height + BConst::layer2_height)/2. + BConst::mirror_thickness,
                                                                         0.),
                                                           layer2_Logical,
                                                           "layer2",
                                                           worldLogical,
                                                           false,
                                                           0);

    /////////////////////////////////////////////////////////////////////

    G4VSolid *layer3_Solid = new G4Box("layer3",
                                      BConst::box_width/2.,
                                      BConst::layer3_height/2.,
                                      BConst::box_width/2.);


    layer3_Logical = new G4LogicalVolume(layer3_Solid,
                                        Aerogel,
                                        "layer3");


    G4VPhysicalVolume *layer3_Physical = new G4PVPlacement(0,
                                                           G4ThreeVector(0.,
                                                                         (BConst::layer1_height + BConst::layer3_height)/2. + 2*BConst::mirror_thickness + BConst::layer2_height,
                                                                         0.),
                                                           layer3_Logical,
                                                           "layer3",
                                                           worldLogical,
                                                           false,
                                                           0);

    /////////////////////////////////////////////////////////////////////


    //////////////// Placement of the detectors ///////////////////////
    G4VSolid *detectorSolid = new G4Box("detector",
                                        BConst::detector_thickness/2.,
                                        BConst::layer1_height/2.,
                                        BConst::box_width/2.);

    NorthLogical = new G4LogicalVolume(detectorSolid,
                                       Aerogel,
                                       "North");



    G4RotationMatrix *Ra = new G4RotationMatrix();

    G4VPhysicalVolume *NorthPhysical =  new G4PVPlacement(Ra,
                                                          G4ThreeVector((BConst::detector_thickness + BConst::box_width)/2.,0.,0.),
                                                          NorthLogical,
                                                          "North",
                                                          worldLogical,
                                                          false,
                                                          0);
    ///////////////////////////////////////////////////////


    WestLogical = new G4LogicalVolume(detectorSolid,
                                      Aerogel,
                                      "West");

    Ra = new G4RotationMatrix();
    Ra->rotateY(90*deg);
    G4VPhysicalVolume *WestPhysical =  new G4PVPlacement(Ra,
                                                         G4ThreeVector(0., 0, -(BConst::detector_thickness + BConst::box_width)/2.),
                                                         WestLogical,
                                                         "West",
                                                         worldLogical,
                                                         false,
                                                         0);
    ////////////////////////////////////////////////////////

    EastLogical = new G4LogicalVolume(detectorSolid,
                                      Aerogel,
                                      "East");

    G4VPhysicalVolume *EastPhysical =  new G4PVPlacement(Ra,
                                                         G4ThreeVector(0., 0, (BConst::detector_thickness + BConst::box_width)/2.),
                                                         EastLogical,
                                                         "East",
                                                         worldLogical,
                                                         false,
                                                         0);
    ////////////////////////////////////////////////////////

    SouthLogical = new G4LogicalVolume(detectorSolid,
                                       Aerogel,
                                       "South");

    G4VPhysicalVolume *SouthPhysical =  new G4PVPlacement(new G4RotationMatrix(),
                                                          G4ThreeVector(-(BConst::detector_thickness + BConst::box_width)/2.,0.,0.),
                                                          SouthLogical,
                                                          "South",
                                                          worldLogical,
                                                          false,
                                                          0);

    //////////////////// Mirrors at the top and bottom of the volume//////////////////


    G4VSolid *mirrorSolid = new G4Box("mirror",
                                      BConst::box_width/2,
                                      BConst::mirror_thickness/2,
                                      BConst::box_width/2);

    mirrorLogical = new G4LogicalVolume(mirrorSolid,
                                        AluminumMirr,
                                        "mirror");


    G4VPhysicalVolume *mirror1 = new G4PVPlacement(0,
                                                    G4ThreeVector(0., -(BConst::mirror_thickness/2 + BConst::layer1_height/2), 0.),
                                                    mirrorLogical,
                                                    "mirror1",
                                                    worldLogical,
                                                    false,
                                                    0);

    G4VPhysicalVolume *mirror12 = new G4PVPlacement(0,
                                                   G4ThreeVector(0., (BConst::mirror_thickness/2 + BConst::layer1_height/2), 0.),
                                                   mirrorLogical,
                                                   "mirror12",
                                                   worldLogical,
                                                   false,
                                                   0);

    G4VPhysicalVolume *mirror23 = new G4PVPlacement(0,
                                                    G4ThreeVector(0., (3*BConst::mirror_thickness/2 + BConst::layer1_height/2) + BConst::layer2_height, 0.),
                                                    mirrorLogical,
                                                    "mirror23",
                                                    worldLogical,
                                                    false,
                                                    0);

    G4VPhysicalVolume *mirror3 = new G4PVPlacement(0,
                                                   G4ThreeVector(0., (5*BConst::mirror_thickness/2 + BConst::layer1_height/2) + BConst::layer2_height + BConst::layer3_height, 0.),
                                                   mirrorLogical,
                                                   "mirror3",
                                                   worldLogical,
                                                   false,
                                                   0);


    // Define mirror surface
    const G4int num2 = 50;
    G4double EfficiencyMirrors[num2];
    G4double WaveLength[num2];
    G4double PhotonEnergy[num2];
    G4double MirrorReflectivity[num2];
    for (G4int i=0; i<num2; i++) {
      WaveLength[i] = (200 + i*10)*nanometer;
      PhotonEnergy[num2 - (i+1)] = twopi*hbarc/WaveLength[i];
      EfficiencyMirrors[i] = 0.0;
      MirrorReflectivity[i]=1.0;
    }

    G4MaterialPropertiesTable* MirrorMPT = new G4MaterialPropertiesTable();
    MirrorMPT->AddProperty("REFLECTIVITY", PhotonEnergy, MirrorReflectivity, num2);
    MirrorMPT->AddProperty("EFFICIENCY" , PhotonEnergy, EfficiencyMirrors,  num2);

    G4OpticalSurface* OpMirrorSurface = new G4OpticalSurface("MirrorSurface");
    OpMirrorSurface->SetType(dielectric_metal);
    OpMirrorSurface->SetFinish(polished);
    OpMirrorSurface->SetModel(glisur);
    new G4LogicalSkinSurface("MirrorSurfT",
                 mirrorLogical, OpMirrorSurface);

    OpMirrorSurface->SetMaterialPropertiesTable(MirrorMPT);



    DefineOpticalBorders();
    SetVisAttributes();
    G4cout << "_____________________________________________Volumes are made" << G4endl;
    return worldPhysical;
}



// Definition of absorbtion surfaces
void B_DetectorConstruction::DefineOpticalBorders()
{
    const G4int num1 = 2;
    G4double Ephoton[num1] = {1.5*eV, 5.8*eV};

    G4OpticalSurface* OpVolumeKillSurface =
            new G4OpticalSurface("VolumeKillSurface");
    OpVolumeKillSurface->SetType(dielectric_dielectric);
    OpVolumeKillSurface->SetFinish(polished);
    OpVolumeKillSurface->SetModel(glisur);

    G4double ReflectivityKill[num1] = {0., 0.};
    G4double EfficiencyKill[num1] = {1., 1.};
    G4MaterialPropertiesTable* VolumeKill = new G4MaterialPropertiesTable();
    VolumeKill->AddProperty("REFLECTIVITY", Ephoton, ReflectivityKill, num1);
    VolumeKill->AddProperty("EFFICIENCY",   Ephoton, EfficiencyKill,   num1);
    OpVolumeKillSurface->SetMaterialPropertiesTable(VolumeKill);

    G4OpticalSurface* quartzSurface = new G4OpticalSurface("quartzBorder");
    quartzSurface->SetType(dielectric_dielectric);


    new G4LogicalSkinSurface("DetectorAbsSurface",
                                 WestLogical, OpVolumeKillSurface);

    new G4LogicalSkinSurface("DetectorAbsSurface",
                                 SouthLogical, OpVolumeKillSurface);

    new G4LogicalSkinSurface("DetectorAbsSurface",
                                 NorthLogical, OpVolumeKillSurface);

    new G4LogicalSkinSurface("DetectorAbsSurface",
                                 EastLogical, OpVolumeKillSurface);
}


// Visual attributes
void B_DetectorConstruction::SetVisAttributes()
{
    G4Color blue        = G4Color(0., 0., 1.);
    G4Color green       = G4Color(0., 1., 0.);
    G4Color red         = G4Color(1., 0., 0.);
    G4Color white       = G4Color(1., 1., 1.);
    G4Color cyan        = G4Color(0., 1., 1., 0.3);
    G4Color magenda     = G4Color(1.,0.,1.);
    G4Color DircColor   = G4Color(0.0, 0.0, 1.0, 0.2);
    G4Color SensColor   = G4Color(0.0, 1.0, 1.0, 0.1);

    G4VisAttributes *wlsVisAtt = new G4VisAttributes;
    wlsVisAtt->SetColor(magenda);
    wlsVisAtt->SetVisibility(true);
    wlsVisAtt->SetForceSolid(true);

    G4VisAttributes *mirrorVisAtt = new G4VisAttributes;
    mirrorVisAtt->SetColor(blue);
    mirrorVisAtt->SetVisibility(true);
    mirrorVisAtt->SetForceSolid(true);


    G4VisAttributes *detectorVisAtt = new G4VisAttributes;
    detectorVisAtt->SetColor(white);
    detectorVisAtt->SetVisibility(true);
    detectorVisAtt->SetForceSolid(true);


//    wls_tubeLogical->SetVisAttributes(wlsVisAtt);
//    smallMirrorLogical->SetVisAttributes(mirrorVisAtt);
    NorthLogical->SetVisAttributes(detectorVisAtt);
    EastLogical->SetVisAttributes(detectorVisAtt);
    WestLogical->SetVisAttributes(detectorVisAtt);
    SouthLogical->SetVisAttributes(detectorVisAtt);
}
