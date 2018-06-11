import os

## check if file exist then ask if you want to delete old one and create new one.
## if answer is yes then deelte the old and create this one new.
fout = open("datasetdetails_Summer17.txt","w")
## each line will contain four parameters.
## taskname   cfg.py  datasetname  numberofdiles
## cfg.py is configurable because data and MC will have different configurations.
## And number of files canbe used as number of lumis in that case.


fout.write("MET-Run2017B-31Mar2018-v1 treeMaker_Summer17_cfg.py /MET/Run2017B-31Mar2018-v1/MINIAOD 20 \n")
fout.write("MET-Run2017C-31Mar2018-v1 treeMaker_Summer17_cfg.py /MET/Run2017C-31Mar2018-v1/MINIAOD 20 \n")
fout.write("MET-Run2017D-31Mar2018-v1 treeMaker_Summer17_cfg.py /MET/Run2017D-31Mar2018-v1/MINIAOD 20 \n")
fout.write("MET-Run2017E-31Mar2018-v1 treeMaker_Summer17_cfg.py /MET/Run2017E-31Mar2018-v1/MINIAOD 20 \n")
fout.write("MET-Run2017F-31Mar2018-v1 treeMaker_Summer17_cfg.py /MET/Run2017F-31Mar2018-v1/MINIAOD 20 \n")


fout.write("SingleElectron-Run2017B-31Mar2018-v1 treeMaker_Summer17_cfg.py /SingleElectron/Run2017B-31Mar2018-v1/MINIAOD 20 \n")
fout.write("SingleElectron-Run2017C-31Mar2018-v1 treeMaker_Summer17_cfg.py /SingleElectron/Run2017C-31Mar2018-v1/MINIAOD 20 \n")
fout.write("SingleElectron-Run2017D-31Mar2018-v1 treeMaker_Summer17_cfg.py /SingleElectron/Run2017D-31Mar2018-v1/MINIAOD 20 \n")
fout.write("SingleElectron-Run2017E-31Mar2018-v1 treeMaker_Summer17_cfg.py /SingleElectron/Run2017E-31Mar2018-v1/MINIAOD 20 \n")
fout.write("SingleElectron-Run2017F-31Mar2018-v1 treeMaker_Summer17_cfg.py /SingleElectron/Run2016F-03Feb2017-v1/MINIAOD 20 \n")

fout.write("SinglePhoton-Run2017B-31Mar2018-v1 treeMaker_Summer17_cfg.py /SinglePhoton/Run2017B-31Mar2018-v1/MINIAOD 20 \n")
fout.write("SinglePhoton-Run2017C-31Mar2018-v1 treeMaker_Summer17_cfg.py /SinglePhoton/Run2017C-31Mar2018-v1/MINIAOD 20 \n")
fout.write("SinglePhoton-Run2017D-31Mar2018-v1 treeMaker_Summer17_cfg.py /SinglePhoton/Run2017D-31Mar2018-v1/MINIAOD 20 \n")
fout.write("SinglePhoton-Run2017E-31Mar2018-v1 treeMaker_Summer17_cfg.py /SinglePhoton/Run2017E-31Mar2018-v1/MINIAOD 20 \n")
fout.write("SinglePhoton-Run2017F-31Mar2018-v1 treeMaker_Summer17_cfg.py /SinglePhoton/Run2016F-03Feb2017-v1/MINIAOD 20 \n")



fout.close()


def submit():
    print "submitting"
    f = open('datasetdetails_Summer17.txt','r')
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
            period='EF'
        elif '2016F' in c:
            period='EF'
        elif '2016G' in c:
            period='G'
        else:
            period='H'
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
