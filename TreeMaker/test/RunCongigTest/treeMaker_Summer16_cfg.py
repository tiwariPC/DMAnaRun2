import FWCore.ParameterSet.Config as cms
## removed cleaning from Exo VV package 
## 

process = cms.Process('NCUANA')
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
		  False,
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

MCJEC='Summer16_23Sep2016V3_MC'
#DATAJEC='Summer16_23Sep2016'+options.period+'V3_DATA'
DATAJEC='Summer16_07Aug2017'+options.period+'_V1_DATA'

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
	process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_TrancheIV_v8', '')  
else:## Data no global tag yet
        process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_2016SeptRepro_v7', '')



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
    input = cms.untracked.int32(500)
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
	testFile='/store/mc/RunIISummer16MiniAODv2/BulkGravTohhTohbbhbb_narrow_M-2000_13TeV-madgraph/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/70000/EC905616-A5B5-E611-A1CB-008CFA14FA64.root'
else:
	testFile='/store/data/Run2016G/JetHT/MINIAOD/03Feb2017-v1/100000/006E7AF2-AEEC-E611-A88D-7845C4FC3B00.root'


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


postfix = "PFlow"
pfCandidates = 'packedPFCandidates'
jetSource = 'ak4PFJets'
#jetSource = 'slimmedJets' # did not affect
pvSource = 'offlineSlimmedPrimaryVertices'
svSource = 'slimmedSecondaryVertices'
muSource = 'slimmedMuons'
elSource = 'slimmedElectrons'
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
	jetCorrectionLevelsFullCHS = ['L1FastJet', 'L2Relative', 'L3Absolute']
	jetCorrectionLevels23CHS   = ['L2Relative', 'L3Absolute']
	jetCorrectionLevelsPuppi   = ['L2Relative', 'L3Absolute']

	AK4JECTextFiles = [
		MCJEC+'_L1FastJet_AK4PFchs.txt',
		MCJEC+'_L2Relative_AK4PFchs.txt',
		MCJEC+'_L3Absolute_AK4PFchs.txt'
		]
	AK4JECUncTextFile = MCJEC+'_Uncertainty_AK4PFchs.txt'

else:
        jetCorrectionsAK4CHS       = ('AK4PFchs', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual'], 'None')
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


process.load('CommonTools/PileupAlgos/Puppi_cff')
process.puppi.candName       = cms.InputTag('packedPFCandidates')
process.puppi.vertexName     = cms.InputTag('offlineSlimmedPrimaryVertices')


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


###########

updateJetCollection(
        process,
        jetSource = cms.InputTag('slimmedJets'),
        jetCorrections = ('AK4PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'None'),
        btagDiscriminators = ['deepFlavourJetTags:probudsg', 'deepFlavourJetTags:probb', 'deepFlavourJetTags:probc', 'deepFlavourJetTags:probbb', 'deepFlavourJetTags:probcc'], ## to add discriminators
        btagPrefix = 'TEST',
        )
outputModules = ['AODEventContent','AODSIMEventContent','FEVTDEBUGEventContent','FEVTDEBUGHLTEventContent','FEVTEventContent','FEVTHLTALLEventContent','FEVTSIMEventContent','RAWAODSIMEventContent','RAWRECODEBUGHLTEventContent','RAWRECOEventContent','RAWRECOSIMHLTEventContent','RECODEBUGEventContent','RECOEventContent','RECOSIMEventContent','RecoJetsAOD','RecoJetsFEVT','RecoJetsRECO']

for module in outputModules:
   exec("process."+module+".outputCommands.append('keep *_selectedUpdatedPatJets_*_*')")

##########

process.load('DelPanj.TreeMaker.TreeMaker_cfi')
process.tree.filterLabel           = cms.InputTag(filterlabel)
process.tree.useJECText            = cms.bool(options.useJECText)
process.tree.THINjecNames          = cms.vstring(AK4JECTextFiles)
process.tree.THINjecUncName        = cms.string(AK4JECUncTextFile)
process.tree.AK4deepCSVjecNames          = cms.vstring(AK4JECTextFiles)
process.tree.AK4deepCSVjecUncName        = cms.string(AK4JECUncTextFile)

#if options.useJECText:
#	process.tree.THINJets      = cms.InputTag("slimmedJets")
#	process.tree.AK4deepCSVJets      = cms.InputTag("selectedUpdatedPatJets")
process.tree.THINJets      = cms.InputTag("slimmedJets")
process.tree.AK4deepCSVJets      = cms.InputTag("selectedUpdatedPatJets")


process.TFileService = cms.Service("TFileService",
				   fileName = cms.string("NCUGlobalTuples.root")          
				   # fileName = cms.string('$outputFileName')          
				   )



## New MET Filters
process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
process.BadPFMuonFilter.muons = cms.InputTag("slimmedMuons")
process.BadPFMuonFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadPFMuonFilter.taggingMode = cms.bool(True)

process.load('RecoMET.METFilters.badGlobalMuonTaggersMiniAOD_cff')
process.badGlobalMuonTaggerMAOD.taggingMode = cms.bool(True)
process.cloneGlobalMuonTaggerMAOD.taggingMode = cms.bool(True)

##
process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.BadChargedCandidateFilter.muons = cms.InputTag("slimmedMuons")
process.BadChargedCandidateFilter.PFCandidates = cms.InputTag("packedPFCandidates")
process.BadChargedCandidateFilter.taggingMode = cms.bool(True)
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
		process.BadPFMuonFilter +
		process.BadChargedCandidateFilter +
		process.badGlobalMuonTaggerMAOD + 
		process.cloneGlobalMuonTaggerMAOD +
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
		process.BadPFMuonFilter +
		process.BadChargedCandidateFilter +
		process.badGlobalMuonTaggerMAOD + 
		process.cloneGlobalMuonTaggerMAOD +
		#process.HBHENoiseFilterResultProducer+ ## by raman
		process.tree
		)

#print process.dumpPython()
