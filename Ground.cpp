#include "Ground.h"
#include"Engine/Model.h"
namespace {
	using std::vector;
	int model_t = 1;
	std::vector<std::vector<int>> mapData = {
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
	};
}
Ground::Ground(GameObject* parent):GameObject(parent,"Ground"), hModel_(-1), bModel_(-1)
{
	mapData_ = mapData;
}

void Ground::Initialize()
{
	hModel_ = Model::Load("Ground.fbx");
	bModel_ = Model::Load("Block.fbx");
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
		}
	}
}

void Ground::Release()
{
}
