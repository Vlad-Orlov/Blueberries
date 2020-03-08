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
    volumeLogical->SetSensitiveDetector(LSD);

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
    G4Element* Al =
            new G4Element("Aluminum", symbol = "Al", z = 13., a = 26.98*g/mole);
    G4Element* elementXe =
            new G4Element("Xenon","Xe",54.,131.29*g/mole);

    G4Material* Air =
            new G4Material("Air", density = 0.000290*mg/cm3, ncomponents = 2);
    Air->AddElement(N, fractionmass = 0.7);
    Air->AddElement(O, fractionmass = 0.3);
    worldMaterial = Air;

    AluminumMirr = new G4Material("mirrAluminum", density = 2.7*g/cm3, ncomponents = 1);
    AluminumMirr->AddElement(Al, fractionmass = 1.0);

    LXe =
            new G4Material ("LXe", 3.02*g/cm3, 1, kStateLiquid, 173.15*kelvin, 1.5*atmosphere);

    LXe->AddElement(elementXe, 1);


    const G4int num = 36;
    G4double WaveLength[num];
    G4double Absorption[num];      // Default value for absorption
    G4double AirAbsorption[num];
    G4double AirRefractiveIndex[num];
    G4double PhotonEnergy[num];
    G4double AerogelRefractiveIndex[num];
    // Absorption of quartz per 1m
    G4double QuartzAbsorption[num] =
    {0.999572036,0.999544661,0.999515062,0.999483019,0.999448285,
     0.999410586,0.999369611,0.999325013,0.999276402,0.999223336,
     0.999165317,0.999101778,0.999032079,0.998955488,0.998871172,
     0.998778177,0.99867541 ,0.998561611,0.998435332,0.998294892,
     0.998138345,0.997963425,0.997767484,0.997547418,0.99729958 ,
     0.99701966 ,0.99670255 ,0.996342167,0.995931242,0.995461041,
     0.994921022,0.994298396,0.993577567,0.992739402,0.991760297,
     0.990610945};

    for (int i=0; i<num; i++) {
        WaveLength[i] = (300 + i*10)*nanometer;
        Absorption[i] = 100*m;      // Fake number for no absorption
        AirAbsorption[i] = 100*m;   // Place small value to kill photons
        AirRefractiveIndex[i] = 1.; // Rough air refraction
        PhotonEnergy[num - (i+1)] = twopi*hbarc/WaveLength[i];

        /* Absorption is given per length and G4 needs mean free path
         length, calculate it here
         mean free path length - taken as probablility equal 1/e
         that the photon will be absorbed */
        AerogelRefractiveIndex[i] = 1.03;
        QuartzAbsorption[i] = (-1)/log(QuartzAbsorption[i])*100*cm;
    }

    G4double QuartzRefractiveIndex[num] =
    {1.456535,1.456812,1.4571  ,1.457399,1.457712,1.458038,
     1.458378,1.458735,1.459108,1.4595  ,1.459911,1.460344,
     1.460799,1.46128 ,1.461789,1.462326,1.462897,1.463502,
     1.464146,1.464833,1.465566,1.46635 ,1.46719 ,1.468094,
     1.469066,1.470116,1.471252,1.472485,1.473826,1.475289,
     1.476891,1.478651,1.480592,1.482739,1.485127,1.487793};


    const G4int NUMENTRIES = 9;
    G4double Scnt_PP[NUMENTRIES] = {2.0*eV, 2.1*eV, 2.2*eV, 2.3*eV,
                                    2.4*eV, 2.5*eV, 2.6*eV, 2.7*eV, 2.8*eV };
    G4double Scnt_FAST[NUMENTRIES] = {0.000134, 0.004432, 0.053991, 0.241971,
                                       0.398942, 0.000134, 0.004432, 0.053991,0.241971 };
    G4double Scnt_SLOW[NUMENTRIES] = { 0.000010, 0.000020, 0.000030,
                                       0.004000,0.008000, 0.005000,
                                       0.020000, 0.001000, 0.000010 };

    G4MaterialPropertiesTable* Scnt_MPT =new G4MaterialPropertiesTable();
    Scnt_MPT->AddProperty("FASTCOMPONENT", Scnt_PP, Scnt_FAST, NUMENTRIES);
    Scnt_MPT->AddProperty("SLOWCOMPONENT", Scnt_PP, Scnt_SLOW, NUMENTRIES);
    Scnt_MPT->AddConstProperty("SCINTILLATIONYIELD", .1/keV);
    Scnt_MPT->AddConstProperty("RESOLUTIONSCALE", 2.0);
    Scnt_MPT->AddConstProperty("FASTTIMECONSTANT",  1.*ns);
    Scnt_MPT->AddConstProperty("SLOWTIMECONSTANT", 10.*ns);
    Scnt_MPT->AddConstProperty("YIELDRATIO", 0.8);
    Scnt_MPT->AddProperty("RINDEX", PhotonEnergy, QuartzRefractiveIndex, num);
    Scnt_MPT->AddProperty("ABSLENGTH", PhotonEnergy, QuartzAbsorption, num);
    LXe->SetMaterialPropertiesTable(Scnt_MPT);

    G4MaterialPropertiesTable* AirMPT = new G4MaterialPropertiesTable();
    AirMPT->AddProperty("RINDEX", PhotonEnergy, AirRefractiveIndex, num);
    AirMPT->AddProperty("ABSLENGTH", PhotonEnergy, AirAbsorption, num);



    Vacuum = new G4Material( "Galactic", z=1., a=1.01*g/mole, density= universe_mean_density,
                             kStateGas, 2.73*kelvin, 3.e-18*pascal );

    Vacuum->SetMaterialPropertiesTable(AirMPT);

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


    G4VSolid *volumeSolid = new G4Box("volume",
                                     LConst::box_width/2,
                                     LConst::box_height/2,
                                     LConst::box_width/2);

    volumeLogical = new G4LogicalVolume(volumeSolid,
                                        LXe,
                                        "volume");

//    volumeLogical->SetSensitiveDetector(LSD);

    G4VPhysicalVolume *volumePhysical = new G4PVPlacement(0,
                                                          G4ThreeVector(),
                                                          volumeLogical,
                                                          "volume",
                                                          worldLogical,
                                                          false,
                                                          0);


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



    //////////////////// Mirrors at the top and bottom of the volume///////////////////////////////


    G4VSolid *mirrorSolid = new G4Box("mirror",
                                      LConst::box_width/2,
                                      LConst::mirror_thickness/2,
                                      LConst::box_width/2);

    mirrorLogical = new G4LogicalVolume(mirrorSolid,
                                        AluminumMirr,
                                        "mirror");


    G4VPhysicalVolume *mirrorTopPhysical = new G4PVPlacement(0,
                                                             G4ThreeVector(0., (LConst::mirror_thickness/2 + LConst::box_height/2), 0.),
                                                             mirrorLogical,
                                                             "mirrorTop",
                                                             worldLogical,
                                                             false,
                                                             0);

    G4VPhysicalVolume *mirrorBottomPhysical = new G4PVPlacement(0,
                                                                G4ThreeVector(0., -(LConst::mirror_thickness/2 + LConst::box_height/2), 0.),
                                                                mirrorLogical,
                                                                "mirrorTop",
                                                                worldLogical,
                                                                false,
                                                                0);


    // Define mirror surface
        const G4int num2 = 36;
        G4double EfficiencyMirrors[num2];
        G4double WaveLength[num2];
        G4double PhotonEnergy[num2];
        G4double MirrorReflectivity[num2];
        for (G4int i=0; i<num2; i++) {
          WaveLength[i] = (300 + i*10)*nanometer;
          PhotonEnergy[num2 - (i+1)] = twopi*hbarc/WaveLength[i];
          EfficiencyMirrors[i] = 0.0;
          MirrorReflectivity[i]=0.9800000000000004; //ya tut
        }
        /*
        G4double MirrorReflectivity[num2]=
          {0.87,0.88,0.885,0.89,0.895,0.9,0.905,0.91,0.915,0.92,0.923,0.9245,
           0.926,0.928,0.93,0.935,0.936,0.937,0.938,0.94,0.94,0.939,0.9382,
           0.938,0.937,0.937,0.936,0.935,0.934,0.932,0.93,0.928,0.926,0.924,
           0.922,0.92};
        */
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

    G4cout << "_____________________________________________Volumes are made" << G4endl;
    return worldPhysical;
}

// Definition of absorbtion surfaces
void L_DetectorConstruction::DefineOpticalBorders()
{
    const G4int num1 = 2;
    G4double Ephoton[num1] = {1.5*eV, 5.8*eV};

    G4OpticalSurface* OpVolumeKillSurface =
            new G4OpticalSurface("VolumeKillSurface");
    OpVolumeKillSurface->SetType(dielectric_metal);
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
                                 L1PlaneLogInner, OpVolumeKillSurface);

    new G4LogicalSkinSurface("DetectorAbsSurface",
                                 L2PlaneLogInner, OpVolumeKillSurface);

    new G4LogicalSkinSurface("DetectorAbsSurface",
                                 L1PlaneLogOuter, OpVolumeKillSurface);

    new G4LogicalSkinSurface("DetectorAbsSurface",
                                 L2PlaneLogOuter, OpVolumeKillSurface);
}

