import FWCore.ParameterSet.Config as cms
## removed cleaning from Exo VV package 
## 

process = cms.Process('MVAMET')
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')

options.register ('runOnMC',
		  True,
		  VarParsing.multiplicity.singleton,
		  VarParsing.varType.bool,
		  "runOnMC")

options.register ('isReReco',
		  True,
		  VarParsing.multiplicity.singleton,
		  VarParsing.varType.bool,
		  "isReReco")

options.register ('useMiniAOD',
		  True,
		  VarParsing.multiplicity.singleton,
		  VarParsing.varType.bool,
		  "useMiniAOD")

options.register ('useJECText',
		  True,
		  VarParsing.multiplicity.singleton,
		  VarParsing.varType.bool,
		  "useJECText")

options.register ('period',
		  'p2',
		  VarParsing.multiplicity.singleton,
		  VarParsing.varType.string,
		  "period")


options.register ('textfiletovetoEvents',
		  'MET_Oct29/eventlist_MET_csc2015.txt',
		  VarParsing.multiplicity.singleton,
		  VarParsing.varType.string,
		  "textfiletovetoEvents")

options.parseArguments()



listEventsToSkip = []
## Only apply this for data
#if not options.runOnMC:
#    fileEventsToSkip = open(options.textfiletovetoEvents,"r")
#    for line in fileEventsToSkip:
#        cleanLine = line.rstrip()
#        listEventsToSkip.append(cleanLine+"-"+cleanLine)

#print listEventsToSkip

MCJEC='Spring16_25nsV10_MC'
DATAJEC='Spring16_25nsV10'+options.period+'_DATA'

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
if options.runOnMC:
	process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_miniAODv2_v1', '')  
else:## Data no global tag yet
        process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_Prompt_ICHEP16JEC_v0', '')



## for Filters

if options.runOnMC:
	filterlabel="TriggerResults::PAT"
else:
	## for re-reco
	if options.isReReco:
		filterlabel="TriggerResults::RECO"
	## for prompt-reco
	else:
		filterlabel="TriggerResults::PAT"


'''
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.HBHENoiseFilterResultProducer.minZeros = cms.int32(99999)
'''

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

'''
##___________________________HCAL_Noise_Filter________________________________||
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.HBHENoiseFilterResultProducer.minZeros = cms.int32(99999)
process.HBHENoiseFilterResultProducer.IgnoreTS4TS5ifJetInLowBVRegion=cms.bool(False) 
#process.HBHENoiseFilterResultProducer.defaultDecision = cms.string("HBHENoiseFilterResultRun2Loose")
'''
##process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
##   inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
##   reverseDecision = cms.bool(False)
##)
##
##process.ApplyBaselineHBHEIsoNoiseFilter = cms.EDFilter('BooleanFlagFilter',
##   inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHEIsoNoiseFilterResult'),
##   reverseDecision = cms.bool(False)
##)
##


# Input source
if options.runOnMC:
	testFile='/store/mc/RunIISpring16MiniAODv2/ZprimeToA0hToA0chichihbb_2HDM_MZp-1000_MA0-300_13TeV-madgraph/MINIAODSIM/PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/80000/1C0089C1-163B-E611-9931-FA163EA5733B.root'
else:
	testFile='/store/data/Run2016B/SingleMuon/MINIAOD/PromptReco-v2/000/273/158/00000/02D9C19F-571A-E611-AD8E-02163E013732.root'

process.source = cms.Source("PoolSource",
                            secondaryFileNames = cms.untracked.vstring(),
                            fileNames = cms.untracked.vstring(testFile),
			    #skipEvents = cms.untracked.uint32(0)         
                            )




## skip the events 
## using the MET tails events
if options.runOnMC:
    print "No events to skip"
else:
    rangeEventsToSkip = cms.untracked.VEventRange(listEventsToSkip)
    process.source.eventsToSkip = rangeEventsToSkip

## For MVA MET
## Check this reciepe when using MVA MET
process.load("RecoJets.JetProducers.ak4PFJets_cfi")
process.ak4PFJets.src = cms.InputTag("packedPFCandidates")

from JetMETCorrections.Configuration.DefaultJEC_cff import ak4PFJetsL1FastL2L3

process.load("RecoMET.METPUSubtraction.mvaPFMET_cff")
#process.pfMVAMEt.srcLeptons = cms.VInputTag("slimmedElectrons")
process.pfMVAMEt.srcPFCandidates = cms.InputTag("packedPFCandidates")
process.pfMVAMEt.srcVertices = cms.InputTag("offlineSlimmedPrimaryVertices")
process.pfMVAMEt.inputFileNames = cms.PSet(
        U     = cms.FileInPath('RecoMET/METPUSubtraction/data/gbru_7_4_X_miniAOD_25NS_July2015.root'),
        DPhi  = cms.FileInPath('RecoMET/METPUSubtraction/data/gbrphi_7_4_X_miniAOD_25NS_July2015.root'),
        CovU1 = cms.FileInPath('RecoMET/METPUSubtraction/data/gbru1cov_7_4_X_miniAOD_25NS_July2015.root'),
        CovU2 = cms.FileInPath('RecoMET/METPUSubtraction/data/gbru2cov_7_4_X_miniAOD_25NS_July2015.root')
	)

process.puJetIdForPFMVAMEt.jec =  cms.string('AK4PF')
#process.puJetIdForPFMVAMEt.jets = cms.InputTag("ak4PFJets")
process.puJetIdForPFMVAMEt.vertexes = cms.InputTag("offlineSlimmedPrimaryVertices")
process.puJetIdForPFMVAMEt.rho = cms.InputTag("fixedGridRhoFastjetAll")


## MVA MET Ends Here 
##



##
## This is for Uncorrected MET
from RecoMET.METProducers.PFMET_cfi import pfMet
process.pfMet = pfMet.clone(src = "packedPFCandidates")
process.pfMet.calculateSignificance = False # this can't be easily implemented on packed PF candidates at the moment
## Uncorrected MET edns here 
##





### for adding jet collection

process.packedGenParticlesForJetsNoNu = cms.EDFilter("CandPtrSelector", src = cms.InputTag("packedGenParticles"), cut = cms.string("abs(pdgId) != 12 && abs(pdgId) != 14 && abs(pdgId) != 16"))
## produce Fat ak8 jets (Gen and Reco)
from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
process.genJetsNoNuAK8 = ak4GenJets.clone(
    jetAlgorithm = cms.string('AntiKt'),
    rParam = cms.double(0.8),
    src = cms.InputTag("packedGenParticlesForJetsNoNu")
)

'''
## produce Pruned ak8 fat jets (Gen and Reco) (each module produces two jet collections, fat jets and subjets)
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters

## Produce SoftDrop ak8 fat jets (Gen and Reco) (each module produces two jet collections, fat jets and subjets)
process.genJetsNoNuSoftDrop = ak4GenJets.clone(
    jetAlgorithm = cms.string('AntiKt'),
    rParam = cms.double(0.8),
    src = cms.InputTag("packedGenParticlesForJetsNoNu"),
    useSoftDrop = cms.bool(False),# because it crashes for PHYS14 samples
    zcut = cms.double(0.1),
    beta = cms.double(0.0),
    R0 = cms.double(0.8),
    writeCompound = cms.bool(True),
    jetCollInstanceName=cms.string("SubJets")
)
from RecoJets.JetProducers.ak4PFJetsSoftDrop_cfi import ak4PFJetsSoftDrop
process.PFJetsCHSSoftDrop = ak4PFJetsSoftDrop.clone(
    jetAlgorithm = cms.string('AntiKt'),
    rParam = cms.double(0.8),
    R0 = cms.double(0.8),
    src = getattr(process,"ak4PFJets").src,
    srcPVs = getattr(process,"ak4PFJets").srcPVs,
    doAreaFastjet = cms.bool(True),
    writeCompound = cms.bool(True),
    jetCollInstanceName=cms.string("SubJets"),
    jetPtMin = cms.double(170.0)
)
'''












postfix = "PFlow"
pfCandidates = 'packedPFCandidates'
jetSource = 'ak4PFJets'
pvSource = 'offlineSlimmedPrimaryVertices'
svSource = 'slimmedSecondaryVertices'
muSource = 'slimmedMuons'
elSource = 'slimmedElectrons'
genParticles = 'prunedGenParticles'
genJetCollection = 'ak4GenJetsNoNu'


bTagInfos = [
    'pfImpactParameterTagInfos'
   ,'pfSecondaryVertexTagInfos'
   ,'pfInclusiveSecondaryVertexFinderTagInfos'
   ,'softPFMuonsTagInfos'
   ,'softPFElectronsTagInfos'
]
bTagDiscriminators = [
     'pfJetBProbabilityBJetTags'
    ,'pfJetProbabilityBJetTags'
    ,'pfPositiveOnlyJetBProbabilityBJetTags'
    ,'pfPositiveOnlyJetProbabilityBJetTags'
    ,'pfNegativeOnlyJetBProbabilityBJetTags'
    ,'pfNegativeOnlyJetProbabilityBJetTags'
    ,'pfTrackCountingHighPurBJetTags'
    ,'pfTrackCountingHighEffBJetTags'
    ,'pfNegativeTrackCountingHighPurBJetTags'
    ,'pfNegativeTrackCountingHighEffBJetTags'
    ,'pfSimpleSecondaryVertexHighEffBJetTags'
    ,'pfSimpleSecondaryVertexHighPurBJetTags'
    ,'pfNegativeSimpleSecondaryVertexHighEffBJetTags'
    ,'pfNegativeSimpleSecondaryVertexHighPurBJetTags'
    ,'pfCombinedSecondaryVertexV2BJetTags'
    ,'pfPositiveCombinedSecondaryVertexV2BJetTags'
    ,'pfNegativeCombinedSecondaryVertexV2BJetTags'
    ,'pfCombinedInclusiveSecondaryVertexV2BJetTags'
    ,'pfPositiveCombinedInclusiveSecondaryVertexV2BJetTags'
    ,'pfNegativeCombinedInclusiveSecondaryVertexV2BJetTags'
    ,'softPFMuonBJetTags'
    ,'positiveSoftPFMuonBJetTags'
    ,'negativeSoftPFMuonBJetTags'
    ,'softPFElectronBJetTags'
    ,'positiveSoftPFElectronBJetTags'
    ,'negativeSoftPFElectronBJetTags'
]

## Jet energy corrections

## For jet energy correction
if options.runOnMC:
	jetCorrectionsAK4CHS       = ('AK4PFchs', ['L1FastJet','L2Relative', 'L3Absolute'], 'None')
	jetCorrectionsAK4Puppi     = ('AK4PFPuppi', ['L2Relative', 'L3Absolute'], 'None')
	jetCorrectionsAK8CHS       = ('AK8PFchs', ['L1FastJet','L2Relative', 'L3Absolute'], 'None')
	jetCorrectionsAK8CHSL23    = ('AK8PFchs', ['L2Relative', 'L3Absolute'], 'None')
	jetCorrectionsAK8Puppi     = ('AK8PFPuppi', ['L2Relative', 'L3Absolute'], 'None')
	jetCorrectionLevelsFullCHS = ['L1FastJet', 'L2Relative', 'L3Absolute']
	jetCorrectionLevels23CHS   = ['L2Relative', 'L3Absolute']
	jetCorrectionLevelsPuppi   = ['L2Relative', 'L3Absolute']

	AK4JECTextFiles = [
		MCJEC+'_L1FastJet_AK4PFchs.txt',
		MCJEC+'_L2Relative_AK4PFchs.txt',
		MCJEC+'_L3Absolute_AK4PFchs.txt'
		]
	AK4JECUncTextFile = MCJEC+'_Uncertainty_AK4PFchs.txt'

	AK8JECTextFiles = [
		MCJEC+'_L1FastJet_AK8PFchs.txt',
		MCJEC+'_L2Relative_AK8PFchs.txt',
		MCJEC+'_L3Absolute_AK8PFchs.txt'
		]
	AK8JECUncTextFile = MCJEC+'_Uncertainty_AK8PFchs.txt'  
	prunedMassJECTextFiles = [
		MCJEC+'_L2Relative_AK8PFchs.txt',
		MCJEC+'_L3Absolute_AK8PFchs.txt'
		]

	AK4PuppiJECTextFiles = [
		MCJEC+'_L2Relative_AK4PFPuppi.txt',
		MCJEC+'_L3Absolute_AK4PFPuppi.txt'
		]
	AK4PuppiJECUncTextFile = MCJEC+'_Uncertainty_AK4PFPuppi.txt'  

	AK8PuppiJECTextFiles = [
		MCJEC+'_L2Relative_AK8PFPuppi.txt',
		MCJEC+'_L3Absolute_AK8PFPuppi.txt'
		]
	AK8PuppiJECUncTextFile = MCJEC+'_Uncertainty_AK8PFPuppi.txt'  
else:
        jetCorrectionsAK4CHS       = ('AK4PFchs', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual'], 'None')
	jetCorrectionsAK4Puppi     = ('AK4PFPuppi', ['L2Relative', 'L3Absolute','L2L3Residual'], 'None')
	jetCorrectionsAK8CHS       = ('AK8PFchs', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual'], 'None')
	jetCorrectionsAK8CHSL23    = ('AK8PFchs', ['L2Relative', 'L3Absolute','L2L3Residual'], 'None')
	jetCorrectionsAK8Puppi     = ('AK8PFPuppi', ['L2Relative', 'L3Absolute','L2L3Residual'], 'None')
	jetCorrectionLevelsFullCHS = ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']
	jetCorrectionLevels23CHS   = ['L2Relative', 'L3Absolute','L2L3Residual']
	jetCorrectionLevelsPuppi   = ['L2Relative', 'L3Absolute','L2L3Residual']
	AK4JECTextFiles = [
		DATAJEC+'_L1FastJet_AK4PFchs.txt',
		DATAJEC+'_L2Relative_AK4PFchs.txt',
		DATAJEC+'_L3Absolute_AK4PFchs.txt',
		DATAJEC+'_L2L3Residual_AK4PFchs.txt'
		]
	AK4JECUncTextFile = DATAJEC+'_Uncertainty_AK4PFchs.txt'
	AK8JECTextFiles = [
		DATAJEC+'_L1FastJet_AK8PFchs.txt',
		DATAJEC+'_L2Relative_AK8PFchs.txt',
		DATAJEC+'_L3Absolute_AK8PFchs.txt',
		DATAJEC+'_L2L3Residual_AK8PFchs.txt'
		]
	AK8JECUncTextFile = DATAJEC+'_Uncertainty_AK8PFchs.txt'
	prunedMassJECTextFiles = [
		DATAJEC+'_L2Relative_AK8PFchs.txt',
		DATAJEC+'_L3Absolute_AK8PFchs.txt',
		DATAJEC+'_L2L3Residual_AK8PFchs.txt'
		]

	AK4PuppiJECTextFiles = [
		DATAJEC+'_L2Relative_AK4PFPuppi.txt',
		DATAJEC+'_L3Absolute_AK4PFPuppi.txt',
		DATAJEC+'_L2L3Residual_AK4PFPuppi.txt'
		]
	AK4PuppiJECUncTextFile = DATAJEC+'_Uncertainty_AK4PFPuppi.txt'

	AK8PuppiJECTextFiles = [
		DATAJEC+'_L2Relative_AK8PFPuppi.txt',
		DATAJEC+'_L3Absolute_AK8PFPuppi.txt',
		DATAJEC+'_L2L3Residual_AK8PFPuppi.txt'
		]
	AK8PuppiJECUncTextFile = DATAJEC+'_Uncertainty_AK8PFPuppi.txt'

from PhysicsTools.PatAlgos.tools.jetTools import *


NOTADDHBBTag=False
## Filter for good primary vertex
process.primaryVertexFilter = cms.EDFilter("GoodVertexFilter",
    vertexCollection = cms.InputTag(pvSource),
    minimumNDOF = cms.uint32(4) ,
    maxAbsZ = cms.double(24),
    maxd0 = cms.double(2)
)

#-------------------------------------
from PhysicsTools.PatAlgos.tools.pfTools import *
## Adapt primary vertex collection
adaptPVs(process, pvCollection=cms.InputTag(pvSource))



#### Add reclustered AK8 Puppi jet by Eiko

### For AK8 puppi (imported from Bacon)
#process.load("RecoBTag.ImpactParameter.impactParameter_cff")

#process.load('DelPanj.TreeMaker.myPUPPICorrections_cff')
#process.load('DelPanj.TreeMaker.myJetExtrasAK8Puppi_cff')

#from DelPanj.TreeMaker.myJetExtrasAK8Puppi_cff  import setMiniAODAK8Puppi
#from DelPanj.TreeMaker.myBtagging_cff           import addBTagging

#process.btagging = cms.Sequence()
#addBTagging(process,'AK8PFJetsPuppi' ,0.8,'AK8' ,'Puppi')

#setMiniAODAK8Puppi (process)

process.load('CommonTools/PileupAlgos/Puppi_cff')
process.puppi.candName       = cms.InputTag('packedPFCandidates')
process.puppi.vertexName     = cms.InputTag('offlineSlimmedPrimaryVertices')

from JMEAnalysis.JetToolbox.jetToolbox_cff import jetToolbox


### CA15Puppi
jetToolbox( process, 'ca15', 'jetSequence', 'out', PUMethod='Puppi', miniAOD=options.useMiniAOD, runOnMC=options.runOnMC, 
	    bTagDiscriminators=(bTagDiscriminators + ([] if NOTADDHBBTag else ['pfBoostedDoubleSecondaryVertexCA15BJetTags'])),
	    JETCorrPayload='AK8PFPuppi',JETCorrLevels=jetCorrectionLevelsPuppi, 
	    subJETCorrPayload='AK4PFPuppi',subJETCorrLevels=jetCorrectionLevelsPuppi, 
	    Cut='pt>120',
	    addSoftDrop=True,addSoftDropSubjets=True, betaCut=1.0, zCutSD=0.15,
	    addNsub=True ) 

### AK8Puppi
jetToolbox( process, 'ak8', 'jetSequence', 'out', PUMethod='Puppi', miniAOD=options.useMiniAOD, runOnMC=options.runOnMC, 
	    bTagDiscriminators=(bTagDiscriminators + ([] if NOTADDHBBTag else ['pfBoostedDoubleSecondaryVertexAK8BJetTags'])),
	    JETCorrPayload='AK8PFPuppi',JETCorrLevels=jetCorrectionLevelsPuppi, 
	    subJETCorrPayload='AK4PFPuppi',subJETCorrLevels=jetCorrectionLevelsPuppi, 
	    Cut='pt>170',
	    addSoftDrop=True,addSoftDropSubjets=True,addNsub=True ) 

### ADDjet for doubleb-tagger
jetToolbox( process, 'ak8', 'jetSequence', 'out', PUMethod='CHS', miniAOD=options.useMiniAOD, runOnMC=options.runOnMC,
	    bTagDiscriminators=(bTagDiscriminators + ([] if NOTADDHBBTag else ['pfBoostedDoubleSecondaryVertexAK8BJetTags'])),
	    JETCorrPayload="AK8PFchs", JETCorrLevels=jetCorrectionLevelsFullCHS,
	    Cut='pt>170')


###end of add jet collection
### FOR adding new hbb b-tags in miniaod?

#process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
#process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")

#from RecoJets.Configuration.RecoPFJets_cff import ak8PFJetsCHS




## add value maps for electron IDs
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate
if options.useMiniAOD:
	dataFormat = DataFormat.MiniAOD
else :
	dataFormat = DataFormat.AOD
    
switchOnVIDElectronIdProducer(process, dataFormat)
switchOnVIDPhotonIdProducer(process, dataFormat)
# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Summer16_80X_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff',
		 'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Spring16_GeneralPurpose_V1_cff']


#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

my_phoid_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Spring16_V2p2_cff',
		    'RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Spring16_nonTrig_V1_cff']

#add them to the VID producer
for idmod in my_phoid_modules:
	setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)




#process.egmPhotonIDs.physicsObjectSrc = cms.InputTag("ncuslimmedPhoton")
#process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag("ncuslimmedElectron")



## For normal AK4 jets jet energy correction on top of miniAOD
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJetCorrFactors
process.patJetCorrFactorsReapplyJECAK4 = updatedPatJetCorrFactors.clone(
	src = cms.InputTag("slimmedJets"),
	levels = jetCorrectionLevelsFullCHS,
	payload = 'AK4PFchs' ) # Make sure to choose the appropriate levels and payload here!

from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJets
process.patJetsReapplyJECAK4 = updatedPatJets.clone(
	jetSource = cms.InputTag("slimmedJets"),
	jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJECAK4"))
  )

process.jetCorrSequenceAK4 = cms.Sequence( process.patJetCorrFactorsReapplyJECAK4 + process.patJetsReapplyJECAK4 )




### For normal AK8 jet energy correction on top of miniAOD
process.patJetCorrFactorsReapplyJECAK8 = updatedPatJetCorrFactors.clone(
	src = cms.InputTag("slimmedJetsAK8"),
	levels = jetCorrectionLevelsFullCHS,
	payload = 'AK8PFchs' ) # Make sure to choose the appropriate levels and payload here!

process.patJetsReapplyJECAK8 = updatedPatJets.clone(
	jetSource = cms.InputTag("slimmedJetsAK8"),
	jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJECAK8"))
  )


process.jetCorrSequenceAK8 = cms.Sequence( process.patJetCorrFactorsReapplyJECAK8 + process.patJetsReapplyJECAK8 )



## For normal AK4Puppi jets jet energy correction on top of miniAOD
from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJetCorrFactors
process.patJetCorrFactorsReapplyJECAK4Puppi = updatedPatJetCorrFactors.clone(
	src = cms.InputTag("slimmedJetsPuppi"),
	levels = jetCorrectionLevelsPuppi,
	payload = 'AK4PFPuppi' ) # Make sure to choose the appropriate levels and payload here!

from PhysicsTools.PatAlgos.producersLayer1.jetUpdater_cff import updatedPatJets
process.patJetsReapplyJECAK4Puppi = updatedPatJets.clone(
	jetSource = cms.InputTag("slimmedJetsPuppi"),
	jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJECAK4Puppi"))
  )

process.jetCorrSequenceAK4Puppi = cms.Sequence( process.patJetCorrFactorsReapplyJECAK4Puppi + process.patJetsReapplyJECAK4Puppi )


## For correcting pruned jet mass + CHS
process.patJetCorrFactorsReapplyJECForPrunedMass = updatedPatJetCorrFactors.clone(
	src = cms.InputTag("slimmedJetsAK8"),
	levels = jetCorrectionLevels23CHS,
	payload = 'AK8PFchs' ) # Make sure to choose the appropriate levels and payload here!


process.patJetsReapplyJECForPrunedMass = updatedPatJets.clone(
	jetSource = cms.InputTag("slimmedJetsAK8"),
	jetCorrFactorsSource = cms.VInputTag(cms.InputTag("patJetCorrFactorsReapplyJECForPrunedMass"))
	)

process.jetCorrSequenceForPrunedMass = cms.Sequence( process.patJetCorrFactorsReapplyJECForPrunedMass + process.patJetsReapplyJECForPrunedMass )




process.load('DelPanj.TreeMaker.TreeMaker_cfi')
process.tree.filterLabel           = cms.InputTag(filterlabel)
process.tree.useJECText            = cms.bool(options.useJECText)
process.tree.THINjecNames          = cms.vstring(AK4JECTextFiles)
process.tree.THINjecUncName        = cms.string(AK4JECUncTextFile)
process.tree.FATprunedMassJecNames = cms.vstring(prunedMassJECTextFiles)
process.tree.FATjecNames           = cms.vstring(AK8JECTextFiles)
process.tree.FATjecUncName         = cms.string(AK8JECUncTextFile)
process.tree.ADDjecNames           = cms.vstring(AK8JECTextFiles)
process.tree.ADDjecUncName         = cms.string(AK8JECUncTextFile)
process.tree.AK4PuppijecNames      = cms.vstring(AK4PuppiJECTextFiles)
process.tree.AK4PuppijecUncName    = cms.string(AK4PuppiJECUncTextFile)
process.tree.AK8PuppijecNames      = cms.vstring(AK8PuppiJECTextFiles)
process.tree.AK8PuppijecUncName    = cms.string(AK8PuppiJECUncTextFile)
process.tree.CA15PuppijecNames      = cms.vstring(AK8PuppiJECTextFiles)
process.tree.CA15PuppijecUncName    = cms.string(AK8PuppiJECUncTextFile)
process.tree.fillAddJetInfo        = cms.bool(True)



if options.useJECText:
	process.tree.THINJets      = cms.InputTag("slimmedJets")
	process.tree.FATJets       = cms.InputTag("slimmedJetsAK8")
	process.tree.FATJetsForPrunedMass       = cms.InputTag("slimmedJetsAK8")
	process.tree.AK4PuppiJets  = cms.InputTag("slimmedJetsPuppi")



process.TFileService = cms.Service("TFileService",
				   fileName = cms.string("NCUGlobalTuples.root")          
				   # fileName = cms.string('$outputFileName')          
				   )



## New MET Filters
process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
process.BadPFMuonFilter.muons = cms.InputTag("slimmedMuons")
process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")
##
process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")
##

process.allEventsCounter = cms.EDFilter(
	"EventCounter"
 )


if not options.useJECText:
	process.analysis = cms.Path(
		process.allEventsCounter+
		process.egmGsfElectronIDSequence+## by raman
		process.egmPhotonIDSequence+ ## by raman
		#    process.pfMVAMEtSequence+   # disabled before the official code is fixed
		process.pfMet+
		process.jetCorrSequenceAK4+
		process.jetCorrSequenceAK8+
		process.jetCorrSequenceAK4Puppi+
		process.jetCorrSequenceForPrunedMass+
		process.BadPFMuonFilter *
		process.BadChargedCandidateFilter *
		#process.HBHENoiseFilterResultProducer+ ## by raman
		process.tree
		)
else:
	process.analysis = cms.Path(
		process.allEventsCounter+
		process.egmGsfElectronIDSequence+## by raman
		process.egmPhotonIDSequence+ ## by raman
		#    process.pfMVAMEtSequence+   # disabled before the official code is fixed
		process.pfMet+
		process.BadPFMuonFilter *
		process.BadChargedCandidateFilter *
		#process.HBHENoiseFilterResultProducer+ ## by raman
		process.tree
		)


#print process.dumpPython()
