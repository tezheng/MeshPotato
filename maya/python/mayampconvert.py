import maya.OpenMaya as OpenMaya
import pyopenvdb as vdb
import pymeshpotato.mpmesh as mepo
import pymeshpotato.pyVDBOutputPlugin as vdbout
import maya.cmds as cmds
import os
 
def particleFillSelection(  ):
	print "running particleFillSelection" 
	# get the active selection
	selection = OpenMaya.MSelectionList()
	OpenMaya.MGlobal.getActiveSelectionList( selection )
	iterSel = OpenMaya.MItSelectionList(selection, OpenMaya.MFn.kMesh)
 
	# go through selection
	while not iterSel.isDone():
 
		# get dagPath
		dagPath = OpenMaya.MDagPath()
		iterSel.getDagPath( dagPath )
 
		# create empty point array
		inMeshMPointArray = OpenMaya.MPointArray()
		inMeshMNormalArray = OpenMaya.MFloatVectorArray() 
		# create function set and get points in world space
		currentInMeshMFnMesh = OpenMaya.MFnMesh(dagPath)
		currentInMeshMFnMesh.getPoints(inMeshMPointArray, OpenMaya.MSpace.kWorld)
		currentInMeshMFnMesh.getNormals(inMeshMNormalArray, OpenMaya.MSpace.kWorld)

		numFaces = currentInMeshMFnMesh.numPolygons() 
		normalList = []
		faceList = []
		#get normals
		for i in range(0,numFaces):
			normal = OpenMaya.MVector()
			currentInMeshMFnMesh.getPolygonNormal(i, normal, OpenMaya.MFn.kMesh)
			normal = normal.normal()
			normalList.append([str(normal[0]), str(normal[1]), str(normal[2])])

	
		#get faces
		for i in range(0, numFaces):
			vertexL = OpenMaya.MIntArray()
			currentInMeshMFnMesh.getPolygonVertices(i, vertexL)
			vertices = []
			for n in range(vertexL.length()):
				vertices.append(str(vertexL[n] + 1))
			faceList.append(vertices)
		# put each point to a list
		pointList = []
		for i in range( inMeshMPointArray.length() ) :
			pointList.append( [str(inMeshMPointArray[i][0]), str(inMeshMPointArray[i][1]), str(inMeshMPointArray[i][2])] )

		return [pointList, normalList, faceList]
def runExample(voxelSize, filePath):
	print "running example"
	listing = particleFillSelection()
	omesh_cube = vdbout.VDBOutputMesh()
	meshspec = mepo.MeshSpec()
	meshspec.voxelSize = voxelSize
	omesh_cube.loadMesh(mepo.getPythonList(listing[0]), mepo.getPythonList(listing[1]), mepo.getPythonList(listing[2]), meshspec)
	omesh_cube.writeMesh(filePath)




def printMessage(*args):
	print "Hello World"
try:
	if cmds.window(qtWin, exists=True):
		cmds.deleteUI(qtWin)
except NameError:
	pass
meshpotato_dir = os.environ['MESHPOTATO_DIR']
## Define path to ui file
pathToFile = meshpotato_dir + '/maya/python/mpconvertForMaya/ui/mpconvertForMaya.ui'

## Load our window and put it into a variable.
qtWin = cmds.loadUI(uiFile=pathToFile)
 
#cmds.deleteUI("qtWin", window=True)
## Open our window
def main():
	cmds.showWindow(qtWin)

# Resize the main window
#
def closeWindow():
	cmds.deleteUI(qtWin)
def convert():
	voxelSize = cmds.textField("voxelSizeField", query = True, text = True)
	filePath = cmds.textField("filePathField", query = True, text = True)
	runExample(float(voxelSize), str(filePath))
	if cmds.checkBox("vdbViewCheckBox", query = True, value = True):
		os.system("vdb_view " + str(filePath) + " & ")
	if cmds.checkBox("meshViewerCheckBox", query = True, value = True):
		os.system("meshviewer " + str(filePath) + " & ")


def browse():
	newfilepath = cmds.fileDialog2(fm=0, okc='Save file as')
	cmds.textField("filePathField", e=1, text=str(newfilepath[0]))

class MPConvert:
	def __init__(self, filePath):
		MPConvert.use = self
		
		if cmds.window(self.qtWin, exists=True):
			cmds.deleteUI(self.qtWin)
		self.qtWin = cmds.loadUI(uiFile=filePath)
	def create(self, verbose=False):
		if cmds.window(self.window, exists = True):
			cmds.deleteUI(self.window)
		self.window = cmds.loadUI(uiFile=self.uiFile, verbose=verbose)
		cmds.showWindow(self.window)
	def showUI(cls, uiFile):
		win = cls(uiFile)
		win.create()
		return win
	def closeWindow():
		cmds.deleteUI(self.qtWin)
	def convert():
		voxelSize = self.getVoxelSize()
		filePath = self.getFilePath()
		runExample(voxelSize, filePath)
		print "Done conversion"
		if useVDBView():
			print "Using vdb_view"
			os.system("vdb_view " + filePath + " & ")
		if useMeshViewer():
			print "Using meshviewer"
			os.system("meshviewer " + filePath + " & ")
	def browse():
		newfilepath = cmds.fileDialog2(fm=0, okc='Save file as')
		cmds.textField("filePathField", e=1, text=str(newfilepath[0]))
	def getVoxelSize():
		return float(cmds.textField("voxelSizeField", query = True, text = True))
	def getFilePath():
		return cmds.textField("filePathField", query = True, text = True)
	def useVDBView():
		return cmds.checkBox("vdbViewCheckBox", query = True, value = True)	
	def useMeshViewer():
		return cmds.checkBox("meshViewerCheckBox", query = True, value = True)	
