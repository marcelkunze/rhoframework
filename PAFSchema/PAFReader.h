#ifndef PAFREADER_H
#define PAFREADER_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFReader								//
//                                                                      //
// PAF event reader class						//
//                                                                      //
// Author: Marcel Kunze, RUB, May 2000					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TString.h"
#include "RhoBase/VAbsReader.h"

class THashList;
class TCLonesArray;
class TTree;
class TTreeFormula;
class TList;
class TNode;

class VAbsBit;
class VAbsTag;
class VAbsMicroCandidate;
class PAFListBase;

class TCandList;
class VAbsTruth;
class PAFStream;

class PAFChargedAodBase;
class PAFNeutralAodBase;
class PAFEventBase;
class PAFMcBase;
class PAFMcAssocBase;
class TPrintTree;

class EidInfoR;
class BtaCandAssocMapR;
class BtaCandListR;
class StdHepR;
class StdHepTrkR;

class TCandidate;

template <class T> class RooClonesVectorR;

class PAFReader : public VAbsReader {
    
public:
    PAFReader(const char *, const char *description="PAF Reader");
    virtual ~PAFReader( );
    
    // Operations

    void SetVerbose(Bool_t yesNo = kTRUE) { fVerbose = yesNo; }

    PAFStream* GetStream(Int_t StreamNo);
    Bool_t SetStreamStatus(const char *stream,Bool_t status=kTRUE);
    virtual void InitializeStreams(Bool_t multi=kTRUE);

    UInt_t  GetCurrentIndex();
    Int_t GetEvent(UInt_t number);
    virtual UInt_t GetRunNumber() const;    
    virtual UInt_t GetEventNumber() const;    
    virtual UInt_t GetNumberOfTracks() const;   
    virtual UInt_t GetNumberOfChargedTracks() const;   
    virtual UInt_t GetNumberOfNeutralTracks() const;   
    UInt_t GetNumberOfCandidates() const { return GetNumberOfTracks(); }   
    UInt_t GetNumberOfEventsProcessed() const;
    virtual UInt_t GetNumberOfMcTruthTracks() const;
    virtual UInt_t GetNumberOfCandAssocs() const;
    virtual UInt_t GetNumberOfLists() const;
    virtual const char* GetListName(Int_t i) const;
    virtual UInt_t GetNumberOfComposites() const  { return 0;}
    virtual TCandidate& GetCompositeCandidate(Int_t i) const;
    virtual VAbsMicroCandidate& GetMicroCandidate(Int_t i) const;

    virtual UInt_t GetMajorID() const;
    virtual UInt_t GetMinorID() const;
    
    virtual PAFEventBase& GetHeader() const;    
    virtual VAbsTag&	  GetTag() const;    
    virtual VAbsBit&      GetBit() const;    
    virtual PAFListBase&  GetListBase() const;
    virtual EidInfoR&     GetEidInfo() const;
    
    virtual VAbsTruth& GetMcTruth(Int_t n);
    void CreateMcTruthList();
    virtual PAFChargedAodBase& GetChargedAodBase(Int_t n) const;
    virtual PAFNeutralAodBase& GetNeutralAodBase(Int_t n) const;
    virtual PAFMcBase& GetMcBase(Int_t n) const;
    virtual PAFMcAssocBase& GetMcAssocBase(Int_t i);
    virtual StdHepTrkR& GetStdHepTrk(Int_t n) const;
    virtual BtaCandAssocMapR& GetCandAssocMap(Int_t i) const;
    virtual BtaCandListR& GetCandList(Int_t i) const;

    // Fill lists from micro
    virtual Int_t Fill( TCandList* );
    virtual Int_t FillCharged( TCandList* );
    virtual Int_t FillNeutral( TCandList* );
    virtual Int_t FillFromList(TCandList *l,const char* listname);
    virtual Int_t FillCompositeList(TCandList *l,const char* listname) { return 0; }
    virtual Int_t FillAllCompositeList(TCandList *l) { return 0; }

	// MC handling
    Int_t FillCanonicalMcTruthList(TCandList *);
    Int_t FillMcTruthList(TCandList *);
    void SetPrintTree(TPrintTree* tree) { fPrintTree = tree; }
    TPrintTree *GetPrintTree() const { return fPrintTree; }
    // Helper functions
    void LoadMcNodes( TList* mcList);
    void PrintMcList(TNode& cand );
    void FillMcList( TList& list, TNode& cand );
    Bool_t MakeNodeFromStdTrack(Int_t i, TNode*& mother );

    // Evaluation of tag based cuts
    void SetTagFormula(const char* expr);
    void SetBitFormula(const char* expr);
    Double_t EvalTagFormula();
    Double_t EvalBitFormula();

private:
#define NMCMAX 512
    Bool_t fUseStdHepTrack[NMCMAX]; //!Do not stream

protected:
    PAFEventBase    *fContent;		//!Persistent TAG and HDR
    VAbsBit	    *fBits;		//!Persistent BIT
    PAFListBase     *fAodListsComp;     //!Container for persistent composite lists  					     
    TClonesArray    *fAodListCharged;	//!Persistent AOD, charged part 
    TClonesArray    *fAodListNeutral;	//!Persistent AOD, neutral part
    TClonesArray    *fOwnList;		//!List to hold the MicroCandidates       
    Int_t	    fEntries;		//!Number of candidates
    TClonesArray    *fMctList;          //!Persistent MCT
    TClonesArray    *fAssList;          //!Persistent MCT associations
    Int_t           fNumMctTracks;	//!
    Int_t           fNumMcAssoc;		//!
    Int_t           fNumRecTracks;	//!
    Int_t           fNumMaps;		//!
    TClonesArray*   fPAFMcTruthList;    //!this list holds the VAbsTruth Objects. They are instantiated
                                        // on demand, when FillMcTruthlist is called.
                                        // Every PAFMcTurth capsulates a single PAFMcBase in PAFEvent::fMctList
                                        // and privides mother/daughter access via pointers
    Int_t  fPAFMcTruthEntries;          //!number of valid entries in fVAbsTruthList.
                                        // Set to -1 in this crazy Next/NextIter thing. (invalid flag for fMcTruthList)
                                        // After a call FillMcTruthlist() it is >= 0.
    EidInfoR     *fEidInfo;	 //!kanga EidInfo
    TClonesArray *fBtaMicroList; //!kanga BtaMicroCandR objects
    TObjArray	 *fBtaCandList;  //!kanga BtaCandListR objects
    TObjArray	 *fBtaCompList;	 //!kanga composite objects
    RooClonesVectorR<BtaCandAssocMapR> *fBtaCandAssocMap;  //!kanga BtaCandAssocMap objects
    StdHepR      *fStdHepR;	 //!kanga StdHepR object
    
    PAFStream* NewStream(const char* AFileNameBase, const char* ATreeName);

    // Methods to set up branches
    virtual void InitTagBranches(TTree *theTree); // The float and int tag
    virtual void InitBitBranches(TTree *theTree); // The boolean tag
    virtual void InitAodBranches(TTree *theTree); // The micro DST
    virtual void InitMapBranches(TTree *theTree); // The candidate maps
    virtual void InitCmpBranches(TTree *theTree){} // The candidate maps
    virtual void InitMctBranches(TTree *theTree); // The MC-Truth
    virtual void InitTruBranches(TTree *theTree); // The MC-Truth

    TTreeFormula *fTagFormula;  //!Fromula to evaluate Tag				     
    TTreeFormula *fBitFormula;  //!Fromula to evaluate analysis bits				     
    TString      fTagExpr;      //!Dynamic tag formula				     
    TString      fBitExpr;      //!Dynamic tag formula				     
    TString      fPAFTagExpr;   //!The PAF tag formula uses different syntax				     

    PAFStream *fTagStream;	//!read Tag			     
    PAFStream *fEidStream;	//!read Eid			     
    PAFStream *fAodStream;	//!read micro			     
    PAFStream *fMapStream;	//!read candidate maps			     
    PAFStream *fCmpStream;	//!read candidate maps			     
    PAFStream *fMctStream;	//!read MC-Truth			     
    PAFStream *fTruStream;	//!read MC-Truth			     
    THashList *fStreams;    //!list of PAFStream objects
    TPrintTree *fPrintTree; //!prints MC input

    UInt_t fTimeStampMajor;	//!The timestamp (secs since 1/1/1901)
    UInt_t fTimeStampMinor;	//!Nanos in sec

    Int_t   fCounter;		//!Relative event counter (file)
    Int_t   fTotal;		//!Absolute event counter
    Int_t   fMaxC,fMaxN;        //!Length of longest charged/ 
				//!neutral list in current Tree
    Bool_t  fTag,fAod,fMct,fMap,fCmp;//!stream status
    Bool_t  fVerbose;		//!Debug

    TClonesArray *fBtaList; //! BABAR like McTruth association
public:
    ClassDef(PAFReader,1) // Base class to read ROOT data files
};


#endif





