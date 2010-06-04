#ifndef PAFDISPLAY_H
#define PAFDISPLAY_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFDisplay                                                           //
//                                                                      //
// Utility class to display BABAR outline, tracks, hits,..              //
// Adapted from AliROOT (M.Kunze, June 1999)				//
//                                                                      //
// Default parameters which can be modified (on the command line):	//
//									//
// Boolean:								//
// Boolean:								//
// Label	set	true	#Draw particle labels			//
// Vertexing	set	false	#Perform vertexing operation		//
// Conditions	set	true	#Dump conditions database		//
// Logbook	set	false	#Dump electronic logbook		//
// Verbose	set	false	#Dump event				//
// DET		set	true	#Detector outline visibility		//
// MAG		set	false	#Magnet coil visibility			//
// BPQ		set	false	#Beam pipe/quadrupole visibility	//
// SVT		set	false	#Silicon vertex tracker visibility	//
// DCH		set	false	#Drift chamber visibility		//
// DRC		set	false	#DIRC visibility			//
// EMC		set	false	#Calorimeter visibility			//
// IFR		set	false	#Instrumented flux return visibility	//
//									//
// Integer								//
// BgdColor	set	0	#Color of background			//
// LblColor	set	0	#Color of labels			//
// DetColor	set	50	#Color of detector outline		//
//									//
// Double								//
// PtCut	set	0.05	#Cut on Pt (prongs)			//
// ECut		set	0.05	#Cut on EMC energy (neutrals)		//
// LblSize	set	0.025	#Size of labels				//
// LblOffset	set	0.0017	#Label offset from track		//
//									//
// String								//
// View		set	Front	#Detector and event view		//
// Mode		set	Pick	#Pick/Zoom mode				//
// Pid		set	Reco	#Truth/Selector/Reco PID mode		//
// Kine		set	Reco	#Truth/Reco kinematics mode		//
// List		set	TaggingList #Draw cadidate list			//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Dec. 99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TGeometry.h"

class TCanvas;
class TPaveText;
class TPad;
class TList;
class TSlider;
class TGaxis;
class TButton;
class TArc;
class TPolyLine3D;
class TMarker3DBox;
class TEventManager;
class VAbsVertexSelector;
class TAssociator;
class TCandList;
class TConditions;
class TRunDB;
class TParameterManager;

const Int_t kMAXZOOMS = 20;

class PAFDisplay : public TObject {
    
private:
    Bool_t	      fDraw;		     //Activate drawing
    Bool_t	      fTruthMode;	     //MC truth kinematics mode
    Bool_t	      fListMode;	     //List processing mode
    Int_t             fZoomMode;             //=1 if in zoom mode
    Bool_t            fDrawAllViews;         //Flag True if AllViews selected
    Bool_t            fDrawParticles;        //Flag True if particles to be drawn
    Bool_t            fDrawHits;             //Flag True if Hits to be drawn
    Double_t          fTheta;                //Viewing angle theta
    Double_t          fPhi;                  //Viewing angle phi
    Double_t          fPsi;                  //Viewving angle psi (rotation on display)
    Double_t          fRrange;               //Size of view in R
    Double_t          fZrange;               //Size of view along Z
    Double_t          fZoomX0[20];           //Low x range of zoom number i
    Double_t          fZoomY0[20];           //Low y range of zoom number i
    Double_t          fZoomX1[20];           //High x range of zoom number i
    Double_t          fZoomY1[20];           //High y range of zoom number i
    Int_t             fZooms;                //Number of zooms
    Int_t             fHitsCuts;             //Number of hits surviving cuts
    TCanvas          *fCanvas;               //Pointer to the display canvas
    TCanvas          *fConsole;              //Pointer to the display console window
    TPaveText        *fTruthPad;             //Pointer to the MCTruth pad 
    TPad             *fTrigPad;              //Pointer to the trigger pad 
    TPad             *fCutPad;               //Pointer to the momentum cut slider pad 
    TPad             *fEtaPad;               //Pointer to the rapidity cut slider pad 
    TPad             *fButtons;              //Pointer to the buttons pad
    TPad             *fPad;                  //Pointer to the event display main pad
    TPad             *fPad4;                 //Pointer to the event display main pad
    TSlider          *fCutSlider;            //Momentum cut slider
    TSlider          *fEtaSlider;            //Rapidity slider
    TSlider          *fRangeSlider;          //Range slider
    TGaxis	     *fEtaaxis, *fCutaxis;
    TButton          *fRecoButton;           //Button to activate Reco mode
    TButton          *fTruthButton;          //Button to activate Truth mode
    TButton          *fListButton;           //Button to activate List mode
    TButton          *fPickButton;           //Button to activate Pick mode
    TButton          *fZoomButton;           //Button to activate Zoom mode
    TButton          *fUnzoomButton;         //Button to undo zoom
    TString           fFilter;		    //Filter particles
    TString           fMcTracks;	    //MC tracks to draw
    TList            *fTracks;		    //Lists for track PolyLines
    TList            *fVertices;	    //Lists for vertex boxes and momenta
    TCandList        *fParticles;	    //List to store the reco candidates
    TCandList        *fList;		    //List to display candidates
    TCandList        *fMcParticles;	    //List to store the mc candidates
    TAssociator      *fAssociator;	    //MC-truth associator
    TGeometry	     *fDetGeometry;	    //Detector geometry
    TGeometry	     *fBpqGeometry;	    //Beam pipe and quadrupole geometry
    TGeometry	     *fMagGeometry;	    //Magnet coil and cryostat geometry
    TGeometry	     *fSvtGeometry;	    //Vertex tracker geometry
    TGeometry	     *fDchGeometry;	    //Drift chamber geometry
    TGeometry	     *fDrcGeometry;	    //DIRC geometry
    TGeometry	     *fEmcGeometry;	    //Calorimeter geometry
    TGeometry	     *fIfrGeometry;	    //IFR geometry
    TMarker3DBox     *fSpot;		    //Beam spot
    TMarker3DBox     *fPrimVtx;		    //Primary vertex
    TPolyLine3D	     *fLogo;		    //Logo
    TEventManager    *fEvtMgr;		    //Eventmanager to use
    TButton          *fDetButton,	    //Buttons to show detector parts
		     *fBpqButton,
		     *fMagButton,
		     *fIfrButton,
		     *fEmcButton,
		     *fDrcButton,
		     *fDchButton,
		     *fSvtButton;
    
    VAbsVertexSelector *fVertexSelector;    //Vertex selector to use
    TConditions *fConditions;		    //Conditions database
    TRunDB *fRunDB;			    //Run database
    Int_t	 Filter(Int_t number=0);    //Request specific events
    TParameterManager  *fMgr;
    
public:
    PAFDisplay();
    PAFDisplay(TEventManager*, Int_t size=400);
    virtual     ~PAFDisplay();
    Bool_t	 AllViews() {return fDrawAllViews;}
    void         Clear(Option_t *option="");
    TSlider     *CutSlider() {return fCutSlider;}
    void         ShowTrack(Int_t track_number); // *MENU*
    void	 HideTrack(Int_t track_number); // *MENU*
    void	 DisableDetector(const char *name); // *MENU*
    void	 DisplayButtons();
    Int_t	 DistancetoPrimitive(Int_t px, Int_t py);
    void	 DrawEvent(Option_t *option="");
    void	 DumpEvent();
    void	 DrawHits();
    void	 DrawTracks(TCandList *list);
    void	 DrawVertices(TCandList *list);
    void	 DrawPrimaryVertex();
    void	 DrawBeamspot();
    void	 DrawLogo();
    void	 DrawTitle(Option_t *option="");
    void	 DrawMcTruth();
    void	 DrawView(Double_t theta, Double_t phi, Double_t psi=0);
    void	 DrawAllViews();
    void	 DrawViewGL();
    void	 DrawViewX3D();
    void	 EnableDetector(const char *name); // *MENU*
    TSlider     *EtaSlider() {return fEtaSlider;}
    void	 ExecuteEvent(Int_t event, Int_t px, Int_t py);
    Int_t        GetZoomMode() {return fZoomMode;}
    void	 LoadPoints();
    TPad        *Pad() {return fPad;}
    void	 SetBackgroundColor(UInt_t color);
    void	 SetLabelColor(UInt_t color);
    void	 SetLabelSize(Double_t size);
    void	 SetLabelOffset(Double_t offset);
    void	 SetDrawHits(Bool_t draw=kTRUE) {fDrawHits=draw;}   // *MENU*
    void	 SetDrawParticles(Bool_t draw=kTRUE) {fDrawParticles=draw;} // *MENU*
    void	 SetPTcut(Double_t ptcut=0.05); // *MENU*
    Double_t     PTcut();
    void	 SetEcut(Double_t ecut=0.05); // *MENU*
    Double_t     Ecut();
    void	 SetRange(Double_t rrange=350, Double_t zrange=350); // *MENU*
    void	 SetView(Double_t theta, Double_t phi, Double_t psi=0);
    void	 SetTruthMode(Bool_t yesno = kTRUE);
    void	 SetListMode(Bool_t yesno = kTRUE);
    void	 SetPickMode();
    void	 SetZoomMode();
    void	 ShowEvent(Int_t number = 0);
    void	 UnZoom(); // *MENU*
    void         SetGeometry(TGeometry* geometry) { fDetGeometry = geometry; }
    TGeometry   *GetGeometry() { return fDetGeometry;}
    TGeometry   *DetGeometry(Int_t color=50);
    TGeometry   *IfrGeometry();
    TGeometry   *MagGeometry();
    TGeometry   *EmcGeometry();
    TGeometry   *DrcGeometry();
    TGeometry   *DchGeometry();
    TGeometry   *SvtGeometry();
    TGeometry   *BpqGeometry();
    void         SetDetVisibility(Bool_t vis=kTRUE);
    void         SetIfrVisibility(Bool_t vis=kTRUE);
    void         SetMagVisibility(Bool_t vis=kTRUE);
    void         SetEmcVisibility(Bool_t vis=kTRUE);
    void         SetDrcVisibility(Bool_t vis=kTRUE);
    void         SetDchVisibility(Bool_t vis=kTRUE);
    void         SetSvtVisibility(Bool_t vis=kTRUE);
    void         SetBpqVisibility(Bool_t vis=kTRUE);
    void         SetVertexSelector(VAbsVertexSelector *v); 
    
    ClassDef(PAFDisplay, 0)   //Utility class to display BABAR outline, tracks, hits,..
};

R__EXTERN PAFDisplay *gDisplay;

#endif
