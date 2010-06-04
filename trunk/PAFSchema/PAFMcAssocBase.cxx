//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFMcAssocBase							//
//                                                                      //
// Definition of the Monte Carlo truth base class in PAF		//
//                                                                      //
// Author: Rolf Dubitzky - TU Dresden, June 99				//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "PAFSchema/PAFMcAssocBase.h"

ClassImp(PAFMcAssocBase)

PAFMcAssocBase::PAFMcAssocBase() :
  fMicroIndex(0),
  fMcTruIndex(0),
  fConsistency(0)
{}

PAFMcAssocBase::~PAFMcAssocBase() 
{}

PAFMcAssocBase::PAFMcAssocBase( const PAFMcAssocBase& other ) :
fMicroIndex( other.GetMicro() ), 
fMcTruIndex( other.GetMcTru() ), 
fConsistency( other.GetConsistency() )
{ }
