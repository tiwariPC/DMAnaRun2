
# DMAnaRun2

# For CMSSW_7_4_5
```
setenv SCRAM_ARCH slc6_amd64_gcc491
cmsrel CMSSW_7_4_5
cd CMSSW_7_4_5/src/
cmsenv
```

## For Electron IDs. Taken from twiki https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2
```

# Get the CMSSW implementation of the MVA
git cms-merge-topic ikrav:egm_id_74X_v2
scram build -j 10
```

## For EXO VV dependence 
```
git cms-init

git clone -b Phys14 git@github.com:cms-edbr/ExoDiBosonResonancesRun2.git ExoDiBosonResonances
```

## For DelPanj and related dependencies

```
git clone git@github.com:syuvivida/DMAnaRun2.git DelPanj

git clone git@github.com:syuvivida/DMAnaRun2_AddModules.git AddModules

rm -rf PhysicsTools/PatAlgos/ 

mv AddModules/PhysicsTools/PatAlgos PhysicsTools/PatAlgos

mv AddModules/EGamma/ .

mv AddModules/RecoBTag/ . 

mv AddModules/goodAK4Jets_cff.py ExoDiBosonResonances/EDBRCommon/python/

mv AddModules/prod/jec/ .

mv AddModules/prod/xml/ .

mv AddModules/redo*  ExoDiBosonResonances/EDBRJets/python/
```

## Compile And Run 
```
scramv1 b clean

scramv1 b

cmsRun DelPanj/TreeMaker/test/RunCongigTest/treeMaker_Spring15_cfg.py
 for PHYS14 samples use : 
 isSpring15    = cms.bool(False),
 
```
