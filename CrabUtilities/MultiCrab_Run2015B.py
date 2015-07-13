import os

## check if file exist then ask if you want to delete old one and create new one. 
## if answer is yes then deelte the old and create this one new. 
fout = open("datasetdetails_Spring15.txt","w")
## each line will contain four parameters. 
## taskname   cfg.py  datasetname  numberofdiles
## cfg.py is configurable because data and MC will have different configurations.
## And number of files canbe used as number of lumis in that case. 

#fout.write("MET-Run2015B-PromptReco-v1_1 treeMaker_Run2015B_cfg.py /MET/Run2015B-PromptReco-v1/MINIAOD 5 \n")
fout.write("SingleElectron_Run2015B-PromptReco-v1 treeMaker_Run2015B_cfg.py /SingleElectron/Run2015B-PromptReco-v1/MINIAOD  5 \n")
fout.write("DoubleEG_Run2015B-PromptReco-v1 treeMaker_Run2015B_cfg.py /DoubleEG/Run2015B-PromptReco-v1/MINIAOD 5 \n")

##
##fout.write("DYToEE_M-50_Tune4C_13TeV-pythia8 treeMaker_Spring15_cfg.py /DYToEE_M-50_Tune4C_13TeV-pythia8/Phys14DR-PU20bx25_tsg_castor_PHYS14_25_V1-v1/MINIAODSIM 1 \n")
##
##fout.write("TT_TuneZ2star_13TeV-powheg-pythia6-tauola treeMaker_Spring15_cfg.py /TT_TuneZ2star_13TeV-powheg-pythia6-tauola/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("ggZH_HToBB_ZToNuNu_M120_13TeV_powheg_pythia8 treeMaker_Spring15_cfg.py /ggZH_HToBB_ZToNuNu_M120_13TeV_powheg_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("QCD_Pt-15TTo7000_TuneZ2star-Flat_13TeV_pythia6 treeMaker_Spring15_cfg.py /QCD_Pt-15TTo7000_TuneZ2star-Flat_13TeV_pythia6/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-1000to1500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /DYJetsToLL_M-1000to1500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-100to200_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /DYJetsToLL_M-100to200_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-1500to2000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /DYJetsToLL_M-1500to2000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-2000to3000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /DYJetsToLL_M-2000to3000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-200to400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /DYJetsToLL_M-200to400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-400to500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /DYJetsToLL_M-400to500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-500to700_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /DYJetsToLL_M-500to700_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-700to800_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /DYJetsToLL_M-700to800_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Spring15_cfg.py /WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Spring15_cfg.py /WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Spring15_cfg.py /WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Spring15_cfg.py /WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##


#fout.write("WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1 \n")

#fout.write("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_cfg.py /DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v3/MINIAODSIM 1 \n")

#fout.write("TT_TuneCUETP8M1_13TeV-powheg-pythia8 treeMaker_Spring15_cfg.py /TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v4/MINIAODSIM 1 \n")

#fout.write("TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Spring15_cfg.py /TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1 \n")
fout.close()


def submit():
    print "submitting"
    f = open('datasetdetails_Spring15.txt','r')
    for line in f:
        print line
        a=[]
        b=[]
        c=[]
        d=[]
        a,b,c,d = line.split()
        datasetdetail=[a,b,c,d]
        print datasetdetail
        os.system('crab submit -c crabConfig_data.py General.requestName='+datasetdetail[0]+' JobType.psetName='+datasetdetail[1]+' Data.inputDataset='+datasetdetail[2]+' Data.unitsPerJob='+datasetdetail[3])
    #name =  'crab submit General.requestName='+datasetdetail[0]+' JobType.psetName='+datasetdetail[1]+' Data.inputDataset='+datasetdetail[2]+' Data.unitsPerJob='+datasetdetail[3]
    #print name 
        
        



def status(crabdirname):
    import os
    os.system ("./Statusall.sh "+crabdirname)
    

## Add a help or usage function here 
def help() :
    print "this is under progress"

    


####################################################################################################################################################
####################################################################################################################################################
## this will control the functions   ##
## convert this to python main.      ##
####################################################################################################################################################
####################################################################################################################################################
import os
import sys
print sys.argv


## safety check
## improve this
if len(sys.argv) < 2 :
    print "insufficient options provided see help function "
    help()
    exit (1)


## submit jobs 
if len(sys.argv) == 2 :
    if sys.argv[1] == "submit" :
        submit()


## check status of jobs 
## send the crab directory 
if len(sys.argv) == 3 : 
    if sys.argv[1] == "status" :
        crabdir = sys.argv[2]
        status(crabdir)




