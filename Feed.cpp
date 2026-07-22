#include "Feed.h"
#include"Engine/Model.h"
#include"Player.h"
#include"Ground.h"
Feed::Feed(GameObject*parent):GameObject(parent, "Feed"), hModel_(-1),type_(FEEDTYPE_NORMAL)
{
}

Feed::~Feed()
{
}

void Feed::Initialize()
{
	collision= new SphereCollider(XMFLOAT3(0, 0.20f, 0), 1.0f);
	pl_ =(Player*)FindObject("Player");
	gr_ =(Ground*)FindObject("Ground");
}

void Feed::Update()
{
	if (type_ == FEEDTYPE_POWER) {
		transform_.rotate_.y += 1.0f;
	}
	if (pl_ != nullptr) {
		SphereCollider* psc = pl_->GetCollider();
		if (psc != nullptr) {
			if (collision->IsHit(psc)) {
				OnCollision(pl_);
			}
		}
	}
	else {
		pl_= (Player*)FindObject("Player");
	}
}

void Feed::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Feed::Release()
{
}

void Feed::SetFeedtype(FeedType type)
{
	type_ = type;
	if (type_ == FEEDTYPE_NORMAL) {
		collision = new SphereCollider(XMFLOAT3(0, 0.20f, 0), 1.0f);
		AddCollider(collision);
		
		hModel_ = Model::Load("Item.fbx");
	}
	else if (type_ == FEEDTYPE_POWER) {
		collision = new SphereCollider(XMFLOAT3(0, 0.20f, 0), 0.5f);
		AddCollider(collision);
		hModel_ = Model::Load("PowerItem.fbx");
		
	}
}

void Feed::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		if (type_ == FEEDTYPE_NORMAL&&pl_!=nullptr) {
			pl_->SetScore(pl_->GetScore() + 3);
		}
		else if (type_ == FEEDTYPE_POWER && pl_ != nullptr) {
			pl_->SetScore(pl_->GetScore() + 5);
		}
		gr_->SetFeedNum(gr_->GetFeedNum() - 1);
		KillMe();
	}
}
