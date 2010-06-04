// -*- C++ -*-
#ifdef __CINT__
// ##################################################
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
// PAFEventBase 
#pragma link C++ class PAFEventBase;
#pragma link C++ function operator << ( ostream &, const PAFEventBase & );
// PAFTagBase
#pragma link C++ class PAFTagBase;
#pragma link C++ function operator << ( ostream &, const PAFTagBase & );
// PAFBitBase
#pragma link C++ class PAFBitBase;
#pragma link C++ function operator << ( ostream &, const PAFBitBase & );
// PAFAodBase
#pragma link C++ class PAFAodTrack;
#pragma link C++ function operator << ( ostream &, const PAFAodTrack & );
#pragma link C++ class PAFAodCluster;
#pragma link C++ function operator << ( ostream &, const PAFAodCluster & );
#pragma link C++ class PAFAodBase;
#pragma link C++ function operator << ( ostream &, const PAFAodBase & );
#pragma link C++ class PAFChargedAodBase;
#pragma link C++ function operator << ( ostream &, const PAFChargedAodBase & );
#pragma link C++ class PAFNeutralAodBase;
#pragma link C++ function operator << ( ostream &, const PAFNeutralAodBase & );
#pragma link C++ class PAFCompAodBase;
#pragma link C++ class PAFListBase;
// PAFMcBase
#pragma link C++ class PAFMcBase;
#pragma link C++ class PAFMcTruth;
#pragma link C++ class PAFMcAssocBase;
// PID
#pragma link C++ class PAFAbsPidInfo;

#pragma link C++ class PAFStream;
#pragma link C++ class PAFMicroCandidate;

#pragma link C++ class PAFReader;
#pragma link C++ class PAFWriter;

// ###################################################
#endif
