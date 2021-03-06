/*
 * H_DetectorConstruction.h
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#ifndef B_DetectorConstruction_H_
#define B_DetectorConstruction_H_

#include <G4VUserDetectorConstruction.hh>
#include "B_SensitiveDetector.h"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Sphere.hh"
#include "G4ExtrudedSolid.hh"
#include "G4VSolid.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Color.hh"
#include "G4TwoVector.hh"
#include "G4SDManager.hh"
#include "globals.hh"

#include "BConst.hh"

class B_DetectorConstruction: public G4VUserDetectorConstruction {
public:
    B_DetectorConstruction();
    virtual ~B_DetectorConstruction();

public:
	  G4VPhysicalVolume* Construct();
	  void ConstructSDandField();
      void DefineOpticalBorders();
	  void DefineMateials();
      void SetVisAttributes();
	  G4VPhysicalVolume* DefineVolumes();
      B_SensitiveDetector *LSD;


      G4LogicalVolume *WestLogical;
      G4LogicalVolume *SouthLogical;
      G4LogicalVolume *NorthLogical;
      G4LogicalVolume *EastLogical;
      G4LogicalVolume *volumeLogical;
      G4LogicalVolume *mirrorLogical;
      G4LogicalVolume *wls_tubeLogical;
      G4LogicalVolume *smallMirrorLogical;



private:
	  G4Material *worldMaterial;
	  G4Material *ScintMaterial;
      G4Material *Vacuum;
      G4Material *AluminumMirr;
      G4Material *LXe;
      G4Material *SiO2;
      G4Material *Bis_MSB;





};

#endif /* B_DetectorConstruction_H_ */
