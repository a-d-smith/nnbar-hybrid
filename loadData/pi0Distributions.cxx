#ifndef LARLITE_PI0DISTRIBUTIONS_CXX
#define LARLITE_PI0DISTRIBUTIONS_CXX

#include "pi0Distributions.h"
#include <cmath>

using namespace std;

namespace larlite {

  bool pi0Distributions::initialize() {

		nt = new TNtuple("pi0Data","pi0Data","M:D:Easym:Efrac:Emin:costheta:nContained:pi0Parent");
						
		// The TPC (for microboone) has 6 bounding planes with normals given by:
		nhat.push_back(TVector3( 0, 0, 1 ));
		nhat.push_back(TVector3( 0, 0,-1 ));
		nhat.push_back(TVector3( 0, 1, 0 ));
		nhat.push_back(TVector3( 0,-1, 0 ));
		nhat.push_back(TVector3( 1, 0, 0 ));
		nhat.push_back(TVector3(-1, 0, 0 ));

		// And points in the plane
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
						int pi0Parent;
						if (shi.mcParentID == shj.mcParentID && shi.mcParentPDG == 111 && shj.mcParentPDG == 111 && shi.mcPDG == 22 && shj.mcPDG == 22){
							// shi & shj are the decay products of a pi0
							pi0Parent = 1;
						}
						else{
							pi0Parent = 0;
						}

						// We want to calculate some stuff about these photons, 
						// their invariant mass, M, their impact parameter, D
						// and their energy asymmetry

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

						// Energy asymmetry
						double Easym = abs(shi.KE - shj.KE)/abs(shi.KE + shj.KE);

						// Minimum energy and fractional energy
						double Emin = min(shi.KE, shj.KE);
						double Efrac = Emin / max(shi.KE, shj.KE);
						
						// Flag the number of fully contained showers 
						double L1, L2;
						bool cont1 = true;
	          bool cont2 = true;

	          for (int k = 0; k < nhat.size(); k++){
	            L1 =  ((r0[k] - shi.Start).Dot(nhat[k]))/shi.Dir.Dot(nhat[k]);
	            L2 =  ((r0[k] - shj.Start).Dot(nhat[k]))/shj.Dir.Dot(nhat[k]);

	            if (L1 < shi.L && L1 > 0){ cont1 = false;}
	  	        if (L2 < shj.L && L2 > 0){ cont2 = false;}
			      }

	          if (cont1 && cont2){
	            nt->Fill(M, D, Easym, Efrac, Emin, cosTheta, 2, pi0Parent);
	          }
	          else if (!cont1 && !cont2){
	            nt->Fill(M, D, Easym, Efrac, Emin, cosTheta, 0, pi0Parent);
	          }
	          else{
	            nt->Fill(M, D, Easym, Efrac, Emin, cosTheta, 1, pi0Parent);
	          }
					}
					j++;
				}
				i++;
			}
		}	

		TFile *f;
		f = new TFile("OUTPUTpi0Distributions.root", "NEW");
		nt->Write(); 
		delete f;

		return true;
  }
  
  bool pi0Distributions::analyze(storage_manager* storage) {
  
    return true;
  }

  bool pi0Distributions::finalize() {

    return true;
  }

}
#endif
