#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "TestScene.h"
#include"Engine/Input.h"
#include<vector>
#include"Ground.h"
#include<cmath>
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
	transform_.position_ = { 0,-16,-2 };
}

void Player::Update()
{
	constexpr float GRAVITY = 0.01f;
	constexpr float JUMP_SPEED = 0.35f;
	constexpr float GROUND_Y = -16.0f;
	constexpr float SPEED = 0.05f;

	constexpr float MAP_SIZE = 4.0f;

	// マスの端の座標。
	// Ground側でマスの中心を -20 + x * 4 にしているなら、
	// ここは両方 -22.0f にしてください。
	constexpr float MAP_START_X = -20.0f;
	constexpr float MAP_START_Z = -20.0f;
	constexpr float MAP_START_Y = -20.0f; // MAP_START_Z の代わり
	static float velocityY = 0.0f;
	static float oldAngle = 0.0f;
	static float rotateCount = 0.0f;
	static bool isRotating = false;
	static bool grounded = true;
	static bool previousSpace = false;

	// 乗っているブロックの番号。-1は乗っていない
	static int ridingBlock = -1;
	static float ridingBlockX = 0.0f;

	Ground* ground = static_cast<Ground*>(FindObject("Ground"));
	if (ground == nullptr)
	{
		return;
	}

	const auto& blocks = ground->GetBlocks();

	bool space = Input::IsKey(DIK_SPACE);
	bool jumpPressed = space && !previousSpace;
	previousSpace = space;

	XMFLOAT3 nextPos = transform_.position_;
	XMVECTOR move = XMVectorZero();

	if (!isRotating)
	{
		pstate = PLAYER_IDLE;
		PLAYER_DIRECTION previousDirection = pdir;

		if (Input::IsKey(DIK_LEFT))
		{
			pdir = PLAYER_LEFT;
			pstate = PLAYER_WALK;
		}
		if (Input::IsKey(DIK_RIGHT))
		{
			pdir = PLAYER_RIGHT;
			pstate = PLAYER_WALK;
		}
		if (Input::IsKey(DIK_UP))
		{
			pdir = PLAYER_UP;
			pstate = PLAYER_WALK;
		}
		if (Input::IsKey(DIK_DOWN))
		{
			pdir = PLAYER_DOWN;
			pstate = PLAYER_WALK;
		}
		if (jumpPressed && grounded)
		{
			grounded = false;
			ridingBlock = -1;
			velocityY = JUMP_SPEED;
		}

		if (previousDirection != pdir)
		{
			isRotating = true;
			pstate = PLAYER_ROTATING;
			oldAngle = transform_.rotate_.y;
			rotateCount = 0.0f;
		}
	}

	// 回転中は移動しない
	if (isRotating)
	{
		float difference =
			TurnAngleOptmization(P_ANGLE[pdir] - oldAngle);

		rotateCount += 1.0f;

		float rate = rotateCount / ROT_FRAME;
		if (rate > 1.0f)
		{
			rate = 1.0f;
		}

		transform_.rotate_.y = oldAngle + difference * rate;

		if (rotateCount >= ROT_FRAME)
		{
			transform_.rotate_.y = P_ANGLE[pdir];
			isRotating = false;
			pstate = PLAYER_IDLE;
			rotateCount = 0.0f;
		}
	}
	else if (pstate == PLAYER_WALK)
	{
		move = P_MOVE[pdir];
		transform_.rotate_.y = P_ANGLE[pdir];
	}

	// 判定用サイズ：描画モデルの実寸に合わせて調整する
	constexpr float BLOCK_HALF_X = 2.0f;
	constexpr float BLOCK_HALF_Y = 2.0f;
	constexpr float BLOCK_HALF_Z = 2.0f;

	constexpr float PLAYER_HALF_X = 0.3f;
	constexpr float PLAYER_HALF_Z = 0.3f;
	constexpr float PLAYER_HEIGHT = 1.0f;
	constexpr float EPSILON = 0.001f;

	// 乗っているブロックと一緒に動く
	if (grounded &&
		ridingBlock >= 0 &&
		ridingBlock < static_cast<int>(blocks.size()))
	{
		const auto& block = blocks[ridingBlock];

		nextPos.x += block.position.x - ridingBlockX;
		ridingBlockX = block.position.x;
	}

	// 左右・奥行きの移動
	XMFLOAT3 moveDirection;
	XMStoreFloat3(&moveDirection, move);

	float candidateX = nextPos.x + moveDirection.x * SPEED;
	float candidateZ = nextPos.z + moveDirection.z * SPEED;

	// ブロックの側面に当たるか確認
	bool blocked = false;

	for (const auto& block : blocks)
	{
		float left = block.position.x - BLOCK_HALF_X;
		float right = block.position.x + BLOCK_HALF_X;
		float bottom = block.position.y - BLOCK_HALF_Y;
		float top = block.position.y + BLOCK_HALF_Y;

		bool overlapX =
			candidateX + PLAYER_HALF_X > left + EPSILON &&
			candidateX - PLAYER_HALF_X < right - EPSILON;

		bool overlapY =
			nextPos.y + PLAYER_HEIGHT > bottom + EPSILON &&
			nextPos.y < top - EPSILON;

		bool overlapZ =
			std::fabs(candidateZ - block.position.z) <
			BLOCK_HALF_Z + PLAYER_HALF_Z;

		if (overlapX && overlapY && overlapZ)
		{
			blocked = true;
			break;
		}
	}

	if (!blocked)
	{
		nextPos.x = candidateX;
		nextPos.z = candidateZ;
	}

	// 毎フレーム重力をかける。
	// ブロックから歩いて離れたときも落下する。
	float oldFootY = nextPos.y;

	velocityY -= GRAVITY;
	float candidateY = oldFootY + velocityY;

	grounded = false;
	ridingBlock = -1;

	// 下向きに移動しているなら着地判定
	if (velocityY <= 0.0f)
	{
		float landingY = GROUND_Y;
		int landingBlock = -1;

		for (int i = 0; i < static_cast<int>(blocks.size()); ++i)
		{
			const auto& block = blocks[i];

			float top = block.position.y + BLOCK_HALF_Y;

			bool overlapX =
				nextPos.x + PLAYER_HALF_X >
				block.position.x - BLOCK_HALF_X &&
				nextPos.x - PLAYER_HALF_X <
				block.position.x + BLOCK_HALF_X;

			bool overlapZ =
				std::fabs(nextPos.z - block.position.z) <
				BLOCK_HALF_Z + PLAYER_HALF_Z;

			// 前の足元は上面より上、
			// 移動後の足元は上面以下になった
			bool crossedTop =
				oldFootY >= top - EPSILON &&
				candidateY <= top;

			if (overlapX && overlapZ && crossedTop &&
				top >= landingY)
			{
				landingY = top;
				landingBlock = i;
			}
		}

		if (candidateY <= landingY &&
			oldFootY >= landingY - EPSILON)
		{
			candidateY = landingY;
			velocityY = 0.0f;
			grounded = true;
			ridingBlock = landingBlock;

			if (ridingBlock >= 0)
			{
				ridingBlockX = blocks[ridingBlock].position.x;
			}
		}
	}
	else
	{
		// 上昇中は頭がブロックの下面に当たるか確認
		float oldHeadY = oldFootY + PLAYER_HEIGHT;
		float nextHeadY = candidateY + PLAYER_HEIGHT;
		float hitBottom = 0.0f;
		bool hitCeiling = false;

		for (const auto& block : blocks)
		{
			float bottom = block.position.y - BLOCK_HALF_Y;

			bool overlapX =
				nextPos.x + PLAYER_HALF_X >
				block.position.x - BLOCK_HALF_X &&
				nextPos.x - PLAYER_HALF_X <
				block.position.x + BLOCK_HALF_X;

			bool overlapZ =
				std::fabs(nextPos.z - block.position.z) <
				BLOCK_HALF_Z + PLAYER_HALF_Z;

			if (overlapX && overlapZ &&
				oldHeadY <= bottom + EPSILON &&
				nextHeadY >= bottom)
			{
				if (!hitCeiling || bottom < hitBottom)
				{
					hitBottom = bottom;
					hitCeiling = true;
				}
			}
		}

		if (hitCeiling)
		{
			candidateY = hitBottom - PLAYER_HEIGHT;
			velocityY = 0.0f;
		}
	}

	nextPos.y = candidateY;
	transform_.position_ = nextPos;
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
