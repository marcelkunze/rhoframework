#ifdef __CINT__
// ##################################################
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
// Time and date
#pragma link C++ class TRhoTime;
#pragma link C++ function operator << ( ostream &, const TRhoTime & );
#pragma link C++ class TRhoDate;
#pragma link C++ function operator << ( ostream &, const TRhoDate & );
// Conditions
#pragma link C++ class TConditions;
#pragma link C++ class TDchCond;
#pragma link C++ class TSvtPidCalib;
#pragma link C++ class TBeams;
#pragma link C++ class TEnergies;
#pragma link C++ class TBoostCal;
#pragma link C++ class TBeamSpotCal;
#pragma link C++ class TFillPattern;
#pragma link C++ class RooCondInterval;
#pragma link C++ class RooCondData;
#pragma link C++ class PepBeamsR;
#pragma link C++ class PepEnergiesR;
#pragma link C++ class PepBoostCalR;
#pragma link C++ class PepBeamSpotCalR;
#pragma link C++ class PepFillPatternR;
#pragma link C++ class DchBetheBlochR;
#pragma link C++ class SvtPidSimpleTMCalibR;

#endif
