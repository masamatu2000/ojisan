#include "TestScene.h"
#include "Player.h"
#include"Ground.h"
#include"Engine/Camera.h"

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{	
	//pWp = Instantiate<Weapon>(this);
	Ground* pGround = Instantiate<Ground>(this);
	Player* pPlayer=Instantiate <Player>(this);
	pPlayer->SetGround(pGround);
	Camera::SetPosition({ 0,25,-30 });
	Camera::SetTarget({ 0,5,-12 });
}

//更新
void TestScene::Update()
{
}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
}
