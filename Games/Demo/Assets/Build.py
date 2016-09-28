import os
import subprocess
import shutil
import Image

def walk(dir):
	curPath = os.path.join(defaultData, dir)
	for name in os.listdir(curPath):
		path = os.path.join(curPath, name)
		if os.path.isfile(path):
			dest1 = os.path.join(androidData, dir)
			dest = os.path.join(dest1, name)
			fileExt = os.path.splitext(path)
			if fileExt[1] == ".png":
				img = Image.open(path, 'r')
				if img.mode == 'RGBA':
					print ("Hase Alpha")
					print (path)
					shutil.copy(path, dest)
				else:
					print ("Not Alpha")
					print (path)
					filename = os.path.splitext(name)
					dest = os.path.join(dest1, filename[0])
					dest = dest + ".pkm"
					outFile = "-o " + dest
					args = "etc1tool.exe " + path + " " + outFile
					print args
					subprocess.call(args)
					
			else:
				shutil.copy(path, dest)
		else:
			nextDir = os.path.join(dir, name)
			fullDir = os.path.join(androidData, nextDir)
			if not os.path.exists(fullDir):
				os.mkdir(fullDir)
			walk(nextDir)

cwd = os.getcwd()
defaultData = os.path.join(cwd, "Default")
androidData = os.path.join(cwd, "Android")

if not os.path.exists(androidData):
	os.mkdir(androidData)

walk("")