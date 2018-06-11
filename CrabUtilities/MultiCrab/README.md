# MultiCrab
Script to automatically submit CRAB jobs for all samples in a list.

## Instructions
1. Move required files from directory inputFiles to script directory.  

2. Set `cmsenv`:
   ```
   $ cmsenv
   ```  

3. Source CRAB3:
   ```
   $ source /cvmfs/cms.cern.ch/crab3/crab.sh
   ```  

4. Initiate voms-proxy:
   ```
   $ voms-proxy-init --voms cms --valid 168:00
   ```  

5. Edit crabConfig.py to change CRAB configurations and output directory structure.

6. Copy the treeMaker_Summer17_cfg file:
   ```
   $ cp $CMSSW_BASE/src/DelPanj/TreeMaker/test/RunConfigTest/treeMaker_Summer17_cfg.py .

6. To submit CRAB jobs for all background samples listed in allbkg_2017.txt:
   ```
   $ python MultiCrab_bkg.py
   ```  

7. To submit CRAB jobs for all signal samples listed in NLO-Samples.txt:
   ```
   $ python MultiCrab_Signal.py
   ```  

8. To check status of one set of CRAB jobs (bkg/signal), edit `workname` in MultiCrabStatus.py ('bbDM_bkg'/'bbDM_signal'), and run:
   ```
   $ python MultiCrabStatus.py
   ```
