import os

## check if file exist then ask if you want to delete old one and create new one. 
## if answer is yes then deelte the old and create this one new. 
fout = open("datasetdetails_Spring15.txt","w")
## each line will contain four parameters. 
## taskname   cfg.py  datasetname  numberofdiles
## cfg.py is configurable because data and MC will have different configurations.
## And number of files canbe used as number of lumis in that case. 

fout.write("DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ForEIKO treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM 1 \n")
fout.write("DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_ForMonika treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1 \n")

fout.write("WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1 \n")
fout.write("TT_TuneCUETP8M1_13TeV-powheg-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /TT_TuneCUETP8M1_13TeV-powheg-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v4/MINIAODSIM 1 \n")
fout.write("TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1 \n")
fout.write("TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /TTJets_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1 \n")
fout.write("WW_TuneCUETP8M1_13TeV-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /WW_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1 \n")
fout.write("ZZ_TuneCUETP8M1_13TeV-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /ZZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM 1 \n")
fout.write("WZ_TuneCUETP8M1_13TeV-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /WZ_TuneCUETP8M1_13TeV-pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM 1 \n")
fout.write("TT_TuneZ2star_13TeV-powheg-pythia6-tauola treeMaker_Spring15_Nocleaning_cfg.py /TT_TuneZ2star_13TeV-powheg-pythia6-tauola/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ggZH_HToBB_ZToNuNu_M120_13TeV_powheg_pythia8try treeMaker_Spring15_Nocleaning_cfg.py /ggZH_HToBB_ZToNuNu_M120_13TeV_powheg_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZH_HToBB_ZToNuNu_M120_13TeV_amcatnloFXFX_madspin_pythia8 treeMaker_Spring15_Nocleaning_cfg.py /ZH_HToBB_ZToNuNu_M120_13TeV_amcatnloFXFX_madspin_pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v2/MINIAODSIM 1 \n")
fout.write("QCD_Pt_10to15_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_10to15_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1  \n")
fout.write("QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_15to30_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1  \n")
fout.write("QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_30to50_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM 1  \n")
fout.write("QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_50to80_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1  \n")
fout.write("QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_80to120_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1  \n")
fout.write("QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_120to170_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1  \n")
fout.write("QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_170to300_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM 1  \n")
fout.write("QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_300to470_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1  \n")
fout.write("QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_470to600_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM 1  \n")
fout.write("QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_600to800_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM 1  \n")
fout.write("QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_800to1000_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM 1  \n")
fout.write("QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_1000to1400_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v2/MINIAODSIM 1  \n")
fout.write("QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_1400to1800_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1  \n")
fout.write("QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_1800to2400_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1  \n")
fout.write("QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_2400to3200_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1  \n")
fout.write("QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8  treeMaker_Spring15_Nocleaning_cfg.py   /QCD_Pt_3200toInf_TuneCUETP8M1_13TeV_pythia8/RunIISpring15DR74-Asympt50ns_MCRUN2_74_V9A-v1/MINIAODSIM 1  \n")
fout.write("QCD_Pt-15TTo7000_TuneZ2star-Flat_13TeV_pythia6 treeMaker_Spring15_Nocleaning_cfg.py /QCD_Pt-15TTo7000_TuneZ2star-Flat_13TeV_pythia6/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")

## For Eiko

fout.write("ZprimeToZhToZlephbb_narrow_M-1000_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-1000_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-1200_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-1200_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-1400_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-1400_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-1600_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-1600_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-1800_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-1800_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-2000_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-2000_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-2500_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-2500_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-3000_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-3000_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-3500_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-3500_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-4000_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-4000_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-4500_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-4500_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-600_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-600_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
fout.write("ZprimeToZhToZlephbb_narrow_M-800_13TeV-madgraph treeMaker_Spring15_Nocleaning_cfg.py /ZprimeToZhToZlephbb_narrow_M-800_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n") 

##
##fout.write("DYJetsToLL_M-1000to1500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-1000to1500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-100to200_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-100to200_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-10to50_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-1500to2000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-1500to2000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-2000to3000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-2000to3000_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-200to400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-200to400_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-400to500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-400to500_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-500to700_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-500to700_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("DYJetsToLL_M-700to800_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /DYJetsToLL_M-700to800_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /WJetsToLNu_HT-100To200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /WJetsToLNu_HT-1200To2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /WJetsToLNu_HT-200To400_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##
##fout.write("WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Spring15_Nocleaning_cfg.py /WJetsToLNu_HT-600ToInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM 1 \n")
##


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
        os.system('crab submit General.requestName='+datasetdetail[0]+' JobType.psetName='+datasetdetail[1]+' Data.inputDataset='+datasetdetail[2]+' Data.unitsPerJob='+datasetdetail[3])
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




