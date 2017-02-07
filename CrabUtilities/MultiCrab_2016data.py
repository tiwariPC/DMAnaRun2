import os

## check if file exist then ask if you want to delete old one and create new one. 
## if answer is yes then deelte the old and create this one new. 
fout = open("datasetdetails_Summer16.txt","w")
## each line will contain four parameters. 
## taskname   cfg.py  datasetname  numberofdiles
## cfg.py is configurable because data and MC will have different configurations.
## And number of files canbe used as number of lumis in that case. 

#fout.write("JetHT-Run2016B treeMaker_Summer16_cfg.py /JetHT/Run2016B-23Sep2016-v3/MINIAOD 20 \n")
#fout.write("JetHT-Run2016C treeMaker_Summer16_cfg.py /JetHT/Run2016C-23Sep2016-v1/MINIAOD 20 \n")
#fout.write("JetHT-Run2016D treeMaker_Summer16_cfg.py /JetHT/Run2016D-23Sep2016-v1/MINIAOD 20 \n")
#

fout.write("MET-Run2016B treeMaker_Summer16_cfg.py /MET/Run2016B-23Sep2016-v3/MINIAOD 20 \n")
fout.write("MET-Run2016C treeMaker_Summer16_cfg.py /MET/Run2016C-23Sep2016-v1/MINIAOD 20 \n")
fout.write("MET-Run2016D treeMaker_Summer16_cfg.py /MET/Run2016D-23Sep2016-v1/MINIAOD 20 \n")

fout.write("MET-Run2016E treeMaker_Summer16_cfg.py /MET/Run2016E-23Sep2016-v1/MINIAOD 20 \n")
fout.write("MET-Run2016F treeMaker_Summer16_cfg.py /MET/Run2016F-23Sep2016-v1/MINIAOD 20 \n")
fout.write("MET-Run2016G treeMaker_Summer16_cfg.py /MET/Run2016G-23Sep2016-v1/MINIAOD 20 \n")
fout.write("MET-Run2016H treeMaker_Summer16_cfg.py /MET/Run2016H-PromptReco-v3/MINIAOD 20 \n")


fout.close()


def submit():
    print "submitting"
    f = open('datasetdetails_Summer16.txt','r')
    for line in f:
        print line
        a=[]
        b=[]
        c=[]
        d=[]
        a,b,c,d = line.split()
        datasetdetail=[a,b,c,d]
        if '2016B' in c:
            period='BCD'
        elif '2016C' in c:
                period='BCD'
        elif '2016D' in c:
            period='BCD'
        elif '2016E' in c:
            period='E'
        elif '2016F' in c:
            period='F'
        else:
            period='p2'
        print c, period
        os.system('cp -p crabConfig_data.py crabConfig.py')
        os.system('sed -i \'s/PERIOD/'+period+'/g\' crabConfig.py')

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




