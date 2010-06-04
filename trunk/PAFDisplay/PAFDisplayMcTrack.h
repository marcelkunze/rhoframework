#ifndef TPICODISPLAYMCTRACK_H
#define TPICODISPLAYMCTRACK_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFDisplayMcTrack                                                    //
//                                                                      //
// Graphics interface to MCTruth		                        //
// Adapted from ATLFAST (M.Kunze, June 1999)				//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Dec. 99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "TVector3.h"

class TText;
class TPolyLine3D;
class TList;
class PAFDisplay;
class TCandidate;
class VAbsTruth;

class PAFDisplayMcTrack : public TNamed {
    
private:
    PAFDisplay        *fDisplay;             //pointer to PAFDisplay object
    TText             *fText;                //pointer to particle name
    Int_t             fTextColor;            //color of text
    Float_t           fTextSize;             //size of text
    Double_t	      fScale;		     //scale factor for text
    TPolyLine3D       *fLine;                //pointer to line3D
    TList	      *fObjs;		     //list of drawable objects
    TVector3          fPosition;	     //initial position
    TVector3	      fMomentum;	     //initial momentum
    VAbsTruth	     *fTruth;		     //pointer to MC-Truth
    Int_t	      fIndex;		     //index of graphics primitive
    Int_t             fPid;		     //PID hypothesis
    Int_t             fNumber;		     //track number
    Double_t          fLength;		     //track length
    
public:
    PAFDisplayMcTrack(TCandidate *a, const char* name = "", Double_t length = 0);
    virtual          ~PAFDisplayMcTrack();
    virtual Int_t     DistancetoPrimitive(Int_t px, Int_t py);
    PAFDisplay      *Display() {return fDisplay;}
    virtual void      ExecuteEvent(Int_t event, Int_t px, Int_t py);
    virtual char     *GetObjectInfo(Int_t px, Int_t py) const;
    TPolyLine3D      *HelixCurve(Float_t field, Float_t pmom, Float_t *vin, Float_t rin=150., Float_t zout=200.,Float_t totlen=500.);
    virtual void      HelixStep(Float_t field, Float_t step, Float_t pmom, Float_t *vin, Float_t *vout);
    virtual void      SetMarker(Int_t color,Float_t x,Float_t y,Float_t z,Float_t dx=0.1,Float_t dy=0.1,Float_t dz=0.1,Float_t theta=0.0,Float_t phi=0.0);
    virtual void      DrawTrack(Option_t *option="", Bool_t useCache = kFALSE);
    virtual void      SetLineAttributes(); // *MENU*
    TList            *GetObjects() { return fObjs; }
    void	      SetTextColor(Int_t c) { fTextColor = c; }
    void	      SetTextSize(Double_t s) { fTextSize = s; }
    void	      SetScale(Double_t s) { fScale = s; }
    void	      HideText();
    void	      SetNumber(Int_t n) { fNumber = n; }
    Float_t	      GetDecayLength(VAbsTruth *truth) ;
    
    ClassDef(PAFDisplayMcTrack, 0)   //Graphics interface to draw MC particle
};

#endif
