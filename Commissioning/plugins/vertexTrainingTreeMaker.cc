
// -*- C++ -*-
//
// Package:    Commissioning/test
// Class:      test
// 
/**\class test test.cc 

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Julie Malcles
//         Created:  Tue, 02 Dec 2014 10:57:22 GMT
//
//


#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include "flashgg/MicroAODAlgos/interface/VertexSelectorBase.h"
#include "flashgg/MicroAODFormats/interface/Photon.h"
#include "flashgg/MicroAODFormats/interface/DiPhotonCandidate.h"

#include "TTree.h"

// **********************************************************************

// define the structures used to create tree branches and fill the trees 

struct GenInfo {
  float genVertexZ;
  float genHiggsPt;
};

struct SignalInfo {
 
  int nvertex;
  int ndipho; 
  int dipho_index;

  float LogSumPt2;
  float PtBal;
  float PtAsym;
  float NConv;
  float PullConv;

};

struct BackgroundInfo {
 
  int nvertex;
  int ndipho;
  int dipho_index;

  float LogSumPt2;
  float PtBal;
  float PtAsym;
  float NConv;
  float PullConv;

};

// **********************************************************************

using namespace std;
using namespace edm;
using namespace flashgg;


// **********************************************************************

class test : public edm::EDAnalyzer {
   public:
      explicit test(const edm::ParameterSet&);
      ~test();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:

      edm::Service<TFileService> fs_;

     

      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

  void initEventStructure();
  int getMCTruthVertexIndex( const PtrVector<reco::GenParticle>& gens, const edm::PtrVector<reco::Vertex>&, double dzMatch = 0.1);
  int getSortedIndex( const unsigned int trueVtxIndex, const unsigned int sizemax, const Ptr<flashgg::DiPhotonCandidate> diphoPtr  );

  //  edm::EDGetTokenT<edm::View<flashgg::Photon> >            photonToken_;
  edm::EDGetTokenT<edm::View<flashgg::DiPhotonCandidate> > diphotonToken_;
 /* edm::EDGetTokenT<edm::View<reco::Vertex> >               vertexToken_; 

  //edm::EDGetTokenT<View<reco::Conversion> > conversionToken_;
  edm::EDGetTokenT<reco::BeamSpot > beamSpotToken_; 
  edm::EDGetTokenT<VertexCandidateMap> vertexCandidateMapTokenDz_;
  edm::EDGetTokenT<View<reco::GenParticle> > genParticleToken_;*/


      TTree* signalTree; 
      TTree* backgroundTree; 

  BackgroundInfo bkgInfo;
  SignalInfo sigInfo;
  GenInfo genInfo;



};

// ******************************************************************************************


//
// constructors and destructor
//
test::test(const edm::ParameterSet& iConfig):
  //  photonToken_(consumes<View<flashgg::Photon> >(iConfig.getUntrackedParameter<InputTag> ("PhotonTag", InputTag("flashggPhotons")))),
  diphotonToken_(consumes<View<flashgg::DiPhotonCandidate> >(iConfig.getUntrackedParameter<InputTag> ("DiPhotonTag", InputTag("flashggDiPhotons"))))
  //vertexToken_(consumes<View<reco::Vertex> >(iConfig.getUntrackedParameter<InputTag> ("VertexTag", InputTag("offlineSlimmedPrimaryVertices")))),
  //  conversionToken_(consumes<View<reco::Conversion> >(iConfig.getUntrackedParameter<InputTag>("ConversionTag",InputTag("reducedConversions")))),
//  beamSpotToken_(consumes<reco::BeamSpot >(iConfig.getUntrackedParameter<InputTag>("BeamSpotTag",InputTag("offlineBeamSpot")))), 
//  vertexCandidateMapTokenDz_(consumes<VertexCandidateMap>(iConfig.getParameter<InputTag>("VertexCandidateMapTagDz"))),
 // genParticleToken_(consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles"))))  
{
 
}


test::~test()
{
 
  
}



	void
test::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

	// ********************************************************************************

	// access edm objects
	/*
		 Handle<View<flashgg::Photon> > photons;
		 iEvent.getByToken(photonToken_,photons);
		 const PtrVector<flashgg::Photon>& photonPointers = photons->ptrVector();
		 */

	Handle<View<flashgg::DiPhotonCandidate> > diphotons;
	iEvent.getByToken(diphotonToken_,diphotons);
	const PtrVector<flashgg::DiPhotonCandidate>& diphotonPointers = diphotons->ptrVector();  


	std::cout << diphotonPointers.size() << std::endl;

	initEventStructure();
}


	void 
test::beginJob()
{
	signalTree = fs_->make<TTree>("signalTree","per-diphoton tree");
	signalTree->Branch("nvertex",&sigInfo.nvertex,"nvertex/I");
	signalTree->Branch("ndipho",&sigInfo.ndipho,"ndipho/I");
	signalTree->Branch("dipho_index",&sigInfo.dipho_index,"dipho_index/I");
	signalTree->Branch("LogSumPt2",&sigInfo.LogSumPt2,"LogSumPt2/F");
	signalTree->Branch("PtBal",&sigInfo.PtBal,"PtBal/F");
	signalTree->Branch("PtAsym",&sigInfo.PtAsym,"PtAsym/F");
	signalTree->Branch("NConv",&sigInfo.NConv,"NConv/F");
	signalTree->Branch("PullConv",&sigInfo.PullConv,"PullConv/F");

	backgroundTree = fs_->make<TTree>("backgroundTree","per-diphoton tree");
	backgroundTree->Branch("nvertex",&bkgInfo.nvertex,"nvertex/I");
	backgroundTree->Branch("ndipho",&bkgInfo.ndipho,"ndipho/I");
	backgroundTree->Branch("dipho_index",&bkgInfo.dipho_index,"dipho_index/I");
	backgroundTree->Branch("LogSumPt2",&bkgInfo.LogSumPt2,"LogSumPt2/F");
	backgroundTree->Branch("PtBal",&bkgInfo.PtBal,"PtBal/F");
	backgroundTree->Branch("PtAsym",&bkgInfo.PtAsym,"PtAsym/F");
	backgroundTree->Branch("NConv",&bkgInfo.NConv,"NConv/F");
	backgroundTree->Branch("PullConv",&bkgInfo.PullConv,"PullConv/F");

}

	void 
test::endJob() 
{
}

	void
test::initEventStructure() 
{
	genInfo.genVertexZ=-999.;
	genInfo.genHiggsPt=-999.;

	sigInfo.nvertex = -999;
	sigInfo.ndipho = -999;
	sigInfo.dipho_index = -999;

	sigInfo.LogSumPt2  = -999;
	sigInfo.PtBal  = -999;
	sigInfo.PtAsym  = -999;
	sigInfo.NConv  = -999;
	sigInfo.PullConv  = -999;


	bkgInfo.nvertex = -999;
	bkgInfo.ndipho = -999;
	bkgInfo.dipho_index = -999;

	bkgInfo.LogSumPt2  = -999;
	bkgInfo.PtBal  = -999;
	bkgInfo.PtAsym  = -999;
	bkgInfo.NConv  = -999;
	bkgInfo.PullConv  = -999;

}


/*
	 void 
	 test::beginRun(edm::Run const&, edm::EventSetup const&)
	 {
	 }
	 */

/*
	 void 
	 test::endRun(edm::Run const&, edm::EventSetup const&)
	 {
	 }
	 */

/*
	 void 
	 test::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
	 {
	 }
	 */

/*
	 void 
	 test::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
	 {
	 }
	 */

void
test::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}



DEFINE_FWK_MODULE(test);
