import FWCore.ParameterSet.Config as cms

process = cms.Process('MVAMET')
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')


process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = cms.string("PHYS14_25_V2")


option = 'RECO' # 'GEN' or 'RECO'

### GEN level studies
if option == 'GEN':
    process.load("ExoDiBosonResonances.EDBRGenStudies.genMuons_cff")
    process.load("ExoDiBosonResonances.EDBRGenStudies.genElectrons_cff")
    process.load("ExoDiBosonResonances.EDBRGenStudies.genFatJets_cff")

### RECO level studies
if option == 'RECO':
    process.load("ExoDiBosonResonances.EDBRCommon.goodMuons_cff")
    process.load("ExoDiBosonResonances.EDBRCommon.goodElectrons_cff")
    process.load("ExoDiBosonResonances.EDBRJets.redoSubstructure_cff")
    process.load("ExoDiBosonResonances.EDBRJets.redoPatJets_cff")
    process.load("ExoDiBosonResonances.EDBRCommon.goodJets_cff")
    process.load("ExoDiBosonResonances.EDBRCommon.goodAK4Jets_cff")


# Updates
if option == 'RECO':
    process.goodMuons.src = "slimmedMuons"
    process.goodElectrons.src = "isolatedElectrons"
    process.goodJets.src = "slimmedJetsAK8"
    process.goodAK4Jets.src = "slimmedJets"

process.leptonSequence = cms.Sequence(process.muSequence +
                                      process.eleSequence)



process.jetSequence = cms.Sequence(process.fatJetsSequence + process.substructureSequence + process.redoPatJets + process.ak4JetsSequence)




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


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)



# Input source
process.source = cms.Source("PoolSource",
                            secondaryFileNames = cms.untracked.vstring(),
                            fileNames = cms.untracked.vstring(
        '/store/mc/Phys14DR/DYJetsToLL_M-50_13TeV-madgraph-pythia8/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/0432E62A-7A6C-E411-87BB-002590DB92A8.root'
        #'/store/relval/CMSSW_7_4_0/RelValProdQCD_Pt_3000_3500_13/MINIAODSIM/MCRUN2_74_V7_GENSIM_7_1_15-v1/00000/08D1D655-7BDE-E411-8402-0025905A6060.root'   
        #'file:step3_miniAOD_M1500_9.root'
        #  'root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_9.root'
        #"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_1.root",
        #"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_10.root",
        #"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_11.root",
        #"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_12.root",
        #"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_13.root"
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_14.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_15.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_16.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_17.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_18.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_19.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_2.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_20.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_21.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_22.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_23.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_24.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_25.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_26.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_27.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_28.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_29.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_3.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_30.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_4.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_5.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_6.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_7.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_8.root",
        ##"root://eoscms.cern.ch//eos/cms/store/user/khurana/MonoHStep3/step3_miniAOD_M1500_9.root"
        ),
                            skipEvents = cms.untracked.uint32(0),
                            duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
                            )


process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)



process.load("RecoJets.JetProducers.ak4PFJets_cfi")
process.ak4PFJets.src = cms.InputTag("packedPFCandidates")

from JetMETCorrections.Configuration.DefaultJEC_cff import ak4PFJetsL1FastL2L3

process.load("RecoMET.METPUSubtraction.mvaPFMET_cff")
#process.pfMVAMEt.srcLeptons = cms.VInputTag("slimmedElectrons")
process.pfMVAMEt.srcPFCandidates = cms.InputTag("packedPFCandidates")
process.pfMVAMEt.srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices")

process.puJetIdForPFMVAMEt.jec =  cms.string('AK4PF')
#process.puJetIdForPFMVAMEt.jets = cms.InputTag("ak4PFJets")
process.puJetIdForPFMVAMEt.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")
process.puJetIdForPFMVAMEt.rho = cms.InputTag("fixedGridRhoFastjetAll")



# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, 'PHYS14_25_V1', 'All')

from RecoMET.METProducers.PFMET_cfi import pfMet
process.pfMet = pfMet.clone(src = "packedPFCandidates")
process.pfMet.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment


process.tree = cms.EDAnalyzer(
    'TreeMaker',
    fillPUweightInfo_ = cms.bool(True),
    fillEventInfo_ = cms.bool(True),
    fillGenInfo_   = cms.bool(True),
    fillMuonInfo_  = cms.bool(True),
    fillElecInfo_  = cms.bool(True),
    fillJetInfo_   = cms.bool(True), ## Raman switched it off
    fillMetInfo_   = cms.bool(True),
    fillTrigInfo_  = cms.bool(True),
    fillPhotInfo_  = cms.bool(False),
    fillTauInfo_   = cms.bool(True),
    
    genPartLabel=cms.InputTag("prunedGenParticles"),
    genJetLabel=cms.InputTag("slimmedGenJets"),
    maxNumGenPar  =  cms.uint32(30),
    patMuons=cms.InputTag("slimmedMuons"),
    patElectrons = cms.InputTag("slimmedElectrons"),
    
    #PrunedJets=cms.InputTag("ak8PFJetsCHSPrunedLinks"),
    #PrunedJets=cms.InputTag("slimmedJetsAK8"),
    
    pvSrc  = cms.InputTag('offlineSlimmedPrimaryVertices'),
    tauLabel_ = cms.untracked.InputTag("slimmedTaus"),
    rhoSrc = cms.InputTag('kt6PFJets','rho'),
    ### CA8Jet
    CA8Jets=cms.InputTag("slimmedJetsAK8"),
    CA8jecPayloadNames = cms.vstring( CA8jecLevels ),
    CA8jecUncName = cms.string(CA8jecUnc),    
    ### AK5Jet
    #AK5Jets=cms.InputTag("ak8PFJetsCHS"),
    AK5Jets=cms.InputTag("slimmedJets"),
    AK5jecPayloadNames = cms.vstring( AK5jecLevels ),
    AK5jecUncName = cms.string(AK5jecUnc),    
    
    patMetRaw=cms.InputTag("pfMet"),
    patMet = cms.InputTag("slimmedMETs"),
    #patMet = cms.InputTag("pfMet"),
    #patMetRaw=cms.InputTag("slimmedMETs"),
    outFileName=cms.string('outputFileName.root')
    )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("mvamet.root")
                                   )





process.analysis = cms.Path(process.leptonSequence+
                            process.jetSequence+                            
                            process.pfMVAMEtSequence+
                            process.pfMet+
                            process.tree
                            )


#print process.dumpPython()
