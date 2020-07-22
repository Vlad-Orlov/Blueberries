/*
 * B_Hit.cpp
 *
 *  Created on: July, 2020
 *      Author: orlov
 */


#include "B_Hit.h"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Square.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"

G4Allocator<B_Hit> B_HitAllocator;

B_Hit::B_Hit() :
  G4VHit()
{}

B_Hit::~B_Hit() {}

B_Hit::B_Hit(const B_Hit& right) :
  G4VHit()
{
  myData = right.myData;
}

const B_Hit& B_Hit::operator=(const B_Hit& right)
{
  myData = right.myData;
  return *this;
}

G4int B_Hit::operator==(const B_Hit& right) const
{
  return (this == &right) ? 1 : 0;
}

void B_Hit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (pVVisManager) {
    G4ThreeVector pos =
      G4ThreeVector(myData.X, myData.Y, myData.Z);
    G4Circle circle(pos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Color color(1., 0., 0.);
    G4VisAttributes att(color);
    circle.SetVisAttributes(att);
    pVVisManager->Draw(circle);
  }
}

void B_Hit::Print()
{
  /*
  G4cout << "Hit Print: "
	 << " trackID: " << trackID
	 << " energy: " << G4BestUnit(energy, "Energy")
	 << " position: " << G4BestUnit(pos, "Length")
	 << " time: " << G4BestUnit(time, "Time")
	 << G4endl;
  */
}
