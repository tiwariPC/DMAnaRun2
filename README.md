
# DMAnaRun2

# For CMSSW_7_6_3
```
setenv SCRAM_ARCH slc6_amd64_gcc493
cmsrel CMSSW_7_6_3
cd CMSSW_7_6_3/src/
cmsenv
```

## For photon ID (before CMSSW_8_0_2)
```
git cms-merge-topic 12742
```

## For DelPanj and related dependencies

```
git clone git@github.com:syuvivida/DMAnaRun2.git DelPanj

cd DelPanj

git checkout 76X_dev

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
voms-proxy-init --voms cms
cmsRun DelPanj/TreeMaker/test/RunCongigTest/treeMaker_Spring15_Nocleaning_cfg.py
 
```

Note, you need to add these text files as extra input files when submitting CRAB jobs.

## To submit MC crab jobs 
modify directories in crabConfig.py and dataset in MultiCrab_nocleaning.py according to your need
```
cd DelPanj/CrabUtilities
cp -p ../TreeMaker/test/RunCongigTest/treeMaker_Spring15_Nocleaning_cfg.py .
cp -p ../miniIso_effectiveArea/*txt .
cp -p crabConfig_MC.py crabConfig.py
python MultiCrab_nocleaning.py submit
```

## To submit data crab jobs
modify directories in crabConfig.py and dataset in MultiCrab_nocleaning.py according to your need
```
cd DelPanj/CrabUtilities
cp -p ../TreeMaker/test/RunCongigTest/treeMaker_Spring15_Nocleaning_cfg.py .
cp -p ../miniIso_effectiveArea/*txt .
cp -p crabConfig_data.py crabConfig.py
python MultiCrab_nocleaning.py submit
```