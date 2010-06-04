#include <math.h> 

#include "TROOT.h" 
#include "TFile.h" 
#include "TTree.h" 
#include "TCut.h" 
#include "TH2.h" 
#include "TF1.h" 
#include "TObjArray.h" 
#include "TCanvas.h" 
#include "TApplication.h" 
#include "TEventList.h" 
#include "TRandom.h" 
#include "TMath.h" 
#include "TStyle.h"
#include "TStringLong.h"

#include <iostream>
#include <fstream>
#include <strstream>
#include <cstdlib>
#include <string>
using namespace std;

#include <RhoGA/ga.h> 
#include <RhoGA/GARealGenome.h> 
#include <RhoGA/GARealGenome.cxx> 


// -------------------------------------------------------------------------- 

Int_t		nPop =  10;	// how many individuals in the population 
Int_t		nGen = 500;	// number of generations to evolve 
Double_t	pMutMin = 0.05;	// likelihood of mutating new offspring 
Double_t	pMutMax = 0.5;	// likelihood of mutating new offspring 
Double_t	pCross = 0.7;	// likelihood of crossing over parents 
Double_t        mutSigma = 0.2; 

Double_t SBLimit=1.0; 

Double_t Gsignal = 0; 
Double_t Gside = 0; 


TString algorithmStr = "simple"; 
TString selectorStr = "tournament"; 
TString scalerStr = "linear"; 
Float_t sigma = 1.0; 
Float_t alpha = 1.0; 

Int_t samples = 0; 

// tuple location 
TString	treefileName	= ""; 
TString	treeName	= ""; 

// PreSelection 
TCut	preCut		= "1"; 
// Selection; 
TString rawcut          =""; 

// signal and sidebins 
TCut	 sideCut	= ""; 
TCut	 signalCut	= ""; 
Double_t sideFactor = 1.0;   

// the toDraw 
TString toDraw = ""; 

// the cuts we want to optimize 
typedef struct { 
    char   type;  // "f" or "i" float or integer  
    Float_t min;   // minimum allowed value 
    Float_t max;   // maximum allowed value 
} PARA; 

Int_t	Ngene  = 0;   // number of cuts 
Int_t gene2para[100]; 

const int maxpar =100; 
PARA para[maxpar];     

TString outfilename = ""; 
TString dilustr = ""; 

// --------------------------------------------------------------------------- 

TTree* tree; 
TTree* sideTree; 
TTree* signalTree; 
//TTree* MCTree; 

TCanvas* c1=0; 

float Objective(GAGenome &); 
float Comparator(const GAGenome&, const GAGenome&); 
const char* getCut(GAGenome &); 
int KaniggelsRealGaussianMutator(GAGenome& , float); 
int KaniggelsRealBlendCrossover(const GAGenome&, const GAGenome&, 
				GAGenome*, GAGenome*); 
void  KaniggelsRealInitializer(GAGenome & c); 

int main(int argc, char** argv)
{ 

    TROOT root("root","Kaniggels"); 

    gROOT->Macro("~/rootlogon.C"); 

    // See if we've been given a seed to use (for testing purposes).  When you 
    // specify a random seed, the evolution will be exactly the same each time 
    // you use that seed number. 
    for(int jj=1; jj<argc; jj++) { 
	if(strcmp(argv[jj++],"seed") == 0) { 
	    GARandomSeed((unsigned int)atoi(argv[jj])); 
	} 
    } 

    // --------------------------- read the setup file 
    int i;  
    for(i=0; i<maxpar; ++i) 
	para[i].type = 'x'; 


    cout << "Reading the setup file" << endl; 

    char* filename = argv[1]; 
    outfilename = filename; 
    outfilename += ".pop"; 


    std::ifstream setup( filename, ios::in ); 
    if (setup.good()) { 

	string str=""; 
	while( getline(setup,str) ){ 

	    if( str.find("#")!=string::npos ) continue; 

	    if( str.find("PRESELECTCUT")!=string::npos ){ 
		getline(setup,str); 
		preCut = str.c_str(); 
		continue; 
	    } 
	    if( str.find("SAMPLES")!=string::npos ){ 	 
		setup >> samples; 
		continue; 
	    } 
	    if( str.find("SIDECUT")!=string::npos ){ 
		getline(setup,str); 
		sideCut = str.c_str(); 
		continue; 
	    } 
	    if( str.find("SIGNALCUT")!=string::npos ){ 
		getline(setup,str); 
		signalCut = str.c_str(); 
		continue; 
	    } 
	    if( str.find("SIDESIGNALFACTOR")!=string::npos ){ 
		setup >> sideFactor; 
		continue; 
	    } 
	    if( str.find("GRAPHICS")!=string::npos ){ 
		getline(setup,str); 
		toDraw = str.c_str(); 
		continue; 
	    } 
	    if( str.find("DILU")!=string::npos ){ 
		getline(setup,str); 
		dilustr = str.c_str(); 
		continue; 
	    } 
	    if( str.find("TUPLE")!=string::npos ){
		string s1, s2;
		setup >> s1  >> s2;
		treefileName = s1.c_str();
		treeName = s2.c_str();
		continue;  
	    } 
	    if( str.find("PAR")!=string::npos ){ 
		int num; 
		char type; 
		float min,max; 
		setup >> num >> type >> min >> max; 
		para[num].type = type; 
		para[num].max = max; 
		para[num].min = min; 
		continue; 
	    } 
	    if( str.find("CUT")!=string::npos ){ 
		getline(setup,str); 
		rawcut = str.c_str(); 
		cout << rawcut.Data() << endl; 
	    } 
	    if( str.find("ALGORITHM")!=string::npos ){ 
		setup >> str; 
		algorithmStr = str.c_str(); 
		continue; 
	    } 
	    if( str.find("SELECTOR")!=string::npos ){ 
		setup >> str; 
		selectorStr = str.c_str(); 
		continue; 
	    } 
	    if( str.find("SCALER")!=string::npos ){ 
		setup >> str; 
		scalerStr = str.c_str(); 
		continue; 
	    } 
	    if( str.find("SHARING_SETTINGS")!=string::npos ){ 
		setup >> sigma >> alpha; 
		continue; 
	    } 
	    if( str.find("SETTINGS")!=string::npos ){ 
		setup >> nGen >> nPop >> pCross >> pMutMin >> pMutMax >> mutSigma; 
		continue; 
	    } 
	} 
    }else 
	cout << "can't read setup file, bad things will happen soon"; 


    cout << "preselection: " << preCut.GetTitle() << "\n" 
	<< "signal region:  " << signalCut.GetTitle() << "\n" 
	<< "sidebin region:  " << sideCut.GetTitle() << "\n" 
	<< "signal-side factor: " << sideFactor << endl; 
    cout << "populationsize: " << nPop << "\n" 
	<< "number of generations: " << nGen << "\n" 
	<< "crossover probability: " << pCross << "\n" 
	<< "mutation probability: " << pMutMin << " <= p <= " << pMutMax  << "\n" 
	<< "mutation sigma: " << mutSigma  
	<< endl; 

    // ----------------------- root graphics 

    TApplication* theApp; 

    if( toDraw!="" ){ 
	cout << "setting up ROOT graphics" << endl; 
	theApp = new TApplication("App", &argc, argv); 
	c1 = new TCanvas("c1", "kaniggels", 600, 480); 
	//c1->Divide(2,1); 
	//c1->cd(); 
	gStyle->SetPalette(1); 
    } 

    // --------------------------- setup the tuples 

    cout << "setting up the tuples" << endl; 
    TFile* file = new TFile( treefileName, "READ"); 
    tree = (TTree*) file->Get( treeName ); 

    // ------- speed it up 
    // creates two smart trees/tuples (side and signal) that contains only the  
    // preselected rows of necessary branches   
    TObjArray* branches = tree->GetListOfBranches(); 
    TStringLong monster(preCut); 
    monster.Append(sideCut); 
    monster.Append(signalCut);   
    monster.Append(rawcut); 
    monster.Append(toDraw); 

    tree->SetBranchStatus( "*",  0);
    for( i=0; i<=branches->GetLast(); ++i){ 
	TBranch* branch = (TBranch*)branches->UncheckedAt(i); 
	TString name(branch->GetName()); 
	if (monster.Contains( name )) tree->SetBranchStatus( name,  1); 
    } 


    tree->Draw(">>select", preCut);//, "" , 100000); 
    TEventList *elist = (TEventList*)gDirectory->Get("select"); 
    tree->SetEventList(elist); 

    if (samples == 0) 
	samples = int(tree->GetEntries()); 
    sideTree = tree->CopyTree( sideCut ,"", samples); 
    sideTree->SetName("sideTree"); 
    signalTree = tree->CopyTree( signalCut ,"", samples); 
    signalTree->SetName("signalTree"); 

    monster = ""; 
    monster.Append(rawcut); 
    monster.Append(toDraw); 

    sideTree->SetBranchStatus("*",0);
    signalTree->SetBranchStatus("*",0);
    for( i=0; i<=branches->GetLast(); ++i){ 
	TBranch* branch = (TBranch*)branches->UncheckedAt(i); 
	TString name(branch->GetName()); 
	if (monster.Contains(name)) {
	    sideTree->SetBranchStatus( name,  1 ); 
	    signalTree->SetBranchStatus( name,  1 ); 
	}
    } 

    // ----------------------- analyze cut 

    cout << "setup done" << endl; 

    int start = -1; 
    int stop = 0; 
    while( (start = rawcut.Index("[", stop)) != -1 ){ 
	stop = rawcut.Index("]", start); 
	TSubString str = rawcut(start+1,stop-start-1); 
	Int_t idx = atoi(str.Data()); 
	cout << "parameter " << idx << " read." << endl; 
	Bool_t newpara = kTRUE; 
	for(Int_t i=0; i<Ngene;++i) 
	    if(gene2para[i] == idx) 
		newpara = kFALSE; 
	if(newpara) 
	    gene2para[Ngene++] = idx;  
    } 

    // ----------------------- setup the genome 
    GARealAlleleSetArray alleles; 
    for( i=0; i<Ngene; ++i) 
	if( para[gene2para[i]].type == 'f' ) 
	    alleles.add( para[gene2para[i]].min, para[gene2para[i]].max); 
	else 
	    if(para[gene2para[i]].type == 'i') 
		alleles.add( para[gene2para[i]].min, para[gene2para[i]].max, 1.0); 
	    else 
		cout << "can't set up the genome, bad thing will happen soon" << endl;  

    GARealGenome genome( alleles, Objective); 

    genome.crossover(KaniggelsRealBlendCrossover); 
    genome.mutator(KaniggelsRealGaussianMutator); 
    genome.comparator(Comparator); 
    genome.initializer(KaniggelsRealInitializer); 


    // ----------------------- setup the genetic algorithm 
    // GASteady State converges faster than GASimple but some kind of incest 
    // happens in late generations. If it finds the global maximum this would be no problem. 
    // Choosing huge starting population should prevent the algorithm from getting stuck in a local 
    // maximum. 


    // choose a genetic algorithm  (have a look at galib245/doc) 
    GAGeneticAlgorithm* ga=0; 
    if( algorithmStr == "simple" ){ 
	cout << "will use simple genetic algorithm" << endl; 
	ga = new  GASimpleGA(genome); 
    }
    else if ( algorithmStr == "steadystate" ){ 
	cout << "will use steadyState  genetic algorithm" << endl; 
	ga = new GASteadyStateGA(genome); 
    }
    else 
	cout << "algorithm undefined, bad things will happen soon." << endl; 

    ga->maximize();		// by default we want to maximize the objective 
    ga->populationSize(nPop);	// how many individuals in the population 
    ga->nGenerations(nGen);	// number of generations to evolve 
    ga->pMutation(pMutMin);	// likelihood of mutating new offspring 
    ga->pCrossover(pCross);	// likelihood of crossing over parents 

    // choose a scaling function: fitness=scaler(objective) 
    GAScalingScheme* scaler=0; 
    if( scalerStr == "linear" ){ 
	cout << "will use linear scaling" << endl; 
	scaler = new GALinearScaling(1.2); 
    }
    else if( scalerStr == "sharing" ){ 
	cout << "will use fairshare scaling: sigma = " << sigma << "\t alpha = " << alpha << endl; 
	scaler = new GASharing( Comparator, sigma, alpha ); 
    }
    else 
	cout << "scaler undefined, bad things will happen soon." << endl; 

    ga->scaling(*scaler);	 


    // choose a parent selector for crossing 
    GASelectionScheme* selector=0;   
    if( selectorStr == "tournament" ){ 
	cout << "will use tournament selection scheme" << endl; 
	selector = new GATournamentSelector; 
    }
    else if( selectorStr == "uniform" ){ 
	cout << "will use uniform selection scheme" << endl; 
	selector = new GAUniformSelector; 
    }
    else if( selectorStr == "roulett" ){ 
	cout << "will use roulettwheel selection scheme" << endl; 
	selector = new GARouletteWheelSelector; 
    }
    else 
	cout << "selector undefined, bad things will happen soon." << endl; 

    ga->selector(*selector); 


    //ga.recordDiversity(gaTrue); 
    //ga.scoreFilename("kaniggels.dat");		// name of file for scores 
    //ga.scoreFrequency(1);				// keep the scores of every generation 
    //ga.flushFrequency(10);			// specify how often to write the score to disk 
    //ga.selectScores(GAStatistics::AllScores); 


    ga->parameters(argc, argv, gaFalse);		// parse commands, don't complain if bogus args 

    ga->initialize(); 
    ga->population().write(cout); 
    if(argc>=3){ 
	cout << "trying to read population in " << argv[2] << " "; 
	ifstream inFile( outfilename ); 
	if(inFile.good()){ 
	    const_cast<GAPopulation&>(ga->population()).read(inFile); 
	    cout << " ... OK" << endl; 
	    ga->population().write(cout); 
	}
	else
	{ 
	    cout << " ... nixglop !" << endl; 
	} 
	inFile.close(); 
    } 

    // --------------- start the algorithm 
    float diversity = -1; 
    float pMut; 
    while(!ga->done()) { 

	ga->step(); 

	// prevent population from incest, adjust the mutation probability  
	// depending on the diversity of the population  
	if( ga->generation() % 10 == 0 || diversity==-1){ 
	    diversity = ga->population().div(); 
	    pMut = exp( -50.0 * diversity) * (pMutMax-pMutMin) + pMutMin;   
	    ga->pMutation(pMut); 
	    std::ofstream outFile( outfilename.Data() ); 
	    ga->population().write(outFile); 
	    outFile.close(); 
	} 

	// fancy output 
	cout << ga->generation() << ". generation: " << "-----------------------------------------------------" << endl; 
	cout << "diversity of genome : " << diversity << endl; 
	cout << "mutation probability: " << pMut << endl; 

	//for( Int_t h=0; h<1; ++h){   
	//    GARealGenome genome = (GARealGenome &) ga->population().individual(h); 
	//    cout << getCut(genome); 
	//    Objective(genome); 
	//    cout << "  dilution:  "  
	//	<< genome.score() << " " 
	//	<< Gsignal << " " 
	//	<< Gsignal/Gside << endl; 

	//    if( h==0 && c1){  // plot the best one 
	//	TCut cc( getCut( genome ) );  
	//	tree->SetLineColor(1); 
	//	tree->SetFillColor(2); 
	//	tree->Draw( toDraw.Data(), cc); 
	//	tree->SetFillColor(0); 
	//	tree->SetLineColor(2); 
	//	tree->Draw( toDraw.Data() , sideCut && cc,"same"); 
	//	tree->SetLineColor(3); 
	//	tree->Draw( toDraw.Data() , signalCut && cc,"same"); 
	//	c1->Update(); 
	//    } 
	//} 

	cout.precision(4); 
	static double tst = 0; 
	double max = 0; 
	double min = 999999999.9; 
	double mean = 0; 
	double minp[100]; 
	double maxp[100]; 
	double meanp[100]; 
	double sigmap[100]; 

	for(i=0; i<Ngene; ++i){   
	    minp[i] = 99999999.9; 
	    maxp[i] = -99999999.9; 
	    meanp[i] = 0.0; 
	    sigmap[i] = 0.0; 
	} 

	for( Int_t h=0; h<nPop; ++h){   
	    GARealGenome genome = (GARealGenome &) ga->population().individual(h); 

	    double score = genome.score(); 
	    mean += score; 
	    if(score<min) 
		min = score; 
	    if(score>max) 
		max = score; 

	    for(i=0; i<Ngene; ++i){   
		double val = genome.gene(i); 
		if(val<minp[i]) 
		    minp[i] = val; 
		if(val>maxp[i]) 
		    maxp[i] = val; 
		meanp[i] += val; 
	    } 

	    if(h==0){ 
		cout << getCut(genome) << endl; 
		Objective(genome); 
		cout << "score signal side S/N: "  
		    << genome.score() << "\t" 
		    << Gsignal << "\t" 
		    << Gside << "\t" 
		    << Gsignal/Gside << endl; 

		if(c1){  // plot the best one 
		    if( tst != score ){ 
			tst = score; 
			//cout << "\a"; 
			TCut cc( getCut( genome ) );  
			//c1->cd(1); 
			tree->SetLineColor(1); 
			tree->SetFillColor(2); 
			tree->Draw( toDraw.Data(), cc); 
			tree->SetFillColor(0); 
			sideTree->SetLineColor(1); 
			sideTree->Draw( toDraw.Data() , cc,"same"); 
			signalTree->SetLineColor(1); 
			signalTree->Draw( toDraw.Data() , cc,"same"); 
		    } 
		    c1->Update(); 
		} 
	    } 
	} 
	for(i=0; i<Ngene; ++i){  
	    meanp[i] /= nPop; 
	    double val = genome.gene(i); 
	    val -= meanp[i]; 
	    val *= val; 
	    sigmap[i] += val; 
	} 
	cout << "score (min,max,mean): " << min << " " << max << " " << mean/nPop << endl; 
	cout << "gene\tpar\tmin\tmax\tmean\tsigma" << endl;   
	for(i=0; i<Ngene; ++i){   
	    sigmap[i] /= nPop; 
	    sigmap[i] = sqrt(sigmap[i]); 
	    cout << i << "\t " 
		<< gene2para[i] << "\t" 
		<< minp[i] << "\t" 
		<< maxp[i] << "\t" 
		<< meanp[i] << "\t" 
		<< sigmap[i] << "\t" 
		<< endl; 
	} 
    } 

    //file2->Write(); 

    return 0; 

} 

float Objective(GAGenome& g){ 

    static TH1F* hist = new TH1F("hist","",100,1.97-0.05,1.97+0.05); 
    static TF1* fun = new TF1("fun", "[0]/[2]/sqrt(2.0*pi)*exp(-0.5*((x-[1])/[2])^2)+pol0(3)"); 

    TCut cut(getCut(g)); 

    tree->Project("hist", "fdsmass", cut ); 

    Double_t min = hist->GetMinimum(); 
    Double_t max = hist->GetMaximum(); 
    Double_t width = hist->GetBinWidth(1); 

    fun->SetParameters( (max-min)*0.5*0.005*6.0, 1.97, 0.005, min); 

    hist->Fit("fun","q"); 

    hist->Draw(); 
    c1->Update(); 

    Float_t back = fun->GetParameter(3); 
    Float_t N = fun->GetParameter(0); 
    Float_t sigma = fun->GetParameter(2); 

    Float_t Nside = back * sigma * 4.0 / width; 
    Float_t Nsignal = N * 0.9544 / width; 
    Nsignal += Nside; 

    Float_t dilu=0.0;  
    if(Nsignal>Nside && Nside>0){ 
	Nsignal -= Nside; 
	Float_t ss = Nsignal; 
	Float_t bb = Nside; 

	if(dilustr == "tight") 
	    dilu = ss/(ss+bb)*ss/(ss+bb)*ss; 
	else 
	    if(dilustr == "loose") 
		dilu = ss/(ss+bb)*ss; 
	    else{ 
		const Float_t sn = atof(dilustr.Data()); 
		if( ss/bb > sn ) 
		    dilu = Nsignal; 
	    } 
    } 
    Gsignal = Nsignal; 
    Gside = Nside; 

    return dilu; 
}  

// setup the cut 
const char* getCut(GAGenome &g){ 

    static TString cut =""; 
    cut = rawcut; 
    GARealGenome& genome = (GARealGenome &)g; 
    for(Int_t i=0; i<Ngene ; ++i){ 
	TString parstr = "["; 
	parstr += gene2para[i]; 
	parstr += "]"; 
	Float_t val = genome.gene(i); 
	TString valstr = ""; 
	if(para[gene2para[i]].type == 'i') 
	    valstr += int(val); 
	else{ 
	    char ch[50]=""; 
	    sprintf( ch, "%g", val); 
	    valstr = TString(ch); 
	} 
	cut.ReplaceAll( parstr, valstr); 
    } 

    return cut.Data(); 
} 


// calculates the normalized distance of two genomes 
// necessary for population diversity calculation and GASharing-scaling 
float Comparator(const GAGenome& g1, const GAGenome& g2) { 
    GARealGenome& a = (GARealGenome &)g1; 
    GARealGenome& b = (GARealGenome &)g2; 

    Float_t dist=0; 
    int ngene = a.length(); 
    for(Int_t i=0; i<ngene ; ++i){ 
	Float_t tmp = (a.gene(i)-b.gene(i)) / ( a.alleleset(i).upper() -  a.alleleset(i).lower()); 
	dist += tmp*tmp; 
    } 
    dist /= ngene; 
    dist = sqrt(dist); 
    //dist = pow(dist,0.1); 

    return dist; 
} 





//taken from GARealGenome.C, sigma of the gaussian adjusted to the limits of the according allele  
int KaniggelsRealGaussianMutator(GAGenome& g, float pmut){ 

    static TRandom randomizer(0); 
    static TF1 mutfun("mutfun","[0]/(x^2+[0]^2)",-0.5,0.5); 
    static Bool_t first = kTRUE; 
    if(first){ 
	mutfun.SetParameters(mutSigma,0); 
	first = kFALSE; 
    } 

    GA1DArrayAlleleGenome<float> &child= 
	DYN_CAST(GA1DArrayAlleleGenome<float> &, g);  
    int n=0; 
    int i=0; 

    if(pmut <= 0.0) return(0); 

    float nMut = pmut * (float)(child.length()); 
    int length = child.length()-1; 
    if(nMut < 1.0){		// we have to do a flip test on each element 
	nMut = 0; 
	for(i=length; i>=0; i--){ 
	    float value = child.gene(i); 
	    if(GAFlipCoin(pmut)){ 
		if(child.alleleset(i).type() == GAAllele::ENUMERATED || 
		    child.alleleset(i).type() == GAAllele::DISCRETIZED) 
		    value = child.alleleset(i).allele(); 
		else if(child.alleleset(i).type() == GAAllele::BOUNDED){ 
		    //value += GAUnitGaussian(); 
		    float min = child.alleleset(i).lower() ; 
		    float max = child.alleleset(i).upper() ; 
		    //value += randomizer.Gaus( 0.0f, mutSigma*(max-min));  
		    value += mutfun.GetRandom()*(max-min); 
		    //cout << mutfun.GetRandom() << endl; 
		    value = GAMax(min,value); 
		    value = GAMin(max,value); 
		} 
		child.gene(i, value); 
		nMut++; 
	    } 
	} 
    } 
    else
    {	// only mutate the ones we need to 
	for(n=0; n<nMut; n++){ 
	    int idx = GARandomInt(0,length); 
	    float value = child.gene(idx); 
	    if(child.alleleset(idx).type() == GAAllele::ENUMERATED || 
		child.alleleset(idx).type() == GAAllele::DISCRETIZED) 
		value = child.alleleset(idx).allele(); 
	    else if(child.alleleset(idx).type() == GAAllele::BOUNDED){ 
		//value += GAUnitGaussian(); 
		float min = child.alleleset(idx).lower() ; 
		float max = child.alleleset(idx).upper() ; 
		//value += randomizer.Gaus( 0.0f, mutSigma*(max-min));  
		value += mutfun.GetRandom()*(max-min);  
		//cout << mutfun.GetRandom() << endl; 
		value = GAMax(min,value); 
		value = GAMin(max,value); 
	    } 
	    child.gene(idx, value); 
	} 
    } 
    return((int)nMut); 
} 



void  KaniggelsRealInitializer(GAGenome & c){ 

    GA1DArrayAlleleGenome<float> &child = DYN_CAST(GA1DArrayAlleleGenome<float> &, c); 
    child.resize(GAGenome::ANY_SIZE); // let chrom resize if it can 
    int countdown = 300; 
    do{ 
	for(int i=child.length()-1; i>=0; i--) 
	    child.gene(i, child.alleleset(i).allele()); 
	cout << "." << flush; 
    }while( child.score() == 0.0 && countdown-->0); 
    cout << endl; 

} 



// taken from GARealGenome.C 
// bugfix: Now the children's genes are always within their limits 
// functionality for enumerated alleles added (it's a hack)   
int KaniggelsRealBlendCrossover(const GAGenome& p1, const GAGenome& p2, 
				GAGenome* c1, GAGenome* c2) 
{ 

    const GA1DArrayAlleleGenome<float> &mom= 
	DYN_CAST(const GA1DArrayAlleleGenome<float> &, p1); 
    const GA1DArrayAlleleGenome<float> &dad= 
	DYN_CAST(const GA1DArrayAlleleGenome<float> &, p2); 



    int n=0; 

    if(c1 && c2){ 
	GA1DArrayAlleleGenome<float> &sis=DYN_CAST(GA1DArrayAlleleGenome<float> &, *c1); 
	GA1DArrayAlleleGenome<float> &bro=DYN_CAST(GA1DArrayAlleleGenome<float> &, *c2); 

	int len = GAMax(mom.length(), dad.length()); 
	for(int i=0; i<len; i++) { 
	    float dist = 0; 
	    if(mom.gene(i) > dad.gene(i))  
		dist = mom.gene(i) - dad.gene(i); 
	    else  
		dist = dad.gene(i) - mom.gene(i); 
	    float lo = (GAMin(mom.gene(i), dad.gene(i)));// - 0.5*dist; 
	    float hi = (GAMax(mom.gene(i), dad.gene(i)));// + 0.5*dist; 
	    //----- 

	    float min = dad.alleleset(i).lower(); 
	    float max = dad.alleleset(i).upper(); 
	    float value = GARandomFloat(lo, hi); 
	    value = GAMax(min,value); 
	    value = GAMin(max,value); 

	    if(sis.alleleset(i).type() == GAAllele::ENUMERATED || 
		sis.alleleset(i).type() == GAAllele::DISCRETIZED) 
		sis.gene( i, float(TMath::Nint(value))); 
	    else  
		if(sis.alleleset(i).type() == GAAllele::BOUNDED) 
		    sis.gene( i, value); 

	    value = GARandomFloat(lo, hi); 
	    value = GAMax(min,value); 
	    value = GAMin(max,value); 

	    if(bro.alleleset(i).type() == GAAllele::ENUMERATED || 
		bro.alleleset(i).type() == GAAllele::DISCRETIZED) 
		bro.gene( i, float(TMath::Nint(value))); 
	    else  
		if(bro.alleleset(i).type() == GAAllele::BOUNDED) 
		    bro.gene( i, value); 

	    //----- 

	    //sis.gene(i, GARandomFloat(lo, hi)); 
	    //bro.gene(i, GARandomFloat(lo, hi)); 
	} 
	n = 2; 
    } 
    else if(c1 || c2){ 
	GA1DArrayAlleleGenome<float> &sis = (c1 ? 
	    DYN_CAST(GA1DArrayAlleleGenome<float> &, *c1) : 
	DYN_CAST(GA1DArrayAlleleGenome<float> &, *c2)); 

	int len = GAMax(mom.length(), dad.length()); 
	for(int i=0; i<len; i++) { 
	    float dist = 0; 
	    if(mom.gene(i) > dad.gene(i))  
		dist = mom.gene(i) - dad.gene(i); 
	    else  
		dist = dad.gene(i) - mom.gene(i); 
	    float lo = (GAMin(mom.gene(i), dad.gene(i)));// - 0.5*dist; 
	    float hi = (GAMax(mom.gene(i), dad.gene(i)));// + 0.5*dist; 

	    //------- 

	    float min = dad.alleleset(i).lower(); 
	    float max = dad.alleleset(i).upper(); 
	    float value = GARandomFloat(lo, hi); 
	    value = GAMax(min,value); 
	    value = GAMin(max,value); 

	    if(sis.alleleset(i).type() == GAAllele::ENUMERATED || 
		sis.alleleset(i).type() == GAAllele::DISCRETIZED) 
		sis.gene( i, float(TMath::Nint(value))); 
	    else  
		if(sis.alleleset(i).type() == GAAllele::BOUNDED) 
		    sis.gene( i, value); 

	    //------- 
	    //sis.gene(i, GARandomFloat(lo, hi)); 
	} 
	n = 1; 
    } 

    return n; 
} 
