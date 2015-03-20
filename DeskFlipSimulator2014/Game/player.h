#pragma once

#include <Engine/Model.h>
#include <Engine/Entity.h>
#include <Engine/Input.h>
#include <Engine/Sound.h>
#include "physics.h"
#include <Bullet/BulletDynamics/Character/btKinematicCharacterController.h>

#define NUM_STEPSOUNDS  10 // Number of step variations
#define NUM_STEPSHEARD  3 // Number of simultaneous step sounds
#define SPEED_TILT 125.0f
class player : public Entity
{
	public:
		player();
		~player();

		void Update();
		void Render();
		void SetPhysicsOrigin();

		Input::KeyCode keyForward;
		Input::KeyCode keyBackward;
		Input::KeyCode keyStrafeLeft;
		Input::KeyCode keyStrafeRight;
		Input::KeyCode keyRun;
		Input::KeyCode keyCrouch;
		DirectX::XMFLOAT2 CameraLook;
		bool running;
		int hp;
		int ifDead;
		float tiltMagnitude;
		float tiltDisplacement;
		int isCrouch; // Use this to check whether or not to check ray casts to make sure it actually hits thing
		btKinematicCharacterController* m_character;
		btConvexShape* shape, *crouchShape;
		class	btPairCachingGhostObject* m_ghostObject;

	private:
		Model *model;
		Vector4 velocity;
		int crouch;/*
		btCollisionShape *collisionShape;
		btDefaultMotionState *motionState;
		btScalar mass;
		btVector3 inertia;
		btRigidBody *bod;*/
		float accelZ, accelX;

		Sound *stepSounds[NUM_STEPSHEARD];
		float distanceSinceStep;
};