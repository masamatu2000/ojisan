#include "TestScene.h"
#include "Player.h"
#include"Ground.h"
#include"Engine/Camera.h"
#include"Enemy.h"
#include"Engine/Text.h"
#include<string>
#include"Feed.h"
#include"Engine/Input.h"
namespace {
	int myscore=0;
	int FeedNum_ = 0;
	Ground* pGround = nullptr;
	Player* pPlayer = nullptr;
}
//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{	
	//pWp = Instantiate<Weapon>(this);
	pGround = Instantiate<Ground>(this);
	pPlayer=Instantiate <Player>(this);
	pPlayer->SetGround(pGround);
	Instantiate<Enemy>(this);
	Camera::SetPosition({ pPlayer->GetPosition().x,-2,-30});
	Camera::SetTarget({pPlayer->GetPosition().x,-6,0});

	pText_ = new Text;
	pText_->Initialize();
	CompleteFeedGet_ = false;

}

//更新
void TestScene::Update()
{
	Camera::SetPosition({ pPlayer->GetPosition().x,-2,-30 });
	Camera::SetTarget({ pPlayer->GetPosition().x,-6,0 });
	myscore=pPlayer->GetScore();
	FeedNum_ = pGround->GetFeedNum();
	if (FeedNum_ <= 0) {
		CompleteFeedGet_ = true;
	}
}

//描画
void TestScene::Draw()
{
	//TODO:餌を数える
	//残りの餌を表示
	//スコアを表示

	std::string scrText;
	std::string FeedText;
	std::string CompleteText;
	scrText = "Score:" + std::to_string(myscore);
	pText_->Draw(20, 20, scrText.c_str());
	FeedText = "Feed_Remain:" + std::to_string(FeedNum_);
	pText_->Draw(20, 60, FeedText.c_str());
	
	if (CompleteFeedGet_) {
		CompleteText = "Complete!!";
		pText_->Draw(20, 100, CompleteText.c_str());
	}

}

//開放
void TestScene::Release()
{
	pText_->Release();
}
