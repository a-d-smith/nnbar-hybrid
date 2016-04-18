#ifndef LARLITE_GETDATA_H
#define LARLITE_GETDATA_H

#include "Analysis/ana_base.h"
#include "DataFormat/mctrack.h"
#include "DataFormat/mcshower.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TLeaf.h"
#include <vector>

namespace larlite {
  class getData : public ana_base{
  
  public:

    /// Default constructor
    getData(){ _name="getData"; _fout=0;}

    /// Default destructor
    virtual ~getData(){}
    virtual bool initialize();
    virtual bool analyze(storage_manager* storage);
    virtual bool finalize();
		
		TFile *f, *hybridPi0Data;

		TTree *track;
		TTree *shower;
		
		TLeaf *tr_event, *tr_KE, *tr_PID, *tr_StartX, *tr_StartY, *tr_StartZ, *tr_EndX, *tr_EndY, *tr_EndZ, *tr_Time;
		TLeaf *sh_event, *sh_KE, *sh_type, *sh_StartX, *sh_StartY, *sh_StartZ, *sh_DirX, *sh_DirY, *sh_DirZ, *sh_L, *sh_R, *sh_Time;

		TNtuple *ntTrack;
		TNtuple *ntShower;

		int tr_entry, sh_entry, badEvents;

		struct showerData{
			int    event;
			double KE;
			char   type;
			double StartX;
			double StartY;
			double StartZ;
			double DirX;
			double DirY;
			double DirZ;
			double L;
			double R;
			double Time;
		};

		struct trackData{
			int    event;
			double KE;
			int    PID;
			double StartX;
			double StartY;
			double StartZ;
			double EndX;
			double EndY;
			double EndZ;
			double Time;
		};

		
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
