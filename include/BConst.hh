/*
 * BConst.hh
 *
 *  Created on: July, 2020
 *      Author: orlov
 */

#ifndef INCLUDE_BConst_HH_
#define INCLUDE_BConst_HH_

#include "TMath.h"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"

namespace BConst{

    const G4double worldSizeX = 1*m;
    const G4double worldSizeY = 1*m;
    const G4double worldSizeZ = 1*m;

    const G4double box_width  = 50.*cm;
    const G4double detector_thickness  = 1.*mm;
    const G4double mirror_thickness  = 1.*mm;

    const G4double layer1_height = 1.*cm;
    const G4double layer2_height = 4.*cm;
    const G4double layer3_height = 3.*cm;

//    const G4double wls_tube_diameter = box_height;
//    const G4double wls_tube_length = box_width - box_height - detector_thickness;

}










#endif /* INCLUDE_BConst_HH_ */
