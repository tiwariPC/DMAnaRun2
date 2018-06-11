import os

f = open('allbkg_2017.txt','r')
for line in f:

	a=line.split()[0]
	b=line.split()[1]
	c=line.split()[2]
	d=line.split()[3]

	workname='bbDM_bkg'

	if len(a)>100:
		reqname=a[0:100]
	else:
		reqname=a
	dataset=c


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

	print "\n==========================\nSubmitting "+a+"\n==========================\n"
	os.system("crab submit -c crabConfigTemp.py")

f.close()
