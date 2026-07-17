#include "Feed.h"
#include"Engine/Model.h"
#include"Player.h"
Feed::Feed(GameObject*parent):GameObject(parent, "Feed"), hModel_(-1),type_(FEEDTYPE_NORMAL),score_(0)
{
}

Feed::~Feed()
{
}

void Feed::Initialize()
{
	collision= new SphereCollider(XMFLOAT3(0, 0.20f, 0), 1.0f);
	pl_ =(Player*)FindObject("Player");
}

void Feed::Update()
{
	if (type_ == FEEDTYPE_POWER) {
		transform_.rotate_.y += 1.0f;
	}
	SphereCollider* psc = pl_->GetCollider();
	if (psc != nullptr) {
		if (collision->IsHit(psc)) {
			OnCollision(pl_);
		}
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
		score_ = 1;
		hModel_ = Model::Load("Item.fbx");
	}
	else if (type_ == FEEDTYPE_POWER) {
		collision = new SphereCollider(XMFLOAT3(0, 0.20f, 0), 0.5f);
		AddCollider(collision);
		hModel_ = Model::Load("PowerItem.fbx");
		score_ = 5;
	}
}

void Feed::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player") {
		KillMe();
	}
}
