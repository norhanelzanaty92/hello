import sys
import re
import os.path

#adding the header to the file
def Addingtofile(Headerfile,ASMfile):
	f1=open(Headerfile,'r')
	f2=open(ASMfile,'r')
	lines1=f1.readlines()
	lines2=f2.readlines()
	f1.close()
	f2.close()
	f2=open(ASMfile,'w')
	for line in lines1:	
		f2.write(line)
	for line in lines2:	
		f2.write(line)
	f2.close()

def includeFileintoAsm(ImportFile,ASMfile):
	Import_File=open(ImportFile,'r')
	lines=Import_File.readlines()
	Import_File.close()
	for line in lines:
		match=re.search(r'.import\W+(\w+[.]inc)',line)
		if match:
			Addingtofile(match.group(1),ASMfile)
		else:
			None

#adding the equ to file 
def findAndReplaceLabel(ASMfile,Labelfile):
	Label_File=open(Labelfile, 'r')
	lines = Label_File.readlines()	
	for line in lines:
#\1 is to be reblaced with \2#######################################################################################
		match=re.search(r'(\w*)[:](\w*)',line)
		if match:
			findAndReplaceHelper(match,ASMfile)				
		else:
			None
	Label_File.close()


#adding the adresses to the asm
def addingAddresses(Asmfile):
	pc=0000
	ASM_File=open(Asmfile, 'r')
	Label_File=open('out.label','w')
	lines = ASM_File.readlines()
	ASM_File.close()
	
	ASM_adress=open('out.asmadd','w')
	for line in lines:
		matchLabel=re.search(r'(\w+:)',line)
		match=re.search(r'.org\W+(\d+)',line)
		matchEmpty=re.search(r'.org\W+(\d+)',line)
		if match:
			pc=int(match.group(1),16)
		elif matchLabel:
			Label_File.write(line.rstrip("\n")+hex(pc)+"\n")      ##adding the label to out.label with its currnt address 
		else:
			if len(line.rstrip(r"\n"))>4:
				#ASM_adress.write(';'+hex(pc)+';'+line)## adding the addresses to file 
				ASM_adress.write(line)## adding the addresses to file 
				pc=pc+1
			else:
				None
	ASM_adress.close()
	Label_File.close()

#adding the adresses to the asm
def addingLineEndToTheLabel(Asmfile):
	ASM_File=open(Asmfile, 'r')
	lines = ASM_File.readlines()
	ASM_File.close()
	ASM_File=open(Asmfile, 'w')
	for line in lines:
		match=re.search(r'(\w+:)',line)
		if match:
			ASM_File.write(re.sub(r'(\w+:)',r'\1\n',line))
		else:
			ASM_File.write(line)
	ASM_File.close()

#adding the equ to file 
def findAndReplace(ASMfile,EQUfile):
	EQU_File=open(EQUfile, 'r')
	lines = EQU_File.readlines()	
	for line in lines:
#\1 is to be reblaced with \2
		match=re.search(r'.equ\W+(\w+)\W*(\w*)',line)
		if match:
			findAndReplaceHelper(match,ASMfile)				
		else:
			None
	EQU_File.close()


def findAndReplaceHelper(match,ASMfile):
	ASM_File=open(ASMfile, 'r')
	ASMLines=ASM_File.readlines()
	ASM_File.close()
	ASM_File=open(ASMfile, 'w')
	for ASMline in ASMLines:
		ASM_File.write(re.sub(match.group(1),match.group(2), ASMline))
	ASM_File.close()



#dont forget to elemenate the comment
def commentElemenate(file_Name):
	open_FileOld  = open(file_Name, 'r')
	lines = open_FileOld.readlines()
	open_FileOld.close()
	open_FileNew  = open(file_Name, 'w')
	for line in lines:
		if re.search(r'[//][//].*',line):
			open_FileNew.write(re.sub(r'[//][//].*','', line))
		elif re.search(r'[;].*',line):
			open_FileNew.write(re.sub(r'[;].*','', line))
		elif re.search(r'[#].*',line):
			open_FileNew.write(re.sub(r'[#].*','', line))
		elif re.search(r'[.]def.*',line):
			None			
			open_FileNew.write(re.sub(r'[.]def.*','', line))
		elif re.search(r'[.]device',line):
			microType_File=open('out.microType','w')
			microType_File.write(line)
			microType_File.close();
		else:	
			open_FileNew.write(line)

	open_FileNew.close() 
	
# find the .include
def findInclude(file_Name):
	open_File  = open(file_Name, 'r')
	import_File= open('out.import', 'w')
	lines = open_File.readlines()
	for line in lines:
		if re.search(r'[.]import', line): 
			import_File.write(line)
		else:  
			None
	open_File.close()       
	import_File.close()

#find (.equ) and replace it in the file  
def divideFile(file_Name):
	open_File  = open(file_Name, 'r')
	import_File= open('out.import', 'w')
	equ_File   = open('out.equ', 'w')
	asm_File   = open('out.asm', 'w')
	def_File   =open('out.def','w')		
	lines = open_File.readlines()
	for line in lines:
		if re.search(r'[.]equ', line):
			equ_File.write(line)
		elif re.search(r'[.]import', line): 
			import_File.write(line)
		#elif re.search(r'[.]def', line):
		#	def_File.write(line)
		else:  
			asm_File.write(line)
            
	open_File.close()       
	import_File.close()
	equ_File.close()
	asm_File.close()
     	def_File.close()




# Define a main() function that prints a little greeting.
def main():
  # Get the name from the command line, using 'World' as a fallback.
  if len(sys.argv) >= 2:
	match=re.search(r'[.]asm',sys.argv[1])
	if match:
		print 'found'
		re.search('.asm',sys.argv[1])
		file_Name = sys.argv[1]
		#file_Name=re.sub('.asm','.mahmoud',file_Name)
		print file_Name
		if os.path.exists(file_Name):
			print 'the file exists'
			#here is the start of the prog 
			findInclude(file_Name)
		#Addingtofile('out.import',file_Name)
			includeFileintoAsm('out.import',file_Name)			

			divideFile(file_Name)
			commentElemenate('out.import')

#### adding the import file to the asm befor elemenating the comment ##############################
			commentElemenate('out.equ')
			commentElemenate('out.asm')

##.def finding and replace  replace 
			findAndReplace('out.asm','out.equ')
			addingLineEndToTheLabel('out.asm')
			addingAddresses('out.asm')#adding addreses to the lines 
			findAndReplaceLabel('out.asmadd','out.label')
		else:
			print 'the file doesnt exist'
	else:
		print 'not avalid file it must be .asm'
  else:
	print 'plz enter the file.asm'

# This is the standard boilerplate that calls the main() function.
if __name__ == '__main__':
  main()

	
