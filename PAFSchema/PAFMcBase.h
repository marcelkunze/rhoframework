#ifndef PAFMCBASE_H
#define PAFMCBASE_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFMcBase								//
//                                                                      //
// Monte Carlo truth persistent class in PAF(Pico Analysis Framework)	//
//									//
// Author List:								//
// Rolf Dubitzky - Thu Oct 14 1999					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////


//  This class is what has to be concidered the persistent part of a 
//  StdHep track. The PAF-Mct-Stream will consist of TClonesArray of this type.
//  The Information is analog to StdHepTrk except:
//    fStatus:   
//       -1 :  unkown
//        0 :  left detector
//        1 :  decay
//        2 :  stopped by interaction WITH decay products
//        3 :  stopped by interaction WITHOUT decay products
//    fAodMatchIndex:
//        The index of the best matching RecoObject in the Aod stream.
//
//  LOG:
//    Oct 14 1999  -  creation
//    Nov 02 1999  -  revision
//
//  ToDo:
//    where to find the information for fStatus ??
//

#include <iostream>

// ROOT stuff
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"

class PAFMcBase : public TObject {
    
  public:
    //
    //  Constructors
    //
    //  default constructor
    PAFMcBase();

    //  in this constructor no default values are privided! 
    //  Think about every value every time :-)
    PAFMcBase(const Int_t status, const Int_t pdtid,
	      const Int_t mother, const Int_t firstd, const Int_t lastd, 
	      const TLorentzVector& momentum, const TVector3& startvertex,
	      const Double_t lifetime, const Int_t aodindex, const Double_t aodcons=0.0);
    // copy ctor
    PAFMcBase( const PAFMcBase& );

    //  Destructor
    virtual ~PAFMcBase() {}

    //  Getter functions
    Int_t GetStatus()         const {return fStatus;}             // Particle status
    Int_t GetPdtID()          const {return fPdtID;}              // Particel Id
    Int_t GetMother()         const {return fMother;}             // Position of mother
    Int_t GetFirstDaughter()  const {return fFirstDaughter;}      // Position of first daughter
    Int_t GetLastDaughter()   const {return fLastDaughter;}       // Position of last daughter
    Double_t GetPx()          const {return fPx;}                 // X momentum
    Double_t GetPy()          const {return fPy;}                 // y momentum
    Double_t GetPz()          const {return fPz;}                 // z momentum
    Double_t GetEnergy()      const {return fEnergy;}             // Track energy
    Double_t GetMass()        const {return fMass;}               // Particle mass
    TLorentzVector GetMomentum() const {return TLorentzVector(fPx,fPy,fPz,fEnergy);}
    Double_t GetStartVx()     const {return fStartVx;}            // x position of vertex
    Double_t GetStartVy()     const {return fStartVy;}            // y position of vertex
    Double_t GetStartVz()     const {return fStartVz;}            // z position of vertex
    TVector3 GetStartVertex() const {return TVector3(fStartVx,fStartVy,fStartVz);}
    Double_t GetLifeTime()    const {return fLifeTime;}           // Production time
    Int_t GetAodMatchIndex()  const {return fAodMatchIndex;}      // Track number of AOD Partner (if none: -1)
    Double_t GetAodMatchConsistency()  const {return fAodMatchConsistency;}      // Match consistency of AOD Partner

    // Setter functions
    void SetStatus(const Int_t i){ fStatus = i;}                  // Particle status
    void SetPdtID(const Int_t i){ fPdtID = i;}                    // Particle ID 
    void SetMother(const Int_t i){ fMother = i;}                  // Positionn of first mother
    void SetFirstDaughter(const Int_t i){ fFirstDaughter= i;}     // Position of first daughter
    void SetLastDaughter(const Int_t i){ fLastDaughter= i;}       // Position of last daughter
    void SetPx(const Double_t f) { fPx = f; }                     // 4-Momentum
    void SetPy(const Double_t f) { fPy = f; }                     // 4-Momentum
    void SetPz(const Double_t f) { fPz = f; }                     // 4-Momentum
    void SetEnergy(const Double_t f){ fEnergy = f; }              // 4-Momentum
    void SetMass(const Double_t f){ fMass = f; }                  // Particle mass
    void SetStartVx(const Double_t f){ fStartVx = f; }            // x position of vertex
    void SetStartVy(const Double_t f){ fStartVy = f; }            // y position of vertex
    void SetStartVz(const Double_t f){ fStartVz = f; }            // z position of vertex
    void SetLifeTime(const Double_t f){ fLifeTime = f;}           // Production time
    void SetAodMatchIndex(const Int_t i){ fAodMatchIndex = i;}    // Track number
    void SetAodMatchConsistency(const Double_t f){ fAodMatchConsistency = f;}    // Track number
    void SetMomentum(const TLorentzVector& p) {
	fPx = p.Px();
	fPy = p.Py();
	fPz = p.Pz();
	fEnergy = p.E();
    }
    void SetMomentum(const Double_t x, const Double_t y, const Double_t z, const Double_t e) {
	fPx = x;
	fPy = y;
	fPz = z;
	fEnergy = e;
    }
    void SetStartVertex(const TVector3& v){
	fStartVx = v.X();
	fStartVy = v.Y();
	fStartVz = v.Z();
    }
    void SetStartVertex(const Double_t x, const Double_t y, const Double_t z){
	fStartVx = x;
	fStartVy = y;
	fStartVz = z;
    }

    // generic printer 
    virtual void PrintOn(std::ostream& o=std::cout) const;

  private:

    // Data members
    Int_t fStatus;                    // status / stop code 
    Int_t fPdtID;                     // The particle id; pdt
    Int_t fMother;                    // The position (index) of the mother particle in list 
    Int_t fFirstDaughter;             // Position (index) of the first daughter... in list 
    Int_t fLastDaughter;              // Position (index) of the first daughter... in list 
    Double_t fPx;                     // x component of 4-Momentum
    Double_t fPy;                     // y component of 4-Momentum
    Double_t fPz;                     // z component of 4-Momentum
    Double_t fEnergy;                 // Energy component of 4-Momentum
    Double_t fMass;                   // Mass (redundant, but easy/fast)
    Double_t fStartVx;                // Vertex information 
    Double_t fStartVy;                // Vertex information 
    Double_t fStartVz;                // Vertex information 
    Double_t fLifeTime;               // Particles actual lifetime

    Int_t fAodMatchIndex;             // index of the best matching reco object in the AOD
    Double_t fAodMatchConsistency;    // consistency of the best matching reco object in the AOD

  public:
    ClassDef(PAFMcBase,1) // Base class for Monte-Carlo truth particle
};      

// standalone print
std::ostream&  operator << (std::ostream&, const PAFMcBase&);

#endif
