"""
  Copyright (C) 2011 Jon Macey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
"""

import maya.OpenMaya as OM
import maya.OpenMayaAnim as OMA
import maya.OpenMayaMPx as OMX
import maya.cmds as cmds
import sys, math


########################################################################################################################
##  @brief simple function to write data out with tabs for a well formatted xml file
##	@param[in] _file the file pointer to write data too
##  @param[in] _nTabs number of tabs to write before the data
##  @param[in] _data the actual data to write out to the file
########################################################################################################################

def WriteData(_file,_nTabs,_data) :
	for i in range(0,_nTabs) :
		_file.write("\t")
	_file.write(_data)
	_file.write("\n")


########################################################################################################################
##  @brief function to extract and write out the xml data to a file, we don't use any XML
##  lib so there is no real check for correct formatting of the data, be carful!
##	@param[in] _fileName the file name to open
##  @param[in] _name name of the mesh selected
##  @param[in] _startFrame  the start frame for the export
##  @param[in] _endFrame  the end frame for the export
########################################################################################################################

def NCCAPointBake(_fileName,_name,_startFrame,_endFrame) :

	# grab the selected object
	selected = OM.MSelectionList()
	obj=OM.MObject( )
	selected.add(_name)
	selected.getDependNode(0,obj)
	# get the parent transform
	fn = OM.MFnTransform(obj)
	Mesh=""
	oChild = fn.child(0)
	# check to see if what we have is a mesh
	if(oChild.apiTypeStr()=="kMesh") :
		print "got Mesh"
		# get our mesh
		Mesh=OM.MFnMesh(oChild)
	else :
		print "Didn't get mesh ", oChild.apiType()
		return

	# now we try and open the file for writing
	try :
		file=open(str(_fileName[0]),'w')
	# if this fails catch the error and exit
	except IOError :
		print "Error opening file",str(_fileName)
		return

	# set the frame to start
	print "PB get anim control"
	currFrame=OM.MTime()
	anim=OMA.MAnimControl()
	# as these can take time to process we have an interupter to allow for the process to be
	# stopped
	interupter=OM.MComputation()
	# set the start of the heavy computation
	interupter.beginComputation()
	# now we set the tab level to 0 for the initial write to the file
	tabIndent=0

	# now we get the mesh number of points
	numPoints = cmds.polyEvaluate( _name, v=True)
	# write the xml headers
	file.write("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n")
	file.write("<NCCAPointBake>\n")
	# up the tab level
	tabIndent=tabIndent+1
	# write the initial header data
	WriteData(file,tabIndent,"<MeshName> %s </MeshName>" %(_name))
	WriteData(file,tabIndent,"<NumVerts> %d </NumVerts>" %(numPoints))
	WriteData(file,tabIndent,"<StartFrame> %s </StartFrame>" %(_startFrame))
	WriteData(file,tabIndent,"<EndFrame> %s </EndFrame>" %(_endFrame))
	WriteData(file,tabIndent,"<NumFrames> %s </NumFrames>" %(_endFrame-_startFrame))
	WriteData(file,tabIndent,"<TranslateMode> %s </TranslateMode>" %("absolute"))

	# now for every frame write out the vertex data
	for frame in range(_startFrame,_endFrame) :
		print "Doing frame %04d" %(frame)
		# move to the correct frame
		currFrame.setValue (frame)
		anim.setCurrentTime(currFrame)
		# write out the frame tag
		WriteData(file,tabIndent,"<Frame number=\"%d\">" %(frame))
		tabIndent=tabIndent+1
		for vertex in range(0,numPoints) :
			# now the actual vertex data for the current mesh index value
			data = cmds.xform( (_name+ ".vtx["+str(vertex)+"]"), q=True, ws=True, t=True )
			WriteData(file,tabIndent,"<Vertex number=\"%d\" attrib=\"translate\"> %f %f %f </Vertex>" %(vertex,data[0],data[1],data[2]))
		# now un-indent as we have ended the frame
		tabIndent=tabIndent-1
		WriteData(file,tabIndent,"</Frame>")
		# if we have interupted exit and finish
		if interupter.isInterruptRequested()  :
			file.write("</NCCAPointBake>\n")
			file.close()
			print "File export interrupted ";
			return
	# now finish
	file.write("</NCCAPointBake>\n")
	# and close the file
	file.close()

########################################################################################################################
##  @brief actual function call used to do the import
########################################################################################################################

class PointBakeExport() :
	########################################################################################################################
	# @brief ctor
	########################################################################################################################

	def __init__(self) :
		# get the currently selected objects and make sure we have only one object
		selected = OM.MSelectionList()
		OM.MGlobal.getActiveSelectionList(selected)
		self.selectedObjects = []
		selected.getSelectionStrings(self.selectedObjects)
		if len(self.selectedObjects) == 0 :
			cmds.confirmDialog( title='No objects Selected', message='Select a Mesh Object', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
		elif len(self.selectedObjects) > 1 :
			cmds.confirmDialog( title='Select One Object', message='Only One Mesh mat be exported at a time', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
		# now we have the correct criteria we can proceed with the export
		else :
			# get the start and end values for our UI sliders
			anim=OMA.MAnimControl()
			minTime=anim.minTime()
			maxTime=anim.maxTime()
			self.m_start=int(minTime.value())
			self.m_end=int(maxTime.value())
			# now we create a window ready to populate the components
			self.m_window = cmds.window( title='NCCA Pointbake Export' )
			# create a layout
			cmds.columnLayout()
			# create two sliders for start and end we also attach methods to be called when the slider
			# changes
			self.m_startSlider=cmds.intSliderGrp( changeCommand=self.startChanged,field=True, label='Start Frame', minValue=self.m_start, maxValue=self.m_end, fieldMinValue=self.m_start, fieldMaxValue=self.m_end, value=self.m_start )
			self.m_endSlider=cmds.intSliderGrp( changeCommand=self.endChanged ,field=True, label='End Frame', minValue=self.m_start, maxValue=self.m_end, fieldMinValue=self.m_end, fieldMaxValue=self.m_end, value=self.m_end )
			# create a button and add the method called when pressed
			cmds.button( label='Export', command=self.export )
			# finally show the window
			cmds.showWindow( self.m_window )

	########################################################################################################################
	# @brief export method attached ot the button, this will be executed once every time
	# the button is pressed
	# @param *args the arguments passed from the button
	########################################################################################################################

	def export(self,*args) :
		# get the file name to save too
		basicFilter = "*.xml"
		file=cmds.fileDialog2(caption="Please select file to save",fileFilter=basicFilter, dialogStyle=2)
		# check we get a filename and then save
		if file !="" :
			if self.m_start >= self.m_end :
				cmds.confirmDialog( title='Range Error', message='start >= end', button=['Ok'], defaultButton='Ok', cancelButton='Ok', dismissString='Ok' )
			else :
				NCCAPointBake(file,self.selectedObjects[0],self.m_start,self.m_end)
				# finally remove the export window
				cmds.deleteUI( self.m_window, window=True )

	########################################################################################################################
	# @brief this is called every time the slider is changed (i.e. a new value)
	# @param *args the arguments passed from the button [0] is the numeric value
	########################################################################################################################

	def startChanged(self, *args) :
		self.m_start=args[0]

	########################################################################################################################
	# @brief this is called every time the slider is changed (i.e. a new value)
	# @param *args the arguments passed from the button [0] is the numeric value
	########################################################################################################################

	def endChanged(self, *args) :
		self.m_end=args[0]




















