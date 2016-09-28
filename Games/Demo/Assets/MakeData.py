import os
import shutil
import Image

def walk(dir):
	curPath = os.path.join(defaultData, dir)
	for name in os.listdir(curPath):
		path = os.path.join(curPath, name)
		if os.path.isfile(path):
			dest = os.path.join(androidData, dir)
			dest = os.path.join(dest, name)
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