
# DMAnaRun2

# For CMSSW_7_4_12
```
setenv SCRAM_ARCH slc6_amd64_gcc491
cmsrel CMSSW_7_4_12
cd CMSSW_7_4_12/src/
cmsenv
```

## For Egamma to get Spring15 ID
```
git cms-merge-topic ikrav:egm_id_7.4.12_v1
```


## For DelPanj and related dependencies

```
git clone git@github.com:syuvivida/DMAnaRun2.git DelPanj

cd DelPanj

git checkout 74X_neutrino

cd -

git clone git@github.com:syuvivida/DMAnaRun2_AddModules.git AddModules

mv AddModules/EGamma/ .

rm -rf AddModules
```

## Compile And Run 
```
scramv1 b clean

scramv1 b -j 5
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
source /cvmfs/cms.cern.ch/crab3/crab.csh (for tcsh) or source /cvmfs/cms.cern.ch/crab3/crab.sh (for bash)
voms-proxy-init --voms cms
python MultiCrab_dihiggs.py submit
```

