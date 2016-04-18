#ifndef LARLITE_LOADDATA_CXX
#define LARLITE_LOADDATA_CXX

#include "loadData.h"

namespace larlite {

  bool loadData::initialize() {
		// Using this section as the normal cpp int main() area 
		
		tracks  = (TTree*)f->Get("track");
		showers = (TTree*)f->Get("shower");
		
		tr_event = tracks->GetLeaf("event");
		tr_KE = tracks->GetLeaf("KE");
		tr_PID = tracks->GetLeaf("PID");
		tr_StartX = tracks->GetLeaf("StartX");
		tr_StartY = tracks->GetLeaf("StartY");
		tr_StartZ = tracks->GetLeaf("StartZ");
		tr_EndX = tracks->GetLeaf("EndX");
		tr_EndY = tracks->GetLeaf("EndY");
		tr_EndZ = tracks->GetLeaf("EndZ");
		tr_Time = tracks->GetLeaf("Time");
		tr_MC_PDG = tracks->GetLeaf("MC_PDG");
		tr_MC_isPrimary = tracks->GetLeaf("MC_isPrimary");
		tr_MC_ParentPDG = tracks->GetLeaf("MC_ParentPDG");
		tr_MC_ID = tracks->GetLeaf("MC_ID");
		tr_MC_ParentID = tracks->GetLeaf("MC_ParentID");
		
		sh_event = showers->GetLeaf("event");
		sh_KE = showers->GetLeaf("KE");
		sh_type = showers->GetLeaf("type");
		sh_StartX = showers->GetLeaf("StartX");
		sh_StartY = showers->GetLeaf("StartY");
		sh_StartZ = showers->GetLeaf("StartZ");
		sh_DirX = showers->GetLeaf("DirX");
		sh_DirY = showers->GetLeaf("DirY");
		sh_DirZ = showers->GetLeaf("DirZ");
		sh_L = showers->GetLeaf("L");
		sh_MC_PDG = showers->GetLeaf("MC_PDG");
		sh_MC_isPrimary = showers->GetLeaf("MC_isPrimary");
		sh_MC_ParentPDG = showers->GetLeaf("MC_ParentPDG");
		sh_MC_ID = showers->GetLeaf("MC_ID");
		sh_MC_ParentID = showers->GetLeaf("MC_ParentID");
	
		// Some awesome method of event filling
		int mT = tracks->GetEntries();
		int mS = showers->GetEntries();
		int N = -1;
		int eT = 0;
		int eS = 0;
		bool trDone = false;
		bool shDone = false;
		bool finished = false;
		vector<track> tempT;
		vector<shower> tempS;
		int nT, nS;
	
		// Run these 4 lines everytime we update nT and nS
		tracks->GetEntry(eT);
		showers->GetEntry(eS);
		nT = tr_event->GetValue(0);	
		nS = sh_event->GetValue(0);	
	
		// Run the process
		while(!finished){
			// Keep increasing the entry number for tracks until the event number changes
			// -> all tracks in the event have been found.
			bool trEvDone = false;
		
			while(!trEvDone){
				if(nT == N && !trDone){
					// Make the track object and add it to the temp vector
					track tr;
					tr.KE = tr_KE->GetValue(0);
					tr.PID = tr_PID->GetValue(0);
					tr.Start = TVector3(tr_StartX->GetValue(0), tr_StartY->GetValue(0), tr_StartZ->GetValue(0));
					tr.End = TVector3(tr_EndX->GetValue(0), tr_EndY->GetValue(0), tr_EndZ->GetValue(0));
					tr.T = tr_Time->GetValue(0);
					tr.mcPDG = tr_MC_PDG->GetValue(0);
					tr.mcPrimary = tr_MC_isPrimary->GetValue(0);
					tr.mcParentPDG = tr_MC_ParentPDG->GetValue(0);
					tr.mcID = tr_MC_ID->GetValue(0);
					tr.mcParentID = tr_MC_ParentID->GetValue(0);
						
					tempT.push_back(tr);				
	
					if (eT < mT - 1){
						eT++;
						tracks->GetEntry(eT);
						nT = tr_event->GetValue(0);
					}
					else{
						trDone = true;
					}
				}	
				else{
					trEvDone = true;
				}
			}
			// Once the tracks for the current value of N have been checked: 
			// Keep increasing the entry number for showers until the event number changes
			// -> all showers in the event have been found.
			bool shEvDone = false;
	
			while(!shEvDone){
			if(nS == N && !shDone){
					// Make the shower object and add it to the temp vector
					shower sh;
					sh.KE = sh_KE->GetValue(0);
					sh.type = sh_type->GetValue(0);
					sh.Start = TVector3(sh_StartX->GetValue(0), sh_StartY->GetValue(0), sh_StartZ->GetValue(0));
					sh.Dir = TVector3(sh_DirX->GetValue(0), sh_DirY->GetValue(0), sh_DirZ->GetValue(0));
					sh.L = sh_L->GetValue(0);
					sh.mcPDG = sh_MC_PDG->GetValue(0);
					sh.mcPrimary = sh_MC_isPrimary->GetValue(0);
					sh.mcParentPDG = sh_MC_ParentPDG->GetValue(0);
					sh.mcID = sh_MC_ID->GetValue(0);
					sh.mcParentID = sh_MC_ParentID->GetValue(0);
						
					// ...
					tempS.push_back(sh);
				
					if (eS < mS - 1){
						eS++;
						showers->GetEntry(eS);
						nS = sh_event->GetValue(0);
					}
					else{
						shDone = true;
					}
				}	
				else{
					shEvDone = true;
				}
			}
			// If we have found something, add it to the event list
			if(tempT.size() != 0 || tempS.size() != 0){
				ev evnt;
				evnt.trList = tempT;
				evnt.shList = tempS;
				evList.push_back(evnt);
	
				tempT.clear();
				tempS.clear();
			}
			
			// Now we decide whether we should increase N
			if(nT > N || nS > N){
				N++;
			}
			else{
				if(trDone && shDone){
					finished = true;
				}
				else{
					N = -1;
				}
			}
		}	
	
		delete f;
		
		cout << "Loaded " << evList.size() << " events." << endl << endl;
	
    return true;
  }
  
  bool loadData::analyze(storage_manager* storage) {
  
    // Doing nothing in here since we are just using LArLite 
    // as a compiler to get the code working
  
    return true;
  }

  bool loadData::finalize() {

    return true;
  }

}
#endif
