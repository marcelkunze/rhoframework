#include "TGeometry.h"
#include "TMaterial.h"
#include "TMixture.h"
#include "TNode.h"
#include <TPGON.h>
#include <TTUBS.h>
#include <TCONE.h>
#include <TCONS.h>
#include <TBRIK.h>
#include <TPARA.h>
#include <TTRAP.h>
#include <TTRD1.h>
#include <TPGON.h>
#include <THYPE.h>
#include "PAFDisplay/PAFDisplay.h"

TGeometry *PAFDisplay::DetGeometry(Int_t color)
{
    //  This file has been generated automatically via the root
    //  utility g2root from an interactive version of GEANT
    //   (see ROOT class TGeometry header for an example of use)
    //
    TMaterial *mat;
    TNode *Node;
    
    TGeometry *detectorGeometry = new TGeometry("DetectorGeometry","DetectorGeometry.C");
    
    //-----------List of Materials and Mixtures--------------

    mat = new TMaterial("mat9","ALUMINIUM",26.98,13,2.7);
    mat = new TMaterial("mat10","IRON",55.85,26,7.87);
    mat = new TMaterial("mat15","AIR",14.61,7.3,0.1205000E-02);
    mat = new TMaterial("mat16","VACUUM",0,0,0);

    //-----------List of Rotation matrices--------------

    TRotMatrix *rot604 = new TRotMatrix("rot604","rot604",90,180,90,90,180,90);
    TRotMatrix *rot611 = new TRotMatrix("rot611","rot611",90,60,90,150,0.2414836E-05,75.00001);
    TRotMatrix *rot612 = new TRotMatrix("rot612","rot612",90,300,90.00001,29.99999,0.9659346E-05,15);
    TRotMatrix *rot613 = new TRotMatrix("rot613","rot613",90,30,90,120,0.2957559E-05,60);
    TRotMatrix *rot614 = new TRotMatrix("rot614","rot614",90,330,90.00001,60,0.5122642E-05,30);

    //-----------List of Volumes--------------

    TTUBE *BFAC = new TTUBE("BFAC","BFAC","mat16",0,2000,2000);
    BFAC->SetVisibility(0);
    TBRIK *ZZZZ = new TBRIK("ZZZZ","ZZZZ","mat15",500,5,500);

    TTUBE *MBAR = new TTUBE("MBAR","MBAR","mat15",139,415,202.5);
    MBAR->SetVisibility(0);
    TBRIK *MBPH = new TBRIK("MBPH","MBPH","mat10",87.8496,6,187.5);
    MBPH->SetLineColor(color);
    TPGON *MGPT = new TPGON("MGPT","MGPT","mat10",0,360,6,2);
    MGPT->DefineSection(0,-7.5,182,323.7);
    MGPT->DefineSection(1,7.5,182,323.7);
    MGPT->SetLineColor(color);
    TBRIK *MGC1 = new TBRIK("MGC1","MGC1","mat15",27.12,70.85,7.5);
    MGC1->SetLineColor(20);
    TBRIK *MGC2 = new TBRIK("MGC2","MGC2","mat15",70.85,41.745,7.5);
    MGC2->SetLineColor(20);

    TPCON *MCPF = new TPCON("MCPF","MCPF","mat15",0,360,2);
    MCPF->DefineSection(0,-63.8,64.2821,415);
    MCPF->DefineSection(1,63.8,103.753,415);
    MCPF->SetVisibility(0);
    TPCON *MCPB = new TPCON("MCPB","MCPB","mat15",0,360,2);
    MCPB->DefineSection(0,-63.8,95.5,415);
    MCPB->DefineSection(1,63.8,95.5,415);
    MCPB->SetVisibility(0);

    // BABAR Detector (iron)

    TNode *Node1 = new TNode("BFAC0","BFAC0",BFAC);
    Node1->cd();

    Node = new TNode("ZZZZ0","ZZZZ0",ZZZZ,0,-350,0,0);

    TNode *Node751;
    Node751 = new TNode("MBAR0","MBAR0",MBAR,0,0,37,0);
    Node751->cd();

    TPGON *MBFB133 = new TPGON("MBFB133","MBFB133","mat10",0,360,6,2);
    MBFB133->DefineSection(0,10,290.9,294.1);
    MBFB133->DefineSection(1,-10,290.9,294.1);
    MBFB133->SetLineColor(color);
    Node = new TNode("MBFB1331","MBFB1331",MBFB133,0,0,177.5,0);
    TPGON *MBFB134 = new TPGON("MBFB134","MBFB134","mat10",0,360,6,2);
    MBFB134->DefineSection(0,10,290.9,294.1);
    MBFB134->DefineSection(1,-10,290.9,294.1);
    MBFB134->SetLineColor(color);
    Node = new TNode("MBFB1342","MBFB1342",MBFB134,0,0,-177.5,0);
    TPGON *MBFB135 = new TPGON("MBFB135","MBFB135","mat10",0,360,6,2);
    MBFB135->DefineSection(0,10,304.1,320.5);
    MBFB135->DefineSection(1,-10,304.1,320.5);
    MBFB135->SetLineColor(color);
    Node = new TNode("MBFB1353","MBFB1353",MBFB135,0,0,177.5,0);
    TPGON *MBFB136 = new TPGON("MBFB136","MBFB136","mat10",0,360,6,2);
    MBFB136->DefineSection(0,10,304.1,320.5);
    MBFB136->DefineSection(1,-10,304.1,320.5);
    MBFB136->SetLineColor(color);
    Node = new TNode("MBFB1364","MBFB1364",MBFB136,0,0,-177.5,0);
    Node = new TNode("MBPH1","MBPH1",MBPH,271.736,0,0,0);
    Node = new TNode("MBPH2","MBPH2",MBPH,-271.736,0,0,0);
    Node = new TNode("MBPH3","MBPH3",MBPH,135.868,235.33,0,rot611);
    Node = new TNode("MBPH4","MBPH4",MBPH,-135.868,-235.33,0,rot611);
    Node = new TNode("MBPH5","MBPH5",MBPH,-135.868,235.33,0,rot612);
    Node = new TNode("MBPH6","MBPH6",MBPH,135.868,-235.33,0,rot612);

    TNode *Node866;
    Node866 = new TNode("MGPT1","MGPT1",MGPT,0,0,195,0);
    Node866->cd();
    Node = new TNode("MGC11","MGC11",MGC1,67.12,252.85,0,0);
    Node = new TNode("MGC12","MGC12",MGC1,-67.12,252.85,0,0);
    Node = new TNode("MGC13","MGC13",MGC1,67.12,-252.85,0,0);
    Node = new TNode("MGC14","MGC14",MGC1,-67.12,-252.85,0,0);
    Node = new TNode("MGC21","MGC21",MGC2,218.975,126.425,0,rot613);
    Node = new TNode("MGC22","MGC22",MGC2,218.975,-126.425,0,rot614);
    Node = new TNode("MGC23","MGC23",MGC2,-218.975,126.425,0,rot614);
    Node = new TNode("MGC24","MGC24",MGC2,-218.975,-126.425,0,rot613);
    Node751->cd();
    TNode *Node867;
    Node867 = new TNode("MGPT2","MGPT2",MGPT,0,0,-195,0);
    Node867->cd();
    Node = new TNode("MGC11","MGC11",MGC1,67.12,252.85,0,0);
    Node = new TNode("MGC12","MGC12",MGC1,-67.12,252.85,0,0);
    Node = new TNode("MGC13","MGC13",MGC1,67.12,-252.85,0,0);
    Node = new TNode("MGC14","MGC14",MGC1,-67.12,-252.85,0,0);
    Node = new TNode("MGC21","MGC21",MGC2,218.975,126.425,0,rot613);
    Node = new TNode("MGC22","MGC22",MGC2,218.975,-126.425,0,rot614);
    Node = new TNode("MGC23","MGC23",MGC2,-218.975,126.425,0,rot614);
    Node = new TNode("MGC24","MGC24",MGC2,-218.975,-126.425,0,rot613);
    Node751->cd();

    Node1->cd();

    TNode *Node871;
    Node871 = new TNode("MCPB0","MCPB0",MCPB,0,0,-229.3,rot604);
    Node871->cd();

    TPGON *MXPC02 = new TPGON("MXPC02","MXPC02","mat10",0,360,6,2);
    MXPC02->DefineSection(0,-1,102,285.9);
    MXPC02->DefineSection(1,1,102,285.9);
    MXPC02->SetLineColor(color);

    Node = new TNode("MXPC02","MXPC02",MXPC02,0,0,-62.8,0);
    TPCON *MCFC02 = new TPCON("MCFC02","MCFC02","mat10",0,360,2);
    MCFC02->DefineSection(0,-1,102,118.779);
    MCFC02->DefineSection(1,1,102,118.779);
    MCFC02->SetLineColor(color);

    TBRIK *MCBE2205 = new TBRIK("MCBE2205","MCBE2205","mat10",6.5,91.95,57.2);
    MCBE2205->SetLineColor(color);
    Node = new TNode("MCBE22051","MCBE22051",MCBE2205,0,-193.95,-6.6,0);
    TBRIK *MCBE2206 = new TBRIK("MCBE2206","MCBE2206","mat10",6.5,91.95,34.3);
    MCBE2206->SetLineColor(color);
    Node = new TNode("MCBE22062","MCBE22062",MCBE2206,0,193.95,16.3,0);
    TTRAP *MCBT2207 = new TTRAP("MCBT2207","MCBT2207","mat10",22.9,0.5773503,0,95.2,3.75278,3.75278,0,95.2,3.75278,3.75278,0);
    MCBT2207->SetLineColor(color);
    Node = new TNode("MCBT22071","MCBT22071",MCBT2207,-16.0792,190.7,-40.9,0);
    TTRAP *MCBT2208 = new TTRAP("MCBT2208","MCBT2208","mat10",22.9,-0.5773503,0,95.2,3.75278,3.75278,0,95.2,3.75278,3.75278,0);
    MCBT2208->SetLineColor(color);
    Node = new TNode("MCBT22082","MCBT22082",MCBT2208,16.0792,190.7,-40.9,0);
    TTRD1 *MCRH2209 = new TTRD1("MCRH2209","MCRH2209","mat15",26.4,0,95.2,22.9);
    Node = new TNode("MCRH22090","MCRH22090",MCRH2209,0,190.7,-40.9,0);
    TPCON *MCCO2210 = new TPCON("MCCO2210","MCCO2210","mat10",0,360,2);
    MCCO2210->DefineSection(0,-57.2,95.5,102);
    MCCO2210->DefineSection(1,57.2,95.5,102);
    MCCO2210->SetLineColor(color);
    Node = new TNode("MCCO22100","MCCO22100",MCCO2210,0,0,-6.6,0);

    Node1->cd();
 
    return detectorGeometry;
}

TGeometry *PAFDisplay::DchGeometry()
{
    TMaterial *mat;
    TMixture  *mix;
    TNode *Node;
    
    TGeometry *dchGeometry = new TGeometry("DchGeometry","DchGeometry.C");
    
    //-----------List of Materials and Mixtures--------------

    mat = new TMaterial("mat15","AIR",14.61,7.3,0.1205000E-02);
    mat = new TMaterial("mat16","VACUUM",0,0,0);
    mix = new TMixture("mix48","Air",2);
    mix->DefineElement(0,14.007,7,0.7);
    mix->DefineElement(1,16,8,0.3);
 
    // Geometry setup

    TTUBE *BFAC = new TTUBE("BFAC","BFAC","mat16",0,2000,2000);
    BFAC->SetVisibility(0);
    TNode *Node1 = new TNode("BFAC2","BFAC2",BFAC);
    Node1->cd();

    // Drift Chamber
    
    TNode *Node512;
    TTUBE *DCHA = new TTUBE("DCHA","DCHA","mix48",23.6,80.9,164.8);
    DCHA->SetLineColor(kWhite);
    Node512 = new TNode("DCHA1","DCHA1",DCHA,0,0,14.8,0);
    Node512->cd();
    Node1->cd();

    return dchGeometry;
}


TGeometry *PAFDisplay::EmcGeometry()
{
    TMaterial *mat;
    TMixture  *mix;
    TNode *Node;
    
    TGeometry *emcGeometry = new TGeometry("EmcGeometry","EmcGeometry.C");
    
    //-----------List of Materials and Mixtures--------------

    mat = new TMaterial("mat15","AIR",14.61,7.3,0.1205000E-02);
    mat = new TMaterial("mat16","VACUUM",0,0,0);
    mat = new TMaterial("mat55","em_Air",14.61,7.3,0.1205000E-02);
    mix = new TMixture("mix59","em_CFC",3);
    mix->DefineElement(0,13.208,6.6,0.717);
    mix->DefineElement(1,10.43,5.28,0.143);
    mix->DefineElement(2,26.982,13,0.14);

    //-----------List of Volumes--------------

    TPCON *EMCA = new TPCON("EMCA","EMCA","mat55",0,360,5);
    EMCA->DefineSection(0,-142.728,90.1999,136);
    EMCA->DefineSection(1,-112.857,90.1999,136);
    EMCA->DefineSection(2,179.813,90.1999,136);
    EMCA->DefineSection(3,197.212,50.364,136);
    EMCA->DefineSection(4,231.878,59.7964,136);
    TPCON *EMBA = new TPCON("EMBA","EMBA","mix59",0,360,4);
    EMBA->DefineSection(0,-140.228,114.891,125.1);
    EMBA->DefineSection(1,-112.857,91.8999,125.1);
    EMBA->DefineSection(2,181.054,91.8999,125.1);
    EMBA->DefineSection(3,214.628,110.813,125.1);
    TPCON *EM1E = new TPCON("EM1E","EM1E","mix59",0,360,4);
    EM1E->DefineSection(0,181.079,91.6061,91.6063);
    EM1E->DefineSection(1,198.174,50.6469,101.25);
    EM1E->DefineSection(2,215.621,55.3943,111.092);
    EM1E->DefineSection(3,229.378,59.1374,75.1963);
 
    // Geometry setup

    TTUBE *BFAC = new TTUBE("BFAC","BFAC","mat16",0,2000,2000);
    BFAC->SetVisibility(0);
    TNode *Node1 = new TNode("BFAC3","BFAC3",BFAC);
    Node1->cd();

    // Calorimeter
        
    EMCA->SetLineColor(kYellow);
    TNode *Node747 = new TNode("EMCA1","EMCA1",EMCA,0,0,0,0); // EMC mother volume
    Node747->cd();
    /*
    Node = new TNode("EMBR1","EMBR1",EMBR,0,0,33.4779,0); // Barrel
    TNode *Node748;
    Node748 = new TNode("EMBS1","EMBS1",EMBS,0,0,0,0);
    Node748->cd();
    TNode *Node749;
    Node749 = new TNode("EMBM1","EMBM1",EMBM,0,0,0,0);
    Node749->cd();
    */
    EMBA->SetLineColor(kYellow);
    Node = new TNode("EMBA1","EMBA1",EMBA,0,0,0,0); // em-CFC
    //Node748->cd();
    Node747->cd();
    /*
    TNode *Node750; // Endcap
    Node750 = new TNode("EM1S1","EM1S1",EM1S,0,0,0,0);
    Node750->cd();
    */
    EM1E->SetLineColor(kYellow);
    Node = new TNode("EM1E1","EM1E1",EM1E,0,0,0,0); // em-CFC
    Node747->cd();
    //Node = new TNode("EM1R1","EM1R1",EM1R,0,0,0,0);

    return emcGeometry;
}
   

TGeometry *PAFDisplay::MagGeometry()
{

    TMaterial *mat;
    TNode *Node;
    
    TGeometry *magGeometry = new TGeometry("MagnetGeometry","MagnetGeometry.C");
    
    //-----------List of Materials and Mixtures--------------

    mat = new TMaterial("mat9","ALUMINIUM",26.98,13,2.7);
    mat = new TMaterial("mat10","IRON",55.85,26,7.87);
    mat = new TMaterial("mat15","AIR",14.61,7.3,0.1205000E-02);
    mat = new TMaterial("mat16","VACUUM",0,0,0);

    //-----------List of Volumes--------------

    TTUBE *CCRY = new TTUBE("CCRY","CCRY","mat9",142,177,192.5);
    TTUBE *CBOB = new TTUBE("CBOB","CBOB","mat9",154.25,159.25,190.5);

    // Geometry setup

    TTUBE *BFAC = new TTUBE("BFAC","BFAC","mat16",0,2000,2000);
    BFAC->SetVisibility(0);
    TNode *Node1 = new TNode("BFAC4","BFAC4",BFAC);
    Node1->cd();

    TTUBE *MBAR = new TTUBE("MBAR","MBAR","mat15",139,415,202.5);
    MBAR->SetVisibility(0);
    TNode *Node751 = new TNode("MBAR4","MBAR4",MBAR,0,0,37,0);
    Node751->cd();

    // Cryostat and Coil

    TNode *Node868;
    CCRY->SetLineColor(kWhite);
    Node868 = new TNode("CCRY0","CCRY0",CCRY,0,0,0,0);
    Node868->cd();
    //Node = new TNode("CAIR0","CAIR0",CAIR,0,0,0,0);
    Node = new TNode("CBOB0","CBOB0",CBOB,0,0,0,0);
    //Node = new TNode("CABL0","CABL0",CABL,0,0,0,0);

    return magGeometry;
}
