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

from flashgg.TagAlgos.VBFMVATrainingDumpConf_cff import VBFMVATrainingDumpConf
import flashgg.TagAlgos.dumperConfigTools as cfgTools

process.analyzer = VBFMVATrainingDumpConf
process.analyzer.dumpTrees = True
process.analyzer.dumpWorkspace = False
process.analyzer.quietRooFit = True
#cfgTools.addCategory(process.analyzer,
#                     "Reject", ## cuts are applied in cascade
#                      "abs(leadingPhoton.superCluster.eta)>=1.4442&&abs(leadingPhoton.superCluster.eta)<=1.566||abs(leadingPhoton.superCluster.eta)>=2.5"
#                      "||abs(subLeadingPhoton.superCluster.eta)>=1.4442 && abs(subLeadingPhoton.superCluster.eta)<=1.566||abs(subLeadingPhoton.superCluster.eta)>=2.5",
#                      -1)
cfgTools.addCategories(process.analyzer,
                       [## cuts are applied in cascade
                        ("All","1",0),
                        ],
                       variables=["jet1pt:=dijet_LeadJPt", 
                                  "jet2pt:=dijet_LeadJPt",
                                  ],
                       histograms=[
                                   
                                   
                                   
                                   ]
                       )


# customization for job splitting, lumi weighting, etc.
from flashgg.MetaData.JobConfig import customize
customize.setDefault("maxEvents",500)
customize.setDefault("targetLumi",1.e+4)
customize(process)
process.p = cms.Path(process.flashggTagSequence*
										 process.analyzer
										)

process.e = cms.EndPath(process.out)
