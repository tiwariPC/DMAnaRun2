import FWCore.ParameterSet.Config as cms
tree = cms.EDAnalyzer(
    'TreeMaker',
    fillPUweightInfo = cms.bool(True),
    fillEventInfo    = cms.bool(True),
    fillMetInfo      = cms.bool(True),
    fillTrigInfo     = cms.bool(True),
    fillFilterInfo   = cms.bool(True),

    fillGenInfo      = cms.bool(True),

    fillElecInfo     = cms.bool(True), 
    fillMuonInfo     = cms.bool(True),
    fillTauInfo      = cms.bool(True),
    fillPhotInfo     = cms.bool(True),

    fillJetInfo      = cms.bool(True), 
    fillFATJetInfo   = cms.bool(True), 
    fillAddJetInfo   = cms.bool(True),

    pvSrc            = cms.InputTag('offlineSlimmedPrimaryVertices'),

    patMetRaw        = cms.InputTag("pfMet"),
    patMet           = cms.InputTag("slimmedMETs"),
    pfMetRaw         = cms.InputTag("pfMet"),
    pfType1Met       = cms.InputTag("slimmedMETs"),
    pfMVAMET         = cms.InputTag("pfMVAMEt"),

    triggerLabel     = cms.InputTag("TriggerResults::HLT"),
    filterLabel      = cms.InputTag("TriggerResults::PAT"),

    genPartLabel         = cms.InputTag("prunedGenParticles"),
    genJetLabel          = cms.InputTag("slimmedGenJets"),
    maxNumGenPar         =  cms.uint32(30),
    applyStatusSelection = cms.bool(True),
    applyPromptSelection = cms.bool(False),
    saveLHEWeights       = cms.bool(True),
##### when applyPromptSelection is True
#    maxNumGenPar  =  cms.uint32(60), 
    
    ## For electron and muon
    r_iso_min    = cms.double(0.05),
    r_iso_max    = cms.double(0.2),
    kt_scale     = cms.double(10.),
    charged_only = cms.bool(False),
    pfForMiniIso = cms.InputTag("packedPFCandidates"),

    ### Electrons
    eleLabel       = cms.InputTag("slimmedElectrons"),
    eleVetoIdMap   = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto"),
    eleLooseIdMap  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose"),
    eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium"),
    eleTightIdMap  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight"),
    eleHEEPIdMap   = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
    #
    # ID decisions (common to all formats)
    #
    eleMVAMediumIdMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-nonTrig-V1-wp90"),
    eleMVATightIdMap  = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-nonTrig-V1-wp80"),
    #
    # ValueMaps with MVA results
    #
    mvaValuesMap     = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values"),
    mvaCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Categories"),    

    muoLabel     = cms.InputTag("slimmedMuons"),
    tauLabel     = cms.untracked.InputTag("slimmedTaus"),

    ## Photons
    photonLabel  = cms.InputTag("slimmedPhotons"),
    
    phoLooseIdMap        = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-loose"),
    phoMediumIdMap       = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-medium"),
    phoTightIdMap        = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring15-25ns-V1-standalone-tight"),
    phoMVAValuesMapToken = cms.InputTag("photonMVAValueMapProducer:PhotonMVAEstimatorRun2Spring15NonTrig25nsV2Values"),
    
    phoChargedIsolationToken       = cms.InputTag("photonIDValueMapProducer:phoChargedIsolation"),
    phoNeutralHadronIsolationToken = cms.InputTag("photonIDValueMapProducer:phoNeutralHadronIsolation"),
    phoPhotonIsolationToken        = cms.InputTag("photonIDValueMapProducer:phoPhotonIsolation"),
    ## AllJet
    useJECText = cms.bool(False),
    
    ### THINJet
#    THINJets=cms.InputTag("slimmedJets"),
    THINJets         = cms.InputTag("patJetsReapplyJECAK4"),
    THINjecNames     = cms.vstring(
        'Summer15_25nsV6_MC_L1FastJet_AK4PFchs.txt',
        'Summer15_25nsV6_MC_L2Relative_AK4PFchs.txt',
        'Summer15_25nsV6_MC_L3Absolute_AK4PFchs.txt'
        ),
    THINjecUncName   = cms.string('Summer15_25nsV6_MC_Uncertainty_AK4PFchs.txt'),
    THINjecUncPayLoad= cms.string('AK4PFchs'),
    # jec still need to be checked 
    
    ### FatJets  
#    FATJets=cms.InputTag("slimmedJetsAK8"),
    FATJets              = cms.InputTag("patJetsReapplyJECAK8"),
    FATJetsForPrunedMass = cms.InputTag("patJetsReapplyJECForPrunedMass"),
    FATprunedMassJecNames= cms.vstring(
        'Summer15_25nsV6_MC_L2Relative_AK8PFchs.txt',
        'Summer15_25nsV6_MC_L3Absolute_AK8PFchs.txt'
        ),
    FATjecNames          = cms.vstring(
        'Summer15_25nsV6_MC_L1FastJet_AK8PFchs.txt',
        'Summer15_25nsV6_MC_L2Relative_AK8PFchs.txt',
        'Summer15_25nsV6_MC_L3Absolute_AK8PFchs.txt'
        ),
    FATjecUncName        = cms.string('Summer15_25nsV6_MC_Uncertainty_AK8PFchs.txt'),
    FATjecUncPayLoad     = cms.string('AK8PFchs'), ## Uncertainty does not exist yet
    
    ### AddJets
    ADDJets              = cms.InputTag("packedPatJetsPFCHSAK8"),
    svTagInfosPY         = cms.string('pfInclusiveSecondaryVertexFinder'),    
    ADDjecNames          = cms.vstring(
        'Summer15_25nsV6_MC_L1FastJet_AK8PFchs.txt',
        'Summer15_25nsV6_MC_L2Relative_AK8PFchs.txt',
        'Summer15_25nsV6_MC_L3Absolute_AK8PFchs.txt'
        ),
    ADDjecUncName        = cms.string('Summer15_25nsV6_MC_Uncertainty_AK8PFchs.txt'),
    ADDjecUncPayLoad     = cms.string('AK8PFchs'), ## Uncertainty does not exist yet
    
    outFileName=cms.string('outputFileName.root')

)
