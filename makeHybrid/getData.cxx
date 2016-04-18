#ifndef LARLITE_GETDATA_CXX
#define LARLITE_GETDATA_CXX

#include "getData.h"

namespace larlite {

  bool getData::initialize() {
		f = new TFile("ERAnaData.root");
	
		if (!(f->IsOpen())){
			std::cerr << "Error: Could not open root file from ERAna stage" << std::endl;
			exit(1);
		}
	
		track  = (TTree*)f->Get("track");
		shower = (TTree*)f->Get("shower");
		

		tr_event = track->GetLeaf("event");
		sh_event = shower->GetLeaf("event");

		tr_KE = track->GetLeaf("KE");
		tr_PID = track->GetLeaf("PID");
		tr_StartX = track->GetLeaf("StartX");
		tr_StartY = track->GetLeaf("StartY");
		tr_StartZ = track->GetLeaf("StartZ");
		tr_EndX = track->GetLeaf("EndX");
		tr_EndY = track->GetLeaf("EndY");
		tr_EndZ = track->GetLeaf("EndZ");
		tr_Time = track->GetLeaf("Time");

		sh_KE = shower->GetLeaf("KE");
		sh_type = shower->GetLeaf("type");
		sh_StartX = shower->GetLeaf("StartX");
		sh_StartY = shower->GetLeaf("StartY");
		sh_StartZ = shower->GetLeaf("StartZ");
		sh_DirX = shower->GetLeaf("DirX");
		sh_DirY = shower->GetLeaf("DirY");
		sh_DirZ = shower->GetLeaf("DirZ");
		sh_L = shower->GetLeaf("L");
		sh_R = shower->GetLeaf("R");
		sh_Time = shower->GetLeaf("Time");

		tr_entry = 0;
		sh_entry = 0;

		hybridPi0Data = new TFile("hybridPi0Data.root","NEW");	
		ntTrack  = new TNtuple("track" , "track" , "event:KE:PID:StartX:StartY:StartZ:EndX:EndY:EndZ:Time:MC_PDG:MC_isPrimary:MC_ParentPDG:MC_ID:MC_ParentID");
		ntShower = new TNtuple("shower", "shower", "event:KE:type:StartX:StartY:StartZ:DirX:DirY:DirZ:L:MC_PDG:MC_isPrimary:MC_ParentPDG:MC_ID:MC_ParentID");
	

		std::cout << "Loaded " << track->GetEntries() << " tracks" << std::endl;
		std::cout << "       " << shower->GetEntries() << " showers" << std::endl;

		badEvents = 0;

    return true;
  }
  
  bool getData::analyze(storage_manager* storage) {

		std::vector<trackData> trData;
		std::vector<showerData> shData;

		track->GetEntry(tr_entry);
		shower->GetEntry(sh_entry);			

		while(tr_event->GetValue(0) == storage->event_id()){
			// Get the stuff for the track		
			trackData tr;
			tr.KE = tr_KE->GetValue(0);
			tr.PID = tr_PID->GetValue(0);
			tr.StartX = tr_StartX->GetValue(0);
			tr.StartY = tr_StartY->GetValue(0);
			tr.StartZ = tr_StartZ->GetValue(0);
			tr.EndX = tr_EndX->GetValue(0);
			tr.EndY = tr_EndY->GetValue(0);
			tr.EndZ = tr_EndZ->GetValue(0);
			tr.Time = tr_Time->GetValue(0);

			trData.push_back(tr);

			tr_entry++;
			if (tr_entry < track->GetEntries()){
				track->GetEntry(tr_entry);
			}
			else{
				std::cout << "Tracks: Reached end of file" << std::endl;
				break;
			}
		}

		while(sh_event->GetValue(0) == storage->event_id()){
			// Get the stuff for the shower 	
			showerData sh;
			sh.KE = sh_KE->GetValue(0);
			sh.type = sh_type->GetValue(0);
			sh.StartX = sh_StartX->GetValue(0);
			sh.StartY = sh_StartY->GetValue(0);
			sh.StartZ = sh_StartZ->GetValue(0);
			sh.DirX = sh_DirX->GetValue(0);
			sh.DirY = sh_DirY->GetValue(0);
			sh.DirZ = sh_DirZ->GetValue(0);
			sh.L = sh_L->GetValue(0);
			sh.R = sh_R->GetValue(0);
			sh.Time = sh_Time->GetValue(0);

			shData.push_back(sh);

			sh_entry++;
			if (sh_entry < shower->GetEntries()){
				shower->GetEntry(sh_entry);
			}
			else{
				std::cout << "Showers: Reached end of file" << std::endl;
				break;
			}
		}
	
		auto ev_mct = storage->get_data<event_mctrack>("mcreco");
		auto ev_mcs = storage->get_data<event_mcshower>("mcreco");
	
		if (!ev_mct){
			std::cerr << "MCTrack pointer is invalid" << std::endl;
			exit(1);
		}
		if (!ev_mcs){
			std::cerr << "MCShower pointer is invalid" << std::endl;
			exit(1);
		}

		std::vector<larlite::mctrack> mc_trData;
		for (auto &mct : *ev_mct){
			// Drop any that start outside of the cryostat
			if (mct.Start().X() >= 0 && mct.Start().X() <= 256.04 && mct.Start().Y() >= -116.25 && mct.Start().Y() <= 116.25 && mct.Start().Z() >= 0 && mct.Start().Z() <= 1036.8){
				mc_trData.push_back(mct);
			}
		}

		std::vector<larlite::mcshower> mc_shData;
		for (auto &mcs : *ev_mcs){
			// Drop any that start outside of the cryostat
			if (mcs.Start().X() >= 0 && mcs.Start().X() <= 256.04 && mcs.Start().Y() >= -116.25 && mcs.Start().Y() <= 116.25 && mcs.Start().Z() >= 0 && mcs.Start().Z() <= 1036.8){
				mc_shData.push_back(mcs);	
			}
		}

		bool shouldFill = true;	
		if (trData.size() != mc_trData.size()){
			badEvents++;
			shouldFill = false;
		}

		if (shData.size() != mc_shData.size()){
			badEvents++;
			shouldFill = false;
		}
	
		// Now (hopefully) everything should line up nicely
		// We now fill up a new NTuple
		if(shouldFill){
			for(int i = 0; i < trData.size(); i++){
				bool isPrimary = false;
				if (mc_trData[i].TrackID() == mc_trData[i].MotherTrackID()){
					isPrimary = true;
				}
				ntTrack->Fill(storage->event_id(), trData[i].KE, trData[i].PID, trData[i].StartX, trData[i].StartY, trData[i].StartZ, trData[i].EndX, trData[i].EndY, trData[i].EndZ, trData[i].Time, mc_trData[i].PdgCode(), isPrimary, mc_trData[i].MotherPdgCode(), mc_trData[i].TrackID(), mc_trData[i].MotherTrackID());
			}
	
			for(int i = 0; i < shData.size(); i++){
				bool isPrimary = false;
				if (mc_shData[i].TrackID() == mc_shData[i].MotherTrackID()){
					isPrimary = true;
				}
				ntShower->Fill(storage->event_id(), shData[i].KE, shData[i].type, shData[i].StartX, shData[i].StartY, shData[i].StartZ, shData[i].DirX, shData[i].DirY, shData[i].DirZ, shData[i].L, mc_shData[i].PdgCode(), isPrimary, mc_shData[i].MotherPdgCode(), mc_shData[i].TrackID(), mc_shData[i].MotherTrackID());
			}
		}		
    return true;
  }

  bool getData::finalize() {
		ntTrack->Write();
		ntShower->Write();

		std::cout << "The events for which the ERAna matched the Anaunit have been merged and added to hybridData.root" << std::endl;
		std::cout << "A total of " << badEvents << " unmatched events were omitted" << std::endl;
    return true;
  }

}
#endif
