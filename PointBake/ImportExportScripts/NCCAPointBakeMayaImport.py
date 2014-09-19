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
import xml.sax
import maya.cmds as cmds
import sys, math


"""@package docstring
This module will allow the selection of an obj file and a xml based NCCA Point bake file and
load in point baked animation to a maya scene. The user is prompted for a base node name
which is pre-pended to all elements create in the loading process,

@author Jonathan Macey
@version 1.0
@date Last Revision 10/01/11 fixed bug in xml parse
 previous updates :
 Updated to NCCA Coding standard

"""

########################################################################################################################
## @class ParseHandler
## @brief a class to wrap the processing of the xml.sax parser, all of the methods are called
## from this class however the class data is specific to the parsing process. In this case it is a
## parser for the NCCA PointBake file format
########################################################################################################################


class ParseHandler(xml.sax.ContentHandler):

	## @brief ctor for the class passing in the houdini channel we wish to load the
	## PB data into
	## @param[in] _selectedText the mesh the data is to be loaded too
	def __init__(self,_selectedText):
		## @brief the object selected to load the data too.
		self.m_selectedObject=_selectedText
		## @brief the Character Data stored as part of parsing
		self.m_charData=""
		## @brief the m_meshName extracted from the PointBake file
		self.m_meshName=""
		## @brief number of vertices in the mesh, we will check this against the number of points in
		## the mesh / obj loaded as a basic compatibility check
		self.m_numVerts=0
		## @brief the Start frame for the data loaded
		self.m_startFrame=0
		## @brief m_endFrame of the data loaded
		self.m_endFrame=0
		## @brief number of frames stored in file not used in this example
		self.m_numFrames=0
		## @brief the Offset into the vertex list for the current data to be set too
		self.m_offset=None
		## @brief the Current frame to be stored / keyed
		self.m_currentFrame=0
		# the maya time control
		self.m_anim=OMA.MAnimControl()
		# a point array structure, we will load each frame's worth of data into this then
		# load it to the mesh point data each frame, once this is done we need to clear this data for
		# the next frame
		self.m_vertData=OM.MFloatPointArray()
		# grab the object ready to set the point data
		selected = OM.MSelectionList()
		obj=OM.MObject( )
		selected.add(self.m_selectedObject)
		selected.getDependNode(0,obj)

		fn = OM.MFnTransform(obj)
		self.m_mesh=""
		oChild = fn.child(0)

		if(oChild.apiTypeStr()=="kMesh") :
			print "got Mesh"
			# get our mesh
			self.m_mesh=OM.MFnMesh(oChild)
		# set the frame to start


	def __del__(self) :
		print "done"
	## @brief here we trigger events for the start elements In this case we grab the Offset and Frame
	## @param[in] _name the name of the tag to process
	## @param[in] _attrs the attribute associated with the current tag
	def startElement(self, _name, _attrs):
		# this is important the characters method may be called many times so we
		# clear the char data each start element then append each time we read it
		self.m_charData=""
		# if we have a vertex start tag process and extract the offset
		if _name == "Vertex" :
			self.m_offset=int(_attrs.get("number"))
		# if we have the Frame we grab the number attribute
		elif _name == "Frame" :
			# set the frame here
			self.m_currentFrame=int(_attrs.get("number"))
			self.m_anim.setCurrentTime(OM.MTime(self.m_currentFrame))
			# we have a new frame so re-set the vertexPoint data ready to be filled
			# with the new dara
			self.m_vertData.clear()

	## @brief trigger method if we have data between the <> </> tags, copy it to the class m_charData so
	## we can re-use it later
	## @param[in] _content the character string passed from the parser.
	def characters(self,_content):
		# here we append the content data passed into the method, we need to append
		# as this function may be called more than once if we have a long string
		self.m_charData += _content

	## @brief most of the hard processing is done here. Once an end tag is encountered we
	## process the current char data and add it to the channel created. This does
	## rely on the order of the data but this is always machine generated so we should
	## be safe if it does go wrong it will be this data ordering
	## @brief[in] _name the name of the end element tag
	def endElement(self, _name):
		# extract the m_meshName and save it
		if _name == "MeshName":
			self.m_meshName=self.m_charData
		# get the number of vertices and set this to the channel
		elif _name == "NumVerts" :
			# store value
			self.m_numVerts=int(self.m_charData)

		# parse and sel the m_startFrame
		elif _name == "StartFrame" :
			self.m_startFrame=int(self.m_charData)
			# set the time control to this value
			self.m_anim.setMinTime(OM.MTime(self.m_startFrame))
		## found an end frame value
		elif _name == "EndFrame" :
			self.m_endFrame=int(self.m_charData)
			# set the end animation time

		## found the number of frames
		elif _name == "NumFrames" :
			self.m_numFrames=int(self.m_charData)
		## found the vertex
		elif _name =="Vertex" :
			self.m_charData=self.m_charData.strip()
			data=self.m_charData.split(" ")
			## now we check to see if there are enough values to parse
			if len(data) == 3 :
				# append the vertex data to the array for later loading into the mesh
				self.m_vertData.append(float(data[0]),float(data[1]),float(data[2]))
		elif _name=="Frame" :
			# now we have the end of the frame we should have all the vertex data in the array
			# so we can set this point position for our mesh
			self.m_mesh.setPoints(self.m_vertData)
			# once we have done this we can set this as a keyframe
			cmds.setKeyframe(breakdown=0, hierarchy="none",controlPoints=0 ,shape=0,attribute="vtx[*]")
			# now we clear the point data ready for the next frame to load hte data in
			self.m_vertData.clear()


########################################################################################################################
## @class PointBakeImport
## @brief this is the main function we call for the import of the mesh
## it will create a simple ui and once things are setup the data will be loaded
########################################################################################################################


class PointBakeImport() :
	# ctor
	def __init__(self) :

		# create a promptDialog for the base group name of our mesh this will help to
		# avoid name conflicts, may be good to modify this at some stage to check if mesh
		# exists and prompt to replace data / key
		result = cmds.promptDialog(
															title='Name',
															message='Enter Name for import',
															button=['OK', 'Cancel'],
															defaultButton='OK',
															cancelButton='Cancel',
															dismissString='Cancel')

		# if ok was pressed lets process the data
		if result == 'OK':
			# first we get the text entered by the user
			self.m_text = cmds.promptDialog(query=True, text=True)
			# now get the obj file to import
				self.m_objFileName=cmds.fileDialog2(caption="Please select obj file to import",fileFilter=basicFilter, fm=1)

			cmds.file(self.m_objFileName,i=True,type="OBJ",ns=self.m_text)
			# now the xml file
			basicFilter = "*.xml"
			self.m_pointBakeFile=cmds.fileDialog2(caption="Please select xml file to import",fileFilter=basicFilter, fm=1)
			# select the object imported
			print self.m_pointBakeFile
			cmds.select("%s:Mesh"%(self.m_text))
			# and pass control back to the parser
			parser = xml.sax.make_parser()
			parser.setContentHandler(ParseHandler("%s:Mesh"%(self.m_text)))
			parser.parse(open(str(self.m_pointBakeFile[0]),"r"))
