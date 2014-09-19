#include "PhysicsWorld.h"
#include "CollisionShape.h"
#include <ngl/Obj.h>
#include <ngl/Quaternion.h>
#include <ngl/Util.h>
#include <BulletCollision/CollisionDispatch/btSimulationIslandManager.h>

PhysicsWorld::PhysicsWorld()
{
//    int maxNumOutstandingTasks = 4;

//    PosixThreadSupport::ThreadConstructionInfo constructionInfo("collision",
//                                processCollisionTask,
//                                createCollisionLocalStoreMemory,
//                                maxNumOutstandingTasks);
//    m_threadSupportCollision = new PosixThreadSupport(constructionInfo);


//    m_threadSupportSolver = createSolverThreadSupport(maxNumOutstandingTasks);
//    m_solver = new btParallelConstraintSolver(m_threadSupportSolver);

//    btDefaultCollisionConstructionInfo cci;

//    cci.m_defaultMaxPersistentManifoldPoolSize = 32768;
//    m_collisionConfiguration = new btDefaultCollisionConfiguration(cci);

//    //this solver requires the contacts to be in a contiguous pool, so avoid dynamic allocation
//    m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

//    m_dispatcher->setDispatcherFlags(btCollisionDispatcher::CD_DISABLE_CONTACTPOOL_DYNAMIC_ALLOCATION);


/*

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    m_collisionConfiguration = new btDefaultCollisionConfiguration();


	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btVector3 worldAabbMin(-10,-10,-10);
		btVector3 worldAabbMax(100,100,100);

    const int maxProxies = 32766;


    m_broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);


    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);

	m_dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_USE_WARMSTARTING + SOLVER_SIMD;

    m_dynamicsWorld->getSimulationIslandManager()->setSplitIslands(true);
    m_dynamicsWorld->getSolverInfo().m_numIterations = 4;

    m_dynamicsWorld->getDispatchInfo().m_enableSPU = true;
    m_dynamicsWorld->getDispatchInfo().m_useEpa = false;
*/
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	m_overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_overlappingPairCache,m_solver,m_collisionConfiguration);



}






void PhysicsWorld::addGroundPlane(const ngl::Vec3 &_pos, const ngl::Vec3 &_size)
{
	m_groundShape = new btStaticPlaneShape(btVector3(0,1,0),_pos.m_y);
//	m_collisionShapes.push_back(m_groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	{
            btScalar mass(0.0f);


			btVector3 localInertia(0,0,0);

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,m_groundShape,localInertia);
//            rbInfo.m_friction=2.0f;
            rbInfo.m_rollingFriction=1.0f;
			btRigidBody* body = new btRigidBody(rbInfo);
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

        delete m_solver;

		delete m_dispatcher;

		delete m_collisionConfiguration;

}

void PhysicsWorld::addSphere(std::string _shapeName,const ngl::Vec3 &_pos,const  ngl::Vec3 &_dir,ngl::Real _mass,const ngl::Vec3 &_inertia)
{
//create a dynamic rigidbody

btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);

/// Create Dynamic Objects
btTransform startTransform;
startTransform.setIdentity();

btScalar	mass(_mass);

startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
btVector3 localInertia(_inertia.m_x,_inertia.m_y,_inertia.m_z);
colShape->calculateLocalInertia(mass,localInertia);

//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
rbInfo.m_restitution = 0.2f;
rbInfo.m_friction = 0.5f;
rbInfo.m_additionalAngularDampingFactor=0.0;
rbInfo.m_additionalDamping=true;

btRigidBody* body = new btRigidBody(rbInfo);
//body->applyCentralImpulse(btVector3(_dir.m_x, _dir.m_y, _dir.m_z));
body->setLinearVelocity(btVector3(_dir.m_x, _dir.m_y, _dir.m_z));
m_dynamicsWorld->addRigidBody(body);
Body b;
b.name=_shapeName;
b.body=body;
m_bodies.push_back(b);

}


void PhysicsWorld::addBox(std::string _shapeName,const ngl::Vec3 &_pos,const ngl::Vec3 &_axis, ngl::Real _angle)
{

	btCollisionShape* colShape = CollisionShape::instance()->getShape(_shapeName);



	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

    btScalar	mass(1.0f);


	btVector3 localInertia(0,0,0);
	colShape->calculateLocalInertia(mass,localInertia);
	startTransform.setOrigin(btVector3(_pos.m_x,_pos.m_y,_pos.m_z));
	startTransform.setRotation(btQuaternion(btVector3(_axis.m_x,_axis.m_y,_axis.m_z),ngl::radians(_angle)));
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
    rbInfo.m_restitution = 0.4f;
    rbInfo.m_friction = 0.5f;
    rbInfo.m_additionalAngularDampingFactor=1.0;
    rbInfo.m_additionalDamping=true;

    btRigidBody* body = new btRigidBody(rbInfo);

    body->setActivationState(DISABLE_DEACTIVATION);
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
    else return ngl::Mat4();
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

