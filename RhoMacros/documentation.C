// This Macro generates HTML documentation
// Author: Marcel Kunze, Bochum University, May 1999

#define OUTDIR    "htmldoc/"

#include <iostream.h>

void documentation(const char *source="tmp/")
{   
    gROOT->Macro("$RHO/RhoMacros/LoadLibs.C");
    gSystem->Load("libHtml.so");

    THtml html;
    html.SetOutputDir(OUTDIR);
    html.SetSourceDir(source);
    html.MakeAll();
}

