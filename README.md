
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

## For Puppi v8
```
git cms-addpkg CommonTools/PileupAlgos

git cms-merge-topic nhanvtran:puppi-etadep-746p2-v8
```


## For DelPanj and related dependencies

```
git clone git@github.com:syuvivida/DMAnaRun2.git DelPanj

cd DelPanj

git checkout jet_CMSSW747

cd -

git clone git@github.com:syuvivida/DMAnaRun2_AddModules.git AddModules

mv AddModules/EGamma/ .

rm -rf AddModules
```

## Compile And Run 
```
scramv1 b clean

scramv1 b

cmsRun DelPanj/TreeMaker/test/RunCongigTest/treeMaker_Spring15_cfg.py
 
```
