import FWCore.ParameterSet.Config as cms
## removed cleaning from Exo VV package 
## 

process = cms.Process('MVAMET')
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)


#option = 'DATA' # 'GEN' or 'RECO'
option = 'MC'

process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')


process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
if option == 'MC':
        process.GlobalTag.globaltag = cms.string("PHYS14_25_V2")
if option == 'DATA':
        process.GlobalTag.globaltag = cms.string("74X_dataRun2_Prompt_v0")





### GEN level studies
###if option == 'GEN':
###    process.load("ExoDiBosonResonances.EDBRGenStudies.genMuons_cff")
###    process.load("ExoDiBosonResonances.EDBRGenStudies.genElectrons_cff")
###    process.load("ExoDiBosonResonances.EDBRGenStudies.genFatJets_cff")
###
### RECO level studies
##if option == 'RECO':
##    #process.load("DelPanj.TreeMaker.slimmer.objectslimmer_cff")
##    process.load("ExoDiBosonResonances.EDBRCommon.goodMuons_cff")
##    process.load("ExoDiBosonResonances.EDBRCommon.goodElectrons_cff")
##    #process.load("ExoDiBosonResonances.EDBRJets.redoSubstructure_cff")
##    #process.load("ExoDiBosonResonances.EDBRJets.redoPatJets_cff")
##    process.load("ExoDiBosonResonances.EDBRCommon.goodJets_cff")
##    process.load("ExoDiBosonResonances.EDBRCommon.goodAK4Jets_cff")
##
##
# Updates
##if option == 'RECO':
##    process.goodMuons.src = "slimmedMuons"
##    process.goodElectrons.src = "slimmedElectrons"
##    #process.goodJets.src = "patJetsTESTAK8PFCHS"
##    process.goodJets.src = "slimmedJetsAK8"
##    #process.goodJets.src ="packedPatJetsPFCHSAK8" 
##    process.goodAK4Jets.src = "slimmedJets"
##
##process.leptonSequence = cms.Sequence(process.muSequence +
##                                      process.eleSequence)
##

#process.jetSequence = cms.Sequence(process.fatJetsSequence + process.substructureSequence + process.redoPatJets + process.ak4JetsSequence)




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
    input = cms.untracked.int32(50)
)



# Input source
process.source = cms.Source("PoolSource",
                            secondaryFileNames = cms.untracked.vstring(),
                            fileNames = cms.untracked.vstring(
        '/store/mc/RunIISpring15DR74/ZprimeToZhToZlephbb_narrow_M-1400_13TeV-madgraph/MINIAODSIM/Asympt25ns_MCRUN2_74_V9-v1/70000/76A0A6F5-7E14-E511-BB28-0026189438AC.root'
        ),
                            skipEvents = cms.untracked.uint32(0)         
                            )






## For MVA MET
## Check this reciepe when using MVA MET
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
## MVA MET Ends Here 
##


# Other statements
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
if option == 'MC':
        process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')
if option == 'DATA':
        process.GlobalTag = GlobalTag(process.GlobalTag, '74X_dataRun2_Prompt_v0', '')


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
from RecoJets.JetProducers.ak4PFJets_cfi import ak4PFJets
process.PFJetsCHSAK8 = ak4PFJets.clone(
    jetAlgorithm = cms.string('AntiKt'),
    rParam = cms.double(0.8),
    src = getattr(process,"ak4PFJets").src ,
    srcPVs = getattr(process,"ak4PFJets").srcPVs ,
    doAreaFastjet = cms.bool(True),
    jetPtMin = cms.double(150.0)
)
## produce Pruned ak8 fat jets (Gen and Reco) (each module produces two jet collections, fat jets and subjets)
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
process.genJetsNoNuPrunedAK8 = ak4GenJets.clone(
    SubJetParameters,
    jetAlgorithm = cms.string('AntiKt'),
    rParam = cms.double(0.8),
    src = cms.InputTag("packedGenParticlesForJetsNoNu") ,
    usePruning = cms.bool(True),
    writeCompound = cms.bool(True),
    jetCollInstanceName=cms.string("SubJets")
)
from RecoJets.JetProducers.ak4PFJetsPruned_cfi import ak4PFJetsPruned
process.PFJetsCHSPrunedAK8 = ak4PFJetsPruned.clone(
    jetAlgorithm = cms.string('AntiKt'),
    rParam = cms.double(0.8),
    src = getattr(process,"ak4PFJets").src ,
    srcPVs = getattr(process,"ak4PFJets").srcPVs ,
    doAreaFastjet = cms.bool(True),
    writeCompound = cms.bool(True),
    jetCollInstanceName=cms.string("SubJets"),
    jetPtMin = cms.double(150.0)
)


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
    jetPtMin = cms.double(150.0)
)













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
jetCorrectionsAK4 = ('AK4PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None')
jetCorrectionsAK8 = ('AK8PFchs', ['L1FastJet', 'L2Relative', 'L3Absolute'], 'None')


from PhysicsTools.PatAlgos.tools.jetTools import *

## Switch the default jet collection (done in order to use the above specified b-tag infos and discriminators)
#switchJetCollection(
#    process,
#    jetSource = cms.InputTag(jetSource),
#    pfCandidates = cms.InputTag(pfCandidates),
#    pvSource = cms.InputTag(pvSource),
#    svSource = cms.InputTag(svSource),
#    muSource = cms.InputTag(muSource),
#    elSource = cms.InputTag(elSource),
#    btagInfos = bTagInfos,
#    btagDiscriminators = bTagDiscriminators,
#    jetCorrections = jetCorrectionsAK4,
#    genJetCollection = cms.InputTag(genJetCollection),
#    genParticles = cms.InputTag(genParticles),
#    explicitJTA = False,
#    postfix = postfix
#)

NOTADDHBBTag=False

# add PFJetsCHSAK8

addJetCollection(
        process,
        labelName='PFCHSAK8',
        jetSource=cms.InputTag('PFJetsCHSAK8'),
        algo='AK',           # needed for jet flavor clustering
        rParam=0.8, # needed for jet flavor clustering
        pfCandidates = cms.InputTag(pfCandidates),
        pvSource = cms.InputTag(pvSource),
        svSource = cms.InputTag(svSource),
        muSource = cms.InputTag(muSource),
        elSource = cms.InputTag(elSource),
        btagInfos = bTagInfos,
        #btagDiscriminators = bTagDiscriminators,
        btagDiscriminators = (bTagDiscriminators + ([] if NOTADDHBBTag else ['pfBoostedDoubleSecondaryVertexAK8BJetTags'])),
        jetCorrections = jetCorrectionsAK8,
        genJetCollection = cms.InputTag('genJetsNoNuAK8'),
        genParticles = cms.InputTag(genParticles),
        explicitJTA = False,
        runIVF = False,
        postfix = postfix
    )

# used pat selector on PFCHSAK8
getattr(process,'selectedPatJetsPFCHSAK8'+postfix).cut = cms.string('abs(eta) < 2.5')


# Add SoftDrop jet
addJetCollection(
        process,
        labelName='SoftDropPFCHS',
        jetSource=cms.InputTag('PFJetsCHSSoftDrop'),
        algo='AK',
        btagInfos=['None'],
        btagDiscriminators=['None'],
        jetCorrections=jetCorrectionsAK8,
        genJetCollection = cms.InputTag('genJetsNoNuAK8'),
        genParticles = cms.InputTag(genParticles),
        getJetMCFlavour = False, # jet flavor disabled
        postfix = postfix
    )

# Add SoftDrop subjet
addJetCollection(
        process,
        labelName='SoftDropSubjetsPFCHS',
        jetSource=cms.InputTag('PFJetsCHSSoftDrop','SubJets'),
        algo='AK',           # needed for subjet flavor clustering
        rParam=0.8, # needed for subjet flavor clustering
        pfCandidates = cms.InputTag(pfCandidates),
        pvSource = cms.InputTag(pvSource),
        svSource = cms.InputTag(svSource),
        muSource = cms.InputTag(muSource),
        elSource = cms.InputTag(elSource),
        btagInfos = bTagInfos,
        btagDiscriminators = bTagDiscriminators,
        jetCorrections = jetCorrectionsAK4,
        genJetCollection = cms.InputTag('genJetsNoNuSoftDrop','SubJets'),
        genParticles = cms.InputTag(genParticles),
        explicitJTA = True,  # needed for subjet b tagging
        svClustering = True, # needed for subjet b tagging
        fatJets = cms.InputTag('PFJetsCHSAK8'),              # needed for subjet flavor clustering
        groomedFatJets = cms.InputTag('PFJetsCHSSoftDrop'), # needed for subjet flavor clustering
        runIVF = False,
        postfix = postfix
    )

## Softdrop  Establish references between PATified fat jets and subjets using the BoostedJetMerger
process.selectedPatJetsSoftDropPFCHSPacked = cms.EDProducer("BoostedJetMerger",
        jetSrc=cms.InputTag("selectedPatJetsSoftDropPFCHS"+postfix),
        subjetSrc=cms.InputTag("selectedPatJetsSoftDropSubjetsPFCHS"+postfix)
)



#add pruned PFJetsCHSAK8
addJetCollection(
        process,
        labelName='PrunedPFCHSAK8',
        jetSource=cms.InputTag('PFJetsCHSPrunedAK8'),
        algo='AK',
        btagInfos=['None'],
        btagDiscriminators=['None'],
        jetCorrections=jetCorrectionsAK8,
        genJetCollection = cms.InputTag('genJetsNoNuAK8'),
        genParticles = cms.InputTag(genParticles),
        getJetMCFlavour = False, # jet flavor disabled
        postfix = postfix
    )

#add subjet from pruned PFJetsCHSAK8

addJetCollection(
        process,
        labelName='PrunedSubjetsPFCHSAK8',
        jetSource=cms.InputTag('PFJetsCHSPrunedAK8','SubJets'),
        algo='AK',           # needed for subjet flavor clustering
        rParam=0.8, # needed for subjet flavor clustering
        pfCandidates = cms.InputTag(pfCandidates),
        pvSource = cms.InputTag(pvSource),
        svSource = cms.InputTag(svSource),
        muSource = cms.InputTag(muSource),
        elSource = cms.InputTag(elSource),
        btagInfos = bTagInfos,
        btagDiscriminators = bTagDiscriminators,
        jetCorrections = jetCorrectionsAK4,
        genJetCollection = cms.InputTag('genJetsNoNuPrunedAK8','SubJets'),
        genParticles = cms.InputTag(genParticles),
        explicitJTA = True,  # needed for subjet b tagging
        svClustering = True, # needed for subjet b tagging
        fatJets = cms.InputTag('PFJetsCHSAK8'),              # needed for subjet flavor clustering
        groomedFatJets = cms.InputTag('PFJetsCHSPrunedAK8'), # needed for subjet flavor clustering
        runIVF = False,
        postfix = postfix
)






## Establish references between PATified fat jets and subjets using the BoostedJetMerger

process.selectedPatJetsPrunedPFCHSAK8Packed = cms.EDProducer("BoostedJetMerger",
    jetSrc=cms.InputTag("selectedPatJetsPrunedPFCHSAK8"+postfix),
    subjetSrc=cms.InputTag("selectedPatJetsPrunedSubjetsPFCHSAK8"+postfix)
)



## Pack fat jets with subjets
process.packedPatJetsPFCHSAK8 = cms.EDProducer("JetSubstructurePacker",
            jetSrc = cms.InputTag('selectedPatJetsPFCHSAK8'+postfix),
            distMax = cms.double(0.8),
            algoTags = cms.VInputTag(),
            algoLabels = cms.vstring(),
            fixDaughters = cms.bool(False)
)

SaveSoftDrop=True;

if SaveSoftDrop:
        process.packedPatJetsPFCHSAK8.algoTags.append( cms.InputTag('selectedPatJetsSoftDropPFCHSPacked') )
        process.packedPatJetsPFCHSAK8.algoLabels.append( 'SoftDrop' )
else:
        process.packedPatJetsPFCHSAK8.algoTags.append( cms.InputTag('selectedPatJetsPrunedPFCHSAK8Packed') )
        process.packedPatJetsPFCHSAK8.algoLabels.append( 'Pruned' )



#process.packedPatJetsPFCHSAK8.algoTags.append( cms.InputTag('selectedPatJetsPrunedPFCHSAK8Packed') )
#process.packedPatJetsPFCHSAK8.algoLabels.append( 'Pruned' )


for m in ['patJets'+postfix, 'patJetsPFCHSAK8'+postfix, 'patJetsSoftDropSubjetsPFCHS'+postfix, 'patJetsPrunedSubjetsPFCHSAK8'+postfix]:
    if hasattr(process,m) and getattr( getattr(process,m), 'addBTagInfo' ):
        print "Switching 'addTagInfos' for " + m + " to 'True'"
        setattr( getattr(process,m), 'addTagInfos', cms.bool(True) )
    if hasattr(process,m):
        print "Switching 'addJetFlavourInfo' for " + m + " to 'True'"
        setattr( getattr(process,m), 'addJetFlavourInfo', cms.bool(True) )



jetRadius=0.8

        # Set the cone size for the jet-track association to the jet radius
getattr(process,'pfImpactParameterTagInfosPFCHSAK8'+postfix).maxDeltaR = cms.double(jetRadius) # default is 0.4
getattr(process,'pfSecondaryVertexTagInfosPFCHSAK8'+postfix).trackSelection.jetDeltaRMax = cms.double(jetRadius)   # default is 0.3
getattr(process,'pfSecondaryVertexTagInfosPFCHSAK8'+postfix).vertexCuts.maxDeltaRToJetAxis = cms.double(jetRadius) # default is 0.4
        # Set the jet-SV dR to the jet radius
getattr(process,'pfInclusiveSecondaryVertexFinderTagInfosPFCHSAK8'+postfix).vertexCuts.maxDeltaRToJetAxis = cms.double(jetRadius) # default is 0.4
getattr(process,'pfInclusiveSecondaryVertexFinderTagInfosPFCHSAK8'+postfix).extSVDeltaRToJet = cms.double(jetRadius) # default is 0.3
        # Set the JP track dR cut to the jet radius
process.candidateJetProbabilityComputerFat = process.candidateJetProbabilityComputer.clone( deltaR = cms.double(jetRadius) ) # default is 0.3
getattr(process,'pfJetProbabilityBJetTagsPFCHSAK8'+postfix).jetTagComputer = cms.string('candidateJetProbabilityComputerFat')
        # Set the JBP track dR cut to the jet radius
process.candidateJetBProbabilityComputerFat = process.candidateJetBProbabilityComputer.clone( deltaR = cms.double(jetRadius) ) # default is 0.4
getattr(process,'pfJetBProbabilityBJetTagsPFCHSAK8'+postfix).jetTagComputer = cms.string('candidateJetBProbabilityComputerFat')
        # Set the CSV track dR cut to the jet radius
process.candidateCombinedSecondaryVertexComputerFat = process.candidateCombinedSecondaryVertexComputer.clone()
process.candidateCombinedSecondaryVertexComputerFat.trackSelection.jetDeltaRMax = cms.double(jetRadius) # default is 0.3
process.candidateCombinedSecondaryVertexComputerFat.trackPseudoSelection.jetDeltaRMax = cms.double(jetRadius) # default is 0.3
getattr(process,'pfCombinedSecondaryVertexV2BJetTagsPFCHSAK8'+postfix).jetTagComputer = cms.string('candidateCombinedSecondaryVertexComputerFat')
        # Set the CSVv2 track dR cut to the jet radius
process.candidateCombinedSecondaryVertexV2ComputerFat = process.candidateCombinedSecondaryVertexV2Computer.clone()
process.candidateCombinedSecondaryVertexV2ComputerFat.trackSelection.jetDeltaRMax = cms.double(jetRadius) # default is 0.3
process.candidateCombinedSecondaryVertexV2ComputerFat.trackPseudoSelection.jetDeltaRMax = cms.double(jetRadius) # default is 0.3
getattr(process,'pfCombinedInclusiveSecondaryVertexV2BJetTagsPFCHSAK8'+postfix).jetTagComputer = cms.string('candidateCombinedSecondaryVertexV2ComputerFat')




from PhysicsTools.SelectorUtils.pfJetIDSelector_cfi import pfJetIDSelector

process.goodaddJets = cms.EDFilter("PFJetIDSelectionFunctorFilter",
                        filterParams = pfJetIDSelector.clone(),
                        src = cms.InputTag("packedPatJetsPFCHSAK8")
                        )

### Cleaning
# We want to make sure that the jets are not the electrons or muons done previously

import PhysicsTools.PatAlgos.cleaningLayer1.jetCleaner_cfi as jetCleaner_cfi

process.cleanaddJets = jetCleaner_cfi.cleanPatJets.clone()
process.cleanaddJets.src = "goodaddJets"
process.cleanaddJets.checkOverlaps.muons.src = "slimmedMuons"
process.cleanaddJets.checkOverlaps.muons.deltaR = 0.8
process.cleanaddJets.checkOverlaps.muons.requireNoOverlaps = True
process.cleanaddJets.checkOverlaps.electrons.src = "slimmedElectrons"
process.cleanaddJets.checkOverlaps.electrons.deltaR = 0.8
process.cleanaddJets.checkOverlaps.electrons.requireNoOverlaps = True
process.cleanaddJets.checkOverlaps.photons = cms.PSet()
process.cleanaddJets.checkOverlaps.taus = cms.PSet()
process.cleanaddJets.checkOverlaps.tkIsoElectrons = cms.PSet()
process.cleanaddJets.finalCut = "pt > 20 & abs(eta) < 2.4"

process.fataddJetsSequence = cms.Sequence(
               process.goodaddJets + 
               process.cleanaddJets
               )

















#process.patJetsAK8.addTagInfos = cms.bool(True)




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


process.miniAODjetSequence = cms.Sequence(
                             process.selectedPatJetsPFCHSAK8PFlow+
                             process.selectedPatJetsPrunedPFCHSAK8Packed +
                             process.fataddJetsSequence
                             #process.fatJetsSequence+ 
                             #process.ak4JetsSequence
                             
                             )




###end of add jet collection


### FOR adding new hbb b-tags in miniaod?

#process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
#process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")

#from RecoJets.Configuration.RecoPFJets_cff import ak8PFJetsCHS

#addJetCollection(
#   process,
#   labelName = 'TESTAK8PFCHS',
#   jetSource = cms.InputTag('ak8PFJetsCHS'),
#   #jetSource = cms.InputTag('slimmedJetsAK8'),
#   jetCorrections = ('AK8PFchs', cms.vstring(['L1FastJet', 'L2Relative', 'L3Absolute']), 'Type-2'),
#   algo = 'AK',
#   rParam = 0.8,
#   btagDiscriminators = ['pfBoostedDoubleSecondaryVertexAK8BJetTags']
#)
#process.patJetsTESTAK8PFCHS.addTagInfos = True





## add value maps for electron IDs
useAOD  = False
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
# turn on VID producer, indicate data format to be
# DataFormat.AOD or DataFormat.MiniAOD, as appropriate
if useAOD == True :
    dataFormat = DataFormat.AOD
else :
    dataFormat = DataFormat.MiniAOD
    
switchOnVIDElectronIdProducer(process, dataFormat)
switchOnVIDPhotonIdProducer(process, dataFormat)
# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_PHYS14_PU20bx25_V2_cff',
                 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV60_cff',
                 'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_PHYS14_PU20bx25_nonTrig_V1_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)

my_phoid_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_PHYS14_PU20bx25_V2_cff']
#add them to the VID producer
for idmod in my_phoid_modules:
	setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)










process.tree = cms.EDAnalyzer(
    'TreeMaker',
    fillPUweightInfo_ = cms.bool(True),
    fillEventInfo_ = cms.bool(True),
    fillGenInfo_   = cms.bool(True),
    fillMuonInfo_  = cms.bool(True),
    fillElecInfo_  = cms.bool(True),
    fillFATJetInfo_   = cms.bool(True), 
    fillJetInfo_   = cms.bool(True), 
    fillAddJetInfo_   = cms.bool(True),
    fillMetInfo_   = cms.bool(True),
    fillTrigInfo_  = cms.bool(True),
    fillFilterInfo_ = cms.bool(True),
    fillPhotInfo_  = cms.bool(True),
    fillTauInfo_   = cms.bool(True),
    
    ##photonLabel    = cms.InputTag("slimmedPhotons"),
    photonLabel    = cms.InputTag("slimmedPhotons"),
    triggerLabel   = cms.InputTag("TriggerResults::HLT"),
    filterLabel    = cms.InputTag("TriggerResults::PAT"),
    genPartLabel=cms.InputTag("prunedGenParticles"),
    genJetLabel=cms.InputTag("slimmedGenJets"),
    maxNumGenPar  =  cms.uint32(30),
    applyStatusSelection = cms.bool(True),
    
    ## For Muon
    r_iso_min    = cms.double(0.05),
    r_iso_max    = cms.double(0.2),
    kt_scale     = cms.double(10.),
    charged_only = cms.bool(False),
    pfForMiniIso = cms.InputTag("packedPFCandidates"),

    ##CA8Jets=cms.InputTag("slimmedJetsAK8"),
    ##CA8jecPayloadNames = cms.vstring( CA8jecLevels ),
    ##CA8jecUncName = cms.string(CA8jecUnc),
    ##
    ##AK5Jets=cms.InputTag("slimmedJets"),
    ##AK5jecPayloadNames = cms.vstring( AK5jecLevels ),
    ##AK5jecUncName = cms.string(AK5jecUnc),  
    
    ##patMuons=cms.InputTag("slimmedMuons"),
    ##patElectrons = cms.InputTag("slimmedElectrons"),
    patMuons=cms.InputTag("slimmedMuons"),
    patElectrons = cms.InputTag("slimmedElectrons"),
    
    
    pvSrc  = cms.InputTag('offlineSlimmedPrimaryVertices'),
    ##tauLabel_ = cms.untracked.InputTag("slimmedTaus"),
    tauLabel_ = cms.untracked.InputTag("slimmedTaus"),
    rhoSrc = cms.InputTag('kt6PFJets','rho'),
    ### FatJets 
    #FATJets=cms.InputTag("cleanJets"),
    FATJets=cms.InputTag("slimmedJetsAK8"),
    # jec still need to be checked 
    #CA8jecPayloadNames = cms.vstring( CA8jecLevels ),
    #CA8jecUncName = cms.string(CA8jecUnc),    
    ### THINJet
    #THINJets=cms.InputTag("cleanAK4Jets"),
    THINJets=cms.InputTag("slimmedJets"),
    # jec still need to be checked 
    #AK5jecPayloadNames = cms.vstring( AK5jecLevels ),
    #AK5jecUncName = cms.string(AK5jecUnc),    
    
    patMetRaw=cms.InputTag("pfMet"),
    patMet = cms.InputTag("slimmedMETs"),
    #patMet = cms.InputTag("pfMet"),
    #patMetRaw=cms.InputTag("pfMet"),
    pfMetRaw   = cms.InputTag("pfMet"),
    pfType1Met = cms.InputTag("slimmedMETs"),
    pfMVAMET   = cms.InputTag("pfMVAMEt"),
    
    runAddjetPY=cms.bool(True),
    ADDJets= cms.InputTag("packedPatJetsPFCHSAK8"),
    AddjetlabelPY= cms.InputTag("packedPatJetsPFCHSAK8"),
    #ADDJets= cms.InputTag("cleanaddJets"),
    #AddjetlabelPY= cms.InputTag("cleanaddJets"),
    #AddjetlabelPY= cms.InputTag("packedPatJetsPFCHSAK8"),
    SubJetsPY= cms.InputTag('selectedPatJetsPrunedSubjetsPFCHSAK8','SubJets'),
    svTagInfosPY  = cms.string('pfInclusiveSecondaryVertexFinder'),
    # boolean to switch off part of the subjets which doesn't work in PHYS14 samples
    # True for Spring15 and False for PHYS14
    isSpring15    = cms.bool(True),
        
    eleVetoIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-veto"),
    eleLooseIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-loose"),
    eleMediumIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-medium"),
    eleTightIdMap = cms.InputTag("egmGsfElectronIDs:cutBasedElectronID-PHYS14-PU20bx25-V2-standalone-tight"),
    eleHEEPIdMap = cms.InputTag("egmGsfElectronIDs:heepElectronID-HEEPV60"),
    
    phoLooseIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-loose"),
    phoMediumIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-medium"),
    phoTightIdMap = cms.InputTag("egmPhotonIDs:cutBasedPhotonID-PHYS14-PU20bx25-V2-standalone-tight"),

    
    #
    # ID decisions (common to all formats)
    #
    eleMVAMediumIdMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-PHYS14-PU20bx25-nonTrig-V1-wp80"),
    eleMVATightIdMap = cms.InputTag("egmGsfElectronIDs:mvaEleID-PHYS14-PU20bx25-nonTrig-V1-wp90"),
    #
    # ValueMaps with MVA results
    #
    mvaValuesMap     = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Phys14NonTrigValues"),
    mvaCategoriesMap = cms.InputTag("electronMVAValueMapProducer:ElectronMVAEstimatorRun2Phys14NonTrigCategories"),
    
    outFileName=cms.string('outputFileName.root')
    )

process.TFileService = cms.Service("TFileService",
                                   #fileName = cms.string("TreeMaker_TTbar.root")
                                   #fileName = cms.string("TreeMaker_QCD.root") 
                                  fileName = cms.string("NCUGlobalTuples.root")          
                                  # fileName = cms.string('$outputFileName')          
                                  )



process.allEventsCounter = cms.EDFilter(
    "EventCounter"
    )

#process.egmPhotonIDs.physicsObjectSrc = cms.InputTag("ncuslimmedPhoton")
#process.egmGsfElectronIDs.physicsObjectSrc = cms.InputTag("ncuslimmedElectron")


process.analysis = cms.Path(
    process.allEventsCounter+
    #process.ncuslimmer+
    process.egmGsfElectronIDSequence+
    process.egmPhotonIDSequence+
    #process.leptonSequence+                                               
    #process.egmGsfElectronIDSequence+
    #process.egmPhotonIDSequence+
    process.pfMVAMEtSequence+
    process.pfMet+
    process.miniAODjetSequence+                     
    #process.selectedPatJetsPFCHSAK8PFlow + 
    #process.selectedPatJetsPrunedPFCHSAK8Packed +
    process.tree
    )


#print process.dumpPython()
