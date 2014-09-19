#include "PhysicsWorld.h"
#include "CollisionShape.h"
#include <ngl/Obj.h>
PhysicsWorld::PhysicsWorld()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_overlappingPairCache,m_solver,m_collisionConfiguration);

	//m_dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_USE_WARMSTARTING + SOLVER_SIMD;

}

void PhysicsWorld::addMesh(std::string _shapeName,const ngl::Vec3 &_pos)
{
	//create a dynamic rigidbody
	btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);
	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass(2.0);


	btVector3 localInertia(10,10,10);
	colShape->calculateLocalInertia(mass,localInertia);
	startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	body->setFriction(1.0);
	body->setRollingFriction(1.0);
	m_dynamicsWorld->addRigidBody(body);
	Body b;
	b.name=_shapeName;
	b.body=body;
	m_bodies.push_back(b);

}

void PhysicsWorld::addGroundPlane(const ngl::Vec3 &_pos, const ngl::Vec3 &_size)
{
	m_groundShape = new btStaticPlaneShape(btVector3(0,1,0),_pos.m_y);
//	m_collisionShapes.push_back(m_groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	{
			btScalar mass(0.);


			btVector3 localInertia(0,0,0);

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,m_groundShape,localInertia);

			btRigidBody* body = new btRigidBody(rbInfo);
			body->setFriction(1.);
			body->setRollingFriction(2.);
			//add the body to the dynamics world
			m_dynamicsWorld->addRigidBody(body);
			Body b;
			b.name="groundPlane";
			b.body=body;
			m_bodies.push_back(b);

		}

}

PhysicsWorld::~PhysicsWorld()
{
	//delete dynamics world
		delete m_dynamicsWorld;

		//delete solver
		delete m_solver;

		//delete broadphase
		delete m_overlappingPairCache;

		//delete dispatcher
		delete m_dispatcher;

		delete m_collisionConfiguration;

}

void PhysicsWorld::addSphere(std::string _shapeName,const ngl::Vec3 &_pos,ngl::Real _mass,const ngl::Vec3 &_inertia)
{
//create a dynamic rigidbody

btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);

/// Create Dynamic Objects
btTransform startTransform;
startTransform.setIdentity();

btScalar	mass(_mass);


btVector3 localInertia(_inertia.m_x,_inertia.m_y,_inertia.m_z);
colShape->calculateLocalInertia(mass,localInertia);
startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
btRigidBody* body = new btRigidBody(rbInfo);
body->setFriction(1.0);
body->setRollingFriction(1.0);

m_dynamicsWorld->addRigidBody(body);
Body b;
b.name=_shapeName;
b.body=body;
m_bodies.push_back(b);

}


void PhysicsWorld::addBox(std::string _shapeName,const ngl::Vec3 &_pos)
{

	btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);



	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass(2.f);


	btVector3 localInertia(20,120,120);
	colShape->calculateLocalInertia(mass,localInertia);
	startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	rbInfo.m_restitution = 0.1f;
	rbInfo.m_friction = 100.5f;
	rbInfo.m_additionalAngularDampingFactor=4.0;
	rbInfo.m_additionalDamping=true;
	btRigidBody* body = new btRigidBody(rbInfo);
	m_dynamicsWorld->addRigidBody(body);
	Body b;
	b.name=_shapeName;
	b.body=body;
	m_bodies.push_back(b);

}

void PhysicsWorld::addCapsule(std::string _shapeName,const ngl::Vec3 &_pos)
{
//create a dynamic rigidbody

	btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);


/// Create Dynamic Objects
btTransform startTransform;
startTransform.setIdentity();

btScalar	mass(2.f);


btVector3 localInertia(0,0,0);
colShape->calculateLocalInertia(mass,localInertia);
startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
rbInfo.m_restitution = 0.1f;
rbInfo.m_friction = 100.5f;
rbInfo.m_additionalAngularDampingFactor=4.0;
rbInfo.m_additionalDamping=true;
btRigidBody* body = new btRigidBody(rbInfo);
m_dynamicsWorld->addRigidBody(body);
Body b;
b.name=_shapeName;
b.body=body;
m_bodies.push_back(b);

}


void PhysicsWorld::addCylinder(std::string _shapeName,const ngl::Vec3 &_pos)
{
//create a dynamic rigidbody

	btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);


	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass(2.f);


	btVector3 localInertia(0,0,0);
	colShape->calculateLocalInertia(mass,localInertia);
	startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	rbInfo.m_restitution = 0.1f;
	rbInfo.m_friction = 100.5f;
	rbInfo.m_additionalAngularDampingFactor=4.0;
	rbInfo.m_additionalDamping=true;
	btRigidBody* body = new btRigidBody(rbInfo);
	m_dynamicsWorld->addRigidBody(body);
	Body b;
	b.name=_shapeName;
	b.body=body;
	m_bodies.push_back(b);

}


void PhysicsWorld::addCone(std::string _shapeName,const ngl::Vec3 &_pos)
{
	//create a dynamic rigidbody

	btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);


	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass(2.f);


	btVector3 localInertia(0,0,0);
	colShape->calculateLocalInertia(mass,localInertia);
	startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
	rbInfo.m_restitution = 0.2f;
	rbInfo.m_friction = 100.5f;
	rbInfo.m_additionalAngularDampingFactor=4.0;
	rbInfo.m_additionalDamping=true;
	btRigidBody* body = new btRigidBody(rbInfo);
	m_dynamicsWorld->addRigidBody(body);
	Body b;
	b.name=_shapeName;
	b.body=body;
	m_bodies.push_back(b);

}


void PhysicsWorld::step(float _time, float _step)
{
  m_dynamicsWorld->stepSimulation(_time,_step);
}


int PhysicsWorld::getCollisionShape(unsigned int _index) const
{
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btCollisionShape *collisionShape = obj->getCollisionShape();

  return collisionShape->getShapeType();
}

void * PhysicsWorld::getUserData(unsigned int _index)
{
  btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
  btCollisionShape *collisionShape = obj->getCollisionShape();
  return collisionShape->getUserPointer();
}


ngl::Mat4 PhysicsWorld::getTransformMatrix(unsigned int _index)
{
	btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;

		body->getMotionState()->getWorldTransform(trans);
		float matrix[16];
		trans.getOpenGLMatrix(matrix);
		return ngl::Mat4( matrix[0],matrix[1],matrix[2],matrix[3],
											 matrix[4],matrix[5],matrix[6],matrix[7],
											 matrix[8],matrix[9],matrix[10],matrix[11],
											 matrix[12],matrix[13],matrix[14],matrix[15]
				);
	}

}

ngl::Vec3 PhysicsWorld::getPosition(unsigned int _index)
{
	btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[_index];
	btRigidBody* body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);
		return ngl::Vec3(trans.getOrigin().getX(),
										 trans.getOrigin().getY(),
										 trans.getOrigin().getZ()
										);
	}
}


void PhysicsWorld::reset()
{
	// start at 1 to leave the ground plane
	for(unsigned int i=1; i<m_bodies.size(); ++i)
	{
		m_dynamicsWorld->removeRigidBody(m_bodies[i].body);
	}
	m_bodies.erase(m_bodies.begin()+1,m_bodies.end());

}

void PhysicsWorld::addImpulse(const ngl::Vec3 &_i)
{
	// don't add impuls to ground plane
	for(unsigned int i=1; i<m_bodies.size(); ++i)
	{
		m_bodies[i].body->applyCentralImpulse(btVector3(_i.m_x,_i.m_y,_i.m_z));
	}
}
