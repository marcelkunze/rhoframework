//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFReader								//
//                                                                      //
// Base class of the PAF event reader classes				//
//                                                                      //
// Author: Marcel Kunze, RUB, May 2000					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "THashList.h"
#include "TClonesArray.h"
#include "TList.h"
#include "TNode.h"
#include "TSPHE.h"
#include "TMaterial.h"
#include "TTreeFormula.h"

// PAF includes

#include "PAFSchema/PAFReader.h"
#include "PAFSchema/PAFStream.h"
#include "RhoBase/TRho.h"
#include "PAFSchema/PAFMcTruth.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/TCandidate.h"
#include "RhoTools/TPrintTree.h"
#include "RhoBase/VAbsMicroCandidate.h"
#include "RhoBase/VAbsBit.h"
#include "PAFSchema/PAFEventBase.h"
#include "PAFSchema/PAFBitBase.h"
#include "PAFSchema/PAFMcBase.h"
#include "PAFSchema/PAFListBase.h"
#include "RhoBase/TSmartPointer.h"

ClassImp(PAFReader)

#include <iostream>
#include <cstdlib>
using namespace std;

// Implementation of the PAFReader base class

PAFReader::PAFReader(const char *file, const char * description) :
VAbsReader(file,description),
fPAFMcTruthList(0),            // created only on purpose in FillMcTruthList()
fPAFMcTruthEntries(-1),
fBtaCandAssocMap(0),
fTagStream(0),
fEidStream(0),
fAodStream(0),
fMapStream(0),
fCmpStream(0),
fMctStream(0),
fTruStream(0),
fCounter(-1),
fTotal(0),
fStreams(new THashList),
fTag(kTRUE),
fAod(kTRUE),
fMap(kFALSE),
fCmp(kFALSE),
fMct(kFALSE),
fTagFormula(0),
fBitFormula(0),
fVerbose(kFALSE),
fPrintTree()
{
    fTagExpr = TString("");
    fBitExpr = TString("");
    fContent = new PAFEventBase();
    fBits    = new PAFBitBase();
    fAodListCharged = 0;
    fAodListNeutral = 0;
    fMctList = 0;
    fAssList = 0;
    fBtaList = 0;
    fMaxC = 0;
    fMaxN = 0;
    
    fOwnList = new TClonesArray("PAFMicroCandidate");
    fEntries = 0;
}

PAFReader::~PAFReader( ) 
{    
    delete fPAFMcTruthList;
    delete fTagFormula;
    delete fBitFormula;

    delete fContent; 
    delete fBits;
    delete fOwnList;
    delete fAodListsComp;
    delete fAodListCharged;
    delete fAodListNeutral;
    delete fMctList;
    delete fAssList;
    delete fBtaList;

    // Close all connected files
    TIter iter(fStreams);
    PAFStream *s = 0;
    while ( (s = (PAFStream *) iter.Next()) ) {
	if (s->GetFile()) s->GetFile()->Close();
    }
    
    fStreams->Delete(); delete fStreams;
}

void PAFReader::SetTagFormula(const char* formula)
{
    TString tagExpr(formula);
    fTagExpr = tagExpr;
    fPAFTagExpr = tagExpr.ReplaceAll("TAG_i","_tag.");
    fPAFTagExpr = fPAFTagExpr.ReplaceAll("TAG_f","_tag.");
    if (fTagFormula!=0) { delete fTagFormula; fTagFormula=0; }
    InitTagBranches(fTagStream->GetTree());
}

void PAFReader::SetBitFormula(const char* formula)
{
    TString bitExpr(formula);
    fBitExpr = bitExpr;
    if (fBitFormula!=0) { delete fBitFormula; fBitFormula=0; }
    InitBitBranches(fTagStream->GetTree());
}

Double_t PAFReader::EvalTagFormula()
{
    if (!fTagFormula) { return -1; }
    for (Int_t i=0; i<fTagFormula->GetNcodes(); i++) {
	fTagFormula->GetLeaf(i)->GetBranch()->GetEntry(fCounter);
    }
    return fTagFormula->EvalInstance(99999);
}

Double_t PAFReader::EvalBitFormula()
{
    if (!fBitFormula) { return -1; }
    for (Int_t i=0; i<fBitFormula->GetNcodes(); i++) {
	fBitFormula->GetLeaf(i)->GetBranch()->GetEntry(fCounter);
    }
    return fBitFormula->EvalInstance(99999);
}

UInt_t
PAFReader::GetCurrentIndex() 
{
    return fCounter;
}

PAFStream* PAFReader::NewStream(const char* AFileNameBase, const char* ATreeName)
{
    PAFStream *s= new PAFStream(AFileNameBase,ATreeName);  
    fStreams->Add(s);
    return s;
}

PAFStream* PAFReader::GetStream(Int_t StreamNo)
{
    return (PAFStream*)fStreams->At(StreamNo);
}

UInt_t PAFReader::GetNumberOfEventsProcessed() const 
{
    // Return the number of events currently processed
    return fTotal;
}

// __________________________________
// Read a specific Event from Streams
// __________________________________

Int_t PAFReader::GetEvent(UInt_t number) 
{
    // Tell the Streams to read next event; they 
    // know themselves whether they are "on" or "off"
    
    // Check if we reached the end of file
    
    if (fTagStream!=0 && fTagStream->IsActive())
	if (number>=fTagStream->GetTree()->GetEntries()) return 0;
	
	fCounter = number; // Note the actual event
	Int_t nbytes = 1;
	
	// It is enough for the tag and bit to set the event number
	// In case of a request, the corresponding branch is switched on
	// and the entry is read from the tree.
	fContent->SetOtherEventNumber(number);
	fBits->SetEventNumber(number);
	TSmartPointer::SetEventNumber(number);
	
	TIter iter(fStreams);
	PAFStream *s = 0;
	while ( (s = (PAFStream *) iter.Next()) ) {
	    if (s==fTagStream) continue; // Do not read this
	    if (!s->IsActive()) continue; // Do not read this
	    Int_t n = s->GetEvent(number);	
	    if (n<0) return 0; // Check the end
	    nbytes += n;
	}
	
	fTotal++;
	
	return nbytes;
}

VAbsTruth& PAFReader::GetMcTruth(Int_t n)
{
    // Provide access to McTruth Objects by index
    CreateMcTruthList();
    
    if ( n<0 || n>=fPAFMcTruthEntries ) {
	// correct would be: throw range_error;  no exceptions in PAF,  bummer ;(
	// memory leaking due to frustration
	cout << "PAFReader::GetMcTruth:  Index out of Bounds index:"<<n<<" max: "<<fPAFMcTruthEntries-1<< endl;
	return (*(new PAFMcTruth()));  
    }
    return *(VAbsTruth*)(*fPAFMcTruthList)[n];
}

void PAFReader::CreateMcTruthList()
{
    // Regenerate cache in case of a need
    static Int_t oldEvent = -1;
    if (GetEventNumber() == oldEvent) return;
    oldEvent = GetEventNumber();
    
    //    Ceate the TClonesArray of PAFMcTruth objects from an existing TCl.Arr. from PAFMcBases in fMctList.
    if (!fPAFMcTruthList) fPAFMcTruthList = new TClonesArray("PAFMcTruth");
    fPAFMcTruthList->Delete();  // PAFMcTruth owns a ObjArray with pointers to daughters. Maybe I shouldn't use a ClonesArray at all, no speed gain when deleting every time.
    fPAFMcTruthEntries = GetNumberOfMcTruthTracks();
    if (fVerbose) cout << "PAFReader::CreateMcTruthList :  MctTracks: #" << fPAFMcTruthEntries << endl;
    Int_t firstMother = -1;
    for (int i=0;i<fPAFMcTruthEntries;++i) { 
	PAFMcBase& mcbase = GetMcBase(i);
	new ( (*fPAFMcTruthList)[i] ) PAFMcTruth( mcbase );
	PAFMcTruth &mc = *(PAFMcTruth*)(*fPAFMcTruthList)[i];
	// the next line works because the indices are equal in fMctList and fMcTruthEntries
	int iBaseMother = mcbase.GetMother();
	if ( iBaseMother>=0 ) {
	    if (firstMother<0) firstMother = iBaseMother;
	    ((PAFMcTruth*)(*fPAFMcTruthList)[i])->SetMother(  (PAFMcTruth*)(*fPAFMcTruthList)[ iBaseMother ] );
	    ((PAFMcTruth*)(*fPAFMcTruthList)[iBaseMother])->AddDaughter( (PAFMcTruth*)(*fPAFMcTruthList)[i] );
	}
    }

    if (fPrintTree && firstMother!=-1) {
	const PAFMcTruth &mother = *(PAFMcTruth*)(*fPAFMcTruthList)[firstMother];
	cout << (fPrintTree->Print(mother)).Data() << endl;
    }
    
}

Int_t PAFReader::FillCanonicalMcTruthList(TCandList *aList)
{
    // Fill a list of McTruth candidates in canonical form (vertical trees)
    // This format is necessary to run e.g. the BABAR associators
    // 1) Fill a list of candidates in StdHep order
    // 2) Do a inversion based on TNodes (canonical representation)
    // 3) Construct a corresponding candidate list

    if ( !aList ) { cout << "PAFReader::FillCanonicalMcTruthList: Try to fill non-existant list" << endl; return 0; }
    
    aList->Reset();
    static TCandList truthList;
    if (FillMcTruthList(&truthList)==0) return 0;
    static TList mcList;
    mcList.Delete();
    LoadMcNodes(&mcList);
    Int_t entries = mcList.GetSize();
    for (int m=0;m<entries;m++) {
      Int_t id = mcList.At(m)->GetUniqueID();
      const TCandidate *c = truthList.Get(id);
      aList->Add(*c);
    }

    return entries;
}

Int_t PAFReader::FillMcTruthList(TCandList *aList)
{
    // The TCandList aList is filled with TCandidates with valid MCTruth Pointers.
    // The McTruth Pointers point to a PAFMcTruth Object that is owned by the EventManager.
    // It is more or less a mirror of the TClonesArray of McBases, but pointer linked.
    
    if ( !aList ) { cout << "PAFReader::FillMcTruthList: Try to fill non-existant list" << endl; return 0; }
    
    CreateMcTruthList();

    Float_t *bestMatch = new float[GetNumberOfCandidates()];
    for (int n=0;n<GetNumberOfCandidates();n++)  bestMatch[n] = 0.0;

    // Filll aList with TCandidates from PAFMcTruthList
    aList->Cleanup();
    for (int i=0;i<fPAFMcTruthEntries;++i) {
	PAFMcTruth *mcTruth = (PAFMcTruth*)(*fPAFMcTruthList)[i];
	aList->Put(TCandidate(*mcTruth,i));
	TCandidate *cand = aList->Get(i);
	Int_t index = mcTruth->GetAodMatchIndex();
	Double_t consistency = mcTruth->GetAodMatchConsistency();
	if (index>=0 && index<GetNumberOfCandidates()) {
	    VAbsMicroCandidate &micro = GetMicroCandidate(index);
	    if (consistency>bestMatch[index]) { // Found a better match
	      bestMatch[index] = consistency;
	      micro.SetMcPid(cand->PdtEntry()->PdgCode());
	      micro.SetMcConsistency(consistency);
	      micro.SetMcIndex(i);
	      cand->SetMicroCandidate(micro);
	      if (fVerbose) cout << "MCBase#" << i << "->Micro#" << index << " " << consistency << " "<< TRho::Instance()->GetPDG()->GetParticle(micro.GetMcPid())->GetName() << endl;
	    }
	}
    }

    delete bestMatch;
    return fPAFMcTruthEntries;
}

void 
PAFReader::LoadMcNodes( TList* mcList)
{
    CreateMcTruthList(); // Make sure the MCTruth is present
    Int_t nmc = GetNumberOfMcTruthTracks();
    if (nmc>NMCMAX) {
	cerr << "PAFReader::LoadMcNodes: nmc>" << NMCMAX << endl;
	return;
    }

    TList list;
    
    for(int j=0; j<nmc; j++ ) fUseStdHepTrack[j]=kTRUE;

    for(int i=0; i<nmc; i++ )
    {
	// Check STDHEP_INCOMING
	if ( GetMcTruth(i).GetStatus()==3) { fUseStdHepTrack[i]=kFALSE; continue;}
	if ( fUseStdHepTrack[i]==kFALSE ) continue;

	TNode* cand=0;   
	
	if( !MakeNodeFromStdTrack( i, cand ) )
	{
	    cerr << "PAFReader::LoadMcNodes: No TNode built for this StdHep entry ";
	    cerr << endl;
	    GetMcTruth(i).PrintOn( cerr );
	    cerr << endl;
	    continue;
	}
	
	// add the candidate to the temporary list
	if (fVerbose) cout << "PAFReader::LoadMcNodes: Add node " << cand->GetName() << endl;
	list.Add( cand );
    }

    // now iterate on the temporary list
    TListIter iterList( &list );
    TNode* cand=0;
    while( (cand=(TNode*)iterList.Next()) )
    {
	FillMcList( *mcList, *cand );
    }

}

void
PAFReader::PrintMcList(TNode& node)
{
    static Int_t tab = 0;
    for (int i=0; i<tab; i++) cout << " ";
    cout << node.GetName() << " " << node.GetUniqueID() << endl;
    TList *dauList = node.GetListOfNodes();
    if (dauList!=0) {
      tab += 2;
      for (int j=0; j<tab; j++) cout << " ";
      cout << "{ " << dauList->GetSize() << " daughters" << endl;
      TListIter iter(dauList);
      TNode* dau=0;
      while( (dau=(TNode*)iter.Next()) )
      {
	PrintMcList(*dau );
      }
      for (int k=0; k<tab; k++) cout << " ";
      cout << "}" << endl;
      tab -= 2;
    }
}

void
PAFReader::FillMcList( TList& list, TNode& cand )
{
    if (fVerbose) {
	cout << "PAFReader::FillMcList: Add to mcList " << cand.GetName();
	cout << " index=" << cand.GetUniqueID() << endl;
    }
    list.Add( (TObject*)&cand );
    TList *dauList = cand.GetListOfNodes();
    if (dauList!=0) {
	if (dauList->GetSize()==0) return;
	if (fVerbose) {
	    cout << " dau=" << dauList->GetSize();
	    for (int i=0;i<dauList->GetSize();i++) cout << " " << dauList->At(i)->GetName();
	    cout << endl;
	}
	TListIter iter(dauList);
	TNode* dau=0;
	while( (dau=(TNode*)iter.Next()) )
	{
	    FillMcList( list, *dau );
	}
    }
}

Bool_t
PAFReader::MakeNodeFromStdTrack(Int_t i, TNode*& mother )
{
    if (mother!=0) mother->cd();

    TDatabasePDG *pdt = TRho::Instance()->GetPDG();
    static TMaterial air("air","air",14.61,7.3,0.1205000E-02);
    static TSPHE sphere("Particle","Sphere","air",10.0);

// Create a TNode's descendents from the STDHEP structure   
    Int_t _nStdHepTracks = GetNumberOfMcTruthTracks();
    PAFMcBase *stdTrack = &GetMcBase(i);
    
    // this track should not be treated
    if( fUseStdHepTrack[i]==kFALSE ) {
	return kFALSE;
    }
        
    // get parameters
    double vx = stdTrack->GetStartVx();
    double vy = stdTrack->GetStartVy();
    double vz = stdTrack->GetStartVz();
    
    // make a node
    TParticlePDG *p = pdt->GetParticle(stdTrack->GetPdtID());
    const char *name = p->GetName();
    TNode* cand = new TNode(name,"StdHepTree",&sphere,vx,vy,vz);
    cand->SetUniqueID(i); // Note the MCTruth index
    if (mother==0) {
	mother = cand; // Set the first mother
	if (fVerbose) cout << "PAFReader::MakeNodeFromStdTrack: Setting first mother " << cand->GetName() << endl;
    }
    else if (fVerbose) {
	cout << "PAFReader::MakeNodeFromStdTrack: Add node " << cand->GetName() << " with index=";
	cout << cand->GetUniqueID();
	cout << " to " << mother->GetName() << endl;
    }
    
    // don't use this std track anymore
    fUseStdHepTrack[i]=kFALSE;
    
    // now Std daughters
    int firstDaughter = stdTrack->GetFirstDaughter();
    int lastDaughter  = stdTrack->GetLastDaughter();
    if( firstDaughter>=0 && lastDaughter>=firstDaughter )
    {
	for( int j=firstDaughter; j<=lastDaughter; j++ )
	{
	    if( fUseStdHepTrack[j]==kFALSE ) continue;
	    if( !MakeNodeFromStdTrack( j, cand ) )	    
	    {
		cerr << "PAFReader::MakeNodeFromStdTrack: No TNode was built from Std Track" << endl;
		break;
	    }
	}
    }

    mother->cd();
    
    // ok
    return kTRUE;
    
}

Bool_t PAFReader::SetStreamStatus(const char *stream,Bool_t status)
{
    TString s(stream);
    if (s=="tag")
	fTag = status;
    else if (s=="aod"||s=="micro")
	fAod = status;
    else if (s=="mct")
	fMct = status;
    else if (s=="map"||s=="readMaps")
	fMap = status;
    else if (s=="cmp"||s=="readCmpList")
	fCmp = status;
    else {
	cerr << "PAFReader::SetStreamStatus: Stream not defined " << stream << endl;
	return kFALSE;
    }

    return kTRUE;
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFReader								//
//                                                                      //
// Implementation of the PAF event reader classes			//
// This class reads the PAF schema					//
//                                                                      //
// Author: Marcel Kunze, RUB, May 2000					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#define KTAG      "TagTree"
#define SUFFTAG	  "Tag"
#define KAOD      "AodTree"
#define SUFFAOD	  "Aod"
#define KMAP      "CompTree"
#define SUFFMAP	  "Aod"
#define KMCT      "MctTree"
#define SUFFMCT	  "Mct"

#define TAGBRANCH  "TagList"
#define AODCBRANCH "AodListC"
#define AODNBRANCH "AodListN"
#define MCTBRANCH  "MctList"
#define ASSBRANCH  "AssList"
#define MAPBRANCH  "BtaCandMaps"

// Root includes

#include "TTreeFormula.h"
#include "TClonesArray.h"

// PAF includes

#include "PAFSchema/PAFEventBase.h"
#include "PAFSchema/PAFChargedAodBase.h"
#include "PAFSchema/PAFNeutralAodBase.h"
#include "PAFSchema/PAFCompAodBase.h"
#include "PAFSchema/PAFBitBase.h"
#include "PAFSchema/PAFListBase.h"
#include "PAFSchema/PAFStream.h"
#include "PAFSchema/PAFMicroCandidate.h"
#include "RhoBase/TRho.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/TCandidate.h"
#include "RhoBase/VAbsMicroCandidate.h"

#include "KangaSchema/BtaCandAssocMapR.h"


void PAFReader::InitializeStreams(Bool_t multi)
{
    // Initialize the "common blocks" (sorry for using fortran slang)
    
    fAodListsComp  = new PAFListBase();
    fAodListsComp->SetTree(0);
    fMctList = new TClonesArray("PAFMcBase");
    fNumMctTracks = 0;
    fAssList = new TClonesArray("PAFMcAssocBase");
    fNumMcAssoc = 0;
    //fBtaList = new TClonesArray("BtaCandAssocMapR");
    
    // Allocate PAF streams
    cout << "PAFReader: Allocate PAF streams" << endl;
    
    // Set up the default streams:
    
    if (fTag) {
	fTagStream = NewStream(SUFFTAG,KTAG); 
	fTagStream->AddObject("",TAGBRANCH,&fContent,"PAFEventBase");
	fTagStream->AddLeaflist("","TStampMajor",&fTimeStampMajor,"i");
	fTagStream->AddLeaflist("","TStampMinor",&fTimeStampMinor,"i");
	fTagStream->Enable();
	if (fVerbose) {
	    cout << "Add Tag-Stream" << endl; 
	    fTagStream->PrintOn(cout);
	}
    }
    
    if (fAod) {
	fAodStream = NewStream(SUFFAOD,KAOD); 
	fAodStream->AddClonesArray("",AODCBRANCH,&fAodListCharged,"PAFChargedAodBase");
	fAodStream->AddClonesArray("",AODNBRANCH,&fAodListNeutral,"PAFNeutralAodBase");
	fAodStream->AddLeaflist("","TStampMajor",&fTimeStampMajor,"i");
	fAodStream->AddLeaflist("","TStampMinor",&fTimeStampMinor,"i");
        fAodStream->Enable();
	if (fVerbose) {
	    cout << "Add Aod-Stream" << endl; 
	    fAodStream->PrintOn(cout);
	}
    }
    
    if (fMct) {
	fMctStream = NewStream(SUFFMCT,KMCT); 
	fMctStream->AddClonesArray("",MCTBRANCH,&fMctList,"PAFMcBase");
	fMctStream->AddClonesArray("",ASSBRANCH,&fAssList,"PAFMcAssocBase");
	fMctStream->AddClonesArray("",MAPBRANCH,&fBtaList,"BtaCandAssocMapR");
	fMctStream->AddLeaflist("","NumberOfTracks",&fNumMctTracks,"I");
	fMctStream->AddLeaflist("","TStampMajor",&fTimeStampMajor,"i");
	fMctStream->AddLeaflist("","TStampMinor",&fTimeStampMinor,"i");
        fMctStream->Enable();  
	if (fVerbose) {
	    cout << "Add Mct-Stream" << endl;
	    fMctStream->PrintOn(cout);
	}
    }
    
    if (fMap) {
	fMapStream = NewStream(SUFFMAP,KMAP); 
	fMapStream->AddObject("","",&fAodListsComp,"PAFListBase");
        fMapStream->Enable();
	if (fVerbose) {
	    cout << "Add Map-Stream" << endl; 
	    fMapStream->PrintOn(cout);
	}
    }
    
    // Try Opening the files

    const char *file = GetName();

    if (fTag && fTagStream->OpenFile(file)==0) { 
	fTagStream->Disable();
	fTag = kFALSE;
    }
    
    if (fAod && fAodStream->OpenFile(file)==0) {
	fAodStream->Disable();
	fAod = kFALSE;
    }
    
    
    if (fMct && fMctStream && fMctStream->OpenFile(file)==0) {
	fMctStream->Disable();
	fMct = kFALSE;
    }
    
    if (fMap && fMapStream && fMapStream->OpenFile(file)==0) {
	fMapStream->Disable();
	fMap = kFALSE;
    }
    
    // Initialize the branches
    
    if (fTag && fTagStream) {
	fTagStream->Enable();
	if (fTagStream->IsActive()) { 
	    // Access Tag branch
	    TTree *tagTree = fTagStream->GetTree();
	    if (tagTree!=0) 
		InitTagBranches(tagTree);
	    else
		cerr << "PAFReader::InitializeStreams: Could not locate TAG tree" << endl;
	    
	    // Access Kanga compatible Bit branch
	    // Try separate tree first...
	    TTree *bitTree = (TTree*) fTagStream->GetFile()->Get(SUFFTAG);
	    if (bitTree==0) bitTree = (TTree*) fTagStream->GetTree(); // Try new location
	    if (bitTree!=0) 
		InitBitBranches(bitTree);
	    else
		cerr << "PAFReader::InitializeStreams: Could not locate BIT tree" << endl;
	    
	    fTagStream->GetEvent(1); // This is to force the TAG-stream to read the tree into memory
	}
    }
    
    if (fAod && fAodStream) InitAodBranches(fAodStream->GetTree());
    if (fMap && fMapStream) InitMapBranches(fMapStream->GetTree());
    if (fMct && fMctStream) InitMctBranches(fMctStream->GetTree());
    
    if (!fAod && fAodStream) fAodStream->Disable();
    if (!fMap && fMapStream) fMapStream->Disable();
    if (!fMct && fMctStream) fMctStream->Disable();
}

UInt_t PAFReader::GetNumberOfLists() const
{
    return fAodListsComp->GetNumberOfLists();
}

const char* PAFReader::GetListName(Int_t i) const
{
    return fAodListsComp->GetListName(i);
}

// _______________________________________________________________
// Methods to enable branches dynamically 
// MUST be called each time the TAG- or AOD- trees are changed 
// _______________________________________________________________

void PAFReader::InitTagBranches(TTree *theTree)
{
    if (theTree==0) return;

    theTree->SetBranchStatus("*",1);
    
    // Notify PAFKangaReaderBase 
    fContent->SetTree(theTree);
    
    // Set up formula
    if (fPAFTagExpr!="") { 
	cout << "PAFReader::TagSelection " << fPAFTagExpr << endl;
	if (fTagFormula!=0) { delete fTagFormula; fTagFormula=0; }
	fTagFormula = new TTreeFormula("theTagFormula",fPAFTagExpr.Data(),theTree);
    }
} 

void PAFReader::InitBitBranches(TTree *theTree)
{
    if (theTree==0) return;

    theTree->SetBranchStatus("*",1);
    
    // Notify bit stream
    fBits->SetTree(theTree);
    
    // Set up formula
    if (fBitExpr!="") { 
        if (fBitFormula!=0) { delete fBitFormula; fBitFormula=0; }
	fBitFormula = new TTreeFormula("theBitFormula",fBitExpr.Data(),theTree);
    }
} 

void PAFReader::InitAodBranches(TTree *theTree)
{
    if (theTree==0) return;
 
    theTree->SetBranchStatus("*",1);
}

void PAFReader::InitMapBranches(TTree *theTree)
{
    if (theTree==0) return;
    theTree->SetBranchStatus("*",1);
    fAodListsComp->SetTree(theTree);
}

void PAFReader::InitMctBranches(TTree *theTree)
{
    Int_t version = theTree->GetCurrentFile()->GetVersion();
    theTree->SetBranchStatus("*",1); // ROOT 2.25/03 requires branches to be switched on
}

void PAFReader::InitTruBranches(TTree *theTree)
{}

UInt_t PAFReader::GetNumberOfTracks() const 
{
    // Return the total number of tracks in the event    
    return fContent->GetNumberOfTracks();
}

UInt_t PAFReader::GetNumberOfChargedTracks() const 
{
    // Return the number of charged tracks in the event
    return fContent->GetNumberOfChargedTracks();
}

UInt_t PAFReader::GetNumberOfNeutralTracks() const 
{
    // Return the number of neutral tracks in the event
    return fContent->GetNumberOfNeutralTracks();
}

UInt_t PAFReader::GetRunNumber() const 
{
    // Return the actual run number
    return fContent->GetRunNumber();
}

UInt_t PAFReader::GetEventNumber() const 
{
    // Return the actual event number
    return fContent->GetEventNumber();
}

UInt_t PAFReader::GetMajorID() const 
{
    return fContent->GetMajorID();
}

UInt_t PAFReader::GetMinorID() const 
{
    return fContent->GetMinorID();
}


// Access to object handles

PAFEventBase& PAFReader::GetHeader() const 
{
    // Return a handle to the event header
    return *fContent;
}    

VAbsTag& PAFReader::GetTag() const 
{
    // Return a handle to the TAG
    return fContent->GetTag();
}

VAbsBit& PAFReader::GetBit() const 
{
    // Return a handle to the BIT
    return *fBits;
}    

EidInfoR& PAFReader::GetEidInfo() const 
{
    // Return a handle to the EidInfo
    return *fEidInfo;
}

PAFListBase& PAFReader::GetListBase() const 
{
    return *fAodListsComp;
}


PAFChargedAodBase& PAFReader::GetChargedAodBase(Int_t i) const 
{
    return *((PAFChargedAodBase*)(*fAodListCharged)[i]);
}

PAFNeutralAodBase& PAFReader::GetNeutralAodBase(Int_t i) const 
{
    return *((PAFNeutralAodBase*)(*fAodListNeutral)[i]);
}

PAFMcBase& PAFReader::GetMcBase(Int_t i) const 
{
    // Return a handle to the McB of the i-th Truth Object
    return *((PAFMcBase*)(*fMctList)[i]);   // replace by dynamic_cast<PAFMcBase*> as soon as allowed
}

PAFMcAssocBase& PAFReader::GetMcAssocBase(Int_t i) 
{
    // Return a handle to the McB of the i-th Truth Object
    return *((PAFMcAssocBase*)(*fAssList)[i]);   // replace by dynamic_cast<PAFMcBase*> as soon as allowed
}

StdHepTrkR& PAFReader::GetStdHepTrk(Int_t i) const 
{
    // Return a handle to the MCT of the i-th Truth Object
    static StdHepTrkR *trk=0;
    return *trk;   // This is not implemented
}

BtaCandAssocMapR& PAFReader::GetCandAssocMap(Int_t i) const 
{
    // Return a handle to the MC association of i-th track
    return *((BtaCandAssocMapR*)(*fBtaList)[i]);   // replace by dynamic_cast<PAFMcBase*> as soon as allowed
}

BtaCandListR& PAFReader::GetCandList(Int_t i) const 
{
    // Return a handle to the MC association of i-th track
    static BtaCandListR *trk=0;
    return *trk;   // This is not implemented
}

UInt_t PAFReader::GetNumberOfCandAssocs() const 
{
    if (fNumMcAssoc>0) 
	return fNumMcAssoc;
    else 
	return ( fAssList ? fAssList->GetEntriesFast() : 0 ); 
}

UInt_t PAFReader::GetNumberOfMcTruthTracks() const 
{
    if (fNumMctTracks>0) 
	return fNumMctTracks;
    else 
	return ( fMctList ? fMctList->GetEntriesFast() : 0 ); 
}

VAbsMicroCandidate& PAFReader::GetMicroCandidate(Int_t i) const
{
    // Return a handle to the Micro of the i-th track
    
    Int_t nc = GetNumberOfChargedTracks();
    Int_t nn = GetNumberOfNeutralTracks();
    
    PAFMicroCandidate *aod = 0;
    
    if (i<nc) {
	PAFChargedAodBase &caod = GetChargedAodBase(i);
	new ((*fOwnList)[i]) PAFMicroCandidate(caod.GetBase(),caod.GetCluster(),caod.GetTrack());
	aod = (PAFMicroCandidate*)(*fOwnList)[i];
    }
    else if (i>=nc && i<nc+nn) {
	PAFNeutralAodBase &naod = GetNeutralAodBase(i-nc);
	new ((*fOwnList)[i]) PAFMicroCandidate(naod.GetBase(),naod.GetCluster(),0);
	aod = (PAFMicroCandidate*)(*fOwnList)[i];
    } else {
	cout << " *** WARNING in PAFReader::GetMicroCandidate : " << endl;
	cout << "     Requested MicroCandidate #" << i << " not available " << endl;
    }
    return *aod;
}

Int_t PAFReader::Fill(TCandList *l) 
{
    // Generate candidates and fill them into a list
    
    UInt_t nEntries = GetNumberOfTracks();
    l->Cleanup();
    fEntries = 0;
    for (UInt_t i=0;i<nEntries;++i) {
	l->Put(TCandidate(GetMicroCandidate(i),i));
    }
    return nEntries;
}

Int_t PAFReader::FillCharged(TCandList *l) 
{
    // Generate charged candidates and fill them into a list
    
    UInt_t nEntries = GetNumberOfChargedTracks();
    l->Cleanup();
    fEntries = 0;
    for (UInt_t i=0;i<nEntries;++i) {
	PAFChargedAodBase &aod = GetChargedAodBase(i);
	new ((*fOwnList)[fEntries]) PAFMicroCandidate(aod.GetBase(),aod.GetCluster(),aod.GetTrack());
	PAFMicroCandidate *c = (PAFMicroCandidate*)(*fOwnList)[fEntries];
	l->Put(TCandidate(*c,fEntries));
	fEntries++;
    }
    return fEntries;
}

Int_t PAFReader::FillNeutral(TCandList *l) 
{
    // Generate neutral candidates and fill them into a list
    
    UInt_t nEntries = GetNumberOfNeutralTracks();
    l->Cleanup();
    fEntries = GetNumberOfChargedTracks();
    for (UInt_t i=0;i<nEntries;++i) {
	PAFNeutralAodBase &aod = GetNeutralAodBase(i);
	if (aod.GetEmcNumberOfBumps()<=1) {
	    new ((*fOwnList)[fEntries]) PAFMicroCandidate(aod.GetBase(),aod.GetCluster(),0);
	    PAFMicroCandidate *c = (PAFMicroCandidate*)(*fOwnList)[fEntries];
	    l->Put(TCandidate(*c,fEntries));
	    fEntries++;
	}
    }
    return fEntries;
}

Int_t PAFReader::FillFromList(TCandList *l,const char* listname)
{
    Int_t currentIndex = GetCurrentIndex(); // The current entry
    
    // Clean the list
    l->Cleanup();
    
    // Get handle to the list array
    PAFListBase& theListBase = GetListBase();
    
    // PAF specific lists
    TClonesArray *theList = theListBase.GetList(listname,currentIndex);
    if (theList==0) { return 0; }
    Int_t len = theListBase.GetListLength(listname,currentIndex);
    if (len<0 || len >100) {
	cerr << "PAFReader::FillFromList: Skip irrational list length: " << len << endl;
	return 0;
    }
    
    l->Cleanup();
    
    for (Int_t i=0; i<len; i++) {
	PAFCompAodBase *b = (PAFCompAodBase*) ((*theList)[i]);
	Int_t index = b->GetAodRef();
	if (index<-1 || index>128) {
	    cerr << "PAFReader::FillFromList: Skip irrational index: " << index << endl;
	    continue;
	}
	
	if (index==-1) {    
	    // Produce a V0
	    if ((b->GetDaughter(0)!=-1) && (b->GetDaughter(1)!=-1)) {
		TCandidate *c0 = new TCandidate(GetMicroCandidate(b->GetDaughter(0)),b->GetDaughter(0));
		TCandidate *c1 = new TCandidate(GetMicroCandidate(b->GetDaughter(1)),b->GetDaughter(1));
		TCandidate *c2 = c0->Combine(*c1);
		l->Put(TCandidate(*c2));
		TCandidate& theCand = (*l)[i];
		theCand.AddDaughterLink(c0);  theCand.AddDaughterLink(c1); 
		delete c0; delete c1; delete c2;
		Int_t type = b->GetPdtRef();
		if (type == 0) type = 100; // Geantino
		theCand.SetType(TRho::Instance()->GetPDG()->GetParticle(type));
	    } else {
		cout << " *** WARNING in PAFReader::FillFromList : " << endl;
		cout << "     this V0 has less than 2 Daughters ! " << endl;
	    }
	} else if (b->GetDaughter(0)==-1) {
	    // Make 'simple' PAFCand from specific element in Aod-List
	    VAbsMicroCandidate &c = GetMicroCandidate(index);
	    l->Put(TCandidate(c,index));
	    TCandidate &theCand = (*l)[i];
	    Int_t type = b->GetPdtRef();
	    if (type == 0) type = 100; // Geantino
	    theCand.SetType(TRho::Instance()->GetPDG()->GetParticle(type));
	    fEntries++;
	} else {
	    // Produce a cluster
	    VAbsMicroCandidate &micro = GetMicroCandidate(index);
	    l->Put(TCandidate(micro,index));
	    TCandidate& theCand = (*l)[i];
	    Int_t type = b->GetPdtRef();
	    if (type == 0) type = 100; // Geantino
	    theCand.SetType(TRho::Instance()->GetPDG()->GetParticle(type));
	    Int_t k=0;
	    while (b->GetDaughter(k)>0) {
		VAbsMicroCandidate &micro = GetMicroCandidate(b->GetDaughter(k));
		TCandidate *c = new TCandidate(micro,b->GetDaughter(k));
		theCand.AddDaughterLink(c);
		delete c;
		k++;
	    }      
	    for (Int_t markerindex=0; markerindex<4; markerindex++) {
		Int_t currentMarker = theCand.GetMarker(markerindex);
		for (Int_t dau=0; dau<theCand.NDaughters(); dau++) {
		    currentMarker |= theCand.Daughter(dau)->GetMarker(markerindex);
		}
		theCand.SetMarker(currentMarker,markerindex);
	    }
	}
    }
    
    return len;
} 

TCandidate& PAFReader::GetCompositeCandidate(Int_t i) const
{
    static TCandidate c;
    return c;
}

