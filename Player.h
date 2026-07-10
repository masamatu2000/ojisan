#pragma once
#include "Engine/GameObject.h"

class Ground;//前方宣言(クラスのポインタだけ使いたいときだけはこれでおｋ,企業にアピールポイントとして使えるので覚えておくべき)

class Player :
    public GameObject
{
	
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Player(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
	void SetGround(Ground* ground) { ground_ = ground; }
private:
	int hSilly;
	int hModel_Walk;
	Ground* ground_;//地面オブジェクトのポインタ
};

