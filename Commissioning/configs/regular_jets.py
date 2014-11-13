import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

process = cms.Process("FLASHggMicroAOD")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 10000 )

process.source = cms.Source("PoolSource",
	#fileNames=cms.untracked.vstring("/store/cmst3/user/gpetrucc/miniAOD/v1/GluGluToHToGG_M-125_13TeV-powheg-pythia6_Flat20to50_PAT.root")
	fileNames=cms.untracked.vstring("/store/mc/Spring14miniaod/VBF_HToGG_M-125_13TeV-powheg-pythia6/MINIAODSIM/PU20bx25_POSTLS170_V5-v1/00000/CE926731-9607-E411-B0BA-001E67248A1B.root")
	)

process.source.inputCommands = cms.untracked.vstring("keep *",
			"drop *_MEtoEDMConverter_*_*")
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *

puppiCentral = cms.VPSet(
		cms.PSet(
			algoId           = cms.untracked.int32(5),  #0 is default Puppi
			useCharged       = cms.untracked.bool(True),
			applyLowPUCorr   = cms.untracked.bool(True),
			combOpt          = cms.untracked.int32(0),
			cone             = cms.untracked.double(0.3),
			rmsPtMin         = cms.untracked.double(0.1),
			rmsScaleFactor   = cms.untracked.double(1.0)
			)
		)

puppiForward = cms.VPSet(
		cms.PSet(
			algoId         = cms.untracked.int32(5),  #0 is default Puppi
			useCharged     = cms.untracked.bool(False),
			applyLowPUCorr = cms.untracked.bool(True),
			combOpt        = cms.untracked.int32(0),
			cone           = cms.untracked.double(0.3),
			rmsPtMin       = cms.untracked.double(0.5),
			rmsScaleFactor = cms.untracked.double(1.0)
			)
		)

process.puppi = cms.EDProducer("PuppiProducer",
		PuppiName      = cms.untracked.string("Puppi"),
		UseDeltaZCut   = cms.untracked.bool  (True),
		DeltaZCut      = cms.untracked.double(0.2),
		#candName       = cms.untracked.string('particleFlow'),
		#vertexName     = cms.untracked.string('offlinePrimaryVertices'),
		candName      = cms.untracked.string('packedPFCandidates'),
		vertexName     = cms.untracked.string('offlineSlimmedPrimaryVertices'),
		applyCHS       = cms.untracked.bool  (True),
		useExp         = cms.untracked.bool  (False),
		MinPuppiWeight = cms.untracked.double(0.01),
		algos          = cms.VPSet( 
			cms.PSet( 
				etaMin = cms.untracked.double(0.),
				etaMax = cms.untracked.double( 2.5),
				ptMin  = cms.untracked.double(0.),
				MinNeutralPt   = cms.untracked.double(0.2),
				MinNeutralPtSlope   = cms.untracked.double(0.02),
				puppiAlgos = puppiCentral
				),
			cms.PSet( 
				etaMin = cms.untracked.double(2.5),
				etaMax = cms.untracked.double(3.0),
				ptMin  = cms.untracked.double(0.0),
				MinNeutralPt        = cms.untracked.double(1.0),
				MinNeutralPtSlope   = cms.untracked.double(0.005),
				puppiAlgos = puppiForward
				),
			cms.PSet( 
				etaMin = cms.untracked.double(3.0),
				etaMax = cms.untracked.double(10.0),
				ptMin  = cms.untracked.double(0.0),
				MinNeutralPt        = cms.untracked.double(1.5),
				MinNeutralPtSlope   = cms.untracked.double(0.005),
				puppiAlgos = puppiForward
				)

			)
			)

PFJetParameters = cms.PSet(
					srcPVs = cms.InputTag(''),
					jetType = cms.string('PFJet'),
					doOutputJets = cms.bool(True),
					jetPtMin = cms.double(3.0),
					inputEMin = cms.double(0.0),
					inputEtMin = cms.double(0.0),
					doPVCorrection = cms.bool(False),
# pileup with offset correction
					doPUOffsetCorr = cms.bool(False),
# if pileup is false, these are not read:
					nSigmaPU = cms.double(1.0),
					radiusPU = cms.double(0.5),
# fastjet-style pileup
					doAreaFastjet = cms.bool( False),
					doRhoFastjet = cms.bool( False),
					doAreaDiskApprox = cms.bool( False),
					Active_Area_Repeats = cms.int32( 1),
					GhostArea = cms.double(0.01),
					Ghost_EtaMax = cms.double( 5.0),
					Rho_EtaMax = cms.double( 4.4),
					voronoiRfact = cms.double(-0.9),
					useDeterministicSeed= cms.bool( True ),
					minSeed = cms.uint32( 14327 )
	)

process.ak4PFJets = cms.EDProducer(
			"FastjetJetProducer",
			PFJetParameters,
			AnomalousCellParameters,
			src = cms.InputTag('puppi','Puppi'),
			jetAlgorithm = cms.string("AntiKt"),
			rParam = cms.double(0.4)
			)

process.load("flashgg/MicroAODProducers/flashggVertexMaps_cfi")
process.load("flashgg/MicroAODProducers/flashggPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggPreselectedDiPhotons_cfi")
process.load("flashgg/MicroAODProducers/flashggJets_cfi")

#Tag stuff
process.load("flashgg/TagProducers/flashggDiPhotonMVA_cfi")
process.load("flashgg/TagProducers/flashggVBFMVA_cfi")
process.load("flashgg/TagProducers/flashggVBFDiPhoDiJetMVA_cfi")
process.load("flashgg/TagProducers/flashggTags_cfi")

process.flashggTagSorter = cms.EDProducer('FlashggTagSorter',
		DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
		TagVectorTag = cms.untracked.VInputTag(
			cms.untracked.InputTag('flashggVBFTag'),
			cms.untracked.InputTag('flashggUntaggedCategory'),
			),
		massCutUpper=cms.untracked.double(180),
		massCutLower=cms.untracked.double(100)

		)


process.TFileService = cms.Service("TFileService",fileName = cms.string("flashggTreeWithTags.root"))
#process.flashggTreeMakerWithTags = cms.EDAnalyzer('FlashggFlashggTreeMakerWithTags',
#                                                          VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
#                                                          GenParticleTag=cms.untracked.InputTag('prunedGenParticles'),
#                                                          VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
#                                                          VertexCandidateMapTagAOD = cms.InputTag('flashggVertexMapValidator'),
#                                                          JetTagDz = cms.InputTag("flashggJets"),
#																													DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
#																													METTag = cms.untracked.InputTag('slimmedMETs'),
#																													PileUpTag = cms.untracked.InputTag('addPileupInfo'),
#																													UntaggedTag = cms.untracked.InputTag('flashggUntaggedCategory'),
#																													VBFTag = cms.untracked.InputTag('flashggVBFTag'),
#																													rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
#                                                          )
process.flashggTreeMakerWithTagSorter = cms.EDAnalyzer('FlashggFlashggTreeMakerWithTagSorter',
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices'),
		GenParticleTag=cms.untracked.InputTag('prunedGenParticles'),
		VertexCandidateMapTagDz=cms.InputTag('flashggVertexMapUnique'),
		VertexCandidateMapTagAOD = cms.InputTag('flashggVertexMapValidator'),
		JetTagDz = cms.InputTag("flashggJets"),
		DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
		METTag = cms.untracked.InputTag('slimmedMETs'),
		PileUpTag = cms.untracked.InputTag('addPileupInfo'),
		TagSorter = cms.untracked.InputTag('flashggTagSorter'),
		rhoFixedGridCollection = cms.InputTag('fixedGridRhoAll'),
		)



process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myOutputFile.root'),
		#   outputCommands = cms.untracked.vstring("drop *",
			#                                         "keep *_flashgg*_*_*",
			#                                        "drop *_flashggVertexMap*_*_*",
			#                                       "keep *_offlineSlimmedPrimaryVertices_*_*",
			#                                      "keep *_reducedEgamma_reduced*Clusters_*",
			#                                     "keep *_reducedEgamma_*PhotonCores_*",
			#                                    "keep *_slimmedElectrons_*_*",
			#                                   "keep *_slimmedMuons_*_*",
			#                                  "keep *_slimmedMETs_*_*",
			#                                 "keep *_slimmedTaus_*_*",
			#                                "keep *_fixedGridRhoAll_*_*"
			#                              )
		outputCommands = cms.untracked.vstring("keep *",
			)
		)

process.commissioning = cms.EDAnalyzer('flashggCommissioning',
		PhotonTag=cms.untracked.InputTag('flashggPhotons'),
		DiPhotonTag = cms.untracked.InputTag('flashggDiPhotons'),
		VertexTag=cms.untracked.InputTag('offlineSlimmedPrimaryVertices')
		)

process.TFileService = cms.Service("TFileService",
		fileName = cms.string("regular_tree.root")
		)

process.p = cms.Path(process.flashggVertexMapUnique*
		process.flashggVertexMapNonUnique*
		process.flashggPhotons*
		process.flashggDiPhotons*
		process.flashggPreselectedDiPhotons*
	#	process.puppi*
#		process.ak4PFJets*
    (process.flashggDiPhotonMVA+process.flashggJets)* # These two could run in parallel, so use +
		process.flashggUntaggedCategory*
		(process.flashggVBFMVA)* # Needs to happen after Jets
		(process.flashggVBFDiPhoDiJetMVA)* # Needs to happen after VBF MVA and DiPho MVA
		(process.flashggVBFTag)* # Tag producers, once written, can run in parallel, so they go in here with +
		process.flashggTagSorter*
		process.flashggTreeMakerWithTagSorter
		#process.commissioning*
		)

process.e = cms.EndPath(process.out)
