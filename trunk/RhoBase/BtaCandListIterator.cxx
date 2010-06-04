//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaCandListIterator							//
//                                                                      //
// Iterator class for BtaCandList					//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Feb. 99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoBase/BtaCandListIterator.hh"
#include "RhoBase/BtaCandList.hh"
#include "RhoBase/BtaCandidate.hh"

ClassImp(BtaCandListIterator)

using namespace std;

BtaCandListIterator::BtaCandListIterator(BtaCandList & li) : 
  l(&li), i(0) {}

BtaCandListIterator::BtaCandListIterator(const BtaCandListIterator & li) : 
  l(li.l), i(li.i) {}

BtaCandListIterator::~BtaCandListIterator() {}

BtaCandidate * BtaCandListIterator::next() {
  if (l==0) return 0;
  return i < l->getNumberOfTracks() ? l->get(i++) : 0;
}

BtaCandidate * BtaCandListIterator::current() {
  return i < l->getNumberOfTracks() ? l->get(i) : 0;
}

BtaCandidate * BtaCandListIterator::previous() {
  if ( i >= l->getNumberOfTracks() ) i = l->getNumberOfTracks();
  return i ? l->get(--i) : 0;
}

void BtaCandListIterator::rewind() {
  i = 0;
}

void BtaCandListIterator::skipAll() {
  i = l->getNumberOfTracks();
}

int BtaCandListIterator::index() const {
  return i;
}

void BtaCandListIterator::skip(int si) {
  if ( si + int(i) < 0 )
    rewind();
  else
    i += si;
}
