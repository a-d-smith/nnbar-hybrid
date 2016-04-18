/**
 * \file testAna.h
 *
 * \ingroup loadData
 * 
 * \brief Class def header for a class testAna
 *
 * @author rsjones
 */

/** \addtogroup loadData

    @{*/

#ifndef LARLITE_TESTANA_H
#define LARLITE_TESTANA_H

#include "Analysis/ana_base.h"
#include "loadData.h"

namespace larlite {
  /**
     \class testAna
     User custom analysis class made by SHELL_USER_NAME
   */
  class testAna : public ana_base{
  
  public:

    /// Default constructor
    testAna(){ _name="testAna"; _fout=0;}

    /// Default destructor
    virtual ~testAna(){}

    /** IMPLEMENT in testAna.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in testAna.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in testAna.cc! 
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
