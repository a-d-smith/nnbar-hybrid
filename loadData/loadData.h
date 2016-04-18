#ifndef LARLITE_LOADDATA_H
#define LARLITE_LOADDATA_H

#include "Analysis/ana_base.h"
#include "TFile.h"
#include "TString.h"
#include "TNtuple.h"
#include "TVector3.h"
#include "TH2D.h"
#include "TLeaf.h"
#include "TCanvas.h"
#include <vector>
#include <iostream>

using namespace std;


namespace larlite {
  class loadData : public ana_base{
  
  public:

    /// Parametrised constructor
    loadData(const char *fName){ 
			_name="loadData";
			_fout=0;
			
			f = new TFile(fName);
			initialize();
		}

    /// Default destructor
    virtual ~loadData(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();
		TFile *f;
		TTree *tracks, *showers;
		TLeaf *tr_event, *tr_KE, *tr_PID, *tr_StartX, *tr_StartY, *tr_StartZ, *tr_EndX, *tr_EndY, *tr_EndZ, *tr_Time, *tr_MC_PDG, *tr_MC_isPrimary, *tr_MC_ParentPDG, *tr_MC_ID, *tr_MC_ParentID;
		TLeaf *sh_event, *sh_KE, *sh_type, *sh_StartX, *sh_StartY, *sh_StartZ, *sh_DirX, *sh_DirY, *sh_DirZ, *sh_L, *sh_MC_PDG, *sh_MC_isPrimary, *sh_MC_ParentPDG, *sh_MC_ID, *sh_MC_ParentID;
		
		struct track{
			double KE;
			int PID;
			TVector3 Start;
			TVector3 End;
			double T;
			int mcPDG;
			bool mcPrimary;
			int mcParentPDG;
			int mcID;
			int mcParentID;
		};
		
		struct shower{
			double KE;
			int type;
			TVector3 Start;
			TVector3 Dir;
			double L;
			int mcPDG;
			bool mcPrimary;
			int mcParentPDG;
			int mcID;
			int mcParentID;
		};

		struct ev{
			vector<track> trList;
			vector<shower> shList;
		};
		
		vector<ev> evList;

  protected:
    
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
