#ifndef PAFABSPIDINFO_H
#define PAFABSPIDINFO_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFAbsPidInfo							//
//                                                                      //
// Particle identification info class					//
//                                                                      //
// Author: T. Brandt, TUD						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#define CONS_OK 0
#define CONS_noMeasure 1
#define CONS_underFlow 2
#define CONS_unPhysical 3

#define CONS_left 1
#define CONS_right 2
#define CONS_unknown 0


#include "RhoBase/VAbsPidInfo.h"
#include <iosfwd>

class PAFAbsPidInfo : public VAbsPidInfo {
 
  public :
    PAFAbsPidInfo();
    virtual ~PAFAbsPidInfo() { };

    // Access significance levels
    virtual const Float_t* GetSignificance() const;
    virtual Float_t GetSignificance(Int_t hypo) const;

    // Access likelihoods
    virtual const Float_t* GetLikelihood() const;
    virtual Float_t GetLikelihood(Int_t hypo) const;

    // Access statusbits
    virtual Int_t GetStatus(Int_t hypo) const;
    virtual Int_t GetSign(Int_t hypo) const;
    
    // Check validity
    virtual Int_t IsValid() const;
    virtual UShort_t GetStatus() const { return _status; };


    // Modify contents
    void SetStats(Int_t hypo, Float_t sl, Float_t lhood, Int_t status, Int_t sign);
    virtual void  SetNoMeas();
    virtual void  Invalidate();

    // dump
    void PrintOn(std::ostream& o=std::cout) const;

  private :
    Float_t _likelihood[5];
    Float_t _significanceLevel[5];
    UShort_t _status ;   // 3 Bit for each hypothesis , 1 Bit for validity
    UShort_t _sign ;   // 2 Bit for each hypothesis (left/right/unknwn)

  public :
    ClassDef(PAFAbsPidInfo,1) // PAF base class for PID

};

#endif
