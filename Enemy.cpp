#include "Enemy.h"
#include"Engine/Model.h"
Enemy::Enemy(GameObject* parent) :GameObject(parent, "Enemy"), hModel_(-1)
{
}

void Enemy::Initialize()
{
	hModel_ = Model::Load("Enemy.fbx");
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	//transform_.scale_ = { 0.01,0.01,0.01 };
	//transform_.position_ = { 0, 0.0, 0 };
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}
