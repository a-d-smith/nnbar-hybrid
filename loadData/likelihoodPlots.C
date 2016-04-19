#include "TFile.h"
#include "TNtuple.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TCanvas.h"
#include "TH1D.h"
#include <vector>
#include <iostream>
#include "string.h"

using namespace std;

void likelihoodPlots(){
	// This function will produce plots of the likelihood of a candidate pair of photons 
	// being due to a pi0 as a function of various metrics
	
	// Load in the file
	TFile f("OUTPUTpi0Distributions.root");
	TTree *pi0Data = (TTree*)f.Get("pi0Data");

	TLeaf *M_leaf          = pi0Data->GetLeaf("M");
	TLeaf *D_leaf          = pi0Data->GetLeaf("D");
	TLeaf *Easym_leaf      = pi0Data->GetLeaf("Easym");
	TLeaf *Emin_leaf       = pi0Data->GetLeaf("Emin");
	TLeaf *Efrac_leaf      = pi0Data->GetLeaf("Efrac");
	TLeaf *costheta_leaf   = pi0Data->GetLeaf("costheta");
	TLeaf *nContained_leaf = pi0Data->GetLeaf("nContained");
	TLeaf *pi0Parent_leaf  = pi0Data->GetLeaf("pi0Parent");

	// Give each metric a name for axis labels
	vector<string> metricName;
	metricName.push_back("M");
	metricName.push_back("ln(D)");
	metricName.push_back("E_{asym}");
	metricName.push_back("E_{min}");
	metricName.push_back("E_{frac}");
	metricName.push_back("cos(theta)");

	// Set the limits for each of the metrics
	vector<double> minVal;
	minVal.push_back(0);
	minVal.push_back(-12);
	minVal.push_back(0);
	minVal.push_back(0);
	minVal.push_back(0);
	minVal.push_back(-1);

	vector<double> maxVal;
	maxVal.push_back(540);
	maxVal.push_back(6);
	maxVal.push_back(1);
	maxVal.push_back(600);
	maxVal.push_back(1);
	maxVal.push_back(1);

	// Make a histogram for each of the metrics
	//vector<TH1D*>	hi;
	int i = 0;
	for (string name : metricName){
		TH1D *h = new TH1D("h", name, 100, minVal[i], maxVal[i]);
		//hi.push_back(h);
		i++;
	}

	/*
	int N = pi0Data->GetEntries();	
	// Loop over each of the entries
	for (int i=0; i<N; i++){
		pi0Data->GetEntry(i);
		// 
	}*/

}
