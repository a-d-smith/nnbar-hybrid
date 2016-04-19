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

void pi0LikelihoodPlots(){
	// This function will produce plots of the likelihood of a candidate pair of photons 
	// being due to a pi0 as a function of various metrics

	// Load in the file
	TFile f("OUTPUTpi0Distributions.root");
	TTree *pi0Data = (TTree*)f.Get("pi0Data");

	TLeaf *M_leaf          = pi0Data->GetLeaf("M");
	TLeaf *lnD_leaf        = pi0Data->GetLeaf("lnD");
	TLeaf *Easym_leaf      = pi0Data->GetLeaf("Easym");
	TLeaf *Emin_leaf       = pi0Data->GetLeaf("Emin");
	TLeaf *Efrac_leaf      = pi0Data->GetLeaf("Efrac");
	TLeaf *costheta_leaf   = pi0Data->GetLeaf("costheta");
	TLeaf *nContained_leaf = pi0Data->GetLeaf("nContained");
	TLeaf *pi0Parent_leaf  = pi0Data->GetLeaf("pi0Parent");

	// Make a root file to hold the output histograms
	TFile fOut("pi0LikelihoodHisto.root","RECREATE");

	// Give each metric a name for axis labels
	char *metricName[6];
	metricName[0] = "M";
	metricName[1] = "log(D)";
	metricName[2] = "Easym";
	metricName[3] = "Emin";
	metricName[4] = "Efrac";
	metricName[5] = "cos(theta)";

	// Set the limits for each of the metrics
	double minVal[6];
	minVal[0] =  0;
	minVal[1] = -12;
	minVal[2] =  0;
	minVal[3] =  0;
	minVal[4] =  0;
	minVal[5] = -1;

	double maxVal[6];
	maxVal[0] = 540;
	maxVal[1] = 6;
	maxVal[2] = 1;
	maxVal[3] = 600;
	maxVal[4] = 1;
	maxVal[5] = 1;

	int N = pi0Data->GetEntries();
	
	// For each metric
	for (int i = 0; i < 6; i++){
		cout << "Processing " << metricName[i] << "..." << endl;

		// Prefix the names for each histogram
		char histName_S[80];
		strcpy(histName_S, "h_");
		strcat(histName_S, metricName[i]);
		strcat(histName_S, "_S");

		char histName_B[80];
		strcpy(histName_B, "h_");
		strcat(histName_B, metricName[i]);
		strcat(histName_B, "_B");

		char histName_T[80];
		strcpy(histName_T, "h_");
		strcat(histName_T, metricName[i]);
		strcat(histName_T, "_T");

		char histName_L[80];
		strcpy(histName_L, "h_");
		strcat(histName_L, metricName[i]);
		strcat(histName_L, "_L");

		// Set the histogram titles
		char name_S[80];
		strcpy(name_S, metricName[i]);
		strcat(name_S, " - Pi0 (signal)");

		char name_B[80];
		strcpy(name_B, metricName[i]);
		strcat(name_B, " - non-Pi0 (background)");

		char name_T[80];
		strcpy(name_T, metricName[i]);
		strcat(name_T, " - total (signal + background)");

		char name_L[80];
		strcpy(name_L, metricName[i]);
		strcat(name_L, " - likelihood");

		// Make the histograms
		TH1D *hi_S = new TH1D(histName_S, name_S, 100, minVal[i], maxVal[i]);  // Signal
		TH1D *hi_B = new TH1D(histName_B, name_B, 100, minVal[i], maxVal[i]);  // Background
		TH1D *hi_T = new TH1D(histName_T, name_T, 100, minVal[i], maxVal[i]);  // Total
		TH1D *hi_L = new TH1D(histName_L, name_L, 100, minVal[i], maxVal[i]);  // Likelihood

		// Loop over each of the entries
		for (int ent=0; ent<N; ent++){
			pi0Data->GetEntry(ent);

			// Drop any pair that for which either or both showers leaves the TPC
			if ( int(nContained_leaf->GetValue(0)) == 2){
				// Determine which metric we are talking about
				double metric;
				switch (i){
					case 0:
						metric = M_leaf->GetValue(0);
						break;
					case 1:
						metric = lnD_leaf->GetValue(0);
						break;
					case 2:
						metric = Easym_leaf->GetValue(0);
						break;
					case 3:
						metric = Emin_leaf->GetValue(0);
						break;
					case 4:
						metric = Efrac_leaf->GetValue(0);
						break;
					case 5:
						metric = costheta_leaf->GetValue(0);
						break;
					default:
						break;
				}

				// Fill the signal / background histogram accordigly
				if (int(pi0Parent_leaf->GetValue(0)) == 1){
					hi_S->Fill(metric);
				}
				else{
					hi_B->Fill(metric);
				}
			}
		}
		
		// Now calcualte the likelihood histogram from S & B
		// L = S / (S + B)
		// And Total T = S + B

		// Total histogram
		hi_T->Add(hi_S, 1);
		hi_T->Add(hi_B, 1);

		// Likelihood histogram
		hi_L->Add(hi_S, 1);
		hi_L->Divide(hi_T);
		
		// Save the histograms
		hi_S->Write();
		hi_B->Write();
		hi_T->Write();
		hi_L->Write();

		delete hi_S;
		delete hi_B;
		delete hi_T;
		delete hi_L;
	}
}
