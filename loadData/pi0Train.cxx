#ifndef LARLITE_PI0TRAIN_CXX
#define LARLITE_PI0TRAIN_CXX

#include "pi0Train.h"
#include "loadData.h"
#include <vector>
#include <iostream>
#include "TNtuple.h"
#include "TMath.h"

using namespace std;

namespace larlite {
	
	double likelihood_M(double _M){		
		double A = 0.7441;
		double mu = 130.4;
		double sigma = 27.3;

		if (_M >= 0 && _M <= 540){
			return A * exp(-pow((_M - mu),2) / (2*pow(sigma,2)));
		}
		else{
			return 0;
		}
	}

	double likelihood_lnD(double _lnD){
		double A = 2.762;
		double MPV = -2.936;
		double sigma = 1.763;
		
		if (_lnD >= -12 && _lnD <= 6){
			return TMath::Landau(_lnD, MPV, sigma, kFALSE);
		}
		else{
			return 0;
		}
	}

	double likelihood_Emin(double _Emin){
		if (_Emin >= 0 && _Emin <= 600){
			if (_Emin < 60){
				return 0.2;
			}
			else{
				return 0.4;
			}
		}
		else{
			return 0;
		}
	}

	double likelihood_cosTheta(double _cosTheta){
		double a,b,c,d,e,f,g,h,i,j;
		a = 0.2395;
		b = 0.2880;
		c = 0.3207;
		d = -1.286;
		e = -1.637;
		f = 4.427;
		g = 4.373;
		h = -3.950;
		i = -3.232;
		j = -0.4434;
		
		double pol = a + b*_cosTheta + c*pow(_cosTheta,2)+ d*pow(_cosTheta,3)+ e*pow(_cosTheta,4)+ f*pow(_cosTheta,5)+ g*pow(_cosTheta,6)+ h*pow(_cosTheta,7)+ i*pow(_cosTheta,8)+ j*pow(_cosTheta,9);
		if (_cosTheta >= -1 && _cosTheta <= 0.93){
			if ( pol <= 0){
				cout << " cosTheta = " << _cosTheta << "  | pol = " << pol << endl;
			}	
			return pol;
		}
		else{
			return 0;
		}
	}

	double likelihood(double _M, double _lnD, double _Emin, double _cosTheta){
		return ( likelihood_M(_M) * likelihood_lnD(_lnD) * likelihood_Emin(_Emin) * likelihood_cosTheta(_cosTheta) );
	}
	
	double logL(double _M, double _lnD, double _Emin, double _cosTheta){
		return ( log(likelihood_M(_M)) + log(likelihood_lnD(_lnD)) + log(likelihood_Emin(_Emin)) + log(likelihood_cosTheta(_cosTheta)));
	}

  bool pi0Train::initialize() {

		vector<candidate> pairs;

		TNtuple *nt;
		nt = new TNtuple("pi0Data","pi0Data","l:lnL:pi0Parent");

		// The TPC (for microboone) has 6 bounding planes with normals given by:
		vector<TVector3> nhat;
		nhat.push_back(TVector3( 0, 0, 1 ));
		nhat.push_back(TVector3( 0, 0,-1 ));
		nhat.push_back(TVector3( 0, 1, 0 ));
		nhat.push_back(TVector3( 0,-1, 0 ));
		nhat.push_back(TVector3( 1, 0, 0 ));
		nhat.push_back(TVector3(-1, 0, 0 ));

		// And points in the plane
		vector<TVector3> r0;
		r0.push_back(TVector3( 0 ,   0 , 1036));
		r0.push_back(TVector3( 0 ,   0 ,   0 ));
		r0.push_back(TVector3( 0 ,  115,   0 ));
		r0.push_back(TVector3( 0 , -115,   0 ));
		r0.push_back(TVector3(256,   0 ,   0 ));
		r0.push_back(TVector3( 0 ,   0 ,   0 ));
   
		loadData dat("nnbarData.root");
		auto evList = dat.evList;

		// For each event
		for (auto evnt : evList){
			auto shList = evnt.shList;
			// Loop over the showers in pairs
			int i = 0;
			for (auto shi : shList){
				int j = 0;
				for (auto shj : shList){
					// Avoid double counting
					if (i > j){
						// We want to calculate some stuff about these photons, 
						// their invariant mass, M, their impact parameter, D
						// and their energy asymmetry etc...

						// Invariant mass
						double cosTheta = shi.Dir.Dot(shj.Dir)/(shi.Dir.Mag()*shj.Dir.Mag());
						double M = pow( 2 * shi.KE * shj.KE * (1 - cosTheta), 0.5);
					
						// Distance of closest approach
						TVector3 w0 = shi.Start - shj.Start;
	          double a = shi.Dir.Dot(shi.Dir);
	          double b = shi.Dir.Dot(shj.Dir);
	          double c = shj.Dir.Dot(shj.Dir);
	          double d = shi.Dir.Dot(w0);
	          double e = shj.Dir.Dot(w0);
	
	          TVector3 DVect = w0 + ((b*e - c*d)/(a*c - b*b))*shi.Dir - ((a*e - b*d)/(a*c - b*b))*shj.Dir;
	          double D = DVect.Mag();

						// Minimum energy
						double Emin = min(shi.KE, shj.KE);
						
						// Check to see if the showers are contained
						double L1, L2;
						bool cont1 = true;
	          bool cont2 = true;

	          for (int k = 0; k < nhat.size(); k++){
	            L1 =  ((r0[k] - shi.Start).Dot(nhat[k]))/shi.Dir.Dot(nhat[k]);
	            L2 =  ((r0[k] - shj.Start).Dot(nhat[k]))/shj.Dir.Dot(nhat[k]);

	            if (L1 < shi.L && L1 > 0){ cont1 = false;}
	  	        if (L2 < shj.L && L2 > 0){ cont2 = false;}
			      }

						// Both showers are fully contained 
	          if (cont1 && cont2){
							// Check using AlgoEMpart info if the showers are photons
							if (shi.type == 1 && shj.type == 1){

								// Now we have used all the information we have (that we are allowed to use with real data)
								// From these values calculate the likelihood from the trained distributions

								// Calculate the likelihood for these metrics
								double l = likelihood(M, log(D), Emin, cosTheta);
								double lnL = logL(M, log(D), Emin, cosTheta);

								candidate pair;
								pair.lnL = lnL;	
								// Check to see if this is pair is due to a pi0
								if (shi.mcParentID == shj.mcParentID && shi.mcParentPDG == 111 && shj.mcParentPDG == 111 && shi.mcPDG == 22 && shj.mcPDG == 22){
									nt->Fill(l, lnL, 1);	
									pair.good = true;
								}
								else{
									nt->Fill(l, lnL, 0);	
									pair.good = false;
								}

								pairs.push_back(pair);	
							}
									
	          }
					}
					j++;
				}
				i++;
			}
		}

	
		TFile *f;
		f = new TFile("OUTPUTpi0Train.root", "RECREATE");
		nt->Write(); 
		delete f;

		// Here we try to determine the optimum cut value on the log-likelihood
		// We select a cut value, LLcut, and choose all candidates with:
		//     logL > LLcut
		// as being due to a Pi0. We the determine how good the cut is by calculating
		// the efficiency of the method, eff, and the purity, P, defined as:
		// 		eff = #(Pi0 passing cut) / #(total Pi0)
		// 		P   = #(Pi0 passing cut) / (#(Pi0 passing cut) + #(non-Pi0 passing cut))
		// The optimum cut maximises both eff and P, hence the quantity eff*P, is 
		// the measure used for cut quality.

		// Choose an initial guess for the cut value
		double LLcut = -6;
		double LLcutTemp;
		
		// Choose an impossible metric to be maximised
		double m = -1;

		int SS; // #Signal selected (signal == pi0)
		int SR; // #Signal rejected
		int BS; // #Background selected (background == non-pi0)
		int BR; // #Background rejected

		// Define the efficiency and purity variables
		double eff, effTemp;
		double P, PTemp;

		// Set the step size for chancing LLcut
		double minStep = 0.01;
		double maxStep = 1;
		double step;

		int chosenStep;

		bool keepGoing = true;
		while (keepGoing){
			
			// Assume the current LLcut is the best
			chosenStep = 0;

			// Start at the smallest step
			step = minStep;

			// Keep checking and increasing the step size until 
			// either we improve or reach the max step size
			while (chosenStep == 0 && step <= maxStep){
				// Check before and after the current LLcut
				for (int i=-1; i<=1; i+=2){
					
					// Calculate the LLcut for this step
					LLcutTemp = LLcut + i*step;
	
					// Reset the counters 		
					SS = 0;
					SR = 0;
					BS = 0;
					BR = 0;
			
					// Calculate the counters for the new cut value
					for (candidate pair : pairs){
						if (pair.lnL > LLcutTemp){
							// Selected
							if (pair.good){
								// Signal
								SS++;
							}
							else{
								// Background
								BS++;
							}
						}
						else{
							// Rejected
							if (pair.good){
								// Signal
								SR++;
							}
							else{
								// Background
								BR++;
							}
						}
					}
			
					// Calculate the efficiency & purity
					effTemp = double(SS) / double(SS + SR);
					PTemp   = double(SS) / double(SS + BS);
		
					// Check to see if LLcutTemp is better than LLcut			
					if (effTemp * PTemp > m){
						// Save these values
						eff = effTemp;
						P   = PTemp;
						m   = eff * P;
						
						// Remember to perform the step in this direction
						chosenStep = i;	
					}
				}

				// If we didn't improve, then increase the step size
				if (chosenStep == 0){
					step += minStep;
				}
			}

			// Check to see if a step should be performed
			if (chosenStep == 0){
				keepGoing = false;
			}
			else{
				LLcut += chosenStep * step;
				//cout << "LLcut -> " << chosenStep * step << endl;
				//cout << "LLcut = "  << LLcut << "  |  m = " << m << endl;
			}
		}
	
		cout << "Optimum cut on the log-likelihood was found" << endl;
		cout << "  if (logL > " << LLcut << ") -> Select as Pi0" << endl;
		cout << "  with this cut: efficiency = " << eff << endl;
		cout << "                 purity     = " << P << endl;

  	return true;
  }
  
  bool pi0Train::analyze(storage_manager* storage) {
    return true;
  }

  bool pi0Train::finalize() {
    return true;
  }

}
#endif
