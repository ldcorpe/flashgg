#ifndef flashgg_VBFMVAResultDumpers_h
#define flashgg_VBFMVAResultDumpers_h

#include "flashgg/MicroAODFormats/interface/VBFMVAResult.h"

#include "flashgg/TagAlgos/interface/CollectionDumper.h"

/// #include "PhysicsTools/UtilAlgos/interface/FWLiteAnalyzerWrapper.h"

namespace flashgg 
{ 
	typedef CollectionDumper<std::vector<VBFMVAResult> > VBFMVAResultDumper;

	//// namespace fwlite {
	//// 	typedef ::fwlite::AnalyzerWrapper<DiPhotonDumper> DiPhotonDumper;
	//// 	typedef ::fwlite::AnalyzerWrapper<PhotonDumper> PhotonDumper;
	//// 	typedef ::fwlite::AnalyzerWrapper<DiPhotonTagDumper> DiPhotonTagDumper;
	//// }
	
}

#endif // flashgg_PhotonDumpers_h
