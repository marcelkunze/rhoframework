//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLogInfo								//
//                                                                      //
// Log info class							//
//                                                                      //
// Author: Thorsten Brandt, Dresden University, Aug. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoManager/TLogInfo.h"

ClassImp(TLogInfo)

#include <iostream>
using namespace std;

TLogInfo::TLogInfo(Int_t n) :
fLumi(0),
fYear(0),fDay(0),fMonth(0),
fStartHour(255),fStartMin(0),fStopHour(255),fStopMin(0)
{
    fCollection = TString("");
    SetRunNumber(n);
}

TLogInfo::~TLogInfo()
{
}

void 
TLogInfo::SetDate(Int_t y,Int_t m, Int_t d)
{
    fYear=y; fMonth=m; fDay=d;
}

void
TLogInfo::SetStart(Int_t h,Int_t m)
{
    fStartHour=h; fStartMin=m;
}

void
TLogInfo::SetStop(Int_t h,Int_t m)
{
    fStopHour=h; fStopMin=m;
}


void 
TLogInfo::SetRunNumber(Int_t no)
{
    char str[20];
    sprintf(str,"r%i",no);
    fRunNumber=no;
    SetName(str);
}

Int_t
TLogInfo::Compare(const TObject *obj) const
{
    TLogInfo *otherL = (TLogInfo*) obj;
    if (fRunNumber < otherL->fRunNumber) { return -1; }
    if (fRunNumber > otherL->fRunNumber) { return 1; }
    return 0;
}

void
TLogInfo::PrintOn(std::ostream &o) const
{
    o << " RUN #" << fRunNumber << " : ";
    if (fShiftType==TLogInfo::owl) { o << "OWL Shift"; }
    if (fShiftType==TLogInfo::swing) { o << "SWING Shift"; }
    if (fShiftType==TLogInfo::day) { o << "DAY Shift"; }
    Int_t dt,t1,t2;
    o << " on " << (Int_t) fDay << "/" << (Int_t) fMonth << "/" << (Int_t) fYear << endl;
    o << "   Begin of Data Taking : "; 
    if (fStartHour<24) {
	o << (Int_t) fStartHour << ":" << (Int_t) fStartMin << endl;
    } else {
	o << " unknown" << endl;
    }
    o << "   End of Data Taking   : "; 
    if (fStopHour<24) {
	o << (Int_t) fStopHour << ":" << (Int_t) fStopMin << endl; 
    } else { 
	o << " unknown" << endl;
    }
    o << "   Run - Type           : " ;
    if (fRunType== TLogInfo::colliding) { o << "Colliding Beams"; }
    if (fRunType== TLogInfo::cosmics) { o << "Cosmics"; }
    if (fRunType== TLogInfo::stable) { o << "Stable Beams"; }
    if (fRunType== TLogInfo::calibration) { o << "Calibration"; }
    if (fRunType== TLogInfo::other) { o << "Other"; }
    if (fRunType== TLogInfo::none) { o << "Unknown"; }
    o << endl;
    o << "   Solenoid Current=";
    if (fSolenoid>0) { o << fSolenoid; } else { o << "n/a"; }
    o << "   Beamenergies (GeV)   HER = " << fHer << "  LER = " << fLer << endl;
    o << "   Integrated Luminosity : ";
    if (fLumi>0) { o << fLumi << " nb-1"; } else { o << "n/a"; }
    o << endl;
    if (fCollection.Length()==0) {
	cout << "  NOT yet reconstructed" << endl;
    } else {
	cout << "   OPR : " << fCollection.Data() << endl;
	cout << "          Procspec " << fProcspec.Data() << " , Vers " << (Int_t) fVersion << " , CONS " << (Int_t) fCons << endl;
    }
    o << " ----------------------------------------" << endl;
    
}


/*Bool_t 
TLogInfo::operator== (TLogInfo &other)
{
return ( (fCollection.GetString()==other.fCollection.GetString) && (_oprRelease==other._oprRelease) &&
(_oprVersion==other._oprVersion) && (fSolenoid==other.fSolenoid) && 
(_nL1Events==other._nL1Events) &&  (_nL3Events==other._nL3Events) && 
(fLumi == other.fLumi) && (fRunType==other.fRunType) &&
(_shiftType==other._shiftType) ) ;
}
*/
