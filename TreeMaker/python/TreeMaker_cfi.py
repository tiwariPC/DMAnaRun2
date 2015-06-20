import FWCore.ParameterSet.Config as cms

CA8jecUnc='jec/START53_V23_Uncertainty_AK7PFchs.txt'
CA8jecLevels = [
    'jec/START53_V23_L1FastJet_AK7PFchs.txt',
    'jec/START53_V23_L2Relative_AK7PFchs.txt',
    'jec/START53_V23_L3Absolute_AK7PFchs.txt'
    ]
AK5jecUnc='jec/START53_V23_Uncertainty_AK5PFchs.txt'
AK5jecLevels = [
    'jec/START53_V23_L1FastJet_AK5PFchs.txt',
    'jec/START53_V23_L2Relative_AK5PFchs.txt',
    'jec/START53_V23_L3Absolute_AK5PFchs.txt'
    ]

tree = cms.EDAnalyzer(
    'TreeMaker',
    fillPUweightInfo_    = cms.bool(True),
    fillEventInfo_       = cms.bool(True),
    fillGenInfo_         = cms.bool(True),
    fillMuonInfo_        = cms.bool(True),
    fillElecInfo_        = cms.bool(True),
    fillJetInfo_         = cms.bool(True), ## Raman switched it off
    fillFATJetInfo_      = cms.bool(True),
    fillAddJetInfo_      = cms.bool(True),    
    fillMetInfo_         = cms.bool(True),
    fillTrigInfo_        = cms.bool(True),
    fillPhotInfo_        = cms.bool(False),
    fillTauInfo_         = cms.bool(True),
    
    genPartLabel         = cms.InputTag("prunedGenParticles"),
    genJetLabel          = cms.InputTag("slimmedGenJets"),
    maxNumGenPar         = cms.uint32(30),
    applyStatusSelection = cms.bool(True), 
    patMuons             = cms.InputTag("slimmedMuons"),
    patElectrons         = cms.InputTag("slimmedElectrons"),
    
    #PrunedJets=cms.InputTag("ak8PFJetsCHSPrunedLinks"),
    #PrunedJets=cms.InputTag("slimmedJetsAK8"),
    
    pvSrc                = cms.InputTag('offlineSlimmedPrimaryVertices'),
    tauLabel_            = cms.untracked.InputTag("slimmedTaus"),
    rhoSrc               = cms.InputTag('kt6PFJets','rho'),
    ### CA8Jet
    CA8Jets              = cms.InputTag("slimmedJetsAK8"),
    CA8jecPayloadNames   = cms.vstring( CA8jecLevels ),
    CA8jecUncName        = cms.string(CA8jecUnc),    
    ### AK5Jet
    #AK5Jets=cms.InputTag("ak8PFJetsCHS"),
    AK5Jets              = cms.InputTag("slimmedJets"),
    AK5jecPayloadNames   = cms.vstring( AK5jecLevels ),
    AK5jecUncName        = cms.string(AK5jecUnc),    
    
    patMetRaw            = cms.InputTag("pfMet"),
    patMet               = cms.InputTag("slimmedMETs"),
    #patMet = cms.InputTag("pfMet"),
    #patMetRaw=cms.InputTag("slimmedMETs"),
    outFileName          = cms.string('outputFileName.root')
)
