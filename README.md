
# DMAnaRun2
For organized the NCU DM repo

setenv SCRAM_ARCH slc6_amd64_gcc491

cmsrel CMSSW_7_4_1

cd CMSSW_7_4_1/src/

cmsenv

## For Electron IDs. Taken from twiki https://twiki.cern.ch/twiki/bin/view/CMS/CutBasedElectronIdentificationRun2
```
git cms-merge-topic 9003 
rm -rf RecoEgamma/ElectronIdentification/data
git clone https://github.com/cms-data/RecoEgamma-ElectronIdentification.git RecoEgamma/ElectronIdentification/data
rm -rf RecoEgamma/PhotonIdentification/data
git clone https://github.com/cms-data/RecoEgamma-PhotonIdentification.git RecoEgamma/PhotonIdentification/data
## For electron testing use following after adding remaining packages. 
cmsRun MVA-MET-PFUncorrectedMET-Jet_cfg_raman.py
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

cmsRun DelPanj/TreeMaker/test/RunCongigTest/MVA-MET-PFUncorrectedMET-Jet_cfg_yunju.py 
```