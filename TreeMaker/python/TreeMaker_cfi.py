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
    
    pvSrc            = cms.InputTag('offlineSlimmedPrimaryVertices'),

    patMetRaw        = cms.InputTag("pfMet"),
    patMet           = cms.InputTag("slimmedMETs"),
    pfMetRaw         = cms.InputTag("pfMet"),
    pfType1Met       = cms.InputTag("slimmedMETs"),
    pfMVAMET         = cms.InputTag("slimmedMETs"),
    puppiMET         = cms.InputTag("slimmedMETsPuppi"),
    
    ## filter

    triggerLabel     = cms.InputTag("TriggerResults::HLT"),
    saveAllTrigPaths = cms.bool(False),
    filterLabel      = cms.InputTag("TriggerResults::PAT"),

    genPartLabel         = cms.InputTag("genParticles"),
    genJetLabel       = cms.InputTag("slimmedGenJets"),
    maxNumGenPar         =  cms.uint32(30),
    applyStatusSelection = cms.bool(True),
    applyPromptSelection = cms.bool(False),
    saveLHEWeights       = cms.bool(False),
    saveGenJets          = cms.bool(False),
    saveGenJetSub        = cms.bool(False),
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
    eleVetoIdMap   = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-veto"),
    eleLooseIdMap  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-loose"),
    eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-medium"),
    eleTightIdMap  = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Summer16-80X-V1-tight"),
    eleHEEPIdMap   = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
    #
    # ID decisions (common to all formats)
    #
    eleMVAMediumIdMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp90"),
    eleMVATightIdMap  = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring16-GeneralPurpose-V1-wp80"),
    #
    # ValueMaps with MVA results
    #
    mvaValuesMap     = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Values"),
    mvaCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring16GeneralPurposeV1Categories"),    

    muoLabel     = cms.InputTag("slimmedMuons"),
    tauLabel     = cms.untracked.InputTag("slimmedTaus"),

    ## Photons
    photonLabel  = cms.InputTag("slimmedPhotons"),
    
    phoLooseIdMap        = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-loose"),
    phoMediumIdMap       = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-medium"),
    phoTightIdMap        = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-Spring16-V2p2-tight"),
    phoMVAValuesMapToken = cms.InputTag("photonMVAValueMapProducer:PhotonMVAEstimatorRun2Spring16NonTrigV1Values"),
    
    phoChargedIsolationToken       = cms.InputTag("photonIDValueMapProducer:phoChargedIsolation"),
    phoNeutralHadronIsolationToken = cms.InputTag("photonIDValueMapProducer:phoNeutralHadronIsolation"),
    phoPhotonIsolationToken        = cms.InputTag("photonIDValueMapProducer:phoPhotonIsolation"),
    ## AllJet
    useJECText = cms.bool(False),
    
    ### THINJet
#    THINJets=cms.InputTag("slimmedJets"),
    THINJets         = cms.InputTag("patJetsReapplyJECAK4"),
    THINjecNames     = cms.vstring(
        'Summer16_23Sep2016V3_MC_L1FastJet_AK4PFchs.txt',
        'Summer16_23Sep2016V3_MC_L2Relative_AK4PFchs.txt',
        'Summer16_23Sep2016V3_MC_L3Absolute_AK4PFchs.txt'
        ),
    THINjecUncName   = cms.string('Summer16_23Sep2016V3_MC_Uncertainty_AK4PFchs.txt'),
    THINjecUncPayLoad= cms.string('AK4PFchs'),
    # jec still need to be checked 
    
    outFileName=cms.string('outputFileName.root')

)
