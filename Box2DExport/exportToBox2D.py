import maya.OpenMaya as OM
import maya.OpenMayaAnim as OMA
import maya.OpenMayaMPx as OMX
import maya.cmds as cmds
import sys, math

structure="""
typedef struct
{
	std::string name;
	float tx;
	float ty;
	float width;
	float height;
	b2BodyType type;
}Body; \n
"""


def exportBox2D() :
#	basicFilter = "*.b2d"
#	file=cmds.fileDialog2(caption="Please select file to save",fileFilter=basicFilter, dialogStyle=2)
	file="test.bd2"
	if file !="" :

		dagIt = OM.MItDag(OM.MItDag.kDepthFirst, OM.MFn.kTransform)
		object = OM.MObject
		ofile=open(file[0],'w')
		ofile.write(structure)
		ofile.write('\n\nBody bodies[]={\n')
		numBodies=0
		while not dagIt.isDone():

			object = dagIt.currentItem()
			depNode = OM.MFnDependencyNode(object)
			if object.apiTypeStr() =="kTransform" :

				fn = OM.MFnTransform(object)
				child = fn.child(0)

				if child.apiTypeStr()=="kMesh"  :
					name=fn.name()
					ofile.write('\t{ "%s",' %(name) )

					x=cmds.getAttr("%s.translateX" %(name))
					ofile.write('%sf,' %(x))
					y=cmds.getAttr("%s.translateY" %(name))
					ofile.write('%sf,' %(y))
					width=cmds.getAttr("%s.scaleX" %(name))
					ofile.write('%sf,' %(width))
					height=cmds.getAttr("%s.scaleY" %(name))
					ofile.write('%sf,' %(height))
					type=cmds.getAttr("%s.Box2D" %(name))

					ofile.write('%s },\n' %(type))
					numBodies=numBodies+1


			dagIt.next()
		ofile.write("};\n")

		ofile.close()


exportBox2D()