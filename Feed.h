#pragma once
#include "Engine/GameObject.h"
#include"Engine/Collider.h"

enum FeedType {
	FEEDTYPE_NORMAL,
	FEEDTYPE_POWER,
	FEEDTYPE_MAX
};
class Player;
class Ground;
class Feed :
    public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Feed(GameObject*parent);
	~Feed();
	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
	void SetFeedtype(FeedType type);
	void OnCollision(GameObject* pTarget) override;
	
private:
	int hModel_;
	FeedType type_;
	Player* pl_;
	SphereCollider* collision;
	Ground* gr_;
};

