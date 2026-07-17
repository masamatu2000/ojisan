#pragma once
#include "Engine/GameObject.h"
#include<vector>
#include"Feed.h"
class Ground :
	public GameObject
{
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	Ground(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
	std::vector<std::vector<int>> GetMapData() { return mapData_; }
private:
	int hModel_;
	int bModel_;
	int iModel_;
	int piModel_;
	std::vector<std::vector<int>> mapData_;
	int mapWidth_;
	int mapHeight_;
};

