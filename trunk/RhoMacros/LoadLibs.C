// Macro to load the Rho shared libs
{
#include <iostream.h>

// This is the Windows section
#ifdef WIN32
    gSystem->Load("$RHO/lib/RhoMath.dll");
    gSystem->Load("$RHO/lib/RhoConditions.dll");
    gSystem->Load("$RHO/lib/RhoBase.dll");
    gSystem->Load("$RHO/lib/RhoNNO.dll");
    gSystem->Load("$RHO/lib/KangaSchema.dll");
    gSystem->Load("$RHO/lib/PAFSchema.dll");
    gSystem->Load("$RHO/lib/RhoManager.dll");
    gSystem->Load("$RHO/lib/RhoSelector.dll");
    gSystem->Load("$RHO/lib/RhoHistogram.dll");
    gSystem->Load("$RHO/lib/RhoTools.dll");
#ifdef DISPLAY
    gSystem->Load("$PAF/lib/PAFDisplay.dll");
#endif
#ifdef BETAPID
    gSystem->Load("$PAF/lib/PAFBetaPid.dll");
#endif

// This is the UNIX section
#else
    TString vers = TString(getenv("BFCURRENT"));
    if (vers != "") vers = "_" + vers;
    vers += ".so";
    TString versroot = TString(getenv("BFCURRENT"));
    if ( versroot != "") versroot += "_ROOT.so"; 
    gSystem->Load("libEG.so");              // To use PDG database
    gSystem->Load("libPhysics.so");         // To use TVector
    gSystem->Load("libProof.so");
    gSystem->Load("libTreePlayer.so");      // To use TTreeFormula

    gSystem->Load(TString("libRhoNNO")+vers);
    gSystem->Load(TString("libRhoMath")+vers);
    gSystem->Load(TString("libRhoConditions")+vers);
    gSystem->Load(TString("libRhoBase")+vers);
    gSystem->Load(TString("libRhoTools")+vers);
    gSystem->Load(TString("libRhoHistogram")+vers);
    gSystem->Load(TString("libPAFSchema")+vers);
    gSystem->Load(TString("libKangaSchema")+vers);
   // gSystem->Load(TString("libPAFSchema")+vers);
    gSystem->Load(TString("libRhoManager")+vers);
    gSystem->Load(TString("libRhoSelector")+vers);
    //gSystem->Load(TString("libPAFFitter")+vers);
    //gSystem->Load(TString("libPAFAdapters")+vers);
    //gSystem->Load(TString("libPAFFramework")+vers);

#ifdef DISPLAY
    gSystem->Load(TString("libPAFDisplay")+vers);
#endif

#ifdef BETAPID
    gSystem->Load(TString("libPAFBetaPid")+vers);
#endif

#endif
 
}
