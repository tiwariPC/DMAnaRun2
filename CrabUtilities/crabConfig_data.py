""" In CRAB3 the configuration file is in Python language. It consists of creating a Configuration object imported from the WMCore library: """
from WMCore.Configuration import Configuration
config = Configuration()

"""  Once the Configuration object is created, it is possible to add new sections into it with corresponding parameters."""

config.section_("General")
config.General.requestName = 'dihiggs'
config.General.workArea = 'crab_20160517'
config.General.transferOutputs = True
config.General.transferLogs = True


config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'MVA-And-PFUnCorrectedMET.py'
#config.JobType.pyCfgParams = ['runOnMC=False']  ## When running on Rereco data such as 05Oct
config.JobType.pyCfgParams = ['runOnMC=False', 'isReReco=False'] ### when running on PromptReco data
config.JobType.inputFiles = ['effAreaElectrons_cone03_pfNeuHadronsAndPhotons_25ns.txt','effAreasMuons_cone03_Spring15_25ns.txt']
config.section_("Data")
config.Data.inputDataset = '/JetHT/Run2015D-05Oct2015-v1/MINIAOD'
config.Data.inputDBS = 'global'
config.Data.splitting = 'LumiBased'
config.Data.unitsPerJob = 50

#config.Data.lumiMask = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions15/13TeV/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt'
config.Data.lumiMask = 'Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_Silver_v2.txt'
config.Data.ignoreLocality = True


config.JobType.allowUndistributedCMSSW=True


#maxtarballsize = 50 
config.section_("Site")
config.Site.storageSite = "T2_CH_CERN"
