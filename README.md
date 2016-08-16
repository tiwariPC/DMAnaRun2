
# DMAnaRun2

# For CMSSW_8_0_8_patch1
```
setenv SCRAM_ARCH slc6_amd64_gcc530
cmsrel CMSSW_8_0_8_patch1
cd CMSSW_8_0_8_patch1/src
cmsenv
```

## For double b-tagger
```
setenv CMSSW_GIT_REFERENCE /cvmfs/cms.cern.ch/cmssw.git.daily
git cms-init
git remote add btv-cmssw https://github.com/cms-btv-pog/cmssw.git
git fetch --tags btv-cmssw
git cms-merge-topic -u cms-btv-pog:BoostedDoubleSVTaggerV3-WithWeightFiles-v1_from-CMSSW_8_0_8_patch1
```

## For DelPanj and related dependencies

```
git clone git@github.com:syuvivida/DMAnaRun2.git DelPanj

cd DelPanj

git checkout 80X_dev

cd -

```

## Compile And Run 
```
scramv1 b clean

scramv1 b
```

## Download files that store effective area and JEC files

```
cp -p DelPanj/miniIso_effectiveArea/*txt .

mkdir jec
cd jec
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Spring16_25nsV6_MC.tar.gz 
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Spring16_25nsV6_DATA.tar.gz
tar xvzf Spring16_25nsV6_MC.tar.gz
tar xvzf Spring16_25nsV6_DATA.tar.gz

cd -
mv jec/*PFchs.txt .
mv jec/*AK8PFPuppi.txt .
rm -rf jec

voms-proxy-init --voms cms
cmsRun DelPanj/TreeMaker/test/RunCongigTest/treeMaker_Summer16_cfg.py runOnMC=True
cmsRun DelPanj/TreeMaker/test/RunCongigTest/treeMaker_Summer16_cfg.py runOnMC=False
 
```

Note, you need to add these text files as extra input files when submitting CRAB jobs.

## To submit MC crab jobs 
modify directories in crabConfig.py and dataset in MultiCrab_nocleaning.py according to your need
```
cd DelPanj/CrabUtilities
cp -p ../TreeMaker/test/RunCongigTest/treeMaker_Summer16_cfg.py .
cp -p ../miniIso_effectiveArea/*txt .

mkdir jec
cd jec
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Spring16_25nsV6_MC.tar.gz 
tar xvzf Spring16_25nsV6_MC.tar.gz
cd -
mv jec/*PFchs.txt .
mv jec/*AK8PFPuppi.txt .
rm -rf jec


cp -p crabConfig_MC.py crabConfig.py

source /cvmfs/cms.cern.ch/crab3/crab.csh or source /cvmfs/cms.cern.ch/crab3/crab.sh 
voms-proxy-init --voms cms
python MultiCrab_nocleaning.py submit
```

## To submit data crab jobs (Remember to update your JSON file)
modify directories in crabConfig.py and JSON file dataset in MultiCrab_nocleaning.py according to your need

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
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Spring16_25nsV6_DATA.tar.gz
tar xvzf Spring16_25nsV6_DATA.tar.gz
cd -
mv jec/*PFchs.txt .
mv jec/*AK8PFPuppi.txt .
rm -rf jec

cp -p crabConfig_data.py crabConfig.py

### Remember to update your JSON file

wget https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-278290_13TeV_PromptReco_Collisions16_JSON_NoL1T.txt

source /cvmfs/cms.cern.ch/crab3/crab.csh or source /cvmfs/cms.cern.ch/crab3/crab.sh 
voms-proxy-init --voms cms
python MultiCrab_nocleaning.py submit
```