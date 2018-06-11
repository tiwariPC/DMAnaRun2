import os, datetime
datestr = datetime.date.today().strftime("%Y%m%d")

f = open('NLO-Samples.txt','r')
for line in f:

	if line.startswith("/BBbarDMJets"):
		workname='bbDM_signal_'+datestr

		spt=line.split()
		spt2=line.split("_")

		dataset=spt[0]
		reqname=spt2[1]+"_"+spt2[2]+"_"+spt2[3]+"_"+spt2[4]

		tempfile = open('crabConfigTemp.py','w')
		cfile = open('crabConfig.py','r')
		for cline in cfile:
			if cline.startswith("workname="):
				tempfile.write("workname=\'"+workname+"\'\n")
			elif cline.startswith("reqname="):
				tempfile.write("reqname=\'"+reqname+"\'\n")
			elif cline.startswith("dataset="):
				tempfile.write("dataset=\'"+dataset+"\'\n")
			else:
				tempfile.write(cline)
		cfile.close()
		tempfile.close()

		print "\n==========================\nSubmitting "+reqname+"\n==========================\n"
		os.system("crab submit -c crabConfigTemp.py")

f.close()
