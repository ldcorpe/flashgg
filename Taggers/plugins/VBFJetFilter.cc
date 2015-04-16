#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "flashgg/DataFormats/interface/DiPhotonCandidate.h"
#include "flashgg/DataFormats/interface/Jet.h"
#include "flashgg/DataFormats/interface/VBFMVAResult.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TMVA/Reader.h"
#include "TMath.h"

using namespace std;
using namespace edm;

class Particle {
    public:
        float pt;
        float eta;
        float phi;
        int   pdgid;
        int   status;
        int   number;
        float rapidity;  
        bool  match;
        int   quark;

        Particle(void){

            pt=0;
            eta=0;
            phi=0;
            pdgid=0;
            status=0;
            number=0;
            rapidity=0;
            match = true;

        }

        float deltaR(Particle quark) {

            float error(0);
            error += pow((eta-quark.eta),2);
            error += pow((phi-quark.phi),2);

            return sqrt(error);

        }
};

namespace flashgg {

    class VBFJetFilter : public EDProducer
    {

        public:
            VBFJetFilter( const ParameterSet & );
        private:
            void produce( Event &, const EventSetup & ) override;

            EDGetTokenT<View<flashgg::Jet> > jetTokenDz_;
            EDGetTokenT< edm::View<reco::GenParticle> > genPartToken_;


    };

    VBFJetFilter::VBFJetFilter( const ParameterSet &iConfig ) :
        jetTokenDz_( consumes<View<flashgg::Jet> >( iConfig.getUntrackedParameter<InputTag>( "JetTag", InputTag( "flashggJets" ) ) ) ),
        genPartToken_ (consumes<View<reco::GenParticle> >(iConfig.getUntrackedParameter<InputTag> ("GenParticleTag", InputTag("prunedGenParticles"))))
    {
        produces<vector<flashgg::Jet> >();
    }

    void VBFJetFilter::produce( Event &evt, const EventSetup & )
    {
       Handle<View<flashgg::Jet> > jetsDz;
        evt.getByToken( jetTokenDz_, jetsDz );
        //	const PtrVector<flashgg::Jet>& jetPointersDz = jetsDz->ptrVector();

        Handle<View<reco::GenParticle> > genParticles;
        evt.getByToken(genPartToken_,genParticles);

        vector<int> jetQuarks;

        std::auto_ptr<vector<flashgg::Jet> > result( new vector<flashgg::Jet>()); 
        

       /* 
        for( unsigned int genLoop =0 ; genLoop < genParticles->size(); genLoop++){
            int partStatus = genParticles->ptrAt(genLoop)->status();
            int partID = genParticles->ptrAt(genLoop)->pdgId();
            if (partStatus == 3) {
                if (abs(partID) <= 6 && abs(partID) >= 1) {
                    jetQuarks.push_back(genLoop);               
                }
            }
        }
       
        
        
        //Take the last two and create particle objects
        Particle quark1,quark2;
        if (jetQuarks.size() > 1) {

            quark1.pt = genParticles->ptrAt(jetQuarks.at(jetQuarks.size()-1))->pt();
            quark1.eta = genParticles->ptrAt(jetQuarks.at(jetQuarks.size()-1))->eta();
            quark1.phi = genParticles->ptrAt(jetQuarks.at(jetQuarks.size()-1))->phi();
            quark1.rapidity = genParticles->ptrAt(jetQuarks.at(jetQuarks.size()-1))->rapidity();

            quark2.pt = genParticles->ptrAt(jetQuarks.at(jetQuarks.size()-2))->pt();
            quark2.eta = genParticles->ptrAt(jetQuarks.at(jetQuarks.size()-2))->eta();
            quark2.phi = genParticles->ptrAt(jetQuarks.at(jetQuarks.size()-2))->phi();
            quark2.rapidity = genParticles->ptrAt(jetQuarks.at(jetQuarks.size()-2))->rapidity();


        }*/
       

        for (unsigned int jetLoop=0; jetLoop< jetsDz->size(); jetLoop++){
            float minDR=999.;

            for( unsigned int genLoop =0 ; genLoop < genParticles->size(); genLoop++){
                int partID = genParticles->ptrAt(genLoop)->pdgId();
                if (partID==21 || (abs(partID) <= 6 && abs(partID) >= 1)) {

                    float jetEta = jetsDz->ptrAt(jetLoop)->eta();
                    float jetPhi = jetsDz->ptrAt(jetLoop)->phi();

                    float dEta1 = jetEta - genParticles->ptrAt(genLoop)->eta();

                    float dPhi1 = deltaPhi(jetPhi , genParticles->ptrAt(genLoop)->phi());

                    float dR1 = sqrt(dPhi1*dPhi1 + dEta1*dEta1);

                //    std::cout << "jet " << jetLoop << " , quark/gluon "<< partID  << ", dr " << dR1 << std::endl;


                    if ( dR1 <minDR)
                    {
                        minDR=dR1;
                    }

                }
            }
            if (minDR <0.2){

                edm::Ptr<flashgg::Jet> jet = jetsDz->ptrAt(jetLoop);
                result->push_back(*jet);

            }
        }


        std::cout << " jet total : " << jetsDz->size() << ", result total : " << result->size() << std::endl;
        evt.put( result );

    }
}

typedef flashgg::VBFJetFilter FlashggVBFJetFilter;
DEFINE_FWK_MODULE( FlashggVBFJetFilter );
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

