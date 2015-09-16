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

    pvSrc  = cms.InputTag('offlineSlimmedPrimaryVertices'),

    patMetRaw=cms.InputTag("pfMet"),
    patMet = cms.InputTag("slimmedMETs"),
    pfMetRaw   = cms.InputTag("pfMet"),
    pfType1Met = cms.InputTag("slimmedMETs"),
    pfMVAMET   = cms.InputTag("pfMVAMEt"),

    triggerLabel   = cms.InputTag("TriggerResults::HLT"),
    filterLabel    = cms.InputTag(filterlabel),

    genPartLabel=cms.InputTag("prunedGenParticles"),
    genJetLabel=cms.InputTag("slimmedGenJets"),
    maxNumGenPar  =  cms.uint32(30),
    applyStatusSelection = cms.bool(True),
    applyPromptSelection = cms.bool(False),
    ##### when applyPromptSelection is True
    #    maxNumGenPar  =  cms.uint32(60), 
    
    ## For electron and muon
    r_iso_min    = cms.double(0.05),
    r_iso_max    = cms.double(0.2),
    kt_scale     = cms.double(10.),
    charged_only = cms.bool(False),
    pfForMiniIso = cms.InputTag("packedPFCandidates"),

    ### Electrons
    eleLabel = cms.InputTag("slimmedElectrons"),
    eleVetoIdMap = cms.InputTag  ("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-veto"),
    eleLooseIdMap = cms.InputTag ("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-loose"),
    eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-medium"),
    eleTightIdMap = cms.InputTag ("egmGsfElectronIDs:cutBasedElectronID-Spring15-25ns-V1-standalone-tight"),
    eleHEEPIdMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
    #
    # ID decisions (common to all formats)
    #
    eleMVAMediumIdMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-nonTrig-V1-wp90"),
    eleMVATightIdMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-Spring15-25ns-nonTrig-V1-wp80"),
    #
    # ValueMaps with MVA results
    #
    mvaValuesMap     = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Values"),
    mvaCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Spring15NonTrig25nsV1Categories"),    

    muoLabel     = cms.InputTag("slimmedMuons"),
    tauLabel     = cms.untracked.InputTag("slimmedTaus"),

    ## Photons
    phoLooseIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-loose"),
    phoMediumIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-medium"),
    phoTightIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-tight"),
    photonLabel  = cms.InputTag("slimmedPhotons"),

    ### THINJet
    #    THINJets=cms.InputTag("slimmedJets"),
    THINJets=cms.InputTag("patJetsReapplyJECAK4"),
    THINjecUncPayLoad=cms.string('AK4PFchs'),
    # jec still need to be checked 
    
    ### FatJets  
    #    FATJets=cms.InputTag("slimmedJetsAK8"),
    FATJets=cms.InputTag("patJetsReapplyJECAK8"),
    FATJetsForPrunedMass=cms.InputTag("patJetsReapplyJECForPrunedMass"),
    puppiPrunedMassJet=cms.InputTag("selectedPatJetsPuppiPrunedPFlow"), ## from addJetCollection
    puppiSoftDropMassJet=cms.InputTag("selectedPatJetsPuppiSoftDropPFlow"), ## from addJetCollection
    ATLASTrimMassJetLabel=cms.InputTag("selectedPatJetsATLASTrimmedPFlow"),
    FATjecUncPayLoad=cms.string('AK8PFchs'), ## Uncertainty does not exist yet
    
    ### AddJets
    ADDJets= cms.InputTag("packedPatJetsPFCHSAK8"),
    svTagInfosPY  = cms.string('pfInclusiveSecondaryVertexFinder'),    
    ADDjecUncPayLoad=cms.string('AK8PFchs'), ## Uncertainty does not exist yet
    
    outFileName=cms.string('outputFileName.root')
)
