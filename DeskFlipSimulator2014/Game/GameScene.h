#pragma once

#include <Engine/Game.h>
#include <Engine/Scene.h>
#include <Engine/Camera.h>
#include <Engine/Font.h>
#include <Engine/Types.h>
#include <Engine/Sound.h>

#include "player.h"
#include "DCornerDesk.h"
#include "DDesk.h"
#include "SPanel.h"
#include "Floor.h"
#include "EnemyDesk.h"
#include "SCornerDesk.h"
#include "Message.h"

#define NUM_DESKS 36
#define NUM_CORNER_DESKS 18
#define NUM_PANELS 56

#define NUM_SHOOTSOUNDS 7  // How many variations of the shoot sound there are
#define NUM_SHOTSFIRED  28 // How many shoot sounds can play at once

class GameScene : public Scene
{
	public:
		GameScene();
		~GameScene();

		void Begin();
		void End();
		void Update();
		void Reset();
		void RespawnEnemies();
		void Render();

		player *p1;
		int restart;
		struct	MyContactResultCallback : public btCollisionWorld::ContactResultCallback
		{
			bool hit = 0;
			virtual	btScalar	addSingleResult(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
			{
				hit = true;
				return 1;
			}
		};

	private:
		Camera *camera;
		//DDesk *things[NUM_DESKS]; 
		//DCornerDesk *cornerThings[NUM_CORNER_DESKS];
		//SPanel *panelThings[NUM_PANELS];
		Floor *theFloor;
		DirectX::XMINT2 MouseLast;
		DirectX::XMINT2 MouseNew;
		DirectX::XMINT2 MouseMoved;
		DirectX::XMFLOAT2 lookAt;
		Font *font;
		float fov;
		PhysicsEntity * hitObj;
		//CornerDesk *hitCornerDesk;
		EnemyDesk *enemy1, *enemy2, *enemy3;
		int enemycycle;
		int score, highScore;
		float lastHit;
		float crouchMult;
		float lastDead;
		int ifDead;
		int killedDesks;
		int enemiesAreDead[2];
		Sound *zoomInSound, *zoomOutSound;
		Sound *shootSounds[NUM_SHOTSFIRED];
		Sound *hurtSound, *music;
		int achieveJump;
		bool help;
};

Entity *Callback(const std::string &id);
