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

TGeometry *PAFDisplay::DrcGeometry()
{
    TMaterial *mat;
    TMixture *mix;
    TNode *Node;
    
    TGeometry *drcGeometry = new TGeometry("DrcGeometry","DrcGeometry.C");
    
    //-----------List of Materials and Mixtures--------------
    
    mat = new TMaterial("mat9","ALUMINIUM",26.98,13,2.7);
    mat = new TMaterial("mat15","AIR",14.61,7.3,0.1205000E-02);
    mat = new TMaterial("mat16","VACUUM",0,0,0);
    mat = new TMaterial("mat50","DI_OXYGEN",15.99,8,1.14);
    mat = new TMaterial("mat51","quartz",22.652,10.805,2.2);
    mat = new TMaterial("mat52","carbon-fiber",13.208,6.6,1.59);
    mat = new TMaterial("mat53","stainless-Fe",56,27,4.53);
    mix = new TMixture("mix54","water",2);
    mix->DefineElement(0,1.01,1,0.111);
    mix->DefineElement(1,15.99,8,0.889);
    
    
    //-----------List of Rotation matrices--------------
    
    TRotMatrix *rot486 = new TRotMatrix("rot486","rot486",90,0,90,90,0,0);
    TRotMatrix *rot487 = new TRotMatrix("rot487","rot487",90,30,90,120,0,0);
    TRotMatrix *rot488 = new TRotMatrix("rot488","rot488",90,60,90,150,0,0);
    TRotMatrix *rot489 = new TRotMatrix("rot489","rot489",90,90,90,180,0,0);
    TRotMatrix *rot490 = new TRotMatrix("rot490","rot490",90,120,90,210,0,0);
    TRotMatrix *rot491 = new TRotMatrix("rot491","rot491",90,150,90,240,0,0);
    TRotMatrix *rot492 = new TRotMatrix("rot492","rot492",90,180,90,270,0,0);
    TRotMatrix *rot493 = new TRotMatrix("rot493","rot493",90,210,90,300,0,0);
    TRotMatrix *rot494 = new TRotMatrix("rot494","rot494",90,240,90,330,0,0);
    TRotMatrix *rot495 = new TRotMatrix("rot495","rot495",90,270,90,360,0,0);
    TRotMatrix *rot496 = new TRotMatrix("rot496","rot496",90,300,90,390,0,0);
    TRotMatrix *rot497 = new TRotMatrix("rot497","rot497",90,330,90,420,0,0);
    TRotMatrix *rot498 = new TRotMatrix("rot498","rot498",90,14.99991,90,104.9999,0,-180);
    
    //-----------List of Volumes--------------
    
    TPCON *DIRC = new TPCON("DIRC","DIRC","mat15",89.99992,360,4);
    DIRC->DefineSection(0,-480.92,69.2,273);
    DIRC->DefineSection(1,-314.5,69.2,273);
    DIRC->DefineSection(2,-314.5,81.614,89.385);
    DIRC->DefineSection(3,178.5,81.614,89.385);
    DIRC->SetVisibility(0);
    TPCON *DIRS = new TPCON("DIRS","DIRS","mat15",89.99992,30,4);
    DIRS->DefineSection(0,-480.92,69.2,273);
    DIRS->DefineSection(1,-314.5,69.2,273);
    DIRS->DefineSection(2,-314.5,81.614,89.385);
    DIRS->DefineSection(3,178.5,81.614,89.385);
    DIRS->SetVisibility(0);
    TPCON *DISB = new TPCON("DISB","DISB","mat15",89.99992,30,3);
    DISB->DefineSection(0,-480.92,69.2,273);
    DISB->DefineSection(1,-314.5,69.2,273);
    DISB->DefineSection(2,-314.5,81.614,89.385);
    TPCON *DIST = new TPCON("DIST","DIST","mat15",89.99992,30,2);
    DIST->DefineSection(0,-314.5,81.614,89.385);
    DIST->DefineSection(1,178.5,81.614,89.385);
    TPCON *DISS = new TPCON("DISS","DISS","mat9",89.99992,30,2);
    DISS->DefineSection(0,-314.5,81.614,89.385);
    DISS->DefineSection(1,-133.9,81.614,89.385);
    TPCON *DICS = new TPCON("DICS","DICS","mat15",89.99992,30,2);
    DICS->DefineSection(0,-133.9,81.614,89.385);
    DICS->DefineSection(1,178.5,81.614,89.385);
    TBRIK *DISL = new TBRIK("DISL","DISL","mat15",22,2,245);
    TBRIK *DITR = new TBRIK("DITR","DITR","mat9",1.25,0.1,245);
    TBRIK *DIBX = new TBRIK("DIBX","DIBX","mat15",21.85,1.9,245);
    TBRIK *DIBR = new TBRIK("DIBR","DIBR","mat15",1.75,0.85,245);
    DIBR->SetLineColor(2);
    TBRIK *DIBA = new TBRIK("DIBA","DIBA","mat51",1.75,0.85,61.25);
    TPGON *DIWA = new TPGON("DIWA","DIWA","mix54",89.99992,30,1,10);
    DIWA->DefineSection(0,-445.42,79.614,79.614);
    DIWA->DefineSection(1,-444.5156,79.614,99.0123);
    DIWA->DefineSection(2,-441.1932,79.614,116.648);
    DIWA->DefineSection(3,-435.4904,79.614,133.6637);
    DIWA->DefineSection(4,-427.5142,79.614,149.7396);
    DIWA->DefineSection(5,-417.4145,79.614,164.5737);
    DIWA->DefineSection(6,-405.3811,79.614,177.8873);
    DIWA->DefineSection(7,-390.4258,79.614,190.3051);
    DIWA->DefineSection(8,-385.2728,79.614,193.792);
    DIWA->DefineSection(9,-314.5,79.614,96.72588);
    DIWA->SetLineColor(6);
    TPGON *DISI = new TPGON("DISI","DISI","mat10",89.99992,30,1,2);
    DISI->DefineSection(0,-445.42,79.614,84.614);
    DISI->DefineSection(1,-314.5,79.614,84.614);
    TPCON *DISH = new TPCON("DISH","DISH","mat10",89.99992,360,2);
    DISH->DefineSection(0,-471.3,69.2,273);
    DISH->DefineSection(1,-314.5,69.2,273);
    TPCON *DIS1 = new TPCON("DIS1","DIS1","mat10",89.99992,360,4);
    DIS1->DefineSection(0,-471.3,69.2,273);
    DIS1->DefineSection(1,-466.3,69.2,273);
    DIS1->DefineSection(2,-466.3,69.2,74.2);
    DIS1->DefineSection(3,-314.5,69.2,74.2);
    TPCON *DIS2 = new TPCON("DIS2","DIS2","mat10",89.99992,360,2);
    DIS2->DefineSection(0,-466.3,268,273);
    DIS2->DefineSection(1,-314.5,268,273);
    TBRIK *DIWI = new TBRIK("DIWI","DIWI","mat51",21.5,5.25,0.5);
    TTRAP *DIWE = new TTRAP("DIWE","DIWE","mat51",4.55,0.3478192E-08,-0.2916752,3.949644,1.75,1.75,0,1.35,1.75,1.75,0);
    TTUBE *DIPM = new TTUBE("DIPM","DIPM","mat50",0,1.41,6.75);
    DIPM->SetLineColor(2);
    TBRIK *DISW = new TBRIK("DISW","DISW","mat10",1.41,1.41,1.27);
    TPGON *DILC = new TPGON("DILC","DILC","mat50",90,360,6,2);
    DILC->DefineSection(0,6.75,1.165,1.55);
    DILC->DefineSection(1,7.807779,1.55,1.55);
    
    // Geometry setup
    
    TTUBE *BFAC = new TTUBE("BFAC","BFAC","mat16",0,2000,2000);
    BFAC->SetVisibility(0);
    TNode *Node1 = new TNode("BFAC2","BFAC2",BFAC);
    Node1->cd();
    
    TTUBE *MBAR = new TTUBE("MBAR","MBAR","mat15",139,415,202.5);
    MBAR->SetVisibility(0);
    TNode *Node751 = new TNode("MBAR2","MBAR2",MBAR,0,0,37,0);
    Node751->cd();
    
    Node1->cd();
    TNode *Node517;
    Node517 = new TNode("DIRC1","DIRC1",DIRC,0,0,0,0);
    Node517->cd();
    
    TNode *Node518;
    Node518 = new TNode("DIRS1","DIRS1",DIRS,0,0,0,rot486);
    Node518->cd();
    Node = new TNode("DIWA1","DIWA1",DIWA,0,0,0,0);
    Node = new TNode("DISL1","DISL1",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node537;
    Node537 = new TNode("DIRS2","DIRS2",DIRS,0,0,0,rot487);
    Node537->cd();
    Node = new TNode("DIWA2","DIWA2",DIWA,0,0,0,0);
    Node = new TNode("DISL2","DISL2",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node556;
    Node556 = new TNode("DIRS3","DIRS3",DIRS,0,0,0,rot488);
    Node556->cd();
    Node = new TNode("DIWA3","DIWA3",DIWA,0,0,0,0);
    Node = new TNode("DISL3","DISL3",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node575;
    Node575 = new TNode("DIRS4","DIRS4",DIRS,0,0,0,rot489);
    Node575->cd();
    Node = new TNode("DIWA4","DIWA4",DIWA,0,0,0,0);
    Node = new TNode("DISL4","DISL4",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node594;
    Node594 = new TNode("DIRS5","DIRS5",DIRS,0,0,0,rot490);
    Node594->cd();
    Node = new TNode("DIWA5","DIWA5",DIWA,0,0,0,0);
    Node = new TNode("DISL5","DISL5",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node613;
    Node613 = new TNode("DIRS6","DIRS6",DIRS,0,0,0,rot491);
    Node613->cd();
    Node = new TNode("DIWA6","DIWA6",DIWA,0,0,0,0);
    Node = new TNode("DISL6","DISL6",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node632;
    Node632 = new TNode("DIRS7","DIRS7",DIRS,0,0,0,rot492);
    Node632->cd();
    Node = new TNode("DIWA7","DIWA7",DIWA,0,0,0,0);
    Node = new TNode("DISL7","DISL7",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node651;
    Node651 = new TNode("DIRS8","DIRS8",DIRS,0,0,0,rot493);
    Node651->cd();
    Node = new TNode("DIWA8","DIWA8",DIWA,0,0,0,0);
    Node = new TNode("DISL8","DISL8",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node670;
    Node670 = new TNode("DIRS9","DIRS9",DIRS,0,0,0,rot494);
    Node670->cd();
    Node = new TNode("DIWA9","DIWA9",DIWA,0,0,0,0);
    Node = new TNode("DISL9","DISL9",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node689;
    Node689 = new TNode("DIRS10","DIRS10",DIRS,0,0,0,rot495);
    Node689->cd();
    Node = new TNode("DIWA10","DIWA10",DIWA,0,0,0,0);
    Node = new TNode("DISL10","DISL10",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node708;
    Node708 = new TNode("DIRS11","DIRS11",DIRS,0,0,0,rot496);
    Node708->cd();
    Node = new TNode("DIWA11","DIWA11",DIWA,0,0,0,0);
    Node = new TNode("DISL11","DISL11",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    TNode *Node727;
    Node727 = new TNode("DIRS12","DIRS12",DIRS,0,0,0,rot497);
    Node727->cd();
    Node = new TNode("DIWA12","DIWA12",DIWA,0,0,0,0);
    Node = new TNode("DISL12","DISL12",DISL,-21.87009,81.62077,-69.5,rot498);
    Node517->cd();
    
    /*
    TNode *Node746;
    Node746 = new TNode("DISH1","DISH1",DISH,0,0,0,0);
    Node746->cd();
    Node = new TNode("DIS11","DIS11",DIS1,0,0,0,0);
    Node = new TNode("DIS21","DIS21",DIS2,0,0,0,0);
    Node517->cd();
    */
    Node1->cd();
    
    return drcGeometry;
}