//////////////////////////////////////////////////////////////////////////
//                                                                      //
// KangaReader								//
//                                                                      //
// Implementation of the PAF event reader classes			//
// This class reads the Kanga schema					//
//                                                                      //
// Author: Marcel Kunze, RUB, May 2000					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////
//#define SCHEMA3

// Here are some defines for the LBL Kanga format

#define KTAG      "microDST"
#define SUFFTAG	  "-micro"
#define KAOD      "microDST"
#define SUFFAOD	  "-micro"
#define KMAP      "microDST"
#define SUFFMAP	  "-micro"
#define KCOMP     "microDST"
#define SUFFCOMP  "-micro"
#define KMCT      "microDST"
#define SUFFMCT	  "-micro"

#define EIDBRANCH "BtaEid"
#define AODBRANCH "BtaAod"
#define MAPBRANCH "BtaCandMaps"
#define COMPBRANCH "CompositeCandidates"
#define STDHEPBRANCH "BtaStdHep"
#define MICROTRUBRANCH "MicroTruMap"

// Root includes

#include "TROOT.h"
#include "TTreeFormula.h"
#include "TNode.h"
#include "TMaterial.h"
#include "TSPHE.h"

// Rho includes

#include "RhoBase/TRho.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/TCandidate.h"
#include "RhoBase/VAbsVertex.h"
#include "RhoBase/VAbsTruth.h"
#include "RhoBase/TSmartPointer.h"
#include "RhoBase/VAbsMicroCandidate.h"
#include "RhoTools/TPrintTree.h"

// Kanga includes

#include "KangaSchema/KangaReader.h"
#include "KangaSchema/kanga.h"
#include "KangaSchema/EidInfoR.h"
#include "KangaSchema/TBtaMicroCandR.h"
#include "KangaSchema/BtaCandListR.h"
#include "KangaSchema/BtaCandListR_001.h"
#include "KangaSchema/BtaCandidateR.h"
#include "KangaSchema/BtaCandPtrR.h"
#include "KangaSchema/StdHepR.h"
#include "KangaSchema/StdHepTrkR.h"
#include "KangaSchema/BtaCandAssocMapR.h"
#include "KangaSchema/RooClonesVectorR.h"
#include "KangaSchema/BtaCandidateR.h"
#include "KangaSchema/BtaAbsVertexR.h"
#include "KangaSchema/RooValArray.h"
#include "KangaSchema/BtaMicroCandidate.h"

// PAF includes

#include "PAFSchema/PAFMcBase.h"
#include "PAFSchema/PAFMcTruth.h"
#include "PAFSchema/PAFStream.h"
#include "PAFSchema/PAFCompAodBase.h"
#include "PAFSchema/PAFBitBase.h"
#include "PAFSchema/PAFMcAssocBase.h"
#include "PAFSchema/PAFListBase.h"
#include "PAFSchema/PAFEventBase.h"

ClassImp(KangaReader);

#include <iostream>
#include <cstdlib>
using namespace std;

//Remove this comment to read the old leaking lists
#ifdef SCHEMA3
#define BtaCandListR_001 BtaCandListR
#endif

KangaReader::KangaReader(const char *file, const char * description) : 
VAbsReader(file, description),
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
    fAss     = new PAFMcAssocBase();
    fMcList  = new TList();
    fAodListCharged = 0;
    fAodListNeutral = 0;
    fMctList = 0;
    fEidInfo = 0;
    fBtaMicroList = 0;
    fBtaCandList = 0;
    fBtaCompList = 0;
    
    fOwnList= new TClonesArray("BtaMicroCandidate");
    fEntries = 0;
}

KangaReader::~KangaReader( ) 
{
    delete fPAFMcTruthList;
    delete fTagFormula;
    delete fBitFormula;

    delete fContent; 
    delete fBits;
    delete fAss;
    delete fMcList;
    delete fOwnList;
    delete fMctList;
    delete fEidInfo;
    delete fBtaMicroList;
    delete fBtaCandList;
    delete fBtaCompList;
    delete fBtaCandAssocMap;
    delete fStdHepR;
    delete fAodListsComp;

    // Close all connected files
    TIter iter(fStreams);
    PAFStream *s = 0;
    while (s = (PAFStream *) iter.Next()) {
	if (s->GetFile()) s->GetFile()->Close();
    }
    
    fStreams->Delete(); delete fStreams;
}

void KangaReader::SetTagFormula(const char* formula)
{
    TString tagExpr(formula);
    fTagExpr = tagExpr;
    fPAFTagExpr = tagExpr.ReplaceAll("TAG_i","_tag.");
    fPAFTagExpr = fPAFTagExpr.ReplaceAll("TAG_f","_tag.");
    if (fTagFormula!=0) { delete fTagFormula; fTagFormula=0; }
    InitTagBranches(fTagStream->GetTree());
}

void KangaReader::SetBitFormula(const char* formula)
{
    TString bitExpr(formula);
    fBitExpr = bitExpr;
    if (fBitFormula!=0) { delete fBitFormula; fBitFormula=0; }
    InitBitBranches(fTagStream->GetTree());
}

Double_t KangaReader::EvalTagFormula()
{
    if (!fTagFormula) { return -1; }
    for (Int_t i=0; i<fTagFormula->GetNcodes(); i++) {
	fTagFormula->GetLeaf(i)->GetBranch()->GetEntry(fCounter);
    }
    return fTagFormula->EvalInstance(99999);
}

Double_t KangaReader::EvalBitFormula()
{
    if (!fBitFormula) { return -1; }
    for (Int_t i=0; i<fBitFormula->GetNcodes(); i++) {
	fBitFormula->GetLeaf(i)->GetBranch()->GetEntry(fCounter);
    }
    return fBitFormula->EvalInstance(99999);
}

UInt_t
KangaReader::GetCurrentIndex() 
{
    return fCounter;
}

PAFStream* KangaReader::NewStream(const char* AFileNameBase, const char* ATreeName)
{
    PAFStream *s= new PAFStream(AFileNameBase,ATreeName);  
    fStreams->Add(s);
    return s;
}

PAFStream* KangaReader::GetStream(Int_t StreamNo)
{
    return (PAFStream*)fStreams->At(StreamNo);
}

UInt_t KangaReader::GetNumberOfEventsProcessed() const 
{
    // Return the number of events currently processed
    return fTotal;
}

// __________________________________
// Read a specific Event from Streams
// __________________________________

Int_t KangaReader::GetEvent(UInt_t number) 
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
	while (s = (PAFStream *) iter.Next()) {
	    if (s==fTagStream) continue; // Do not read this
	    if (!s->IsActive()) continue; // Do not read this
	    Int_t n = s->GetEvent(number);	
	    if (n<0) return 0; // Check the end
	    nbytes += n;
	}
	
	fTotal++;
	
	return nbytes;
}

VAbsTruth& KangaReader::GetMcTruth(Int_t n)
{
    // Provide access to McTruth Objects by index
    CreateMcTruthList();
    
    if ( n<0 || n>=fPAFMcTruthEntries ) {
	// correct would be: throw range_error;  no exceptions in PAF,  bummer ;(
	// memory leaking due to frustration
	cout << "KangaReader::GetMcTruth:  Index out of Bounds index:"<<n<<" max: "<<fPAFMcTruthEntries-1<< endl;
	return (*(new PAFMcTruth()));  
    }
    return *(VAbsTruth*)(*fPAFMcTruthList)[n];
}

void KangaReader::CreateMcTruthList()
{
    // Regenerate cache in case of a need
    static Int_t oldEvent = -1;
    if (GetEventNumber() == oldEvent) return;
    oldEvent = GetEventNumber();
    
    //    Ceate the TClonesArray of PAFMcTruth objects from an existing TCl.Arr. from PAFMcBases in fMctList.
    if (!fPAFMcTruthList) fPAFMcTruthList = new TClonesArray("PAFMcTruth");
    fPAFMcTruthList->Delete();  // PAFMcTruth owns a ObjArray with pointers to daughters. Maybe I shouldn't use a ClonesArray at all, no speed gain when deleting every time.
    fPAFMcTruthEntries = GetNumberOfMcTruthTracks();
    if (fVerbose) cout << "KangaReader::CreateMcTruthList :  MctTracks: #" << fPAFMcTruthEntries << endl;
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
	cout << fPrintTree->Print(mother) << endl;
    }
    
}

Int_t KangaReader::FillCanonicalMcTruthList(TCandList *aList)
{
    // Fill a list of McTruth candidates in canonical form (vertical trees)
    // This format is necessary to run e.g. the BABAR associators
    // 1) Fill a list of candidates in StdHep order
    // 2) Do a inversion based on TNodes (canonical representation)
    // 3) Construct a corresponding candidate list

    if ( !aList ) { cout << "KangaReader::FillCanonicalMcTruthList: Try to fill non-existant list" << endl; return 0; }
    
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

Int_t KangaReader::FillMcTruthList(TCandList *aList)
{
    // The TCandList aList is filled with TCandidates with valid MCTruth Pointers.
    // The McTruth Pointers point to a PAFMcTruth Object that is owned by the EventManager.
    // It is more or less a mirror of the TClonesArray of McBases, but pointer linked.
    
    if ( !aList ) { cout << "KangaReader::FillMcTruthList: Try to fill non-existant list" << endl; return 0; }
    
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
KangaReader::LoadMcNodes( TList* mcList)
{
    CreateMcTruthList(); // Make sure the MCTruth is present
    Int_t nmc = GetNumberOfMcTruthTracks();
    if (nmc>NMCMAX) {
	cerr << "KangaReader::LoadMcNodes: nmc>" << NMCMAX << endl;
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
	    cerr << "KangaReader::LoadMcNodes: No TNode built for this StdHep entry ";
	    cerr << endl;
	    GetMcTruth(i).PrintOn( cerr );
	    cerr << endl;
	    continue;
	}
	
	// add the candidate to the temporary list
	if (fVerbose) cout << "KangaReader::LoadMcNodes: Add node " << cand->GetName() << endl;
	list.Add( cand );
    }

    // now iterate on the temporary list
    TListIter iterList( &list );
    TNode* cand=0;
    while( cand=(TNode*)iterList.Next() )
    {
	FillMcList( *mcList, *cand );
    }

}

void
KangaReader::PrintMcList(TNode& node)
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
      while( dau=(TNode*)iter.Next() )
      {
	PrintMcList(*dau );
      }
      for (int k=0; k<tab; k++) cout << " ";
      cout << "}" << endl;
      tab -= 2;
    }
}

void
KangaReader::FillMcList( TList& list, TNode& cand )
{
    if (fVerbose) {
	cout << "KangaReader::FillMcList: Add to mcList " << cand.GetName();
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
	while( dau=(TNode*)iter.Next() )
	{
	    FillMcList( list, *dau );
	}
    }
}

Bool_t
KangaReader::MakeNodeFromStdTrack(Int_t i, TNode*& mother )
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
	if (fVerbose) cout << "KangaReader::MakeNodeFromStdTrack: Setting first mother " << cand->GetName() << endl;
    }
    else if (fVerbose) {
	cout << "KangaReader::MakeNodeFromStdTrack: Add node " << cand->GetName() << " with index=";
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
		cerr << "KangaReader::MakeNodeFromStdTrack: No TNode was built from Std Track" << endl;
		break;
	    }
	}
    }

    mother->cd();
    
    // ok
    return kTRUE;
    
}

Bool_t KangaReader::SetStreamStatus(const char *stream,Bool_t status)
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
	cerr << "KangaReader::SetStreamStatus: Stream not defined " << stream << endl;
	return kFALSE;
    }

    return kTRUE;
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// KangaReader								//
//                                                                      //
// Implementation of the PAF event reader classes			//
// This class reads the Kanga schema					//
//                                                                      //
// Author: Marcel Kunze, RUB, May 2000					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////
//#define SCHEMA3

void KangaReader::InitializeStreams(Bool_t multi)
{
    // MC and map containers
    
    fAodListsComp  = new PAFListBase();
    fAodListsComp->SetTree(0);
    fMctList = new TClonesArray("PAFMcBase");
    fNumMctTracks = 0;
    fNumMcAssoc = 0;
    
    // Kanga containers
    
    fEidInfo = new EidInfoR();
    fBtaCandList  = new TClonesArray("BtaCandListR_001");
    fBtaCompList  = new TClonesArray("BtaCandidateR");
    fBtaMicroList = new TClonesArray("TBtaMicroCandR");
    fBtaCandAssocMap = new RooClonesVectorR<BtaCandAssocMapR>;
    fStdHepR = new StdHepR();
    
    // Allocate Kanga streams (We always need the tag)
    
    cout << "KangaReader: Allocate KanGA streams" << endl;
    
    // Set up the Kanga streams
    
    if (kTRUE) {
	fTagStream = NewStream(SUFFTAG,KTAG);
	fTagStream->Enable();
	if (fVerbose) {
	    cout << "Add Tag-Stream" << endl; 
	    fTagStream->PrintOn(cout);
	}
    }
    
    if (fAod) {
	fAodStream = NewStream(SUFFAOD,KAOD);
	fAodStream->AddObject("",EIDBRANCH,&fEidInfo,"EidInfoR");
	fAodStream->AddClonesArray("",AODBRANCH,&fBtaMicroList,"TBtaMicroCandR");
	fAodStream->AddLeaflist(AODBRANCH,"_nent",&fNumRecTracks,"I");
	fNumRecTracks = 0;
	fAodStream->Enable();
	if (fVerbose) {
	    cout << "Add Aod-Stream" << endl; 
	    fAodStream->PrintOn(cout);
	}
    }
    
    if (fMap) {
	fMapStream = NewStream(SUFFMAP,KMAP);
	fMapStream->AddClonesArray("",MAPBRANCH,&fBtaCandList,"BtaCandListR_001");
	fNumMaps = 0;
	fMapStream->Enable();
	if (fVerbose) {
	    cout << "Add Map-Stream" << endl; 
	    fMapStream->PrintOn(cout);
	}
    }
    
    if (fCmp) {
	fCmpStream = NewStream(SUFFCOMP,KCOMP);
	fCmpStream->AddClonesArray("",COMPBRANCH,&fBtaCompList,"BtaCandidateR");
	fCmpStream->Enable();
	if (fVerbose) {
	    cout << "Add Cmp-Stream" << endl; 
	    fMapStream->PrintOn(cout);
	}
    }
    
    if (fMct) {
	fMctStream = NewStream(SUFFMCT,KMCT);
	fMctStream->AddObject("",STDHEPBRANCH,&fStdHepR,"StdHepR");
	fMctStream->Enable();
	if (fVerbose) {
	    cout << "Add Mct-Stream" << endl; 
	    fMctStream->PrintOn(cout);
	}
	fTruStream = NewStream(SUFFMCT,KMCT);
	fTruStream->AddObject("",MICROTRUBRANCH,&fBtaCandAssocMap,"BtaCandAssocMapR");
	fTruStream->AddLeaflist(MICROTRUBRANCH,"_nent",&fNumMcAssoc,"I");
	fNumMctTracks = 0;
	fTruStream->Enable();
	if (fVerbose) {
	    cout << "Add Tru-Stream" << endl; 
	    fTruStream->PrintOn(cout);
	}
    }
    
    
    const char *file = GetName();
    
    if (!multi) {
	TFile *f = fTagStream->OpenFile(file,kTRUE);	
	if (fAod) fAodStream->Append(f);
	if (fMap) fMapStream->Append(f);
	if (fCmp) fCmpStream->Append(f);
	if (fMct) fMctStream->Append(f);
	if (fMct) fTruStream->Append(f);
    }
    else {
	if (fTag && fTagStream->OpenFile(file)==0) { 
	    fTagStream->Disable();
	    fTag = kFALSE;
	}
	if (fAod && fAodStream->OpenFile(file)==0) {
	    fAodStream->Disable();
	    fAod = kFALSE;
	}
	if (fMap && fMapStream->OpenFile(file)==0) {
	    fMapStream->Disable();
	    fMap = kFALSE;
	}
	if (fCmp && fMapStream->OpenFile(file)==0) {
	    fCmpStream->Disable();
	    fCmp = kFALSE;
	}
	if (fMct && fMctStream->OpenFile(file)==0) {
	    fMctStream->Disable();
	    fMct = kFALSE;
	}
	if (fMct && fTruStream->OpenFile(file)==0) {
	    fTruStream->Disable();
	    fMct = kFALSE;
	}
    }
    
    if (fTag && fTagStream) InitTagBranches(fTagStream->GetTree());
    if (fTag && fTagStream) InitBitBranches(fTagStream->GetTree());
    if (fAod && fAodStream) InitAodBranches(fAodStream->GetTree());
    if (fMap && fMapStream) InitMapBranches(fMapStream->GetTree());
    if (fCmp && fCmpStream) InitCmpBranches(fCmpStream->GetTree());
    if (fMct && fMctStream) InitMctBranches(fMctStream->GetTree());
    if (fMct && fTruStream) InitTruBranches(fTruStream->GetTree());
    
    if (!fAod && fAodStream) fAodStream->Disable();
    if (!fMap && fMapStream) fMapStream->Disable();
    if (!fCmp && fCmpStream) fCmpStream->Disable();
    if (!fMct && fMctStream) fMctStream->Disable();
    if (!fMct && fTruStream) fTruStream->Disable();
}

UInt_t KangaReader::GetNumberOfLists() const
{
    return fBtaCandList->GetEntries();
}

const char* KangaReader::GetListName(Int_t i) const
{
#ifndef SCHEMA3
    if (TBtaMicroCandRDir::GetSchema()<4) return 0;
#endif
    BtaCandListR_001 *x = (BtaCandListR_001 *) fBtaCandList->At(i);
    return x->GetKey();
}

BtaCandListR& KangaReader::GetCandList(Int_t i) const
{
    static BtaCandListR dummy;
    if (TBtaMicroCandRDir::GetSchema()>3) return dummy;
    return *(BtaCandListR *) fBtaCandList->At(i);
}

void KangaReader::InitTagBranches(TTree *theTree)
{    
    if (theTree==0) return;
    fBits->SetTree(theTree);
    fBits->ClearCache();
    theTree->SetBranchStatus("TAG*",1);
    if (fTagExpr!="") {
	cout << "KangaReader::TagSelection " << fTagExpr << endl;
        if (fTagFormula!=0) { delete fTagFormula; fTagFormula=0; }
	fTagFormula = new TTreeFormula("theTagFormula",fTagExpr.Data(),theTree);
    }
}

void KangaReader::InitBitBranches(TTree *theTree)
{    
    if (theTree==0) return;
    fBits->SetTree(theTree);
    fBits->ClearCache();
    theTree->SetBranchStatus("TAG*",1);
    if (fBitExpr!="") {
        if (fBitFormula!=0) { delete fBitFormula; fBitFormula=0; }
	fBitFormula = new TTreeFormula("theBitFormula",fBitExpr.Data(),theTree);
    }
}   

void KangaReader::InitAodBranches(TTree *theTree)
{
    if (theTree==0) return;
    //theTree->SetBranchStatus(EIDBRANCH,1);// Event info
    theTree->SetBranchStatus("_nent",1);    // this is the branch counting the Clones Entries
    theTree->SetBranchStatus("_array_",1);  // In ROOT 2.25/03 the array needs to be enabled !
    fAodListsComp->SetTree(0);
    
    // Initialize the Kanga interface
    TString s = TRho::Instance()->GetParameterStore();
    s+= TString("/Kanga/");
    TBtaMicroCandRDir* dir= TBtaMicroCandRDir::Instance();
    dir->Init(theTree,&fBtaMicroList,s);
}

void KangaReader::InitMapBranches(TTree *theTree)
{
    if (theTree==0) return;
    //theTree->SetBranchStatus(MAPBRANCH,1);
    fMapStream->Enable();
}

void KangaReader::InitCmpBranches(TTree *theTree)
{
    if (theTree==0) return;
    //theTree->SetBranchStatus(COMPBRANCH,1);
    fCmpStream->Enable();
}

void KangaReader::InitMctBranches(TTree *theTree)
{
    if (theTree==0) return;
    //theTree->SetBranchStatus(STDHEPBRANCH,1);
    fMctStream->Enable();
}

void KangaReader::InitTruBranches(TTree *theTree)
{
    if (theTree==0) return;
    theTree->SetBranchStatus("*",1);
    //theTree->SetBranchStatus("_nent",1);    // this is the branch counting the Clones Entries
    //theTree->SetBranchStatus("_array_",1);  // In ROOT 2.25/03 the array needs to be enabled !
    //theTree->SetBranchStatus("fBits",1);
    //theTree->SetBranchStatus("fUniqueID",1);
    fTruStream->Enable();
}

UInt_t KangaReader::GetNumberOfTracks() const 
{
    // Return the total number of tracks in the event
    Int_t n = fNumRecTracks;
    if (n==0) n = fBtaMicroList->GetEntriesFast();
    return n;
}

UInt_t KangaReader::GetNumberOfChargedTracks() const 
{
    // Return the number of charged tracks in the event
    return fBits->GetInt("nTracks");
}

UInt_t KangaReader::GetNumberOfNeutralTracks() const 
{
    // Return the number of neutral tracks in the event
    return GetNumberOfTracks() - GetNumberOfChargedTracks();
}

UInt_t KangaReader::GetRunNumber() const 
{
    // Return the actual run number
    return fEidInfo->GetRunNumber();
}

UInt_t KangaReader::GetEventNumber() const 
{
    // Return the actual event number
    return fCounter+1; // Index starts at 0
}

UInt_t KangaReader::GetMajorID() const 
{
    return fEidInfo->GetMajorID();
}

UInt_t KangaReader::GetMinorID() const 
{
    return fEidInfo->GetMinorID();
}

// Access to object handles

PAFEventBase& KangaReader::GetHeader() const 
{
    // Return a handle to the event header
    return *fContent;
}    

VAbsTag& KangaReader::GetTag() const 
{
    // Return a handle to the TAG
    return *fBits;
}

VAbsBit& KangaReader::GetBit() const 
{
    // Return a handle to the BIT
    return *fBits;
}    

EidInfoR& KangaReader::GetEidInfo() const 
{
    // Return a handle to the EidInfo
    return *fEidInfo;
}    

PAFListBase& KangaReader::GetListBase() const 
{
    if (fAodListsComp->GetTree()==0) {
	fAodListsComp->SetMode(1); // Set to write - mode
	//fAodStream->GetFile()->cd();
	fAodListsComp->SetTree(new TTree("CompTree","Tree of comp. AOD - objects"));
    }
    return *fAodListsComp;
}

PAFMcBase& KangaReader::GetMcBase(Int_t i) const 
{
    // Return a handle to the McB of the i-th Truth Object
    
    StdHepTrkR &stdTrack = GetStdHepTrk(i);
    new((*fMctList)[i]) PAFMcBase();
    PAFMcBase *to = (PAFMcBase*)(*fMctList)[i];
    // convert StdTrack
    to->SetStatus( stdTrack.getStatus() );
    to->SetPdtID( stdTrack.getId() );  
    to->SetPx( stdTrack.getPx() );
    to->SetPy( stdTrack.getPy() );
    to->SetPz( stdTrack.getPz() );
    to->SetEnergy( stdTrack.getEnergy() );
    to->SetMass( stdTrack.getMass() );
    to->SetStartVx( stdTrack.getVx() );
    to->SetStartVy( stdTrack.getVy() );
    to->SetStartVz( stdTrack.getVz() );
    to->SetLifeTime( stdTrack.getTime() );
    to->SetMother( stdTrack.getFirstMother() - 1);
    to->SetFirstDaughter( stdTrack.getFirstDaughter() - 1);
    to->SetLastDaughter( stdTrack.getLastDaughter() - 1);
    to->SetAodMatchIndex(-1);
    to->SetAodMatchConsistency(0.0);
    
    return *((PAFMcBase*)(*fMctList)[i]);   // replace by dynamic_cast<PAFMcBase*> as soon as allowed
}

StdHepTrkR& KangaReader::GetStdHepTrk(Int_t i) const 
{
    // Return a handle to the MCT of the i-th Truth Object
    TClonesArray *x = fStdHepR->getArray();
    return *((StdHepTrkR*)(*x)[i]);   // replace by dynamic_cast<StdHepTrkR*> as soon as allowed
}

PAFMcAssocBase& KangaReader::GetMcAssocBase(Int_t i) 
{
    // Return a handle to the MC association of i-th track
    static Int_t oldEvent = -1;
    
    fAss->SetMcTru(-1);
    fAss->SetMicro(-1);
    fAss->SetConsistency(-1.0);

    // Generate a canonic list
    // On Windows there is a problem with deletion of the TNodes
    // On Unix the program crashes if you delete TNodes in graphics apps
    // Hence, we have a memory leak here...
    if (GetEventNumber()!=oldEvent) {
#ifdef WIN32
	fMcList->Clear("nodelete"); // Mem. leak !
#else
	if (gROOT->GetApplication()==0)
	  fMcList->Delete();
	else
	  fMcList->Clear("nodelete"); // Mem. leak !
#endif
	LoadMcNodes(fMcList);
	oldEvent = GetEventNumber();
    }
    
    if (i<0 || i>GetNumberOfMcTruthTracks()) {
	cerr << "KangaReader::GetMcAssocBase: Requesting index iout of range #" << i << endl;
	return *fAss;
    }

    BtaCandAssocMapR &btaAss = GetCandAssocMap(i);
    Int_t mctru = btaAss.getMcTru();
    Int_t micro = btaAss.getMicro();
    Float_t cons = btaAss.getConsistency();
    
    // Determine the stdhep index of mctruth mapping
    Int_t stdhepIndex = 0;
    TObject *node = fMcList->At(mctru);
    if (node!=0)
	stdhepIndex = node->GetUniqueID(); // This is the MCTruth index
    else {
	cerr << "KangaReader::GetMcAssocBase: No TNode exists for index " << mctru << endl;
	return *fAss;
    }
    
    fAss->SetMcTru(stdhepIndex);
    fAss->SetMicro(micro);
    fAss->SetConsistency(cons);
    
    return *fAss;
}

BtaCandAssocMapR& KangaReader::GetCandAssocMap(Int_t i) const 
{
    // Return a handle to the MC association of i-th track
    TClonesArray *x = fBtaCandAssocMap->getArray();
    return *((BtaCandAssocMapR *)(*x)[i]);
}

UInt_t KangaReader::GetNumberOfCandAssocs() const 
{
    if (fNumMcAssoc>0) 
	return fNumMcAssoc;
    else 
	return ( fBtaCandAssocMap ? fBtaCandAssocMap->getArray()->GetEntriesFast() : 0 ); 
}

UInt_t KangaReader::GetNumberOfMcTruthTracks() const 
{
    if (fNumMctTracks>0) 
	return fNumMctTracks;
    else 
	return ( fStdHepR ? fStdHepR->getArray()->GetEntriesFast() : 0 ); 
}

Bool_t 
KangaReader::makeCompBase_pid(BtaCandPtrR *bcp,PAFCompAodBase &dest)
{
    dest.Reset();
    Int_t index = (*bcp)(0);
    if (index>=0) {
	dest.SetAodRef(index);
	dest.SetPdtRef(bcp->GetLundId());
	return kTRUE;
    } else {
	cout << " *** index in candList is " << (*bcp)(0) << endl;
    }
    return kFALSE;
}

Bool_t 
KangaReader::makeCompBase_cluster(BtaCandPtrR *bcp,PAFCompAodBase &dest)
{
    dest.Reset();
    Int_t m;
    bcp->GetMother(m);
    if (m<0) { return kFALSE; }
    dest.SetAodRef(m);
    dest.SetPdtRef(bcp->GetLundId());
    Bool_t ok = kTRUE;
    for (Int_t i=0; i<bcp->GetNumberOfDaughters(); i++) {
	Int_t index = (*bcp)(i);
	if (index>=0) {
	    dest.AddDaughter(index);
	} else {
	    ok = kFALSE;
	    cout << " *** index is " << (*bcp)(i) << endl;
	}
    }
    return ok;
}

Bool_t 
KangaReader::makeCompBase_V0(BtaCandPtrR *bcp,PAFCompAodBase &dest)
{
    dest.Reset();
    dest.SetPdtRef(bcp->GetLundId());
    Bool_t ok = kTRUE;
    for (Int_t i=0; i<bcp->GetNumberOfDaughters(); i++) {
	Int_t index = (*bcp)(i);
	if (index>=0) {
	    dest.AddDaughter(index);
	} else {
	    ok = kFALSE;
	    cout << " *** index is " << (*bcp)(i) << endl;
	}
    }
    return ok;
}


VAbsMicroCandidate& KangaReader::GetMicroCandidate(Int_t i) const
{
    // Return a handle to the Micro of the i-th track
    
    BtaMicroCandidate *aod = 0;
    new ((*fOwnList)[i]) BtaMicroCandidate((TBtaMicroCandR*)fBtaMicroList->UncheckedAt(i));
    aod = (BtaMicroCandidate*)(*fOwnList)[i];
    return *aod;
}

Int_t KangaReader::Fill(TCandList *l) 
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

Int_t KangaReader::FillCharged(TCandList *l) 
{
    // Generate charged candidates and fill them into a list
    
    UInt_t nEntries = GetNumberOfChargedTracks();
    l->Cleanup();
    fEntries = 0;
    for (UInt_t i=0;i<nEntries;++i) {
	l->Put(TCandidate(GetMicroCandidate(i),i));
    }
    return nEntries;
}

Int_t KangaReader::FillNeutral(TCandList *l) 
{
    // Generate neutral candidates and fill them into a list
    
    UInt_t nEntries = GetNumberOfNeutralTracks();
    l->Cleanup();
    fEntries = GetNumberOfChargedTracks();
    for (UInt_t i=0;i<nEntries;++i) {
	l->Put(TCandidate(GetMicroCandidate(fEntries+i),fEntries+i));
    }
    return nEntries;
}

Int_t KangaReader::FillFromList(TCandList *l,const char* listname)
{
    Int_t currentIndex = GetCurrentIndex(); // The current entry
    
    // Clean the list
    l->Cleanup();

    if (!CreateTCandList(listname)) return 0;
    
    // PAF specific lists
    PAFListBase& theListBase = GetListBase();
    TClonesArray *theList = theListBase.GetList(listname,currentIndex);
    if (theList==0) { return 0; }
    Int_t len = theListBase.GetListLength(listname,currentIndex);
    if (len<0 || len >100) {
	cerr << "KangaReader::FillFromList: Skip irrational list length: " << len << endl;
	return 0;
    }
    
    for (Int_t i=0; i<len; i++) {
	PAFCompAodBase *b = (PAFCompAodBase*) ((*theList)[i]);
	Int_t index = b->GetAodRef();
	if (index<-1 || index>128) {
	    cerr << "KangaReader::FillFromList: Skip irrational index: " << index << endl;
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
		cout << " *** WARNING in KangaReader::FillFromList : " << endl;
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


Bool_t KangaReader::CreateTCandList(const char* listname)
{
#ifndef SCHEMA3
    if (TBtaMicroCandRDir::GetSchema()<4) {
	cerr << "KangaReader::CreateTCandList: Can not read " << listname << endl;
	return kFALSE;
    }
#else
    if (TBtaMicroCandRDir::GetSchema()>3) {
	cerr << "KangaReader::CreateTCandList: Can not read " << listname << endl;
	return kFALSE;
    }
#endif
    // Get handle to the list array
    PAFListBase& theListBase = GetListBase();
    
    // In the case of Kanga, generate a PAF compatible list
    BtaCandListR_001 *bcl = 0;
    int n;
    for (n=0;n<fBtaCandList->GetEntries();n++) {
	bcl = (BtaCandListR_001 *) fBtaCandList->At(n);
	if (bcl->GetKey() == listname) break;
    }
    if (n>=fBtaCandList->GetEntries()) {
	//cerr << "KangaReader::FillFromList: List not found " << listname << endl;
	return kFALSE; // Not found
    }
    
    //
    Int_t len = bcl->GetNumberOfCandidates();
    if (len == 0) return kFALSE;
    
    // Generate a PAF list
    
    theListBase.AddList((char *)listname);
    theListBase.SetActiveList((char *)listname);
    PAFCompAodBase theCompAodBase;
    
    for (Int_t k=0; k<len; k++) {
	int im=-1; 
	BtaCandPtrR *bcp = bcl->GetCandPtr(k);
	if (bcp==0) continue;
	if (bcp->GetMother(im)) {
	    if(makeCompBase_cluster(bcp,theCompAodBase)) {
		theListBase.AddCompAodBase(theCompAodBase);
	    }
	} else if (bcp->GetNumberOfDaughters()==1) {
	    if(makeCompBase_pid(bcp,theCompAodBase)) {
		theListBase.AddCompAodBase(theCompAodBase);
	    }
	} else {
	    if(makeCompBase_V0(bcp,theCompAodBase)) {
		theListBase.AddCompAodBase(theCompAodBase);
	    }
	}
#ifndef SCHEMA3
	delete bcp;
#endif
    }
    //
    return kTRUE;
}

UInt_t KangaReader::GetNumberOfComposites() const
{
    return fBtaCompList->GetEntriesFast();
}

// Get a composite candidate from the composite TClonesArray
// 1) By entry number
// 2) By registry number (negative argument)
TCandidate& KangaReader::GetCompositeCandidate(Int_t index) const
{
    TCandidate *c = 0;
    BtaCandidateR *comp = 0;

    const Int_t nc = GetNumberOfComposites();
    const UInt_t nLookup = TMath::Abs(index);

    if (index>=nc) {
	cerr << "KangaReader::GetCompositeCandidate " << index << " not found" << endl;
	return *c;
    }

    if (index>=0)
	comp = (BtaCandidateR *) fBtaCompList->At(index);
    else { // Lookup the candidate (registry number)
	int i;
	for (i=0;i<nc;i++) {
	    comp = (BtaCandidateR *) fBtaCompList->At(i);
	    if (comp->refToMe().id()==nLookup) break;
	}
	if (i>=nc) {
	    cerr << "KangaReader::GetCompositeCandidate " << nLookup << " not found" << endl;
	    return *c;
	}
    }
	
    c = comp->Transient();

    //Vertex
    if (comp->DecayVtx() != 0) {
       VAbsVertex *vtx = comp->DecayVtx()->Transient();
       c->SetDecayVtx(vtx);
    }	

    // Get the daughters
    for (int n=0;n<comp->NDaughters();++n)
    {
	TCandidate *daug = &GetCompositeCandidate(-comp->Daughter(n));
	c->AddDaughterLink( daug );
    }
   
    return *c;
}

Int_t KangaReader::FillCompositeList(TCandList *l,const char* listname)
{
    static TDatabasePDG *pdt = TRho::Instance()->GetPDG();
    Int_t pdgcode = 0;
    if (listname!=0) pdt->GetParticle(listname)->PdgCode();

    l->Cleanup();
    const Int_t nc = GetNumberOfComposites();
    for (int i=0;i<nc;i++) {
	BtaCandidateR *comp = (BtaCandidateR *) fBtaCompList->At(i);
	if (pdgcode!=0 && comp->PdgId()!=pdgcode) continue;
	const TCandidate &c = GetCompositeCandidate(i);
	l->Put(c);
    }
    return nc;
}

Int_t KangaReader::FillAllCompositeList(TCandList *l)
{
    l->Cleanup();
    const Int_t nc = GetNumberOfComposites();
    for (int i=0;i<nc;i++) {
	BtaCandidateR *comp = (BtaCandidateR *) fBtaCompList->At(i);
	l->Put(*comp->Transient());
    }
    return nc;
}