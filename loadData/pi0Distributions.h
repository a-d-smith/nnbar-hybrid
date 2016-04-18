#ifndef LARLITE_PI0DISTRIBUTIONS_H
#define LARLITE_PI0DISTRIBUTIONS_H

#include "Analysis/ana_base.h"
#include "loadData.h"
#include "TNtuple.h"
#include "TFile.h"

namespace larlite {
  class pi0Distributions : public ana_base{
  
  public:

    /// Default constructor
    pi0Distributions(){ _name="pi0Distributions"; _fout=0;}

    /// Default destructor
    virtual ~pi0Distributions(){}

    virtual bool initialize();

    virtual bool analyze(storage_manager* storage);

    virtual bool finalize();
		
		TNtuple *nt;
		std::vector<TVector3> nhat, r0;

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
