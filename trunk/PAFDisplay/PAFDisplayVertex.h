#ifndef PAFDISPLAYVERTEX_H
#define PAFDISPLAYVERTEX_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFDisplayVertex                                                     //
//                                                                      //
// Draw a vertex				                        //
// Adapted from ATLFAST (M.Kunze, June 1999)				//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Dec. 99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"

class TPolyLine3D;
class TList;
class PAFDisplay;
class VAbsVertexSelector;

class PAFDisplayVertex : public TNamed {
    
private:
    PAFDisplay        *fDisplay;             //pointer to PAFDisplay object
    TPolyLine3D       *fLine;                //pointer to line3D
    TList	      *fObjs;		     //list of drawable objects
    VAbsVertexSelector *fVertexSelector;     //vertex selector to draw    
    Int_t	      fIndex;		     //index of graphics primitive
public:
    PAFDisplayVertex(VAbsVertexSelector *v=0);
    virtual          ~PAFDisplayVertex();
    virtual Int_t     DistancetoPrimitive(Int_t px, Int_t py);
    void	      SetDisplay(PAFDisplay *d) { fDisplay = d;}
    PAFDisplay      *GetDisplay() const {return fDisplay;}
    void	      SetVertexSelector(VAbsVertexSelector *v) { fVertexSelector = v;}
    VAbsVertexSelector  *GetVertexSelector() const {return fVertexSelector;}
    virtual void      ExecuteEvent(Int_t event, Int_t px, Int_t py);
    virtual char     *GetObjectInfo(Int_t px, Int_t py) const;
    virtual void      SetMarker(Int_t color,Float_t x,Float_t y,Float_t z,Float_t dx=0.1,Float_t dy=0.1,Float_t dz=0.1,Float_t theta=0.0,Float_t phi=0.0);
    virtual void      DrawVertex(Option_t *option="", Bool_t useCache = kFALSE);
    virtual void      SetLineAttributes(); // *MENU*
    TList            *GetObjects() { return fObjs; }
    
    ClassDef(PAFDisplayVertex, 0)   //Graphics interface to vertexing
};

#endif
