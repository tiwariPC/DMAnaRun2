
# DMAnaRun2

# For CMSSW_9_2_7
```
setenv SCRAM_ARCH slc6_amd64_gcc530
cmsrel CMSSW_9_2_7
cd CMSSW_9_2_7/src
cmsenv
```

## For BadMuon Filters (default code is reversed logic)

``` 
git cms-addpkg RecoMET/METFilters
```

## For DelPanj

```
git clone git@github.com:syuvivida/DMAnaRun2.git DelPanj

cd DelPanj

git checkout 92X_2017data

cd -

cp -p DelPanj/tempfix/BadGlobalMuonTagger.cc RecoMET/METFilters/plugins/BadGlobalMuonTagger.cc
```

## For jetToolBox
```
git clone git@github.com:cms-jet/JetToolbox.git JMEAnalysis/JetToolbox 
cd JMEAnalysis/JetToolbox
git checkout jetToolbox_91X_v1
cd -
```


## Compile (due to the external packages, will take about 15-20 mins)
```
scramv1 b clean

scramv1 b -j 5
```

## To test the job locally

```
cp -p DelPanj/miniIso_effectiveArea/*txt .

mkdir jec
cd jec
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Summer16_23Sep2016V3_MC.tar.gz
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Summer16_23Sep2016BCDV3_DATA.tar.gz
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Summer16_23Sep2016EFV3_DATA.tar.gz
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Summer16_23Sep2016GV3_DATA.tar.gz
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Summer16_23Sep2016HV3_DATA.tar.gz

tar xvzf Summer16_23Sep2016V3_MC.tar.gz
tar xvzf Summer16_23Sep2016BCDV3_DATA.tar.gz
tar xvzf Summer16_23Sep2016EFV3_DATA.tar.gz
tar xvzf Summer16_23Sep2016GV3_DATA.tar.gz
tar xvzf Summer16_23Sep2016HV3_DATA.tar.gz

cd -
mv jec/*PFchs.txt .
mv jec/*PFPuppi.txt .
rm -rf jec

voms-proxy-init --voms cms
cmsRun DelPanj/TreeMaker/test/RunCongigTest/treeMaker_Summer17_cfg.py runOnMC=True
cmsRun DelPanj/TreeMaker/test/RunCongigTest/treeMaker_Summer17_cfg.py runOnMC=False period=G
 
```

Note, you need to add these text files as extra input files when submitting CRAB jobs.

# The instruction below needs to be updated for 2017 data when more information is available

## To submit MC crab jobs 
modify directories in crabConfig.py and dataset in MultiCrab_dihiggs.py according to your need
```
cd DelPanj/CrabUtilities
cp -p ../TreeMaker/test/RunCongigTest/treeMaker_Summer16_cfg.py .
cp -p ../miniIso_effectiveArea/*txt .

mkdir jec
cd jec
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Summer16_23Sep2016V3_MC.tar.gz
tar xvzf Summer16_23Sep2016V3_MC.tar.gz

cd -
mv jec/*PFchs.txt .
mv jec/*PFPuppi.txt .
rm -rf jec


cp -p crabConfig_MC.py crabConfig.py

source /cvmfs/cms.cern.ch/crab3/crab.csh or source /cvmfs/cms.cern.ch/crab3/crab.sh 
voms-proxy-init --voms cms
python MultiCrab_dihiggs.py submit
```

## To submit data crab jobs (Remember to update your JSON file)
modify directories in crabConfig_data.py and dataset in MultiCrab_2016data.py according to your need

Check this hypernews for the latest JSON file name: 
https://hypernews.cern.ch/HyperNews/CMS/get/physics-validation.html

If you are adding data, you do not need to re-run the full dataset, you could just add data by comparing the difference between the updated JSON and the old JSON files
https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideGoodLumiSectionsJSONFile#How_to_compare_Good_Luminosity_f

```
cd DelPanj/CrabUtilities
cp -p ../TreeMaker/test/RunCongigTest/treeMaker_Summer16_cfg.py .
cp -p ../miniIso_effectiveArea/*txt .

mkdir jec
cd jec
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Summer16_23Sep2016BCDV3_DATA.tar.gz
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Summer16_23Sep2016EFV3_DATA.tar.gz
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Summer16_23Sep2016GV3_DATA.tar.gz
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Summer16_23Sep2016HV3_DATA.tar.gz

tar xvzf Summer16_23Sep2016BCDV3_DATA.tar.gz
tar xvzf Summer16_23Sep2016EFV3_DATA.tar.gz
tar xvzf Summer16_23Sep2016GV3_DATA.tar.gz
tar xvzf Summer16_23Sep2016HV3_DATA.tar.gz

cd -
mv jec/*PFchs.txt .
mv jec/*PFPuppi.txt .
rm -rf jec
```
### Modify crabConfig_data.py and MultiCrab_2016data.py

Change workdirectory and dataset names

#### Remember to update your JSON file
```
wget https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt

or 

cp -p /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt .

source /cvmfs/cms.cern.ch/crab3/crab.csh or source /cvmfs/cms.cern.ch/crab3/crab.sh 
voms-proxy-init --voms cms
python MultiCrab_2016data.py submit
```
