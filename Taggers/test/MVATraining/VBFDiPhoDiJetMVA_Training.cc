///==== include ====
#include "TFile.h"
#include "TChain.h"
#include "TMinuit.h"
#include <sstream>
#include <iostream>
#include "TMVA/Factory.h"
#if not defined(__CINT__) || defined(__MAKECINT__)
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

//#include "TMVAGui.C"

#endif
using namespace std;
// --------- MAIN -------------------
void VBFDiPhoDiJetMVA_Training( TString Nevent = "10000", TString Level = "VBFDiPhoDiJet", bool skipEvtWNoVBF = true )
{
    // you must define $WORKSPACE first
    TString path;
    path = "${WORKSPACE}/test_diphodijet_training/";

    bool useDiphotonPt = 0;
    bool usePhotonsPt = true;
    int nEvents= std::atoi(Nevent.Data());


    //  TFile *inputS = TFile::Open( path + "output_VBF_HToGG_M-125_13TeV-powheg-pythia6_numEvent" + Nevent + "_histos.root" );
    // TFile *inputB = TFile::Open( path + "output_GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6_numEvent" + Nevent + "_histos.root" );

    //  TTree *treeS = ( TTree * )inputS->Get( Level + "MVADumperNew/trees/vbf_m125_13TeV_All" );
    //  TTree *treeB = ( TTree * )inputB->Get( Level + "MVADumperNew/trees/gamJet_13TeV_All" );

    TFile *inputB1 = TFile::Open(path + "output_DYJetsToLL_M-50_13TeV-madgraph-pythia8_numEvent"+Nevent+"_histos.root                  ");
    TFile *inputB2 = TFile::Open(path + "output_GJet_Pt40_doubleEMEnriched_TuneZ2star_13TeV-pythia6_numEvent"+Nevent+"_histos.root     ");
    TFile *inputB3 = TFile::Open(path + "output_GJet_Pt20to40_doubleEMEnriched_TuneZ2star_13TeV-pythia6_numEvent"+Nevent+"_histos.root ");
    TFile *inputS1 = TFile::Open(path + "output_GluGluToHToGG_M-125_13TeV-powheg-pythia6_numEvent"+Nevent+"_histos.root                ");
    //  TFile *inputS2 = TFile::Open(path + "output_TTbarH_HToGG_M-125_13TeV_amcatnlo-pythia8-tauola_numEvent"+Nevent+"_histos.root        ");
    TFile *inputS3 = TFile::Open(path + "output_VBF_HToGG_M-125_13TeV-powheg-pythia6_numEvent"+Nevent+"_histos.root                    ");
    //  TFile *inputS4 = TFile::Open(path + "output_WH_ZH_HToGG_M-125_13TeV_pythia6_numEvent"+Nevent+"_histos.root                         ");

    TTree *treeB1 = (TTree*)inputB1->Get(Level +"MVADumperNew/trees/dyJets_13TeV_All");
    TTree *treeB2 = (TTree*)inputB2->Get(Level +"MVADumperNew/trees/gamJet_13TeV_All");
    TTree *treeB3 = (TTree*)inputB3->Get(Level +"MVADumperNew/trees/gamJet_13TeV_All");
    TTree *treeS1 = (TTree*)inputS1->Get(Level +"MVADumperNew/trees/ggh_m125_13TeV_All");
    // TTree *treeS2 = (TTree*)inputS2->Get(Level +"MVADumperNew/trees/tth_m125_13TeV_All");
    TTree *treeS3 = (TTree*)inputS3->Get(Level +"MVADumperNew/trees/vbf_m125_13TeV_All");
    //  TTree *treeS4 = (TTree*)inputS4->Get(Level +"MVADumperNew/trees/wzh_m125_13TeV_All");


    // declaration of leaf types

    float dijet_mva      ;
    float dipho_mva      ;
    float dipho_PToM     ;

    float dijet_abs_dEta  ;
    float dijet_leadEta   ;
    float dijet_subleadEta;
    float dijet_LeadJPt   ;
    float dijet_SubJPt    ;
    float dijet_Zep       ;
    float dijet_Mjj       ;
    float leadPho_PToM    ;
    float sublPho_PToM    ;
    float dijet_dPhi_trunc;

    float   weightS[5];
    float   weightB[4];

    weightB[0]=0;
    weightB[1]=4746.0/nEvents;// DyJetsToLL
    weightB[2]=17180.0*0.0379/nEvents;//gamJets pt>40
    weightB[3]=145400.0*0.001776/nEvents;//gamJets pt  in 20->40

    weightS[0]=0;
    weightS[1]=43.92*2.28e-3/nEvents; //ggH
    weightS[2]=3.748*2.28e-3/nEvents; //ttH
    weightS[3]=2.2496*2.28e-3/nEvents; //VBH
    weightS[4]=0.5608*2.28e-3/nEvents; //WZH

    /*  weightB[0]=1;
        weightB[1]=1;
        weightB[2]=1;
        weightB[3]=1;

        weightS[0]=1;
        weightS[1]=1;
        weightS[2]=1;
        weightS[3]=1;
        weightS[4]=1;*/


    if( Level == "VBFDiPhoDiJet" ) {
        treeS1->SetBranchAddress( "dipho_mva"         , &dipho_mva );
        treeS1->SetBranchAddress( "dijet_mva" , &dijet_mva );
        treeS1->SetBranchAddress( "dipho_PToM"     , &dipho_PToM );
    } else  if( Level == "VBF" ) {
        treeS1->SetBranchAddress( "dijet_abs_dEta   ", &dijet_abs_dEta );
        treeS1->SetBranchAddress( "dijet_leadEta    ", &dijet_leadEta );
        treeS1->SetBranchAddress( "dijet_subleadEta ", &dijet_subleadEta );
        treeS1->SetBranchAddress( "dijet_LeadJPt    ", &dijet_LeadJPt );
        treeS1->SetBranchAddress( "dijet_SubJPt     ", &dijet_SubJPt );
        treeS1->SetBranchAddress( "dijet_Zep        ", &dijet_Zep );
        treeS1->SetBranchAddress( "dijet_Mjj        ", &dijet_Mjj );
        treeS1->SetBranchAddress( "dipho_PToM       ", &dipho_PToM );
        treeS1->SetBranchAddress( "leadPho_PToM     ", &leadPho_PToM );
        treeS1->SetBranchAddress( "sublPho_PToM     ", &sublPho_PToM );
        treeS1->SetBranchAddress( "dijet_dPhi_trunc ", &dijet_dPhi_trunc );
    }


    // Create a new root output file.
    string outputFileName;
    if( Level == "VBF" ) {
        outputFileName = "Flashgg_VBF";
    } else {
        outputFileName = "Flashgg_DiPhoDiJet";
    }

    // -- reader
    TFile *outputFile = TFile::Open( ( outputFileName + ".root" ).c_str(), "RECREATE" );
    TMVA::Factory *factory = new TMVA::Factory( outputFileName.c_str(), outputFile,
            "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
    // -- variables
    if( Level == "VBFDiPhoDiJet" ) {
        factory->AddVariable( "dipho_mva" );
        factory->AddVariable( "dijet_mva" );
        factory->AddVariable( "dipho_PToM" );

    } else  if( Level == "VBF" ) {
        factory->AddVariable( "dijet_LeadJPt" );
        factory->AddVariable( "dijet_SubJPt" );
        factory->AddVariable( "dijet_abs_dEta" );
        factory->AddVariable( "dijet_Mjj" );
        factory->AddVariable( "dijet_Zep" );
        factory->AddVariable( "dijet_dPhi_trunc" );

        if( useDiphotonPt ) {
            factory->AddVariable( "dipho_PToM" );
        }
        if( usePhotonsPt ) {
            factory->AddVariable( "leadPho_PToM" );
            factory->AddVariable( "sublPho_PToM" );
        }
    }


    //event weights per tree (see below for setting event-wise weights)
    Double_t signalWeight = 1.0;
    Double_t backgroundWeight = 1.0;


    // ====== register trees ====================================================
    /*factory->AddSignalTree    ( treeS3, weightS[3]    , "Training" );//VBF
      factory->AddSignalTree    ( treeS3, weightS[3]    , "Test"     );//VBF
      factory->AddBackgroundTree( treeB1, weightB[1]    , "Training" );//DY
      factory->AddBackgroundTree( treeB1, weightB[1]    , "Test"     );//DY
      factory->AddBackgroundTree( treeB2, weightB[2]    , "Training" );//gamJets1
      factory->AddBackgroundTree( treeB2, weightB[2]    , "Test"     );//gamJets1
      factory->AddBackgroundTree( treeB3, weightB[3]    , "Training" );//gamJets2
      factory->AddBackgroundTree( treeB3, weightB[3]    , "Test"     );//gamJets2
      factory->AddBackgroundTree( treeS1, weightS[1]    , "Training" );//ggH is used as background here
      factory->AddBackgroundTree( treeS1, weightS[1]    , "Test"     );//ggH is used as backrgound here.*/

    factory->AddSignalTree    ( treeS3, weightS[3] );
    factory->AddBackgroundTree( treeB1, weightB[1] );
    factory->AddBackgroundTree( treeB2, weightB[2] );
    factory->AddBackgroundTree( treeB3, weightB[3] );
    factory->AddBackgroundTree( treeS1, weightS[1] );

    // == supress the the negative points on the input variables
    // == this high correlation between variables
    TCut mycuts = ""; // " leadPho_PToM > (60./120.) && sublPho_PToM> (30./120.)";
    TCut mycutb = ""; // " leadPho_PToM> (60./120.) && sublPho_PToM> (30./120.)";
    if(Level =="VBFDiPhoDiJet") {
        if(skipEvtWNoVBF){
            mycuts ="dijet_mva>-99 && jet1genMatch==1 && jet2genMatch==1";// Skip the event with -999
            mycutb ="dijet_mva>-99";//  
        }
    } else  if(Level =="VBF") {
        if(skipEvtWNoVBF){
            mycuts ="vbfMvaResult_value>-99 && jet1genMatch==1 && jet2genMatch==1";// Skip the event with -999
            mycutb ="vbfMvaResult_value>-99";//  
        }
    }




    // tell the factory to use all remaining events in the trees after training for testing:
    factory->PrepareTrainingAndTestTree( mycuts, mycutb,
            //"SplitMode=Random:NormMode=NumEvents:!V" );
        "SplitMode=random:!V" );
    // Boosted Decision Trees: use BDTG ( Gradient Boost )
    factory->BookMethod( TMVA::Types::kBDT, "BDTG",
            "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=5:MaxDepth=3" );
    factory->BookMethod( TMVA::Types::kBDT, "BDT",
            "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    //"!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=15:MaxDepth=5" );
    // book Cuts
    //factory->BookMethod( TMVA::Types::kCuts, "CutsGA",
    // "H:!V:FitMethod=GA:CutRangeMin[0]=20:CutRangeMax[0]=500:CutRangeMin[1]=20:CutRangeMax[1]=500:VarProp=FSmart:VarProp[4]=FMin:EffSel:Steps=30:Cycles=3:PopSize=500:SC_steps=10:SC_rate=5:SC_factor=0.95" );
    // ---- Now you can tell the factory to train, test, and evaluate the MVAs
    // Train MVAs using the set of training events
    factory->TrainAllMethods();
    // ---- Evaluate all MVAs using the set of test events
    factory->TestAllMethods();
    // ----- Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();
    // --------------------------------------------------------------
    // Save the output
    outputFile->Close();
    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;
    delete factory;

    //	if (!gROOT->IsBatch()) TMVAGui( (outputFileName+".root").c_str() );
}
// Local Variables:
// mode:c++
// indent-tabs-mode:nil
// tab-width:4
// c-basic-offset:4
// End:
// vim: tabstop=4 expandtab shiftwidth=4 softtabstop=4

