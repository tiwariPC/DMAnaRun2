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
		  'G',
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
DATAJEC='Summer16_23Sep2016'+options.period+'V3_DATA'

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
#process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
# Other statements
if options.runOnMC:
### Needs to be updated
	process.GlobalTag.globaltag=' 90X_upgrade2017_realistic_v20'
else:## Data no global tag yet
        process.GlobalTag.globaltag='92X_dataRun2_Prompt_v4'




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
	testFile='/store/relval/CMSSW_9_2_7/RelValZpTT_1500_13/MINIAODSIM/92X_upgrade2017_realistic_v7-v2/00000/E23D8E5C-B470-E711-8040-0CC47A4D76C6.root'
else:
	testFile='/store/data/Run2017B/MET/MINIAOD/PromptReco-v1/000/297/046/00000/0C0F484D-3856-E711-AB77-02163E0127E7.root'


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



pvSource = 'offlineSlimmedPrimaryVertices'


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
    ,'pfDeepCSVJetTags:probb'
    ,'pfDeepCSVJetTags:probc'
    ,'pfDeepCSVJetTags:probudsg'
    ,'pfDeepCSVJetTags:probbb'
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

else:
        jetCorrectionsAK4CHS       = ('AK4PFchs', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual'], 'None')
	jetCorrectionsAK4Puppi     = ('AK4PFPuppi', ['L2Relative', 'L3Absolute','L2L3Residual'], 'None')
	jetCorrectionsAK8CHS       = ('AK8PFchs', ['L1FastJet','L2Relative', 'L3Absolute','L2L3Residual'], 'None')
	jetCorrectionsAK8CHSL23    = ('AK8PFchs', ['L2Relative', 'L3Absolute','L2L3Residual'], 'None')
	jetCorrectionsAK8Puppi     = ('AK8PFPuppi', ['L2Relative', 'L3Absolute','L2L3Residual'], 'None')
	jetCorrectionLevelsFullCHS = ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual']
	jetCorrectionLevels23CHS   = ['L2Relative', 'L3Absolute','L2L3Residual']
	jetCorrectionLevelsPuppi   = ['L2Relative', 'L3Absolute','L2L3Residual']

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
#jetToolbox( process, 'ak8', 'jetSequence', 'out', PUMethod='Puppi', miniAOD=options.useMiniAOD, runOnMC=options.runOnMC,
#	    bTagDiscriminators=(bTagDiscriminators + ([] if NOTADDHBBTag else ['pfBoostedDoubleSecondaryVertexAK8BJetTags'])),
#	    JETCorrPayload='AK8PFPuppi',JETCorrLevels=jetCorrectionLevelsPuppi,
#	    subJETCorrPayload='AK4PFPuppi',subJETCorrLevels=jetCorrectionLevelsPuppi,
#	    Cut='pt>170',
#	    addSoftDrop=True,addSoftDropSubjets=True,addNsub=True )


###end of add jet collection



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
	payload = 'AK8PFPuppi' ) # Make sure to choose the appropriate levels and payload here!

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

process.tree.fillCA15PuppiJetInfo  = cms.bool(True)

#process.tree.THINJets      = cms.InputTag("slimmedJets")
#process.tree.FATJets       = cms.InputTag("slimmedJetsAK8")
#process.tree.FATJetsForPrunedMass       = cms.InputTag("slimmedJetsAK8")
#process.tree.AK4PuppiJets  = cms.InputTag("slimmedJetsPuppi")


process.TFileService = cms.Service("TFileService",
				   fileName = cms.string("NCUGlobalTuples.root")

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



process.analysis = cms.Path(
	process.allEventsCounter+
	process.egmGsfElectronIDSequence+
	process.egmPhotonIDSequence+
	process.pfMet+
	process.jetCorrSequenceAK4+
	process.jetCorrSequenceAK8+
	process.jetCorrSequenceAK4Puppi+
	process.jetCorrSequenceForPrunedMass+
	process.BadPFMuonFilter +
	process.BadChargedCandidateFilter +
	process.badGlobalMuonTaggerMAOD +
	process.cloneGlobalMuonTaggerMAOD +
	#process.HBHENoiseFilterResultProducer+ ## by raman
	process.tree
	)


#print process.dumpPython()
