cmsrel CMSSW_9_4_14

cd CMSSW_9_4_14/src

cmsenv

##For DelPanj and related dependencies
##For v4 Double b-tagger

git cms-init

git cms-merge-topic cms-egamma:EgammaPostRecoTools_940

##For MET Filters

git cms-addpkg RecoMET/METFilters
##For DelPanj

git clone git@github.com:tiwariPC/DMAnaRun2.git DelPanj

cd DelPanj

git checkout 2016_v3_94X_reduction

cd -


##For jetToolBox

git clone git@github.com:cms-jet/JetToolbox.git JMEAnalysis/JetToolbox -b jetToolbox_94X_v1

#get the code for deepCSV

##Compile (due to the external packages, will take about 15-20 mins)

scramv1 b clean

scramv1 b -j 8


cd $CMSSW_BASE/src

cmsenv
