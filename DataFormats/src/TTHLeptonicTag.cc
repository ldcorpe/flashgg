#include "flashgg/DataFormats/interface/TTHLeptonicTag.h"
#include <algorithm>

using namespace flashgg;

TTHLeptonicTag::TTHLeptonicTag() : DiPhotonTagBase::DiPhotonTagBase()
{
    std::cout << "DEBUG DEBUG I AM BEING constructed 1 (TTHLept)  " << std::endl;

}


TTHLeptonicTag::~TTHLeptonicTag()
{}

// N.B. Other attributes are set using methods in header file
TTHLeptonicTag::TTHLeptonicTag( edm::Ptr<DiPhotonCandidate> diPho, edm::Ptr<DiPhotonMVAResult> mvares ) : TTHLeptonicTag::TTHLeptonicTag( diPho, *mvares )
{

    std::cout << "DEBUG DEBUG I AM BEING constructed 2 (TTHLept)  " << std::endl;

}
TTHLeptonicTag::TTHLeptonicTag( edm::Ptr<DiPhotonCandidate> dipho, DiPhotonMVAResult mvares ) : DiPhotonTagBase::DiPhotonTagBase( dipho, mvares )
{

    std::cout << "DEBUG DEBUG I AM BEING constructed 3 (TTHLept)  " << std::endl;

}
TTHLeptonicTag::TTHLeptonicTag( const TTHLeptonicTag &b ) : DiPhotonTagBase::DiPhotonTagBase( b )
{
    std::cout << "DEBUG DEBUG I AM BEING COPY CONSTRUCTED (TTHLept)  " << std::endl;
    setJets( b.jets() );
    setBJets( b.bJets() );
    setMuons( b.muons() );
    setElectrons( b.electrons() );
}




// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

