#include "Ground.h"
#include"Engine/Model.h"
#include"Engine/CsvReader.h"
namespace {
	using std::vector;
	int model_t = 1;
	/*std::vector<std::vector<int>> mapData = {
		{0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,0,0,1,1,1,0},
		{0,1,1,1,0,0,1,1,1,0},
		{0,1,1,1,0,0,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,0,0,1,1,1,0},
		{0,1,1,1,0,0,1,1,1,0},
		{0,1,1,1,0,0,1,1,1,0},
		{0,1,1,1,0,0,1,1,1,0},
		{0,0,0,0,0,0,0,0,0,0}
	};*/
}
Ground::Ground(GameObject* parent):GameObject(parent,"Ground"), hModel_(-1), bModel_(-1)
{
	
}

void Ground::Initialize()
{
	hModel_ = Model::Load("Ground.fbx");
	bModel_ = Model::Load("Block.fbx");
	iModel_ = Model::Load("Item.fbx");
	piModel_ = Model::Load("PowerItem.fbx");
	//mapData_ = mapData;
	CsvReader csv;
	csv.Load("map.csv");
	mapWidth_ = csv.GetWidth();
	mapHeight_ = csv.GetHeight();
	//mapData_の初期化　mapHeight_個のvector<int>(マップデータの列数（0で初期化済み）) の配列を作る
	mapData_ = std::vector<std::vector<int>>(mapHeight_, vector<int>(mapWidth_, 0));
	for (int y = 0;y < mapHeight_;y++) {
		for (int x = 0;x < mapWidth_;x++) {
			mapData_[y][x] = csv.GetValue(y, x);
		}
	}
}

void Ground::Update()
{
}

void Ground::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
	for (int j = 0;j < 10;j++) {
		for (int i = 0;i < 10;i++) {
			if (mapData_[j][i] == 1) {
				Transform bt;
				bt.scale_ = { 4.0f,2.0f,4.0f };
				bt.position_ = { float(-18 + i * 4),2,float(-18+j*4)};
				Model::SetTransform(bModel_, bt);
				Model::Draw(bModel_);
			}
			if (mapData_[j][i] == 0) {
				Transform it;
				it.position_ = { float(-18 + i * 4),2,float(-18 + j * 4) };
				Model::SetTransform(iModel_, it);
				Model::Draw(iModel_);
			}
			if (mapData_[j][i] == 2) {
				Transform pit;
				pit.position_ = { float(-18 + i * 4),2,float(-18 + j * 4) };
				Model::SetTransform(piModel_, pit);
				Model::Draw(piModel_);
			}
		}
	}
}

void Ground::Release()
{
}
