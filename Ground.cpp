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

}

void Ground::Update()
{
	
}

void Ground::Draw()
{
	float angleY = 90.0f;
	float angleZ = 90.0f;

	const float moveMaxFrame = 60.0f;
	const float moveDistance = 4.0f;

	static float currentFrame = 0.0f;
	static float frameDirection = 1.0f;

	// フレームを進める・戻す
	currentFrame += frameDirection;

	if (currentFrame >= moveMaxFrame)
	{
		currentFrame = moveMaxFrame;
		frameDirection = -1.0f;
	}
	else if (currentFrame <= (moveMaxFrame*-1))
	{
		currentFrame = moveMaxFrame*-1;
		frameDirection = 1.0f;
	}

	// 0.0～1.0を往復
	float rate = currentFrame / moveMaxFrame;

	transform_.rotate_ = { 0.0f, angleY, angleZ };
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	for (int y = 0; y < mapHeight_; y++)
	{
		for (int x = 0; x < mapWidth_; x++)
		{
			if (mapData_[y][x] == 1)
			{
				Transform bt;

				bt.scale_ = { 4.0f, 2.0f, 4.0f };
				bt.position_ = {
					float(-18 + x * 4),
					float(-18 + y * 4),
					0.0f
				};

				bt.rotate_ = { 0.0f, angleY, angleZ };

				Model::SetTransform(bModel_, bt);
				Model::Draw(bModel_);
			}

			if (mapData_[y][x] == 3)
			{
				Transform bt;

				bt.scale_ = { 4.0f, 2.0f, 4.0f };

				float startX = float(-18 + x * 4);
				float endX = startX + moveDistance;

				// LerpでstartXとendXの間を往復
				float currentX =
					startX + (endX - startX) * rate;

				bt.position_ = {
					currentX,
					float(-18 + y * 4),
					0.0f
				};

				bt.rotate_ = { 0.0f, angleY, angleZ };

				Model::SetTransform(bModel_, bt);
				Model::Draw(bModel_);
			}
		}
	}
}

void Ground::Release()
{
}
