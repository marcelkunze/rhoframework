#ifdef __CINT__
// ##################################################
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
// TEventManager
#pragma link C++ class TEventManager;
#pragma link C++ function operator << ( ostream &, const TEventManager & );
//TDirectoryManager
#pragma link C++ class TDirectoryManager;
#pragma link C++ function operator << ( ostream &, const TDirectoryManager & );
//TPersistenceManager
#pragma link C++ class TPersistenceManager;
#pragma link C++ function operator << ( ostream &, const TPersistenceManager & );
//TCandListManager
#pragma link C++ class TCandListManager;
#pragma link C++ function operator << ( ostream &, const TCandListManager & );
//TObjectManager
#pragma link C++ class TObjectManager;
#pragma link C++ function operator << ( ostream &, const TObjectManager & );
//TCacheManager
#pragma link C++ class TCacheManager;
//TParameterManager
#pragma link C++ class TParameterManager;
#pragma link C++ function operator << ( ostream &, const TParameterManager & );
// Monte-Carlo truth
#pragma link C++ class TAssociator;
#pragma link C++ function operator << ( ostream &, const TAssociator & );
#pragma link C++ class TMicroAssociator;
#pragma link C++ class TChi2Associator;
#pragma link C++ class TMapAssociator;
#pragma link C++ class TMcAssocAssociator;
// Analysis
#pragma link C++ class TAnalysis;
#pragma link C++ class TModule;
// Data Managment
#pragma link C++ class TDataBlock;
#pragma link C++ class TEventBlock;
#pragma link C++ class TRunInfo;
#pragma link C++ class TFileInfo;
#pragma link C++ class TLogInfo;
#pragma link C++ class TLogReader;
#pragma link C++ class TRunDB;
#pragma link C++ class TIndexCollection;
#pragma link C++ class TEventCollection;
#pragma link C++ class TRunCollection;
#pragma link C++ class TEventWriter;

#endif
