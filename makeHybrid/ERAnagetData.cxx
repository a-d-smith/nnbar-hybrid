#ifndef ERTOOL_ERANAGETDATA_CXX
#define ERTOOL_ERANAGETDATA_CXX

#include "ERAnagetData.h"

namespace ertool {

  ERAnagetData::ERAnagetData(const std::string& name) : AnaBase(name)
  {}

  void ERAnagetData::Reset()
  {}

  void ERAnagetData::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnagetData::ProcessBegin()
  {
		ntTrack = new TNtuple("track","track","event:KE:PID:StartX:StartY:StartZ:EndX:EndY:EndZ:Time");
		ntShower = new TNtuple("shower","shower","event:KE:type:StartX:StartY:StartZ:DirX:DirY:DirZ:L:R:Time");
	}

  bool ERAnagetData::Analyze(const EventData &data, const ParticleGraph &ps)
  {
		auto const &mc_graph = MCParticleGraph();
		event = data.Event_ID();
		
		auto const &partArray    = ps.GetParticleArray();
		auto const &mc_partArray = mc_graph.GetParticleArray();

		int j = 0;
		for(int i=0; i < partArray.size(); i++){
			auto part    = partArray[i];
			auto mc_part = mc_partArray[j];

			// Drop unreconstructed particles from the mc_partArray
			while (mc_part.RecoType() == 0){
				j++;
				if (j < mc_partArray.size()){
					mc_part = mc_partArray[j];
				}
				else{
					std::cerr << "Error: Number of visible particles in the MCParticleGraph < ParticleGraph" << std::endl;
					exit(1);
				}
			}
		
			// Check for shower / track mismatches
			if (part.RecoType() != mc_part.RecoType()){
				std::cerr << "Error: Track / Shower mismatch" << std::endl;
				exit(1);
			}
			
			// Fill the NTuples
			if (part.RecoType() == 2){
				auto const track = data.Track(part);
				ntTrack->Fill(event, track._energy, track._pid, track[0][0], track[0][1], track[0][2], track[track.size()-1][0], track[track.size()-1][1], track[track.size()-1][2], track._time);
			}
			if (part.RecoType() == 1){
				int type;
				auto const shower = data.Shower(part);
				if (mc_part.PdgCode() == 11 || mc_part.PdgCode() == -11){
					type = 0;
				}
				else{
					type = 1;
				}
				ntShower->Fill(event, shower._energy, type, shower.Start()[0], shower.Start()[1], shower.Start()[2], shower.Dir()[0], shower.Dir()[1], shower.Dir()[2], shower.Length(), shower.Radius(), shower._time);
			}


			j++;
		}

		// Check to see if we have any visible particles left in the MCParticleGraph
		if (j < mc_partArray.size()){
			for (int i=j; i < mc_partArray.size(); i++){
				if (mc_partArray[i].RecoType() != 0){
					std::cerr << "Error: Number of visible particles in the MCParticleGraph > ParticleGraph" << std::endl;
					exit(1);
				}
			}	
		}


		return true; 
	}

  void ERAnagetData::ProcessEnd(TFile* fout) {
		ntTrack->Write();
		ntShower->Write();
	}

}

#endif
