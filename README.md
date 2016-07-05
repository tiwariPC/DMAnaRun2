
# DMAnaRun2

# For CMSSW_8_0_8
```
setenv SCRAM_ARCH slc6_amd64_gcc530
cmsrel CMSSW_8_0_8
cd CMSSW_8_0_8/src/
cmsenv
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
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Spring16_25nsV3_MC.tar.gz 
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Spring16_25nsV3_DATA.tar.gz
tar xvzf Spring16_25nsV3_MC.tar.gz
tar xvzf Spring16_25nsV3_DATA.tar.gz

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
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Spring16_25nsV3_MC.tar.gz 
tar xvzf Spring16_25nsV3_MC.tar.gz
cd -
mv jec/*PFchs.txt .
mv jec/*AK8PFPuppi.txt .
rm -rf jec


cp -p crabConfig_MC.py crabConfig.py

source /cvmfs/cms.cern.ch/crab3/crab.csh or source /cvmfs/cms.cern.ch/crab3/crab.sh 
voms-proxy-init --voms cms
python MultiCrab_nocleaning.py submit
```

## To submit data crab jobs
modify directories in crabConfig.py and dataset in MultiCrab_nocleaning.py according to your need
```
cd DelPanj/CrabUtilities
cp -p ../TreeMaker/test/RunCongigTest/treeMaker_Summer16_cfg.py .
cp -p ../miniIso_effectiveArea/*txt .

mkdir jec
cd jec
wget https://github.com/cms-jet/JECDatabase/raw/master/tarballs/Spring16_25nsV3_DATA.tar.gz
tar xvzf Spring16_25nsV3_DATA.tar.gz
cd -
mv jec/*PFchs.txt .
mv jec/*AK8PFPuppi.txt .
rm -rf jec

cp -p crabConfig_data.py crabConfig.py
wget https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-275125_13TeV_PromptReco_Collisions16_JSON.txt

source /cvmfs/cms.cern.ch/crab3/crab.csh or source /cvmfs/cms.cern.ch/crab3/crab.sh 
voms-proxy-init --voms cms
python MultiCrab_nocleaning.py submit
```