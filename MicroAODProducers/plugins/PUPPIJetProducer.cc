#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/Jet.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"
#include "RecoJets/JetProducers/interface/PileupJetIdAlgo.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "flashgg/MicroAODFormats/interface/VertexCandidateMap.h"
#include "DataFormats/JetReco/interface/PFJet.h"


using namespace std;
using namespace edm;

namespace flashgg {

  class PUPPIJetProducer : public EDProducer {

  public:
    PUPPIJetProducer( const ParameterSet & );
  private:
    void produce( Event &, const EventSetup & ) override;
    EDGetTokenT<View<reco::PFJet> > jetToken_;
    EDGetTokenT<View<DiPhotonCandidate> > diPhotonToken_;
    //    EDGetTokenT<View<reco::Vertex> >  vertexToken_;
    //    EDGetTokenT<reco::VertexCollection> vertexToken_;
    EDGetTokenT< VertexCandidateMap > vertexCandidateMapToken_;
    unique_ptr<PileupJetIdAlgo>  pileupJetIdAlgo_;
    ParameterSet pileupJetIdParameters_;
    //    bool useAODOnlyPileupJetIdMethod_;
  };


  PUPPIJetProducer::PUPPIJetProducer(const ParameterSet & iConfig) :
    jetToken_(consumes<View<reco::PFJet> >(iConfig.getUntrackedParameter<InputTag> ("JetTag", InputTag("slimmedJets")))),
    diPhotonToken_(consumes<View<DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag>("DiPhotonTag",InputTag("flashggDiPhotons")))),
    //    vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
    //    vertexToken_(consumes<reco::VertexCollection>(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
    vertexCandidateMapToken_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTag"))),
    pileupJetIdParameters_(iConfig.getParameter<ParameterSet>("PileupJetIdParameters"))
    //    useAODOnlyPileupJetIdMethod_(iConfig.getUntrackedParameter<bool>("UseAODOnlyPileupJetIdMethod",false))
    
  {
    pileupJetIdAlgo_.reset(new PileupJetIdAlgo(pileupJetIdParameters_));

    produces<vector<flashgg::Jet> >();
  }

  void PUPPIJetProducer::produce( Event & evt, const EventSetup & ) {
    
    // input jets
    Handle<View<reco::PFJet> > jets;
    evt.getByToken(jetToken_,jets);
    const PtrVector<reco::PFJet>& jetPointers = jets->ptrVector();

    // input DiPhoton candidates
    Handle<View<DiPhotonCandidate> > diPhotons;
    evt.getByToken(diPhotonToken_,diPhotons);
    const PtrVector<DiPhotonCandidate>& diPhotonPointers = diPhotons->ptrVector();

    Handle<VertexCandidateMap> vertexCandidateMap;
    //    Handle<View<reco::Vertex> > primaryVertices;
    //    Handle<reco::VertexCollection> primaryVertices;
    evt.getByToken(vertexCandidateMapToken_,vertexCandidateMap);

    // output jets
    auto_ptr<vector<flashgg::Jet> > jetColl(new vector<flashgg::Jet>);

    for (unsigned int i = 0 ; i < jetPointers.size() ; i++) {
      Ptr<reco::PFJet> pjet = jetPointers[i];
      flashgg::Jet fjet = flashgg::Jet(*pjet);
      for (unsigned int j = 0 ; j < diPhotonPointers.size() ; j++) {
	Ptr<DiPhotonCandidate> diPhoton = diPhotonPointers[j];
	Ptr<reco::Vertex> vtx = diPhoton->getVertex();

	// Method written just for MiniAOD --> MicroAOD
	//COMMENT OUT FOR PUPPI
//	PileupJetIdentifier lPUJetId = pileupJetIdAlgo_->computeIdVariables(pjet.get(),vtx,*vertexCandidateMap,true);
 // fjet.setPuJetId(vtx,lPUJetId.mva());
      }
      jetColl->push_back(fjet);
    }
    
    evt.put(jetColl);
  }
}

typedef flashgg::PUPPIJetProducer FlashggPUPPIJetProducer;
DEFINE_FWK_MODULE(FlashggPUPPIJetProducer);
