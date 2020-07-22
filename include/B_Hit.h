/*
 * B_Hit.h
 *
 *  Created on: July, 2020
 *      Author: orlov
 */

#ifndef SRC_B_Hit_H_
#define SRC_B_Hit_H_

#include <G4VHit.hh>
//G4
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//my
#include "HitDataStructure.hh"

class B_Hit: public G4VHit {
public:
    B_Hit();
    virtual ~B_Hit();

    B_Hit(const B_Hit&);
    const B_Hit& operator=(const B_Hit&);
    G4int operator==(const B_Hit&) const;

	inline void* operator new(size_t);
	inline void  operator delete(void*);

	void Draw();
	void Print();

public:

	HitData myData;

private:

};

typedef G4THitsCollection<B_Hit> B_HitsCollection;

extern G4Allocator<B_Hit> B_HitAllocator;

inline void* B_Hit::operator new(size_t)
{
	void *aHit;
    aHit = (void *) B_HitAllocator.MallocSingle();
	return aHit;
}

inline void B_Hit::operator delete(void *aHit)
{
    B_HitAllocator.FreeSingle((B_Hit*) aHit);
}

#endif
