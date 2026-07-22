#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "TestScene.h"
#include"Engine/Input.h"
#include<vector>
#include"Ground.h"
namespace {
	enum PLAYER_STATE {
		PLAYER_IDLE,
		PLAYER_WALK,
		PLAYER_ROTATING,
		PLAYER_STATE_MAX
	};
	enum PLAYER_DIRECTION {
		PLAYER_LEFT,
		PLAYER_RIGHT,
		PLAYER_UP,
		PLAYER_DOWN,
		PLAYER_MAX,
	};
	PLAYER_STATE pstate = PLAYER_STATE::PLAYER_IDLE;
	/// <summary>
	/// プレイヤーが最短角度で指定方向に回るようにする
	/// </summary>
	/// <param name="disAngle">回りたい方向と今の角度の差</param>
	/// <returns></returns>
	float TurnAngleOptmization(float disAngle) {
		if (disAngle > 180.0f) {
			return disAngle -= 360.0f;
		}
		if (disAngle < -180.0f) {
			 return disAngle += 360.0f;
		}
		return disAngle;
	}
	float P_ANGLE[4] = { 90.0f,270.0f,180.0f,0.0f };
	PLAYER_DIRECTION pdir = PLAYER_DOWN;
	XMVECTOR P_MOVE[4] = { XMVectorSet(-1, 0, 0, 0),XMVectorSet(1, 0, 0, 0), XMVectorSet(0, 0, 1, 0) ,XMVectorSet(0, 0, -1, 0) };
	float ROT_FRAME = 30.0f;//回転にかかるスピード

	std::vector<std::vector<int>> gmap;
}

Player::Player(GameObject* parent)
	:GameObject(parent,"Player"), hSilly(-1), hModel_Walk(-1), collision(nullptr) ,score_(0){
	//swordDirには、初期方向として、ローカルモデルの剣の根っこから
	//先端までのベクトルとして（0,1,0)を代入しておく
	//初期位置は原点

}

void Player::Initialize()
{
	hSilly = Model::Load("Idle.fbx");
	hModel_Walk = Model::Load("Walking.fbx");
	Model::SetAnimFrame(hSilly, 0, 117, 1.0);
	Model::SetAnimFrame(hModel_Walk, 0, 59, 1.0);
	Ground* ground = (Ground*)FindObject("Ground");
	if (ground!= nullptr) {
		gmap = ground->GetMapData();
	}
	collision = new SphereCollider(XMFLOAT3(0, 0.5f, 0), 0.5f);
	AddCollider(collision);
}

void Player::Update()
{
	//transform_.rotate_.y +=1;
	//static float angle = 0.0;
	//angle = angle + 0.3f;
	//XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	//XMMATRIX rotateX = XMMatrixRotationX(XMConvertToRadians(angle));
	//XMMATRIX rotate = XMMatrixRotationY(XMConvertToRadians(angle));
	//XMMATRIX translate = XMMatrixTranslation(1.0f, 0.0f, 0.0f);

	//SetWorldMatrix(scale *  rotate * translate);

	XMVECTOR pos = XMLoadFloat3(&transform_.position_);
	XMVECTOR move = XMVectorSet(0,0,0,0);
	const float SPEED = 0.05f;
	static float angle = 0;
	static float OldAngle;
	static bool isRotating = false;
	if (pstate != PLAYER_ROTATING) {//回転中はアイドル状態にしない
		pstate = PLAYER_IDLE;
	}
	PLAYER_DIRECTION Predir = pdir;
	if (!isRotating) {

		if (Input::IsKey(DIK_LEFT)) {
			if (pstate == PLAYER_IDLE) {
				pstate = PLAYER_WALK;
			}
			pdir = PLAYER_LEFT;
		}
		if (Input::IsKey(DIK_RIGHT)) {
			if (pstate == PLAYER_IDLE) {
				pstate = PLAYER_WALK;
			}
			pdir = PLAYER_RIGHT;
		}
		if (Input::IsKey(DIK_UP)) {
			if (pstate == PLAYER_IDLE) {
				pstate = PLAYER_WALK;
			}
			pdir = PLAYER_UP;
		}
		if (Input::IsKey(DIK_DOWN)) {
			if (pstate == PLAYER_IDLE) {
				pstate = PLAYER_WALK;
			}
			pdir = PLAYER_DOWN;
		}
	}
	if (Predir != pdir&&pstate!=PLAYER_ROTATING) {
		isRotating = true;
		pstate = PLAYER_ROTATING;
		OldAngle = P_ANGLE[Predir];
	}
	if (pstate != PLAYER_IDLE&&pstate!=PLAYER_ROTATING) {
		move = P_MOVE[pdir];
		angle = P_ANGLE[pdir];
	}
	//回転処理
	else if (pstate == PLAYER_ROTATING) {
		float disAngle = TurnAngleOptmization(P_ANGLE[pdir] - OldAngle);
		static float rotateCount = 0.0f;
		rotateCount++;

		float rate = rotateCount / ROT_FRAME;
		if (rate > 1.0f) rate = 1.0f;

		transform_.rotate_.y = OldAngle + disAngle * rate;

		if (rotateCount >= ROT_FRAME) {
			transform_.rotate_.y = P_ANGLE[pdir];
			pstate = PLAYER_IDLE;
			isRotating = false;
			rotateCount = 0.0f;
		}
	}
	XMVECTOR MapPos = pos + SPEED * move;

	XMFLOAT3 PosToMap;
	XMStoreFloat3(&PosToMap, MapPos);

	constexpr float MAP_SIZE = 4.0f;
	constexpr float MAP_START_X = -20.0f;
	constexpr float MAP_START_Z = -20.0f;

	int MapX = static_cast<int>(
		(PosToMap.x - MAP_START_X) / MAP_SIZE);

	int MapZ = static_cast<int>(
		((PosToMap.z - MAP_START_Z) / MAP_SIZE)
		);

	if (!gmap.empty() &&
		MapZ >= 0 &&
		MapZ < static_cast<int>(gmap.size()) &&
		MapX >= 0 &&
		MapX < static_cast<int>(gmap[MapZ].size()))
	{
		if (gmap[MapZ][MapX] != 1) {
			pos += SPEED * move;
			XMStoreFloat3(&transform_.position_, pos);
		}
	}
	
}

void Player::Draw()
{
	//transform_.scale_ = { 0.01,0.01,0.01 };
	//transform_.position_ = { 0, 0.0, 0 };
	switch (pstate) {
	case PLAYER_IDLE:
		Model::SetTransform(hSilly, transform_);
		Model::Draw(hSilly);
		break;
	case PLAYER_WALK:
		Model::SetTransform(hModel_Walk, transform_);
		Model::Draw(hModel_Walk);
		break;
	case PLAYER_ROTATING:
		Model::SetTransform(hModel_Walk, transform_);
		Model::Draw(hModel_Walk);
		break;
	}
}


void Player::Release()
{
}
