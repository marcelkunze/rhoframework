//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaMicroCandidate	                                                //
//                                                                      //
// Implementation of the abstract interface AbsMicroCanidate		//
// for an underlying Kanga BtaMicroCanidateR object.			//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Nov. 99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "KangaSchema/BtaMicroCandidate.h"
#include "PAFSchema/PAFAbsPidInfo.h"
#include "RhoBase/VAbsPidInfo.h"
 
ClassImp(BtaMicroCandidate); 
 
Int_t BtaMicroCandidate::GetIfrFirstHit() const  
{ 
    int i; 
    UShort_t laystrips[20]; 
    fBtaMicroCandR->GetIfrQual()->GetIfrLayStrips(laystrips); 
    for (i=0;i<20;i++) if (laystrips[i]!=0) break;
    if (i==20) return 0;
    return i+1;  
} 
 
Int_t BtaMicroCandidate::GetIfrLastHit() const  
{  
    int i; 
    UShort_t laystrips[20]; 
    fBtaMicroCandR->GetIfrQual()->GetIfrLayStrips(laystrips); 
    for (i=19;i>=0;i--) if (laystrips[i]!=0) break;  
    return i+1; 
} 
 
VAbsPidInfo& BtaMicroCandidate::GetAbsPidInfo(PidSystem::System sys) const
{
    static PAFAbsPidInfo info;
    Float_t* sl = fBtaMicroCandR->GetPidInfo()->GetConsistency(sys);
    Float_t* lh = fBtaMicroCandR->GetPidInfo()->GetLikelihood(sys);
    UChar_t* st = fBtaMicroCandR->GetPidInfo()->GetStatus(sys);

    for (int i=0;i<5;i++) {
	Int_t status, sign;
	if( ( st[i] & 3 ) == 0 ) sign = 1; // unknown
	else if( ( st[i] & 3 ) == 1 ) sign = 0; // left
	else if( ( st[i] & 3 ) == 2 ) sign = 2; // right
	status = (( st[i] >> 2 ) & 3 );
	info.SetStats(i,sl[i],lh[i],status,sign);
    }
    return (VAbsPidInfo&) info;
}
 
std::ostream&  operator << (std::ostream& o, const BtaMicroCandidate& a) { a.PrintOn(o); return o; } 
