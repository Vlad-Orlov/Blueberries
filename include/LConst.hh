/*
 * LConst.hh
 *
 *  Created on: Oct 2, 2018
 *      Author: vsevolod
 */

#ifndef INCLUDE_LCONST_HH_
#define INCLUDE_LCONST_HH_

#include "TMath.h"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"

namespace LConst{

    const G4double worldSizeX = 2*m;
    const G4double worldSizeY = 2*m;
    const G4double worldSizeZ = 8*m;


    const G4double BPOuterRadius = 31*mm;
    const G4double BPInnerRadius = 30*mm;

    const G4double L1pozZ = - 2172.*mm;
//    const G4double L1pozZ = - 2172.*mm + 400*mm;
//    const G4double L1pozZ = - 2172.*mm + 800*mm;

//    const G4double L2pozZ = - 2172.*mm + 800*mm;

//    const G4double BeamStart = - 879.*mm;

    // VELO Sphere
    const G4double capHeight = 280.*mm;
    const G4double capBaseR = 1000./2. *mm;
    const G4double sphereStart = - 500.*mm;
    const G4double sphereThickness = 5.*mm;

    const G4double sphereR = (capHeight*capHeight + capBaseR*capBaseR)/2. / capHeight;
    const G4double sphereTheta = TMath::ATan(capBaseR/2. / (sphereR - capHeight));
    const G4double sphereOuterR = sphereR;
    const G4double sphereInnerR = sphereR - sphereThickness;
    const G4double sphereCenter = sphereStart - capHeight + sphereR;

    const G4double BeamStart = sphereStart - capHeight + 32.*mm;

    const G4double VeloLeft = -175*mm;
    const G4double VeloRight = 350*mm;

    const G4double nSecOut = 10;
    const G4double nSecIn = 10;

    const G4double innerRadIn = 10*mm;
    const G4double outerRadIn = BPInnerRadius;

    const G4double innerRadOut = BPOuterRadius;
    const G4double outerRadOut = 10*cm;



}










#endif /* INCLUDE_LCONST_HH_ */
