#!/usr/bin/env fwliteRun

import FWCore.ParameterSet.Config as cms

import FWCore.ParameterSet.VarParsing as VarParsing
from flashgg.MetaData.samples_utils import SamplesManager
        
process = cms.Process("FWLitePlots")

process.fwliteInput = cms.PSet(
    fileNames = cms.vstring(),
    maxEvents   = cms.int32(100),
    outputEvery = cms.uint32(1),
)


process.fwliteOutput = cms.PSet(
      fileName = cms.string("output.root")      ## mandatory
)

process.load("flashgg.TagProducers.flashggTagSequence_cfi")
#process.load("flashgg.TagProducers.flashggVBFMVA_cff")
##from flashgg.TagProducers.flashggTags_cff import *
##from flashgg.TagProducers.flashggTagSorter_cfi import flashggTagSorter

#process.flashggTagSequence = cms.Sequence(flashggDiPhotonMVA*
                                 # flashggVBFMVA
                                  #flashggVBFDiPhoDiJetMVA*
                                  #(flashggUntaggedCategory+flashggVBFTag+flashggTTHleptonicTag+flashggTTHhadronicTag+flashggVHlooseTag+flashggVHtightTag+flashggVHhadronicTag)*
                                  #flashggTagSorter
                                #  )

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

