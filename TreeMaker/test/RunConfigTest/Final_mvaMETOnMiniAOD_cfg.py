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




process.GlobalTag.globaltag = cms.string("PHYS14_25_V1::All")

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

process.jetSequence = cms.Sequence(process.fatJetsSequence + process.ak4JetsSequence)


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)



# Input source
process.source = cms.Source("PoolSource",
                            secondaryFileNames = cms.untracked.vstring(),
                            fileNames = cms.untracked.vstring(
        #'file:../../../CSA14/CMSSW_7_2_3_patch1/src/E6E81FBF-5769-E411-AD3B-001E6739801B.root'
        'file:/afs/cern.ch/work/s/syu/public/miniAOD/RSGravToZZ_kMpl01_M-4500_Tune4C_13TeV-pythia8_Phys14DR.root'
        #'root://eoscms//eos/cms/store/relval/CMSSW_7_2_1/RelValZEE_13/MINIAODSIM/PU25ns_PHYS14_25_V1_Phys14-v1/00000/C6B792AB-D15E-E411-A787-02163E00F4FB.root'
        ),
                            skipEvents = cms.untracked.uint32(0)         
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
process.load("DelPanj.TreeMaker.TreeMaker_cfi")

process.tree.CA8Jets=cms.InputTag("cleanJets")
process.tree.AK5Jets=cms.InputTag("cleanAK4Jets")
process.tree.patMetRaw=cms.InputTag("slimmedMETs")
process.tree.patMet = cms.InputTag("slimmedMETs")

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("mvamet.root")
                                   )





process.analysis = cms.Path(process.leptonSequence+
                            process.jetSequence+                            
                            process.pfMVAMEtSequence+
                            process.tree
                            )


