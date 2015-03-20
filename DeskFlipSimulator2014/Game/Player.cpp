#include "player.h"
#include <Engine/Time.h>
#include <Engine/Graphics.h>
#include <stdio.h>
#include <Engine/Debug.h>
#include <Bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#define  PLAYER_MAX_SPEED (2.5f)
#define PLAYER_ACCEL_RATE (2.4f)
player::player() : Entity(), model(new Model(L"cube.cmo"))
{

	AddTag("Player");
	accelZ = 0;
	accelX = 0;
	hp = 100;
	tiltMagnitude = 0.0f;
	tiltDisplacement = 0.0f;
	ifDead = 0;
	running = false;
	btTransform startTransform;
	startTransform.setIdentity();
	//startTransform.setOrigin (btVector3(0.0, 4.0, 0.0));
	startTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));

	
	m_ghostObject = new btPairCachingGhostObject();
	m_ghostObject->setWorldTransform(btTransform(Physics::LocalRotationToPhysics(rotation), Physics::LocalTranslationToPhysics(position)));
	Physics::sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	btScalar characterHeight = 1.0;
	btScalar characterWidth = 0.25;
	shape = new btCapsuleShape(characterWidth, characterHeight);
	crouchShape = new btCapsuleShape(characterWidth, characterHeight/3.0);
	m_ghostObject->setCollisionShape(shape);
	m_ghostObject->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
	btScalar stepHeight = btScalar(0.0025);
	m_character = new btKinematicCharacterController(m_ghostObject, shape, stepHeight);
	Physics::world->addCollisionObject(m_ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	Physics::world->addAction(m_character);
	m_character->setGravity(9.80665);
	m_character->setMaxJumpHeight(btScalar(0.45));
	m_character->setJumpSpeed(btScalar(3.1));
	for (int i = 0; i < NUM_STEPSHEARD; i++)
		stepSounds[i] = nullptr;
	distanceSinceStep = 0.0f;
	m_ghostObject->setUserPointer((void *)(this));
}

player:: ~player()
{
	if (model)
		delete model;
	if (m_character)
	{
		Physics::world->removeCollisionObject(m_ghostObject);
	}

}

void player::SetPhysicsOrigin()
{

	btTransform trans = m_ghostObject->getWorldTransform();
	trans.setOrigin(Physics::LocalTranslationToPhysics(position));
	m_ghostObject->setWorldTransform(trans);

}

void player::Update()
{
	if (!ifDead){
		// previous position to calculate distance traveled
		Vector4 previousPosition = position;

		if (isCrouch)
			m_ghostObject->setCollisionShape(crouchShape);
		else
			m_ghostObject->setCollisionShape(shape);
		/*Deals with movement*/
		if (Input::IsKeyHeld(keyForward))
		{
			if (accelZ < 1.0f){
				accelZ += PLAYER_ACCEL_RATE * Time::Delta();
			}
			else
				accelZ = 1.0f;
		}
		else if (Input::IsKeyHeld(keyBackward))
		{
			if (accelZ > -1.0f){
				accelZ -= PLAYER_ACCEL_RATE * Time::Delta();
			}
			else
				accelZ = -1.0f;
		}
		else{
			if (accelZ > 0){
				accelZ -= PLAYER_ACCEL_RATE * Time::Delta();
				if (accelZ < 0)
					accelZ = 0;
			}
			else if (accelZ < 0){
				accelZ += PLAYER_ACCEL_RATE * Time::Delta();
				if (accelZ > 0)
					accelZ = 0;
			}
			else{
				accelZ = 0;
			}
		}

		/*deals with leaning*/
		if (Input::IsKeyHeld(Input::KeyCode('Q'))){
			if (tiltMagnitude < 25.0f){
				tiltMagnitude += Time::Delta() * SPEED_TILT;
				if (tiltMagnitude > 25.0f)
					tiltMagnitude = 25.0f;
				accelZ = -(tiltMagnitude / 25.0f) * 0.75;
			}
		}
		else if (Input::IsKeyHeld(Input::KeyCode('E'))){
			if (tiltMagnitude > -25.0f){
				tiltMagnitude -= Time::Delta() * SPEED_TILT;
				if (tiltMagnitude < -25.0f)
					tiltMagnitude = -25.0f;
				accelZ = -(tiltMagnitude / 25.0f) * 0.75;
			}
		}
		else{
			if (tiltMagnitude < 0.0f){
				tiltMagnitude += Time::Delta() * SPEED_TILT;
				if (tiltMagnitude > 0.0f)
					tiltMagnitude = 0.0f;
				accelZ = (tiltMagnitude / 10.0f) * 0.75;
			}
			else if (tiltMagnitude > 0.0f){
				tiltMagnitude -= Time::Delta() * SPEED_TILT;
				if (tiltMagnitude < 0.0f)
					tiltMagnitude = 0.0f;
				accelZ = (tiltMagnitude / 10.0f) * 0.75;
			}
			/*more movement code*/
			if ((Input::IsKeyHeld(keyStrafeLeft)))
			{
				if (accelX < 1.0f){
					accelX += PLAYER_ACCEL_RATE * Time::Delta();
				}
				else
					accelX = 1.0f;
			}
			else if ((Input::IsKeyHeld(keyStrafeRight)))
			{
				if (accelX > -1.0f){
					accelX -= PLAYER_ACCEL_RATE * Time::Delta();
				}
				else
					accelX = -1.0f;
			}
			else{
				if (accelX > 0){
					accelX -= PLAYER_ACCEL_RATE * Time::Delta();
					if (accelX < 0)
						accelX = 0;
				}
				else if (accelX < 0){
					accelX += PLAYER_ACCEL_RATE * Time::Delta();
					if (accelX >0)
						accelX = 0;
				}
				else{
					accelX = 0;
				}
			}
		}
		if (fabs(accelX) == 1 && fabs(accelZ) == 1){
			accelX = .5 * accelX;
			accelZ = .5 * accelZ;
		}

		/*Deals with being able to run in game*/
		float runMultiplier;
		if (fabs(accelX) > 0.0 || fabs(accelZ) > 0.0){
			if (Input::IsKeyHeld(keyRun)){
				runMultiplier = 2.0f;
				running = true;

			}
			else{
				runMultiplier = 1.0f;
				running = false;
			}
		}
		else{
			runMultiplier = 1.0f;
			running = false;
		}


		DirectX::XMFLOAT3 wCommand;
		wCommand.y = -accelX*cosf(CameraLook.x) + accelZ*sinf(CameraLook.x);
		wCommand.x = -accelX*sinf(CameraLook.x) - accelZ*cosf(CameraLook.x);
		velocity = DirectX::XMVectorSet(wCommand.x, 0.0f, wCommand.y, 0.0f);
		btVector3 test = Physics::LocalTranslationToPhysics(velocity);
		if (Input::IsKeyHeld(keyCrouch)){
			isCrouch = 1;
		}
		else{
			isCrouch = 0;
			if (Input::IsKeyPressed(Input::KEY_SPACE)){
				m_character->jump();
			}
		}
		if (test.length() > 0.0){
			m_character->setVelocityForTimeInterval(test * (PLAYER_MAX_SPEED * runMultiplier), Time::Delta());


		}
		else{
			m_character->setVelocityForTimeInterval(btVector3(0.0, -1.0f, 0.0), Time::Delta());
		}

		m_character->preStep(Physics::world);
		m_character->playerStep(Physics::world, btScalar(Time::Delta()));
		btTransform worldTransform;
		worldTransform = m_character->getGhostObject()->getWorldTransform();
		position = DirectX::XMVectorSet(worldTransform.getOrigin().getX(), worldTransform.getOrigin().getY(), worldTransform.getOrigin().getZ(), 1.0f);

		// --------------------------------------------------------------------- //
		// Step Sounds
		// --------------------------------------------------------------------- //
		distanceSinceStep += DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(position, previousPosition)));
		// free up finished sounds
		for (int i = 0; i < NUM_STEPSHEARD; i++) {
			if (stepSounds[i] && stepSounds[i]->GetState() == Sound::STOPPED) {
				delete stepSounds[i];
				stepSounds[i] = nullptr;
			}
		}
		// play step sounds
		//static bool landed = true;
		//if (Input::IsKeyPressed(Input::KEY_SPACE))
		//	landed = false;
		float currentVelocity = DirectX::XMVectorGetX(DirectX::XMVector3Length(velocity));
		float yPosition = DirectX::XMVectorGetY(position);
		if ((distanceSinceStep > __max(0.28f, currentVelocity * 1.2f) && m_character->onGround()/*yPosition < 1.4f*/)) {
			//landed = true;
			distanceSinceStep = 0.0f;
			int spot = -1;
			for (int i = 0; i < NUM_STEPSHEARD; i++) {
				if (!stepSounds[i]) {
					spot = i;
					break;
				}
			}
			if (spot >= 0) {
				wchar_t name[] = L"step0.wav";
				int soundChoice = rand() % NUM_STEPSOUNDS;
				name[4] = (wchar_t)(soundChoice + L'0');

				stepSounds[spot] = new Sound(name);
				stepSounds[spot]->Play();
				stepSounds[spot]->SetVolume((currentVelocity / PLAYER_MAX_SPEED) * 0.25f + .26);
			}
		}
	}
}
void player::Render()
{
	Graphics::PushMatrix();
	ApplyMatrix();
	model->Draw();
	Graphics::PopMatrix();
}

