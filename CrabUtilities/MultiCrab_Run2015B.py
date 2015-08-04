import os

## check if file exist then ask if you want to delete old one and create new one. 
## if answer is yes then deelte the old and create this one new. 
fout = open("datasetdetails_Spring15.txt","w")
## each line will contain four parameters. 
## taskname   cfg.py  datasetname  numberofdiles
## cfg.py is configurable because data and MC will have different configurations.
## And number of files canbe used as number of lumis in that case. 

#fout.write("MET-Run2015B-PromptReco-v1TotalV3 treeMaker_Spring15_Nocleaning_cfg.py /MET/Run2015B-PromptReco-v1/MINIAOD 12 \n")
#fout.write("MET_Run2015B-17Jul2015-v1_RR  treeMaker_Spring15_Nocleaning_cfg.py /MET/Run2015B-17Jul2015-v1/MINIAOD 5 \n")
fout.write("SingleElectron_Run2015B-PromptReco-v1_WZJSON_V1 treeMaker_Spring15_Nocleaning_cfg.py /SingleElectron/Run2015B-PromptReco-v1/MINIAOD  5 \n")
fout.write("DoubleEG_Run2015B-PromptReco-v1_WZJSON_V1 treeMaker_Spring15_Nocleaning_cfg.py /DoubleEG/Run2015B-PromptReco-v1/MINIAOD 5 \n")

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




