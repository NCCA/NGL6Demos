import maya.OpenMaya as OM
import maya.OpenMayaAnim as OMA
import maya.OpenMayaMPx as OMX
import maya.cmds as cmds
import sys, math


structure="""
typedef struct
{
  b2Body *body;
	std::string name;
	float tx;
	float ty;
	float width;
	float height;
	float rotation;
	float friction;
	float restitution;
	float density;
	b2BodyType type;
}Body; \n
"""



class Box2DTool():

	def __init__(self) :

		#check to see if the window exists:
		if cmds.window("Box2DTool", exists = True):
			cmds.deleteUI("Box2DTool")

		#create the window:
		window = cmds.window("Box2DTool", title = 'Box2D Tool', sizeable = False)

		#create the main layout:
		cmds.columnLayout(columnWidth = 300, adjustableColumn = False, columnAttach = ('both', 10))

		#make dockable:
		allowedAreas = ['right', 'left']
		cmds.dockControl( 'Box2D Tool', area='left', content=window, allowedArea=allowedAreas )

		self.dim=cmds.floatFieldGrp('dim', numberOfFields=2, label='Dimension', extraLabel='pixel', value1=5, value2=1  )
		self.dim=cmds.floatFieldGrp('friction', numberOfFields=1, label='Friction',  value1=0.2 )
		self.dim=cmds.floatFieldGrp('restitution', numberOfFields=1, label='restitution',  value1=0.0 )
		self.dim=cmds.floatFieldGrp('density', numberOfFields=1, label='density',  value1=0.0 )
		cmds.separator()
		self.dim=cmds.floatFieldGrp('rotation', numberOfFields=1, label='rotation',  value1=0.0 )
		cmds.separator()
		cmds.optionMenuGrp( "bodyType",l='Body Type' )
		cmds.menuItem(label='b2_staticBody');
		cmds.menuItem(label='b2_kinematicBody');
		cmds.menuItem(label='b2_dynamicBody');


		cmds.button(label = "PlaceBlock", w = 100, h = 25, c = self.placeBlock)
		cmds.separator()
		cmds.button( label='Export', command=self.export )

	def placeBlock(self, *args) :
		cmds.polyCube(w=1,h=1)
		name=cmds.ls(sl=True)
		w=cmds.floatFieldGrp('dim',query=True, value1=True)
		h=cmds.floatFieldGrp('dim',query=True, value2=True)

		cmds.setAttr('%s.scaleX' %(name[0]),w/2.0)
		cmds.setAttr('%s.scaleY' %(name[0]),h/2.0)

		r=cmds.floatFieldGrp('rotation',query=True, value1=True)
		cmds.addAttr(name[0],ln='BodyType', dt='string')
		bt=cmds.optionMenuGrp("bodyType", query=True ,value=True)
		cmds.setAttr('%s.BodyType' %(name[0]),bt ,type='string')


		cmds.addAttr(name[0],ln='friction')
		f=cmds.floatFieldGrp('friction',query=True, value1=True)
		cmds.setAttr('%s.friction' %(name[0]),f)

		cmds.addAttr(name[0],ln='restitution')
		r=cmds.floatFieldGrp('restitution',query=True, value1=True)
		cmds.setAttr('%s.restitution' %(name[0]),r)

		cmds.addAttr(name[0],ln='density')
		d=cmds.floatFieldGrp('density',query=True, value1=True)
		cmds.setAttr('%s.density' %(name[0]),d)

		r=cmds.floatFieldGrp('rotation',query=True, value1=True)
		cmds.setAttr('%s.rotateZ' %(name[0]),r)



	def export(self, *args) :
		basicFilter = "*.b2d"
		file=cmds.fileDialog2(caption="Please select file to save",fileFilter=basicFilter, dialogStyle=2)
		if file !="" :

			dagIt = OM.MItDag(OM.MItDag.kDepthFirst, OM.MFn.kTransform)
			object = OM.MObject
			ofile=open(file[0],'w')
			ofile.write(structure)
			ofile.write('\n\nBody bodies[]=\n{\n')
			numBodies=0
			while not dagIt.isDone():

				object = dagIt.currentItem()
				depNode = OM.MFnDependencyNode(object)
				if object.apiTypeStr() =="kTransform" :

					fn = OM.MFnTransform(object)
					child = fn.child(0)

					if child.apiTypeStr()=="kMesh"  :
						name=fn.name()
						ofile.write('\t{ 0,"%s",' %(name) )

						x=cmds.getAttr("%s.translateX" %(name))
						ofile.write('%sf,' %(x))
						y=cmds.getAttr("%s.translateY" %(name))
						ofile.write('%sf,' %(y))
						width=cmds.getAttr("%s.scaleX" %(name))
						ofile.write('%sf,' %(width))
						height=cmds.getAttr("%s.scaleY" %(name))
						ofile.write('%sf,' %(height))
						rot=cmds.getAttr("%s.rotateZ" %(name))
						ofile.write('%sf,' %(rot))

						f=cmds.getAttr("%s.friction" %(name))
						ofile.write('%sf,' %(f))
						f=cmds.getAttr("%s.restitution" %(name))
						ofile.write('%sf,' %(f))
						f=cmds.getAttr("%s.density" %(name))
						ofile.write('%sf,' %(f))

						type=cmds.getAttr("%s.BodyType" %(name))

						ofile.write('%s },\n' %(type))
						numBodies=numBodies+1


				dagIt.next()
			ofile.write("};\n")
			ofile.write('const static int numBodies=%d;' %(numBodies) )
			ofile.close()




Box2DTool()