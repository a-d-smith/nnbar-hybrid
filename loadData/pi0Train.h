/**
 * \file pi0Train.h
 *
 * \ingroup loadData
 * 
 * \brief Class def header for a class pi0Train
 *
 * @author rsjones
 */

/** \addtogroup loadData

    @{*/

#ifndef LARLITE_PI0TRAIN_H
#define LARLITE_PI0TRAIN_H

#include "Analysis/ana_base.h"

namespace larlite {
  /**
     \class pi0Train
     User custom analysis class made by SHELL_USER_NAME
   */
  class pi0Train : public ana_base{
  
  public:

    /// Default constructor
    pi0Train(){ _name="pi0Train"; _fout=0;}

    /// Default destructor
    virtual ~pi0Train(){}

    /** IMPLEMENT in pi0Train.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in pi0Train.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in pi0Train.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

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
