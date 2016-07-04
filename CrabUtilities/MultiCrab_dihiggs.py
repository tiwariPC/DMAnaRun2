import os

## check if file exist then ask if you want to delete old one and create new one. 
## if answer is yes then deelte the old and create this one new. 
fout = open("datasetdetails_Fall15.txt","w")
## each line will contain four parameters. 
## taskname   cfg.py  datasetname  numberofdiles
## cfg.py is configurable because data and MC will have different configurations.
## And number of files canbe used as number of lumis in that case. 

## data
fout.write("BulkGravTohhTohbbhbb_narrow_M-1200_13TeV-madgraph treeMaker_Summer16_cfg.py /BulkGravTohhTohbbhbb_narrow_M-1200_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM 1 \n")
fout.write("BulkGravTohhTohbbhbb_narrow_M-1400_13TeV-madgraph treeMaker_Summer16_cfg.py /BulkGravTohhTohbbhbb_narrow_M-1400_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM 1 \n")
fout.write("BulkGravTohhTohbbhbb_narrow_M-1600_13TeV-madgraph treeMaker_Summer16_cfg.py /BulkGravTohhTohbbhbb_narrow_M-1600_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM 1 \n")
fout.write("BulkGravTohhTohbbhbb_narrow_M-1800_13TeV-madgraph treeMaker_Summer16_cfg.py /BulkGravTohhTohbbhbb_narrow_M-1800_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM 1 \n")
fout.write("BulkGravTohhTohbbhbb_narrow_M-2500_13TeV-madgraph treeMaker_Summer16_cfg.py /BulkGravTohhTohbbhbb_narrow_M-2500_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM 1 \n")
fout.write("BulkGravTohhTohbbhbb_narrow_M-3000_13TeV-madgraph treeMaker_Summer16_cfg.py /BulkGravTohhTohbbhbb_narrow_M-3000_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM 1 \n")
fout.write("BulkGravTohhTohbbhbb_narrow_M-4000_13TeV-madgraph treeMaker_Summer16_cfg.py /BulkGravTohhTohbbhbb_narrow_M-4000_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM 1 \n")
fout.write("BulkGravTohhTohbbhbb_narrow_M-4500_13TeV-madgraph treeMaker_Summer16_cfg.py /BulkGravTohhTohbbhbb_narrow_M-4500_13TeV-madgraph/RunIISpring16MiniAODv2-PUSpring16RAWAODSIM_reHLT_80X_mcRun2_asymptotic_v14-v1/MINIAODSIM 1 \n")

## MC


#fout.write("QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Fall15_Nocleaning_cfg.py /QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM 1 \n")
#fout.write("QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Fall15_Nocleaning_cfg.py /QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM 1 \n")
#fout.write("QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Fall15_Nocleaning_cfg.py /QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM 1 \n")
#fout.write("QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Fall15_Nocleaning_cfg.py /QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM 1 \n")
#fout.write("QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Fall15_Nocleaning_cfg.py /QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM 1 \n")
#fout.write("QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Fall15_Nocleaning_cfg.py /QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM 1 \n")
#fout.write("QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Fall15_Nocleaning_cfg.py /QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM 1 \n")

#fout.write("QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Fall15_Nocleaning_cfg.py /QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM 1 \n")
#fout.write("QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8 treeMaker_Fall15_Nocleaning_cfg.py /QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/MINIAODSIM 1 \n")
#fout.write("QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_2 treeMaker_Fall15_Nocleaning_cfg.py /QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIIFall15MiniAODv2-PU25nsData2015v1_76X_mcRun2_asymptotic_v12_ext1-v1/MINIAODSIM 1 \n")



fout.close()


def submit():
    print "submitting"
    f = open('datasetdetails_Fall15.txt','r')
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




