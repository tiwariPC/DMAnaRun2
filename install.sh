export SCRAM_ARCH=slc6_amd64_gcc530
cmsrel CMSSW_8_0_26_patch1

cd CMSSW_8_0_26_patch1/src

cmsenv

##For DelPanj and related dependencies
##For v4 Double b-tagger
export CMSSW_GIT_REFERENCE=/cvmfs/cms.cern.ch/cmssw.git.daily

git cms-init

git remote add btv-cmssw https://github.com/cms-btv-pog/cmssw.git

git fetch --tags btv-cmssw

git cms-merge-topic -u cms-btv-pog:BoostedDoubleSVTaggerV4-WithWeightFiles-v1_from-CMSSW_8_0_21

#For Gamma cut based ID
git cms-merge-topic ikrav:egm_id_80X_v3_photons

git cms-merge-topic ikrav:egm_id_80X_v2

##For MET Filters

git cms-merge-topic -u cms-met:fromCMSSW_8_0_20_postICHEPfilter
##For DelPanj

git clone git@github.com:deepakcern/DMAnaRun2.git DelPanj

cd DelPanj

git checkout 80X_monoH_bbDM_reduced_15072019

cd -

cp -p DelPanj/tempfix/BadGlobalMuonTagger.cc RecoMET/METFilters/plugins/BadGlobalMuonTagger.cc

cp -p DelPanj/tempfix/badGlobalMuonTaggersMiniAOD_cff.py RecoMET/METFilters/python/badGlobalMuonTaggersMiniAOD_cff.py 

##For jetToolBox

git clone git@github.com:cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_80X

#get the code for deepCSV

git cms-merge-topic -u mverzett:DeepFlavour-from-CMSSW_8_0_21 

#get the training 

mkdir RecoBTag/DeepFlavour/data/

cd RecoBTag/DeepFlavour/data/ 

wget http://home.fnal.gov/~verzetti//DeepFlavour/training/DeepFlavourNoSL.json 

cd - 

##Compile (due to the external packages, will take about 15-20 mins)

scramv1 b clean

scramv1 b -j 8

##Checkout the electron/photon MVA weight files

cd $CMSSW_BASE/external

cd slc6_amd64_gcc530/

git clone https://github.com/ikrav/RecoEgamma-ElectronIdentification.git data/RecoEgamma/ElectronIdentification/data

git clone https://github.com/ikrav/RecoEgamma-PhotonIdentification.git data/RecoEgamma/PhotonIdentification/data

cd data/RecoEgamma/ElectronIdentification/data

git checkout egm_id_80X_v1

cd -

cd data/RecoEgamma/PhotonIdentification/data

git checkout egm_id_80X_v1

cd $CMSSW_BASE/src

cmsenv
