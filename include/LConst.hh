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

    const G4double worldSizeX = 1*m;
    const G4double worldSizeY = 1*m;
    const G4double worldSizeZ = 1*m;

    const G4double box_width  = 50.*cm;
    const G4double box_height = 2.*cm;
    const G4double detector_thickness  = 1.*mm;
    const G4double mirror_thickness  = 1.*mm;

}










#endif /* INCLUDE_LCONST_HH_ */
