/**
 * \file ERAnagetData.h
 *
 * \ingroup makeHybrid
 * 
 * \brief Class def header for a class ERAnagetData
 *
 * @author rsjones
 */

/** \addtogroup makeHybrid

    @{*/

#ifndef ERTOOL_ERANAGETDATA_H
#define ERTOOL_ERANAGETDATA_H

#include "ERTool/Base/AnaBase.h"
#include <vector>
#include <iomanip>
#include "TNtuple.h"
#include "TVector3.h"

namespace ertool {

  /**
     \class ERAnagetData
     User custom Analysis class made by kazuhiro
   */
  class ERAnagetData : public AnaBase {
  
  public:

    /// Default constructor
    ERAnagetData(const std::string& name="ERAnagetData");

    /// Default destructor
    virtual ~ERAnagetData(){}

    /// Reset function
    virtual void Reset();

    /// Function to accept fclite::PSet
    void AcceptPSet(const ::fcllite::PSet& cfg);

    /// Called @ before processing the first event sample
    void ProcessBegin();

    /// Function to evaluate input showers and determine a score
    bool Analyze(const EventData &data, const ParticleGraph &ps);

    /// Called after processing the last event sample
    void ProcessEnd(TFile* fout=nullptr);
		
		TNtuple *ntTrack;
		TNtuple *ntShower;
		double event;
  };
}
#endif

/** @} */ // end of doxygen group 
