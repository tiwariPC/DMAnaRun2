from WMCore.Configuration import Configuration
from CRABClient.UserUtilities import getUsernameFromSiteDB
config = Configuration()

workname='bbDM'
reqname='scalar_NLO_Mchi-450_Mphi-1000'
dataset='/BBbarDMJets_scalar_NLO_Mchi-450_Mphi-1000_TuneCUETP8M1_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM'

config.section_("General")
config.General.requestName = reqname
config.General.workArea = 'crab_'+workname
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'treeMaker_Summer17_cfg.py'
config.JobType.maxMemoryMB = 2400
config.JobType.maxJobRuntimeMin = 2750
config.JobType.inputFiles = ['effAreaElectrons_cone03_pfNeuHadronsAndPhotons_25ns.txt','effAreasMuons_cone03_Spring15_25ns.txt',
#'BoostedSVDoubleCA15_withSubjet_v4.weights.xml',
#'Summer16_23Sep2016V3_MC_Uncertainty_AK8PFchs.txt',
#'Summer16_23Sep2016V3_MC_Uncertainty_AK8PFPuppi.txt',
#'Summer16_23Sep2016V3_MC_Uncertainty_AK4PFchs.txt',
#'Summer16_23Sep2016V3_MC_L3Absolute_AK8PFchs.txt',
#'Summer16_23Sep2016V3_MC_L3Absolute_AK8PFPuppi.txt',
#'Summer16_23Sep2016V3_MC_L3Absolute_AK4PFchs.txt',
#'Summer16_23Sep2016V3_MC_L2Relative_AK8PFchs.txt',
#'Summer16_23Sep2016V3_MC_L2Relative_AK8PFPuppi.txt',
#'Summer16_23Sep2016V3_MC_L2Relative_AK4PFchs.txt',
#'Summer16_23Sep2016V3_MC_L2L3Residual_AK8PFchs.txt',
#'Summer16_23Sep2016V3_MC_L2L3Residual_AK8PFPuppi.txt',
#'Summer16_23Sep2016V3_MC_L2L3Residual_AK4PFchs.txt',
#'Summer16_23Sep2016V3_MC_L1RC_AK8PFchs.txt',
#'Summer16_23Sep2016V3_MC_L1RC_AK4PFchs.txt',
#'Summer16_23Sep2016V3_MC_L1FastJet_AK8PFchs.txt',
#'Summer16_23Sep2016V3_MC_L1FastJet_AK8PFPuppi.txt',
#'Summer16_23Sep2016V3_MC_L1FastJet_AK4PFchs.txt'
]
config.JobType.sendExternalFolder = True

config.section_("Data")
config.Data.inputDataset = dataset
config.Data.inputDBS = 'global'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 10000
config.Data.publication = False

config.JobType.allowUndistributedCMSSW=True

config.section_("Site")
config.Site.storageSite = "T2_IN_TIFR"
config.Data.outputDatasetTag = reqname
config.Data.outLFNDirBase = '/store/user/%s/t3store2/%s' % (getUsernameFromSiteDB(), workname)
