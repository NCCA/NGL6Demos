#include "Vehicle.h"
#include "PhysicsWorld.h"
#include "CollisionShape.h"
#include <ngl/NGLStream.h>
const static int CUBE_HALF_EXTENTS=1;


const static float	s_maxEngineForce = 2000.f;//this should be engine/velocity dependent
const static float	s_maxBreakingForce = 100.f;

Vehicle::Vehicle(PhysicsWorld *_world)
{
	m_VehicleSteering = 0.f;
	m_steeringIncrement = 0.04f;
	m_steeringClamp = 0.3f;
	m_EngineForce = 0.f;
	m_BreakingForce = 0.f;
	m_world=_world;
	if(m_world == 0)
	{
		std::cerr<<"Need to construct Physics world first\n";
		exit(EXIT_FAILURE);
	}

	// The vehicle
	btScalar chassisMass(800.0f);
	btVector3 chassisInertia(0.0f, 0.0f, 0.0f);
	CollisionShape *shapes=CollisionShape::instance();
	const static float carWidth=1.5;
	const static float carHeight=1.5;
	const static float carDepth=2.5;


	shapes->addBox("car",carWidth, carHeight,carDepth);
	btCollisionShape* chassisShape = shapes->getShape("car");

	btDefaultMotionState* chassisMotionState = new btDefaultMotionState(btTransform(btQuaternion(0.0f, 0.0f, 0.0f, 1.0f), btVector3(0.0f, 5.0f, 0.0f)));
	chassisShape->calculateLocalInertia(chassisMass, chassisInertia);
	btRigidBody::btRigidBodyConstructionInfo chassisRigidBodyCI(chassisMass, chassisMotionState, chassisShape, chassisInertia);
	m_carChassis = new btRigidBody(chassisRigidBodyCI);
	m_carChassis->setActivationState(DISABLE_DEACTIVATION);
	// Be sure to add the chassis of the vehicle into the world as a rigid body
	m_world->m_dynamicsWorld->addRigidBody(m_carChassis);

	btRaycastVehicle::btVehicleTuning tuning;
	btVehicleRaycaster* raycaster = new btDefaultVehicleRaycaster(m_world->m_dynamicsWorld);
	m_vehicle = new btRaycastVehicle(tuning, m_carChassis, raycaster);
	m_vehicle->setCoordinateSystem(0, 1, 2);

	btVector3 wheelDirection(0.0f, -1.0f, 0.0f);
	btVector3 wheelAxis(-1.0f, 0.0f, 0.0f);
	btScalar suspensionRestLength(0.6f);
	btScalar wheelRadius(0.5f);
	// Be sure to attach the wheels not higher than the upper bounds of the hull of the vehicle chassis
	m_vehicle->addWheel(btVector3(-carWidth/2.0, 0.0f, carDepth/2.0), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, tuning, true);
	m_vehicle->addWheel(btVector3(carWidth/2.0, 0.0f, carDepth/2.0), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, tuning, true);
	m_vehicle->addWheel(btVector3(-carWidth/2.0, 0.0f, -carDepth/2.0), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, tuning, false);
	m_vehicle->addWheel(btVector3(carWidth/2.0, 0.0f, -carDepth/2.0), wheelDirection, wheelAxis, suspensionRestLength, wheelRadius, tuning, false);
	int wheelIndex=2;
	m_vehicle->applyEngineForce(m_EngineForce,wheelIndex);
	m_vehicle->setBrake(m_BreakingForce,wheelIndex);
	wheelIndex = 3;
	m_vehicle->applyEngineForce(m_EngineForce,wheelIndex);
	m_vehicle->setBrake(m_BreakingForce,wheelIndex);


	wheelIndex = 0;
	m_vehicle->setSteeringValue(m_VehicleSteering,wheelIndex);
	wheelIndex = 1;
	m_vehicle->setSteeringValue(m_VehicleSteering,wheelIndex);
	m_world->m_dynamicsWorld->addAction(m_vehicle);


	float	wheelFriction = 1000;//BT_LARGE_FLOAT;
	float	suspensionStiffness = 50.f;
	float	suspensionDamping = 2.3f;
	float	suspensionCompression = 4.4f;
	float	rollInfluence = 0.1f;//1.0f;


	for (int i=0;i<m_vehicle->getNumWheels();i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}

}

void Vehicle::draw()
{
}


ngl::Mat4 Vehicle::getChassisTransform()
{
	float matrix[16];
	m_carChassis->getWorldTransform().getOpenGLMatrix(matrix);
	return ngl::Mat4( matrix[0],matrix[1],matrix[2],matrix[3],
										 matrix[4],matrix[5],matrix[6],matrix[7],
										 matrix[8],matrix[9],matrix[10],matrix[11],
										 matrix[12],matrix[13],matrix[14],matrix[15]);
}

ngl::Mat4 Vehicle::getWheelTransform(int i)
{
		//synchronize the wheels with the (interpolated) chassis worldtransform
		m_vehicle->updateWheelTransform(i,true);
		//draw wheels (cylinders)

		float matrix[16];

		m_vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(matrix);

		ngl::Mat4 tx( matrix[0],matrix[1],matrix[2],matrix[3],
											 matrix[4],matrix[5],matrix[6],matrix[7],
											 matrix[8],matrix[9],matrix[10],matrix[11],
											 matrix[12],matrix[13],matrix[14],matrix[15]);



	//	m_shapeDrawer->drawOpenGL(m,m_wheelShape,wheelColor,getDebugMode(),worldBoundsMin,worldBoundsMax);
	return tx;
}

void Vehicle::left()
{
	m_VehicleSteering += m_steeringIncrement;
	if (	m_VehicleSteering >  m_steeringClamp)
			m_VehicleSteering = m_steeringClamp;
}

void Vehicle::right()
{
	m_VehicleSteering -= m_steeringIncrement;
		if (	m_VehicleSteering < -m_steeringClamp)
				m_VehicleSteering = -m_steeringClamp;
}

void Vehicle::accelerate()
{
	m_EngineForce = s_maxEngineForce;
	m_BreakingForce = 0.f;
}

void Vehicle::stop()
{
	m_BreakingForce = s_maxBreakingForce;
	m_EngineForce = 0.f;
}
void Vehicle::update()
{
	std::cout<<"update vehicle "<<m_EngineForce<<" "<<m_VehicleSteering<<"\n";
	int wheelIndex = 2;
	m_vehicle->applyEngineForce(m_EngineForce,wheelIndex);
	m_vehicle->setBrake(m_BreakingForce,wheelIndex);
	wheelIndex = 3;
	m_vehicle->applyEngineForce(m_EngineForce,wheelIndex);
	m_vehicle->setBrake(m_BreakingForce,wheelIndex);


	wheelIndex = 0;
	m_vehicle->setSteeringValue(m_VehicleSteering,wheelIndex);
	wheelIndex = 1;
	m_vehicle->setSteeringValue(m_VehicleSteering,wheelIndex);

}

void Vehicle::reset()
{
	m_VehicleSteering = 0.f;
	m_carChassis->setCenterOfMassTransform(btTransform::getIdentity());
	m_carChassis->setLinearVelocity(btVector3(0,0,0));
	m_carChassis->setAngularVelocity(btVector3(0,0,0));
	m_world->m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_carChassis->getBroadphaseHandle(),m_world->m_dispatcher);
	if (m_vehicle)
	{
		m_vehicle->resetSuspension();
		for (int i=0;i<m_vehicle->getNumWheels();i++)
		{
			//synchronize the wheels with the (interpolated) chassis worldtransform
			m_vehicle->updateWheelTransform(i,true);
		}
	}

}

