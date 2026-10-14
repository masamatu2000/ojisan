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
Ground::Ground(GameObject* parent):GameObject(parent,"Ground"), hModel_(-1), bModel_(-1),FeedNum_(0)
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
			if (mapData_[y][x] == 0||mapData_[y][x]==2) {
				/*Feed* feed = (Feed*)Instantiate<Feed>(this);
				feed->SetPosition(float(-18 + x * 4),0.5f, float(-18 + y * 4));
				if (mapData_[y][x] == 0) {
					feed->SetFeedtype(FEEDTYPE_NORMAL);
					FeedNum_++;
				}
				if (mapData_[y][x] == 2) {
					feed->SetFeedtype(FEEDTYPE_POWER);
					FeedNum_++;
				}*/
			}
		}
	}
	blocks_.clear();
	moveFrame_ = 0.0f;
	moveDirection_ = 1.0f;

	for (int y = 0; y < mapHeight_; ++y)
	{
		for (int x = 0; x < mapWidth_; ++x)
		{
			int type = mapData_[y][x];

			if (type != 1 && type != 3)
			{
				continue;
			}

			BlockData block;
			block.position = {
				float(-18 + x * 4),
				float(-18 + y * 4),
				0.0f
			};
			block.startX = block.position.x;
			block.moving = (type == 3);

			blocks_.push_back(block);
		}
	}

}

void Ground::Update()
{
	constexpr float MOVE_MAX_FRAME = 60.0f;
	constexpr float MOVE_DISTANCE = 4.0f;

	moveFrame_ += moveDirection_;

	if (moveFrame_ >= MOVE_MAX_FRAME)
	{
		moveFrame_ = MOVE_MAX_FRAME;
		moveDirection_ = -1.0f;
	}
	else if (moveFrame_ <= -MOVE_MAX_FRAME)
	{
		moveFrame_ = -MOVE_MAX_FRAME;
		moveDirection_ = 1.0f;
	}

	// -1～1を往復
	float rate = moveFrame_ / MOVE_MAX_FRAME;

	for (auto& block : blocks_)
	{
		if (block.moving)
		{
			block.position.x =
				block.startX + MOVE_DISTANCE * rate;
		}
	}
}

void Ground::Draw()
{
	transform_.rotate_ = { 0.0f, 90.0f, 90.0f };
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	for (const auto& block : blocks_)
	{
		Transform bt;
		bt.scale_ = { 4.0f, 2.0f, 4.0f };
		bt.position_ = block.position;
		bt.rotate_ = { 0.0f, 90.0f, 90.0f };

		Model::SetTransform(bModel_, bt);
		Model::Draw(bModel_);
	}
}



void Ground::Release()
{
}
