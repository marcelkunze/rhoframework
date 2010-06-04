#ifndef PAFWRITER_H
#define PAFWRITER_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFWriter								//
//                                                                      //
// Write data class to dump TAG and AOD	in PAF format			//
//                                                                      //
// Author: Marcel Kunze, RUB, Oct. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "RhoBase/VAbsWriter.h"

class VAbsMicroCandidate;
class VAbsTag;
class VAbsBit;
class PAFChargedAodBase;
class PAFNeutralAodBase;
class PAFEventBase;
class PAFListBase;
class PAFAodBase;
class PAFMcBase;
class PAFMcAssocBase;
class PAFTagBase;
class TCandidate;
class THashList;
class PAFStream;
class TCandList;
class VAbsCollection;

class BtaCandAssocMapR;

class  PAFWriter: public VAbsWriter {
public:
    
    PAFWriter();
    PAFWriter(const char *file); 
    PAFWriter(VAbsCollection *collection); 
    virtual ~PAFWriter();
    
    UInt_t GetRunNumber() const;    
    UInt_t GetEventNumber() const;    
    PAFEventBase& GetHeader() const;    
    VAbsTag&	  GetTag() const;    
    VAbsBit&	  GetBit() const;    
    PAFListBase&  GetListBase() const;
    
    Bool_t SetStreamStatus(const char *stream,Bool_t status=kTRUE);
    void InitializeStreams();
    PAFStream* NewStream(const char* AFileNameBase, const char* ATreeName);
    void InitWrite(const char *file);   
    void InitUpdate(const char *file); 
    void NewEvent();    
    void NewChargedCandidate();
    void NewNeutralCandidate();    
    // Fill Elements which are to be stored in Tree
    void Fill( VAbsTag*);        
    void Fill( PAFTagBase*);        
    void Fill( VAbsBit* );        
    Int_t Fill( PAFAodBase* );        
    void Fill( PAFMcBase* );        
    void Fill( PAFMcAssocBase* );        
    void Fill( BtaCandAssocMapR* );        
    // Write Event to Trees
    void Fill();    
    void ClearBuffer();   
    void Store();   
    void DumpList(TCandList *list);
    
    VAbsCollection *GetCollection() { return fCollection; }
    void Exclude(TCandidate *c);
    void ConvertCandidate(PAFChargedAodBase *aod,VAbsMicroCandidate *cand);
    void ConvertCandidate(PAFNeutralAodBase *aod,VAbsMicroCandidate *cand);

    Bool_t Tag() const { return fTag; }
    Bool_t Aod() const { return fAod; }
    Bool_t Mct() const { return fMct; }
    Bool_t Map() const { return fMap; }

private :
    
    Bool_t fPAFWriter, fWriteCollection; //!
    Bool_t fFirstEventWritten; //!
    UInt_t fWriteEvents; //!
    VAbsCollection *fCollection; //!
    PAFChargedAodBase *fChargedAod; //!
    PAFNeutralAodBase *fNeutralAod; //!
    PAFMcBase *fMcTruth; //!
    PAFMcAssocBase *fMcAssoc; //!
    BtaCandAssocMapR *fBtaAssoc; //!
    TCandList*   fTrackList; //!
    THashList* fGarbageBin; //!
    THashList *fStreams;       //!list of PAFStream objects    
    PAFStream *fTagStream;     //!the 1st stream in fStreams
    PAFStream *fAodStream;     //!the 2nd stream in fStreams
    PAFStream *fMctStream;     //!the 3nd stream in fStreams
    Int_t     fNumMcTracks; //!
    Int_t     fNumMcAssoc; //!
    Int_t     fNumBtaMcAssoc; //!
    UInt_t    fEventNumber;    //!Number in the collection
    
    PAFEventBase    *fContent;	    //!Persistent TAG and HDR
    VAbsBit	    *fBits;	    //!Persistent BIT
    PAFListBase     *fAodListsComp;   //!Container for persistent composite lists  					     
    TClonesArray    *fAodListCharged, //!Persistent AOD, charged part 
		    *fAodListNeutral; //!Persistent AOD, neutral part
    TClonesArray    *fMctList;	    //!Persistent MCT
    TClonesArray    *fAssList;	    //!Persistent MCT association
    TClonesArray    *fBtaList;	    //!Persistent MCT association (Beta)
    
    UInt_t	    fTimeStampMajor,	//!The timestamp (secs since 1/1/1901)
		    fTimeStampMinor;	//!Nanos in sec

    Bool_t  fTag,fAod,fMct,fMap;    //!stream status

public:
    ClassDef(PAFWriter,1) // Base class to write ROOT data files
};

#endif

