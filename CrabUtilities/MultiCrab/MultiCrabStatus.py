from os import listdir, system
from os.path import isfile, join

workname='bbDM_signal'

mypath="crab_"+workname+"/"
dirs = [f for f in listdir(mypath) if not isfile(join(mypath, f))]

njobs=len(dirs)

for ind in range(njobs):	
	print "\n==========================\nStatus of job "+str(ind+1)+" of "+str(njobs)+"\n==========================\n"
	system("crab status -d "+mypath+dirs[ind])
