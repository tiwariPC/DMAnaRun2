
# DMAnaRun2

# For CMSSW_7_4_7
```
setenv SCRAM_ARCH slc6_amd64_gcc491
cmsrel CMSSW_7_4_7
cd CMSSW_7_4_7/src/
cmsenv
```

## For Egamma to get Spring15 ID
```
git cms-merge-topic ikrav:egm_id_747_v2
```

## For DelPanj and related dependencies

```
git clone git@github.com:syuvivida/DMAnaRun2.git DelPanj

git clone git@github.com:syuvivida/DMAnaRun2_AddModules.git AddModules


mv AddModules/EGamma/ .

mv AddModules/RecoBTag/ . 

mv AddModules/prod/jec/ .

mv AddModules/prod/xml/ .

```

## Compile And Run 
```
scramv1 b clean

scramv1 b

cmsRun DelPanj/TreeMaker/test/RunCongigTest/treeMaker_Spring15_cfg.py
 for PHYS14 samples use : 
 isSpring15    = cms.bool(False),
 
```
