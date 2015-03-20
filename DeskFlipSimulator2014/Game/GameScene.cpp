#include "GameScene.h"
#include <Engine/Debug.h>
#include <Engine/Time.h>
#include <Engine/Platform.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "physics.h"
#include <Bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <algorithm>

static float rotationGain;

#define FOV_INCREMENTAL 20.0f

Entity *Callback(const std::string &id)
{
	if (id.compare("DDesk") == 0){
		return new DDesk();
	}
	else if (id.compare("DCornerDesk") == 0){
		return new DCornerDesk();
	}
	else if (id.compare("SCornerDesk") == 0){
		return new SCornerDesk();
	}
	else if (id.compare("SDesk") == 0){
		return new SDesk();
	}
	else if (id.compare("Panel") == 0){
		return new SPanel(WALL);
	}
	else if (id.compare("PanelIntersection") == 0){
		return new SPanel(INTERSECTION);
	}
	else if (id.compare("PanelCorner") == 0){
		return new SPanel(CORNER);
	}
	else
		return NULL;
}

GameScene::GameScene()
{
	camera = new Camera();
	SetCamera(camera);
	fov = 45.0f;
	crouchMult = 1.0f;
	score = 0;
	highScore = 0;
	lastDead = -1;
	ifDead = 0;
	killedDesks = 0;
	achieveJump = 0;
	help = 0;
	font = new Font(L"comic_sans_16.spritefont", DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));

	for (int i = 0; i < NUM_SHOTSFIRED; i++)
		shootSounds[i] = nullptr;
	rotationGain = 0.004f;

	Message(L"Welcome to the game! Press H for help!");
}

GameScene::~GameScene()
{
	if (camera) delete camera;

	if (p1) 
		delete p1;
}

void GameScene::Begin()
{
	zoomInSound = new Sound(L"zoom_in.wav");
	zoomOutSound = new Sound(L"zoom_out.wav");
	hurtSound = new Sound(L"hurt1.wav");
	music = new Sound(L"music.wav");
	Scene::Begin();
	Physics::InitPhysics();
	music->Play(true);
	restart = 0;

	LoadLevel("level1.xml", Callback);
	p1 = new player();
	p1->position = DirectX::XMVectorSet(2.503489f, 0.5f, 6.000467f, 1.0f);
	p1->SetPhysicsOrigin();
	enemy1 = new EnemyDesk();
	enemy1->position = DirectX::XMVectorSet(6.89f, 0.0f, 5.100467f, 1.0f);
	enemy1->SetPhysicsOrigin();
	float enemyPath[2] = { 2.0, 18.0f };
	enemy1->setPath(2, enemyPath);
	Add(enemy1);
	enemy2 = new EnemyDesk();
	enemy2->position = DirectX::XMVectorSet(6.89f, 1.0f, 5.100467f, 1.0f);
	enemy2->SetPhysicsOrigin();
	enemyPath[0] = 18.0f;
	enemyPath[1] = 2.0f;
	enemy2->setPath(2, enemyPath);
	Add(enemy2);
	enemy3 = new EnemyDesk();
	enemy3->position = DirectX::XMVectorSet(6.89f, 1.0f, 17.100467f, 1.0f);
	enemy3->SetPhysicsOrigin();
	enemyPath[0] = 18.0f;
	enemyPath[1] = 2.0f;
	enemy3->setPath(2, enemyPath);
	Add(enemy3);

	enemycycle = 1;
	enemy1->isEnemy = 1;
	lastHit = 0.0f;
	/*
	p1->keyForward = Input::KEY_W;
	p1->keyBackward = Input::KEY_S;
	p1->keyStrafeLeft = Input::KEY_A;
	p1->keyStrafeRight = Input::KEY_D;
		
	THE REAL KEYMAPPING */
	
	/* Pay no attention to the shifty looking keymapping below
	 * They are a lie, do not believe them, they work, I promise
	 * The key mapping is okay (I promise)
	 * If time try to figure out why the code acts this way and fix  it.
	 */

	p1->keyForward = Input::KEY_D;
	p1->keyBackward = Input::KEY_A;
	p1->keyStrafeLeft = Input::KEY_S;
	p1->keyStrafeRight = Input::KEY_W;
	p1->keyCrouch = Input::KEY_CONTROL;

	p1->keyRun = Input::KEY_SHIFT;
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	p1->CameraLook = lookAt;
	p1->SetPhysicsOrigin();
	Add(p1);
	p1->isVisible = false;
	

	theFloor = new Floor();
	theFloor->position = DirectX::XMVectorSet(10.0f, 0.0f, 10.0f, 1.0f);
	Add(theFloor);


	Input::CaptureMouse();
	Input::HideMouse();
	MouseLast = Input::GetMousePosition();

}



void GameScene::End()
{
	Physics::DeinitPhysics();
	Scene::End();
	if (p1)
		delete p1;
	if (font)
		delete font;
	if (theFloor)
		delete theFloor;
}

void GameScene::Update()
{
	MessageUpdateAll();

	Physics::Process();
	Input::SetMousePosition(Platform::GetWidth() / 2, Platform::GetHeight() / 2);
	Scene::Update();
	if (ifDead){
	
		if (Input::IsKeyPressed(Input::KEY_RETURN)){
			ifDead = 0;
			p1->ifDead = 0;
			enemy1->Enable();
			enemy2->Enable();
		}
	
	}
	else{
		DirectX::XMFLOAT4 position, e1pos, e2pos, e3pos;
		DirectX::XMStoreFloat4(&position, p1->position);
		DirectX::XMStoreFloat4(&e1pos, enemy1->position);
		DirectX::XMStoreFloat4(&e2pos, enemy2->position);
		DirectX::XMStoreFloat4(&e3pos, enemy3->position);

		MouseMoved = Input::GetMouseMovement();
		MouseNew = Input::GetMousePosition();
		if (achieveJump == 0 && Input::IsKeyPressed(Input::KEY_SPACE)){
			achieveJump = 1;
			Message(L"(Jump around)");
			Message(L"House of Pain");
			Message(L"(Achievement Unlocked!)");
		}
		if (Input::IsKeyPressed(Input::KeyCode('H'))){
			help = !help;
		}
		/*ENEMY BEHAVIOR*/
		if (enemy1->bod != NULL){
			if (abs(e1pos.x - position.x) <= 10.0f && abs(e1pos.z - position.z) <= 0.5)
				enemy1->speed = 10.0f;
			else
				enemy1->speed = 5.0f;
			MyContactResultCallback result;
			Physics::world->contactPairTest(enemy1->bod, (btCollisionObject *)p1->m_ghostObject, result/*PUT btCollisionWorld::ContactResultCallback HERE*/);
			if (result.hit){
				if ((Time::Current() - lastHit) > .2){
					p1->hp -= 10;
					hurtSound->Play();
					hurtSound->SetVolume(0.75f);
					lastHit = Time::Current();
				}
				if (p1->hp <= 0){
					p1->ifDead = 1;
					ifDead = 1;
					enemy1->Disable();
					enemy2->Disable();
					enemy3->Disable();
					Reset();

				}
			}

		}
		if (enemy2->bod != NULL && p1->isCrouch == 0){
			if (abs(e2pos.x - position.x) <= 10.0f && abs(e2pos.z - position.z) <= 0.5 && p1->isCrouch == 0)
				enemy2->speed = 15.0f;
			else
				enemy2->speed = 7.0f;
			MyContactResultCallback result;
			Physics::world->contactPairTest(enemy2->bod, (btCollisionObject *)p1->m_ghostObject, result/*PUT btCollisionWorld::ContactResultCallback HERE*/);
			if (result.hit){
					if ((Time::Current() - lastHit) > .2){
						p1->hp -= 10;
						hurtSound->Play();
						hurtSound->SetVolume(0.75f);
						lastHit = Time::Current();
					}
					if (p1->hp <= 0){
						p1->ifDead = 1;
						ifDead = 1;
						enemy1->Disable();
						enemy2->Disable();
						enemy3->Disable();
						Reset();
					}
				
			}

		}
		if (enemy3->bod != NULL){
			enemy3->Rotate(Time::Delta() * 180.0f, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			enemy3->SetPhysicsOrigin();
		}
		if (enemy3->bod != NULL && p1->isCrouch == 0){

			enemy3->Rotate(Time::Delta() * 180.0f, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
			enemy3->SetPhysicsOrigin();
			if (abs(e3pos.x - position.x) <= 10.0f && abs(e3pos.z - position.z) <= 0.5 && p1->isCrouch == 0)
				enemy3->speed = 15.0f;
			else
				enemy3->speed = 7.0f;
			MyContactResultCallback result;
			Physics::world->contactPairTest(enemy3->bod, (btCollisionObject *)p1->m_ghostObject, result/*PUT btCollisionWorld::ContactResultCallback HERE*/);
			if (result.hit){
				if ((Time::Current() - lastHit) > .2){
					p1->hp -= 10;
					hurtSound->Play();
					hurtSound->SetVolume(0.75f);
					lastHit = Time::Current();
				}
				if (p1->hp <= 0){
					p1->ifDead = 1;
					ifDead = 1;
					enemy1->Disable();
					enemy2->Disable();
					enemy3->Disable();
					Reset();
				}

			}

		}
		if (lastDead != -1 && Time::Current() - lastDead > 7.0f){
			RespawnEnemies();
			lastDead = -1;
		}


		/*END ENEMY STUFF*/

		DirectX::XMFLOAT2 pointerDelta;
		pointerDelta.x = (float)MouseMoved.x;
		pointerDelta.y = (float)MouseMoved.y;	// how far did pointer move

		MouseLast = MouseNew;

		DirectX::XMFLOAT2 rotationDelta;
		rotationDelta.x = pointerDelta.x * rotationGain;
		rotationDelta.y = pointerDelta.y * rotationGain;	// scale for control sensitivity

		// update our orientation based on the command

		lookAt.y -= rotationDelta.y;						// mouse y increases down, but pitch increases up
		lookAt.x -= rotationDelta.x;						// yaw defined as CCW around y-axis

		// Limit pitch to straight up or straight down
		lookAt.y = (float)__max(-M_PI / 2.1f, lookAt.y);
		lookAt.y = (float)__min(+M_PI / 2.1f, lookAt.y);

		p1->CameraLook = lookAt;

		float y = sinf(lookAt.y);		// vertical
		float r = cosf(lookAt.y);		// in the plane
		float z = r*cosf(lookAt.x /*+ M_PI*/);		// fwd-back
		float x = r*sinf(lookAt.x /*+ M_PI*/);		// left-right


		x += position.x;
		y += position.y;
		z += position.z;

		if (p1->isCrouch){
			if (crouchMult > 0.5){
				crouchMult -= 3.0f * Time::Delta();
				if (crouchMult < 0.5)
					crouchMult = 0.5;
			}
		}
		else{
			if (crouchMult < 1.0){
				crouchMult += 3.0f * Time::Delta();
				if (crouchMult > 1.0)
					crouchMult = 1.0;
			}
		}

		Vector4 translation = DirectX::XMVectorSet(position.x, (position.y * crouchMult) + 0.5, position.z, 1.0f);
		Vector4 lookAt = DirectX::XMVectorSet(x, (y + 0.5) * crouchMult, z, 1.0f);
		Vector4 upVec = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		// tilt camera



		Vector4 lookAtVector = DirectX::XMVectorSubtract(translation, lookAt);
		DirectX::XMMATRIX tilt = DirectX::XMMatrixRotationAxis(
			lookAtVector,
			DirectX::XMConvertToRadians(p1->tiltMagnitude));
		upVec = DirectX::XMVector3Transform(upVec, tilt);

		// transform camera
		camera->LookAt(
			translation,
			lookAt,
			upVec
			);


		if (p1->running){
			if (fov < 55.0f){
				fov += (FOV_INCREMENTAL * Time::Delta());
				if (fov > 55.0f)
					fov = 55.0f;
			}
		}
		else if (Input::IsMouseButtonHeld(Input::MOUSE_RIGHT)){
			if (fov > 25.0f){
				fov -= (FOV_INCREMENTAL * 10.0f * Time::Delta());
				if (fov < 25.0f) {
					fov = 25.0f;
				}
			}
		}
		else{
			if (fov > 45.0f){
				fov -= (FOV_INCREMENTAL * 2.5f * Time::Delta());
				if (fov < 45.0f)
					fov = 45.0f;
			}
			else if (fov < 45.0f){
				fov += (FOV_INCREMENTAL * 10.0f * Time::Delta());
				if (fov > 45.0f) {
					fov = 45.0f;
				}
			}
		}

		static bool zoomed = false;
		if (Input::IsMouseButtonPressed(Input::MOUSE_RIGHT) && !Input::IsKeyHeld(p1->keyRun)) {
			zoomInSound->Play();
			zoomInSound->SetVolume(0.75f);
			zoomed = true;
		}
		if (Input::IsMouseButtonReleased(Input::MOUSE_RIGHT) && !Input::IsKeyHeld(p1->keyRun)) {
			zoomOutSound->Play();
			zoomOutSound->SetVolume(0.75f);
			zoomed = false;
		}
		if (p1->running && zoomed && zoomOutSound->GetState() != Sound::PLAYING) {
			zoomed = false;
			zoomOutSound->Play();
			zoomOutSound->SetVolume(0.75f);
		}
		camera->SetFOV(fov);
		// free up finished sounds
		for (int i = 0; i < NUM_SHOTSFIRED; i++) {
			if (shootSounds[i] && shootSounds[i]->GetState() == Sound::STOPPED) {
				delete shootSounds[i];
				shootSounds[i] = nullptr;
			}
		}

		if (Input::IsKeyReleased(Input::KEY_PLUS)) {
			rotationGain += 0.001f;
			if (rotationGain > 0.01f)
				rotationGain = 0.01f;
		}
		if (Input::IsKeyReleased(Input::KEY_MINUS)) {
			rotationGain -= 0.001f;
			if (rotationGain < 0.001f)
				rotationGain = 0.001f;
		}

		// --------------------------------------------------------------------- //
		// Shooting
		// --------------------------------------------------------------------- //
		if (Input::IsMouseButtonPressed(Input::MOUSE_LEFT)) {
			int spot = -1;
			for (int i = 0; i < NUM_SHOTSFIRED; i++) {
				if (!shootSounds[i]) {
					spot = i;
					break;
				}
			}
			if (spot >= 0) {
				wchar_t name[] = L"gunshot0.wav";
				int soundChoice = rand() % NUM_SHOOTSOUNDS;
				name[7] = (wchar_t)(soundChoice + L'0');

				shootSounds[spot] = new Sound(name);
				shootSounds[spot]->SetVolume(.3);
				shootSounds[spot]->Play();
			}

			DirectX::XMINT2 point = Input::GetMousePosition();

			Vector4 start, end, endFix;
			camera->ScreenToView(Platform::GetWidth() / 2, Platform::GetHeight() / 2, &start, &end);
			DirectX::XMFLOAT4 fStart, fEnd;
			DirectX::XMStoreFloat4(&fStart, start);
			DirectX::XMStoreFloat4(&fEnd, end);
			end = DirectX::XMVectorSet(fEnd.x * -1.0f, fEnd.y * -1.0f, fEnd.z * -1.0f, fEnd.w);
			DirectX::XMStoreFloat4(&fEnd, end);
			btCollisionWorld::AllHitsRayResultCallback RayCallback(Physics::LocalTranslationToPhysics(start), Physics::LocalTranslationToPhysics(end));
			Physics::world->rayTest(Physics::LocalTranslationToPhysics(start), Physics::LocalTranslationToPhysics(end), RayCallback);
			if (RayCallback.hasHit()){
				btScalar hitFraction = 2.0;
				int whichOne = -1;
				for (int i = 0; i < RayCallback.m_collisionObjects.size(); i++){

					if (RayCallback.m_hitFractions[i] < hitFraction && RayCallback.m_collisionObjects[i]->getCollisionShape()->getUserPointer()){
						whichOne = i;
						hitFraction = RayCallback.m_hitFractions[i];
					}

				}
				if (whichOne > -1 && (RayCallback.m_collisionObjects[whichOne]->getCollisionShape()->getUserPointer()) != NULL){
					hitObj = (PhysicsEntity *)RayCallback.m_collisionObjects[whichOne]->getCollisionShape()->getUserPointer();

					if (hitObj->mass > 0.0){
						hitObj->bod->activate(true);
						end = DirectX::XMVector4Normalize(end);
						DirectX::XMStoreFloat4(&fEnd, end);
						hitObj->bod->applyImpulse(btVector3(btScalar(fEnd.x * 25.0f), btScalar(fEnd.y * 25.0f), btScalar(fEnd.z * 25.0f)), (RayCallback.m_hitPointWorld[whichOne] - hitObj->bod->getWorldTransform().getOrigin()));
					}
					if (hitObj->hp > 0){
						hitObj->hp -= 20;
						score += 20;
						if (hitObj->hp <= 0){
							if (hitObj->Die() == -1){
								score += 100;
								Message(L"DESKALITY!");
								killedDesks++;
								if (killedDesks == 25){
									Message(L"(Destroy 25 desks)");
									Message(L"As if 25 desks suddenly cried out in terror and were suddenly silenced.");
									Message(L"Achievement Unlocked!");
								}
								hitObj = NULL;
								if (lastDead == -1)
									lastDead = Time::Current();
							}
						}
					}



					hitObj = NULL;
				}

			}
		}

	}
	if (Input::IsKeyReleased(Input::KEY_ESCAPE))
		Game::Quit();
	if (Input::IsKeyReleased(Input::KeyCode('P'))){
		restart = 1;
		Game::Quit();
	}

}

void GameScene::Render()
{
	Scene::Render();
	font->SetColor(DirectX::XMVectorSet(1.0, 1.0, 1.0f, 1.0f));
	if (ifDead){
		font->Draw(DirectX::XMFLOAT2((Platform::GetWidth() / 2) - 128, (Platform::GetHeight() / 2)), L"      YOU ARE DEAD\nPress Enter to Continue");
	}
	else
		font->Draw(DirectX::XMFLOAT2((Platform::GetWidth() / 2) - 8, (Platform::GetHeight() / 2) - 16), L"+");
	font->Draw(DirectX::XMFLOAT2(32, (32)), L"HP: ");
	if (!ifDead){
		font->SetColor(DirectX::XMVectorSet(std::min(1.0f, 1.0f - ((float)p1->hp / 200.0f)), std::min(1.0f, ((float)p1->hp / 50.0f)), 0.0f, 1.0f));
		for (int i = 0; i < (p1->hp); i++)
		{
			font->Draw(DirectX::XMFLOAT2(32 + 32 + i, (32)), L"|");

		}
	}
	font->SetColor(DirectX::XMVectorSet(1.0, 1.0, 1.0f, 1.0f));
	if (help){
		font->Draw(DirectX::XMFLOAT2((Platform::GetWidth() / 4), (Platform::GetHeight() / 4)),L"Controls\nWASD: Movement\nQ: Lean Left\nE: Lean Right\nShift: Run\nSpace: Jump\nCTRL: Crouch\nLeft Click: Shoot\nRight Click: Zoom\n\n\nH: Help\nESC: Quit\nP: Reset World");
	}
	font->Draw(DirectX::XMFLOAT2(32, (64)), L"Score: %d", score);
	font->Draw(DirectX::XMFLOAT2(Platform::GetWidth() - Platform::GetWidth() * (.15), (32)), L"High Score: %d", highScore);


	MessageRenderAll();
}

void GameScene::Reset()
{
	p1->position = DirectX::XMVectorSet(2.503489f, 0.5f, 6.000467f, 1.0f);
	p1->SetPhysicsOrigin();
	p1->hp = 100;
	if (score > highScore){
		highScore = score;
		Message(L"New High Score!");
	}
	score = 0;
	RespawnEnemies();

}

void GameScene::RespawnEnemies(){
	float enemyPath[2] = { 2.0, 18.0f };
	if (enemy1->bod == NULL){
		Remove(enemy1);
		enemy1 = new EnemyDesk();
		enemy1->position = DirectX::XMVectorSet(6.89f, 0.0f, 5.100467f, 1.0f);
		enemy1->SetPhysicsOrigin();
		enemy1->setPath(2, enemyPath);
		Add(enemy1);
		Message(L"Enemy Desk 1 Respawned");
	}
	if (enemy2->bod == NULL){
		Remove(enemy2);
		enemy2 = new EnemyDesk();
		enemy2->position = DirectX::XMVectorSet(6.89f, 1.0f, 5.100467f, 1.0f);
		enemy2->SetPhysicsOrigin();
		enemyPath[0] = 18.0f;
		enemyPath[1] = 2.0f;
		enemy2->setPath(2, enemyPath);
		Add(enemy2);
		Message(L"Enemy Desk 2 Respawned");
	}
	if (enemy3->bod == NULL){
		Remove(enemy3);
		enemy3 = new EnemyDesk();
		enemy3->position = DirectX::XMVectorSet(6.89f, 1.0f, 17.100467f, 1.0f);
		enemy3->SetPhysicsOrigin();
		enemyPath[0] = 18.0f;
		enemyPath[1] = 2.0f;
		enemy3->setPath(2, enemyPath);
		Add(enemy3);
		Message(L"Enemy Desk 3 Respawned");
	}

}