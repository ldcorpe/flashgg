import FWCore.ParameterSet.Config as cms

flashggTagTester = cms.EDAnalyzer('FlashggTagTestAnalyzer',
                                  #TagSorter = cms.InputTag('flashggTagSorter'),
                                  TagSorter = cms.InputTag('flashggSystTagMerger'),
                                  )
