//##########################################################################
// Initialization code executed at the start of a ROOT session.
//
// File: $Id: RooLogon.C,v 1.1 2001-05-29 22:42:00 marcel Exp $
// Authors:
//   DK, David Kirkby, Stanford University, kirkby@hep.stanford.edu
//   AT, Alexandre (Sasha) Telnov, UC Berkeley/LBNL, avtelnov@lbl.gov
// History:
//   10-Dec-1999 DK Created initial version
//   19-Apr-2000 DK Add BABAR style for approved plots
//   05-Dec-2000 AT Added mention of a BABARSmartLabel()
//##########################################################################

{
  // use the 'plain' style for plots (white backgrounds, etc)
  cout << "...using style 'Plain'" << endl;
  gROOT->SetStyle("Plain");

  // check that we are running from a release working directory
  void* release= gSystem->OpenDirectory("RELEASE");
  if(0 == release) {
    printf("*** Cannot find RELEASE directory. Are you in a workdir?\n");
  }
  else {
    printf("...found RELEASE directory\n");
    gSystem->FreeDirectory(release);
  }
  void* parent= gSystem->OpenDirectory("PARENT");
  if(0 == parent) {
    printf("*** Cannot find PARENT directory. Are you in a workdir?\n");
  }
  else {
    printf("...found PARENT directory\n");
    gSystem->FreeDirectory(parent);
  }
  const char* current= gSystem->Getenv("BFCURRENT");
  if(0 == current) {
    printf("*** The BFCURRENT environment variable is not set!\n");
    printf("*** Will not be able to find shared libraries from a release");
  }
  else {
    printf("...running in release %s\n", current);
  }
  
  // load shared libraries from the current release
  // (see comments for loadSrtLib in RooAlias.C)
  // for example:
  gSystem.Load("libProof.so");
  gSystem.Load("libTreePlayer.so");
  loadSrtLib("RooFitTools");
  
  // Create the 'BABAR' style for approved plots. Note that this style may need
  // some fine tuning in your macro depending on what you are plotting, e.g.
  //
  //  gStyle->SetMarkerSize(0.75);  // use smaller markers in a histogram with many bins
  //  gStyle->SetTitleOffset(0.65,"y");  // bring y axis label closer to narrow values

  TStyle *babarStyle= new TStyle("BABAR","BaBar approved plots style");

  // use plain black on white colors
  babarStyle->SetFrameBorderMode(0);
  babarStyle->SetCanvasBorderMode(0);
  babarStyle->SetPadBorderMode(0);
  babarStyle->SetPadColor(0);
  babarStyle->SetCanvasColor(0);
  babarStyle->SetTitleColor(0);
  babarStyle->SetStatColor(0);
  babarStyle->SetFillColor(0);

  // set the paper & margin sizes
  babarStyle->SetPaperSize(20,26);
  babarStyle->SetPadTopMargin(0.05);
  babarStyle->SetPadRightMargin(0.05);
  babarStyle->SetPadBottomMargin(0.16);
  babarStyle->SetPadLeftMargin(0.12);

  // use large Times-Roman fonts
  babarStyle->SetTextFont(132);
  babarStyle->SetTextSize(0.08);
  babarStyle->SetLabelFont(132,"x");
  babarStyle->SetLabelFont(132,"y");
  babarStyle->SetLabelFont(132,"z");
  babarStyle->SetLabelSize(0.05,"x");
  babarStyle->SetTitleSize(0.06,"x");
  babarStyle->SetLabelSize(0.05,"y");
  babarStyle->SetTitleSize(0.06,"y");
  babarStyle->SetLabelSize(0.05,"z");
  babarStyle->SetTitleSize(0.06,"z");

  // use bold lines and markers
  babarStyle->SetMarkerStyle(8);
  babarStyle->SetHistLineWidth(1.85);
  babarStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes

  // do not display any of the standard histogram decorations
  babarStyle->SetOptTitle(0);
  babarStyle->SetOptStat(0);
  babarStyle->SetOptFit(0);

  // put tick marks on top and RHS of plots
  babarStyle->SetPadTickX(1);
  babarStyle->SetPadTickY(1);

  cout << endl << "    For approved plots use: gROOT->SetStyle(\"BABAR\");"
       << endl << "  To add a BABAR label use: BABARLabel();"
       << endl << "  To add a better-scaling BABAR label use: BABARSmartLabel();"
       << endl << "    type \"BABARSmartLabel(-2);\" for options"
       << endl << endl;

  // restore the plain style
  gROOT->SetStyle("Plain");
}
