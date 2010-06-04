
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFDisplayVertex                                                     //
//                                                                      //
// Draw a vertex							//
// H.Schmuecker/M.Kunze, Dec. 1999)					//
//////////////////////////////////////////////////////////////////////////

#include "TVirtualX.h"
#include "TROOT.h"
#include "TPolyLine3D.h"
#include "TMarker3DBox.h"
#include "TCanvas.h"
#include "TList.h"
#include "TMath.h"

#include "PAFDisplay/PAFDisplayVertex.h"
#include "RhoBase/TRho.h"
#include "RhoBase/VAbsVertexSelector.h"

const Int_t kRECONS = BIT(16);

ClassImp(PAFDisplayVertex)


//_____________________________________________________________________________
PAFDisplayVertex::PAFDisplayVertex(VAbsVertexSelector *v) : fVertexSelector(v)
{
    fObjs = new TList;
}

//_____________________________________________________________________________
PAFDisplayVertex::~PAFDisplayVertex()
{
    if (fObjs != 0) fObjs->Delete();
    delete fObjs;
}

//_____________________________________________________________________________
Int_t PAFDisplayVertex::DistancetoPrimitive(Int_t px, Int_t py)
{
    Int_t dist = fLine->DistancetoPrimitive(px, py);
    if (dist < 2) {
	gPad->SetSelected(this);
	gPad->SetCursor(kCross);
	return 0;
    }
    return 999;
}

//______________________________________________________________________________
void PAFDisplayVertex::ExecuteEvent(Int_t event, Int_t , Int_t )
{
    switch (event) {
	
    case kButton1Down:
	gVirtualX->SetLineColor(-1);
	gPad->AbsCoordinates(kTRUE);
	fLine->SetLineColor(6);
	fLine->SetLineWidth(6);
	fLine->Paint(); 
	break;
	
    case kMouseMotion:
	break;
	
    case kButton1Motion:
	break;
	
    case kButton1Up:
	gVirtualX->SetLineColor(-1);
	fLine->SetLineColor(kYellow);
	fLine->SetLineWidth(1);
	fLine->Paint(); 
	gPad->AbsCoordinates(kFALSE);
    }
}

//______________________________________________________________________________
char *PAFDisplayVertex::GetObjectInfo(Int_t , Int_t ) const
{
    if (fVertexSelector == 0) return "No VertexSelector activated.";

    static char info[100];
    sprintf(info,"x=%f, y=%f, z=%f, px=%f, py=%f, pz=%f",
	fVertexSelector->GetVertex().X(),
	fVertexSelector->GetVertex().Y(),
	fVertexSelector->GetVertex().Z(),
	fVertexSelector->GetMomentum().X(),
	fVertexSelector->GetMomentum().Y(),
	fVertexSelector->GetMomentum().Z()
	);
    return info;
}


//_____________________________________________________________________________
void PAFDisplayVertex::SetMarker(Int_t color,Float_t x,Float_t y,Float_t z,Float_t dx,Float_t dy,Float_t dz,Float_t theta,Float_t phi)
{
    TMarker3DBox *m = new TMarker3DBox(x,y,z,dx,dy,dz,theta,phi);
    if (m == 0) return;
    m->SetUniqueID(fIndex++);
    m->SetLineColor(color);	    
    fObjs->Add(m);
}

//_____________________________________________________________________________
void PAFDisplayVertex::DrawVertex(Option_t *option, Bool_t useCache)
{
    if (fVertexSelector == 0) return;

    //    Draw the vertex information (position, DOCA, momentum)
    
    if (useCache) {
	TIter next(fObjs);
	TObject *item;
	while(item=next()) {
	    item->Draw(option);
	}
	return;
    }
    else {
	fObjs->Delete();
    }
    
    TVector3 vv=(fVertexSelector->GetVertex());
    Double_t doca=fVertexSelector->GetDistanceOfClosestApproach();
    SetMarker(kYellow,vv.X(),vv.Y(),vv.Z(),doca,doca,doca,0.,0.);
    Float_t x[2],y[2],z[2];

    TVector3 dir=fVertexSelector->GetMomentum();
    //dir.Unit();
    //dir*=5;
    x[0]=vv.X();
    x[1]=vv.X()+dir.X();
    y[0]=vv.Y();
    y[1]=vv.Y()+dir.Y();
    z[0]=vv.Z();
    z[1]=vv.Z()+dir.Z();
    fLine = new TPolyLine3D(2,x,y,z);
    fLine->SetLineColor(kYellow);
    fLine->SetLineWidth(3);
    fObjs->Add(fLine);
    
    dir=fVertexSelector->GetMomentumA();
    //dir.Unit();
    //dir*=5;
    x[0]=vv.X();
    x[1]=vv.X()+dir.X();
    y[0]=vv.Y();
    y[1]=vv.Y()+dir.Y();
    z[0]=vv.Z();
    z[1]=vv.Z()+dir.Z();
    fLine = new TPolyLine3D(2,x,y,z);
    fLine->SetLineColor(41);
    fLine->SetLineWidth(3);
    fObjs->Add(fLine);
    
    dir=fVertexSelector->GetMomentumB();
    //dir.Unit();
    //dir*=5;
    x[0]=vv.X();
    x[1]=vv.X()+dir.X();
    y[0]=vv.Y();
    y[1]=vv.Y()+dir.Y();
    z[0]=vv.Z();
    z[1]=vv.Z()+dir.Z();
    fLine = new TPolyLine3D(2,x,y,z);
    fLine->SetLineColor(41);
    fLine->SetLineWidth(3);
    fObjs->Add(fLine);
    
    this->DrawVertex("",kTRUE); // Draw all objects from the cache
    
}

//______________________________________________________________________________
void PAFDisplayVertex::SetLineAttributes()
{
    //*-*-*-*-*-*-*-*-*Invoke the DialogCanvas Line attributes*-*-*-*-*-*-*
    //*-*              =======================================
    
    gROOT->SetSelectedPrimitive(fLine);
    fLine->SetLineAttributes();
}

