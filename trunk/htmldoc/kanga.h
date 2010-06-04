#ifndef TKANGA_H
#define TKANGA_H
//--------------------------------------------------------------------------
// Class library to read the BABAR KanGA data
// Author: Leif Wilden, TUD, 1999
//------------------------------------------------------------------------

//------------------------------------------------------------

#include "TObject.h"
#include "TBranchClones.h"
#include "THashList.h"
class TSmartPointer;

//------------------------------------------------------------

class TCandVariable: public TNamed
{
public:
    
    TCandVariable(Int_t AnId, const char* AName, const char* AnAlias);
    virtual ~TCandVariable();
    
    Int_t	     fVarID;    //  each variable has a unique id
    TSmartPointer* fBranch;     //! pointer to the associated branch (split mode)
    Int_t	     fOffset;   //! offset in memory
    
    ClassDef(TCandVariable,1) // Rho Kanga interface
};

//------------------------------------------------------------

#define kBranches  256

class TCandDirectory: public TObject
{
public:
    
    virtual ~TCandDirectory();

	void Reset();
    
    // Get the name of the class stored in the TClonesArray bc:
    const char*   GetClassName(const TBranchClones* bc);
    
    // Assign the leaf offset addresses of TBranchClones bc
    // according to the memory layout of TObject o.
    // Call this for EACH object BEFORE reading data:
    void      AssignVars(TBranchClones* bc, const TObject* o);
    
    // Read the aliases names and unique var ids from a file.
    // Call this once at the beginning of a job:
    void      ReadAliases(const char* AFileName); 
    
    // Actually read a variable (i.e. a branch) from the tree:
    void      ReadBranch(const Int_t AVarID);
    
    // Fast hashed access to a variable by its alias name:
    TCandVariable* GetVarByAlias(const char* AVarAlias) const;
    
    // Fast access to a variable by its unique ID:
    TCandVariable* GetVarByID(const Int_t AVarID) const;
    
    // Invalidate a non-existent branch
    void InvalidateBranch(int n) { if (n<kBranches) fDoNotProcess[n] = kFALSE; }
protected:
    
    TCandDirectory();
    TCandDirectory(const char* AFileName);
    TCandDirectory(const TCandDirectory& c) { }
    
private:
    
    THashList *fVarsByAlias; //!
    TCandVariable* fCandArray[kBranches]; //!
    Int_t     fMaxid;        //!
    Bool_t    fDoNotProcess[kBranches]; //!
    
    ClassDef(TCandDirectory,1) // Rho Kanga interface
};

//------------------------------------------------------------

class TBtaMicroCandRDir: public TCandDirectory
{
public:
    
    enum EMicroCandVar {
	// fitParams:
	fp_pos=1, fp_p4=4, fp_cov7=8, fp_q=9, fp_cov=10, fp_params=11,
	fp_diag=12, fp_corr=13,
	// trackQual:
	tq_bytes=31, tq_shorts=32,
	tq_svtPattern=20, tq_nSvtHits=21, tq_firstDch=22, tq_lastDch=23, tq_nDchHits=24,
	tq_pidHypo=25, tq_nDof=26, tq_fitStatus=27, tq_chi2=28, tq_tLen=29, tq_stFound=30,
	// pidQual:
	pq_bytes=35, pq_shorts=36,
	pq_trkEmcTheta=38, pq_trkEmcPhi=39,
	pq_thetaDirc=40, pq_lProb=41, pq_nDof=42, pq_eThetaDirc=43, pq_dEdXDch=44,
	pq_dEdXSvt=45, pq_nSamplesDch=46, pq_nSamplesSvt=47, pi_nent=48, pi_coValues=49,
	pq_drcNumBar=52, pq_drcExit=53, pq_drcXPos=54, pq_nPhot=55, pq_nBkgd=56,
	pq_nExPhot=57, pq_deltaDchMom=58, pq_deltaDrcMom=59,
	// pidInfo:
	pi_cons=14, pi_status=15,
	pi_coLhoods=50, pi_coBits=51,
	// calQual:
	cq_bytes=16, cq_shorts=17,
	cq_pidHypo=60, cq_nBumps=61, cq_nCrystals=62, cq_lat=63, cq_absZern42=64,
	cq_absZern20=65, cq_absZern31=66, cq_secondMomentTP=67, cq_energy=68, 
	cq_cdPhiValue=69, cq_cdPhiLhood=70, cq_cdPhiBits=71, cq_centroidX=72,
	cq_centroidY=73, cq_centroidZ=74, cq_cov=75, cq_s9s25=76, cq_s1s9=77,
	cq_rawEnergy=78, cq_status=79, 
	// ifrQual:
	iq_bytes=18, iq_pattern=19,
	iq_IfrLayStrips=80, iq_IfrNstrip=81, iq_IfrBits=82, iq_expectedInteractionLengths=83,
	iq_interactionLengthsBeforeIron=84, iq_measuredInteractionLengths=85,
	iq_IfrTrkMatch=86, iq_IfrEmcMatch=87, iq_lastBarrel=88,
	iq_interactionLengths=85, iq_interactionLengthsInIron=85,
	iq_rpcProb=89, iq_IfrTrkMatchChi2=90
    };
    
    virtual ~TBtaMicroCandRDir();
    
    void   Init(TTree* t, TClonesArray** c, const char* SchemaDir=0);
    
    static TBtaMicroCandRDir* Instance();
    static Int_t GetSchema();
    static void SetSchema(Int_t s);
    
private:
    
    static TBtaMicroCandRDir* fInstance; //!
    static Int_t fSchema; //!
    
    ClassDef(TBtaMicroCandRDir,1) // Rho Kanga interface
};

//------------------------------------------------------------

#endif






