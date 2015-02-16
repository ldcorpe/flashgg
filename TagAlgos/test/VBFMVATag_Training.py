import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils


import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.samples_utils import SamplesManager

process = cms.Process("TestVBFMVATraining")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'POSTLS170_V5::All'
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring())
#process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32( 100 )

# Uncomment the following if you notice you have a memory leak
# This is a lightweight tool to digg further
#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#                                        ignoreTotal = cms.untracked.int32(1),
#                                        monitorPssAndPrivate = cms.untracked.bool(True)
#                                       )
#process.source = cms.Source ("PoolSource",fileNames = cms.untracked.vstring(""))

process.load("flashgg/TagProducers/flashggTagSequence_cfi")
process.load("flashgg/TagProducers/flashggTagTester_cfi")

from flashgg.TagProducers.flashggTagOutputCommands_cff import tagDefaultOutputCommand

process.out = cms.OutputModule("PoolOutputModule", fileName = cms.untracked.string('myTagOutputFile.root'),
                               outputCommands = tagDefaultOutputCommand			       
                               )

import flashgg.TagAlgos.dumperConfigTools as cfgTools

process.load("flashgg/TagProducers/VBFMVADumper_cff")

process.TFileService = cms.Service("TFileService",
      fileName = cms.string("histo.root"),
			      closeFileFast = cms.untracked.bool(True)
						  )

#process.VBFMVADumper = VBFMVATrainingDumpConf.clone()
process.VBFMVADumper.dumpTrees = True

process.VBFMVADumper.dumpWorkspace = False

process.VBFMVADumper.quietRooFit = True
#cfgTools.addCategory(process.VBFMVADumper,
#		"Reject", ## cuts are applied in cascade
#		"VBFMVAValue<-1",
#		-1)
cfgTools.addCategories(process.VBFMVADumper,
		[## cuts are applied in cascade
		("All","1",0),
		],
		variables=[
		"dijet_abs_dEta   :=  dijet_abs_dEta  ",
		"dijet_leadEta    :=  dijet_leadEta  ",
		"dijet_subleadEta :=  dijet_subleadEta  ",
		"dijet_LeadJPt    :=  dijet_LeadJPt    ",
		"dijet_SubJPt     :=  dijet_SubJPt     ",
		"dijet_Zep        :=  dijet_Zep        ",
		"dijet_Mjj        :=  dijet_Mjj        ",
		"dipho_PToM       :=  dipho_PToM     ",
		"leadPho_PToM     :=  leadPho_PToM     ",
		"sublPho_PToM     :=  sublPho_PToM     ",
		"dijet_dPhi_trunc :=  dijet_dPhi_trunc ",
		],
		histograms=[
		"mvaresult>>VBFMVAValue(100,-1,1)",
		]
		)
# split tree, histogram and datasets by process
process.VBFMVADumper.nameTemplate ="$PROCESS_$SQRTS_$LABEL_$SUBCAT"
## do not split by process
## process.diphotonDumper.nameTemplate = "minitree_$SQRTS_$LABEL_$SUBCAT"

## define categories and associated objects to dump
#cfgTools.addCategory(process.diphotonDumper,
#		"Reject",
		# "abs(leadingPhoton.superCluster.eta)>=1.4442&&abs(leadingPhoton.superCluster.eta)<=1.566||abs(leadingPhoton.superCluster.eta)>=2.5"
		# "||abs(subLeadingPhoton.superCluster.eta)>=1.4442 && abs(subLeadingPhoton.superCluster.eta)<=1.566||abs(subLeadingPhoton.superCluster.eta)>=2.5",
#		"0",
#		-1 ## if nSubcat is -1 do not store anythings
#		)

# interestng categories




# customization for job splitting, lumi weighting, etc.
from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",500)
customize.setDefault("targetLumi",1.e+4)
customize(process)
process.p = cms.Path(process.flashggTagSequence*
			process.VBFMVADumper
			)

process.e = cms.EndPath(process.out)
