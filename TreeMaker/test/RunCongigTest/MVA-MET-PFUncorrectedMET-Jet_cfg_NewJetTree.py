import FWCore.ParameterSet.Config as cms

process = cms.Process('MVAMET')
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.options = cms.untracked.PSet(
    allowUnscheduled = cms.untracked.bool(True)
)

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
    #process.load("ExoDiBosonResonances.EDBRJets.redoSubstructure_cff")
    #process.load("ExoDiBosonResonances.EDBRJets.redoPatJets_cff")
    process.load("ExoDiBosonResonances.EDBRCommon.goodJets_cff")
    process.load("ExoDiBosonResonances.EDBRCommon.goodAK4Jets_cff")


# Updates
if option == 'RECO':
    process.goodMuons.src = "slimmedMuons"
    process.goodElectrons.src = "isolatedElectrons"
    #process.goodJets.src = "patJetsTESTAK8PFCHS"
    process.goodJets.src = "slimmedJetsAK8"
    #process.goodJets.src ="packedPatJetsPFCHSAK8" 
    process.goodAK4Jets.src = "slimmedJets"

process.leptonSequence = cms.Sequence(process.muSequence +
                                      process.eleSequence)





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
    input = cms.untracked.int32(500)
)



# Input source
process.source = cms.Source("PoolSource",
                            secondaryFileNames = cms.untracked.vstring(),
                            fileNames = cms.untracked.vstring(
        #'file:ZPrimeToTTBarMiniAOD.root'
#        'file:/afs/cern.ch/work/k/khurana/RunIIDelPanj/CMSSW_7_2_3_patch1/DYJetsMiniAOD/DYJets_100-200_1.root'
        # 'file:0432E62A-7A6C-E411-87BB-002590DB92A8.root' 
        #'file:58462360-8C07-E411-BCE4-008CFA007B98.root'
        #'file:0C3E1051-ED71-E411-B8C9-002590DB91CE.root'
       #'/store/relval/CMSSW_7_4_0/RelValADDMonoJet_d3MD3_13/MINIAODSIM/MCRUN2_74_V7_GENSIM_7_1_15-v1/00000/C0B7E3DE-6FDD-E411-A127-0025905B85EE.root' 
       #'file:C0B7E3DE-6FDD-E411-A127-0025905B85EE.root' 
      # '/store/relval/CMSSW_7_4_0_pre7/RelValADDMonoJet_d3MD3_13/MINIAODSIM/MCRUN2_74_V7-v1/00000/10661B83-43B7-E411-ADEA-0025905B8582.root'
#       '/store/relval/CMSSW_7_4_0/RelValProdTTbar_13/MINIAODSIM/MCRUN2_74_V7_GENSIM_7_1_15-v1/00000/1E12B842-93DD-E411-AF4F-0025905A48D0.root'
       #'/store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00C90EFC-3074-E411-A845-002590DB9262.root'
#     '/store/relval/CMSSW_7_4_1/RelValTTbar_13_unsch/MINIAODSIM/MCRUN2_74_V9-v1/00000/EA076017-DCEC-E411-BE66-0025905A60F2.root'  
   #'/store/mc/Phys14DR/DYJetsToEEMuMu_M-1400To2300_13TeV-madgraph/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/12B916AF-CE73-E411-8419-002590747DE2.root' 
  '/store/relval/CMSSW_7_4_0/RelValProdQCD_Pt_3000_3500_13/MINIAODSIM/MCRUN2_74_V7_GENSIM_7_1_15-v1/00000/08D1D655-7BDE-E411-8402-0025905A6060.root'   
 # '/store/relval/CMSSW_7_4_0_pre8/RelValZpTT_1500_13TeV/MINIAODSIM/MCRUN2_74_V7-v1/00000/9008F5B0-54BD-E411-96FB-0025905A6110.root'
#  '/store/relval/CMSSW_7_4_0_pre7/RelValTTbar_13/MINIAODSIM/MCRUN2_74_V7-v1/00000/B62A3865-39B7-E411-B76A-002618943880.root'
#test aod
#'/store/relval/CMSSW_7_4_0_pre8/RelValProdTTbar_13/AODSIM/MCRUN2_74_V7-v1/00000/44    E1E4BA-50BD-E411-A57A-002618943949.root'

  ),
                            skipEvents = cms.untracked.uint32(0)         
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

# used pat selector on PFCHSiAK8
getattr(process,'selectedPatJetsPFCHSAK8'+postfix).cut = cms.string('abs(eta) < 2.5')

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


process.packedPatJetsPFCHSAK8.algoTags.append( cms.InputTag('selectedPatJetsPrunedPFCHSAK8Packed') )
process.packedPatJetsPFCHSAK8.algoLabels.append( 'Pruned' )


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
process.cleanaddJets.checkOverlaps.muons.src = "goodMuons"
process.cleanaddJets.checkOverlaps.muons.deltaR = 0.8
process.cleanaddJets.checkOverlaps.muons.requireNoOverlaps = True
process.cleanaddJets.checkOverlaps.electrons.src = "goodElectrons"
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
                             process.fataddJetsSequence+
                             process.fatJetsSequence+ 
                             process.ak4JetsSequence
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
























process.tree = cms.EDAnalyzer(
    'TreeMaker',
    fillPUweightInfo_ = cms.bool(False),
    fillEventInfo_ = cms.bool(False),
    fillGenInfo_   = cms.bool(False),
    fillMuonInfo_  = cms.bool(False),
    fillElecInfo_  = cms.bool(False),
    fillFATJetInfo_   = cms.bool(True), 
    fillJetInfo_   = cms.bool(True), ## Raman switched it off
    fillAddJetInfo_   = cms.bool(True),
    fillMetInfo_   = cms.bool(False),
    fillTrigInfo_  = cms.bool(False),
    fillPhotInfo_  = cms.bool(False),
    fillTauInfo_   = cms.bool(False),
    
    genPartLabel=cms.InputTag("prunedGenParticles"),
    genJetLabel=cms.InputTag("slimmedGenJets"),
    maxNumGenPar  =  cms.uint32(30),
    patMuons=cms.InputTag("slimmedMuons"),
    patElectrons = cms.InputTag("slimmedElectrons"),
    
    
    pvSrc  = cms.InputTag('offlineSlimmedPrimaryVertices'),
    tauLabel_ = cms.untracked.InputTag("slimmedTaus"),
    rhoSrc = cms.InputTag('kt6PFJets','rho'),
    ### FatJets 
    FATJets=cms.InputTag("cleanJets"),
    # jec still need to be checked 
    #CA8jecPayloadNames = cms.vstring( CA8jecLevels ),
    #CA8jecUncName = cms.string(CA8jecUnc),    
    ### THINJet
    THINJets=cms.InputTag("cleanAK4Jets"),
    # jec still need to be checked 
    #AK5jecPayloadNames = cms.vstring( AK5jecLevels ),
    #AK5jecUncName = cms.string(AK5jecUnc),    
    
    patMetRaw=cms.InputTag("pfMet"),
    patMet = cms.InputTag("slimmedMETs"),
    #patMet = cms.InputTag("pfMet"),
    #patMetRaw=cms.InputTag("slimmedMETs"),
        
    runAddjetPY=cms.bool(True),
    ADDJets= cms.InputTag("cleanaddJets"),
    AddjetlabelPY= cms.InputTag("cleanaddJets"),
    #AddjetlabelPY= cms.InputTag("packedPatJetsPFCHSAK8"),
    SubJetsPY= cms.InputTag('selectedPatJetsPrunedSubjetsPFCHSAK8','SubJets'),
    svTagInfosPY  = cms.string('pfInclusiveSecondaryVertexFinder'),
    outFileName=cms.string('outputFileName.root')
    )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("mvamet_QCD.root")
                                   )





process.analysis = cms.Path(
                            process.leptonSequence+                                               
                            process.pfMVAMEtSequence+
                            process.pfMet+
                            process.miniAODjetSequence+                        
                            #process.selectedPatJetsPFCHSAK8PFlow + 
                            #process.selectedPatJetsPrunedPFCHSAK8Packed +
                            process.tree
                            )


#print process.dumpPython()
