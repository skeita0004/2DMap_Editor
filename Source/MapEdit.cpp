#include <Windows.h>
#include "MapEdit.hpp"
#include <cassert>
#include "Input.h"
#include "MapChip.hpp"
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include "CsvReader.h"

MapEdit::MapEdit() :
	GameObject(),
	myMap_(0, -1),
	mapEditRect_({{1, 1},{1, 1}}),
	isOnMapEdit_(false),
	myMapIsEmpty_(true),
	canDelete_(false),
	eraseIndex_(0),
	deleteTimer_(0.f),
	hAlert_(-1),
	hDelMessage_(-1),
	mec_(GetMapEditorConfig())
{
	myMap_ = std::vector<int>(mec_.IMAGE_TILE_NUM.x * mec_.IMAGE_TILE_NUM.y, -1);
	mapEditRect_ = {{ mec_.LEFT_POSITION_MARGIN, mec_.TOP_POSITION_MARGIN }, { mec_.AREA_PX_SIZE.x, mec_.AREA_PX_SIZE.y }};
	eraseIndex_ = myMap_.size();
	hAlert_ = LoadGraph("Assets/img/mapDelAlert.png");
	hDelMessage_ = LoadGraph("Assets/img/mapDelMessage.png");
}

MapEdit::~MapEdit()
{

}

void MapEdit::SetMap(VECTOR2INT _pos, int _value)
{
	assert(_pos.x >= 0 && _pos.x < mec_.IMAGE_TILE_NUM.x);
	assert(_pos.y >= 0 && _pos.y < mec_.IMAGE_TILE_NUM.y);

	myMap_[_pos.y * mec_.IMAGE_TILE_NUM.x + _pos.x] = _value;
}

int MapEdit::GetMap(VECTOR2INT _pos) const
{
	assert(_pos.x >= 0 && _pos.x < mec_.IMAGE_TILE_NUM.x);
	assert(_pos.y >= 0 && _pos.y < mec_.IMAGE_TILE_NUM.y);
	return myMap_[_pos.y * mec_.IMAGE_TILE_NUM.x + _pos.x];
}

void MapEdit::Update()
{
	GetMousePoint(&mousePosition_.x, &mousePosition_.y);

	selected.x = (mousePosition_.x - mec_.LEFT_POSITION_MARGIN) / mec_.IMAGE_PX_SIZE;
	selected.y = (mousePosition_.y - mec_.TOP_POSITION_MARGIN) / mec_.IMAGE_PX_SIZE;


#pragma region PutTile
	{
		int x = mousePosition_.x;
		int y = mousePosition_.y;

		int posX = mapEditRect_.position.x;
		int posY = mapEditRect_.position.y;
		int sizeX = mapEditRect_.imageSize.x;
		int sizeY = mapEditRect_.imageSize.y;

		if (x > posX && y > posY && x < posX + sizeX && y < posY + sizeY)
		{
			if (not(canDelete_))
			{
				MapChip* mapChip_ = FindGameObject<MapChip>();
				isOnMapEdit_ = true;
				if (Input::IsMouseHold(MOUSE_INPUT_LEFT))
				{
					if (Input::IsKeyHold(KEY_INPUT_F))
					{
						FillTile(GetMap({static_cast<int>(selected.x), static_cast<int>(selected.y)}), mapChip_->GetHoldImage(), selected.y * mec_.IMAGE_TILE_NUM.x + selected.x);
					}
					if (mapChip_ && mapChip_->GetIsHold())
					{
						SetMap({ static_cast<int>(selected.x), static_cast<int>(selected.y) }, mapChip_->GetHoldImage());
					}
				}
				if (Input::IsMouseHold(MOUSE_INPUT_MIDDLE))
				{
					if (Input::IsKeyHold(KEY_INPUT_F))
					{
						FillTile(GetMap({static_cast<int>(selected.x), static_cast<int>(selected.y)}), -1, selected.y * mec_.IMAGE_TILE_NUM.x + selected.x);
					}
					SetMap({ static_cast<int>(selected.x), static_cast<int>(selected.y) }, -1);
				}
			}
		}
		else
		{
			isOnMapEdit_ = false;
		}
	}
#pragma endregion

	if (Input::IsKeyHold(KEY_INPUT_LCONTROL) && Input::IsKeyDown(KEY_INPUT_S))
	{
		SaveMapData();
	}

	if (Input::IsKeyHold(KEY_INPUT_LCONTROL) && Input::IsKeyDown(KEY_INPUT_O))
	{
		OpenMapData();
	}

	if (Input::IsKeyDown(KEY_INPUT_DELETE))
	{
		if (IDOK == MessageBox(nullptr, "マップデータの解散を行いますか？", "マップデータの全削除", MB_OKCANCEL | MB_ICONWARNING))
		{
			deleteTimer_ = 0.f;
			canDelete_ = true;
			myMapIsEmpty_ = false;
			eraseIndex_ = myMap_.size();
		}
	}

	if (canDelete_)
	{
		deleteTimer_ += Time::DeltaTime();
		DeleteMapData();
	}
}

void MapEdit::Draw()
{
	{
		int posX = mapEditRect_.position.x;
		int posY = mapEditRect_.position.y;
		int sizeX = mapEditRect_.imageSize.x;
		int sizeY = mapEditRect_.imageSize.y;

		for (int y = 0; y < mec_.IMAGE_TILE_NUM.y; y++)
		{
			for (int x = 0; x < mec_.IMAGE_TILE_NUM.x; x++)
			{
				int hImage = GetMap({ x, y });
				if (hImage > -1)
				{
					DrawGraph(mec_.LEFT_POSITION_MARGIN + x * mec_.IMAGE_PX_SIZE,
						mec_.TOP_POSITION_MARGIN + y * mec_.IMAGE_PX_SIZE,
						hImage, TRUE);
				}
			}
		}

		if (isOnMapEdit_)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			//DrawBox(posX, posY, posX + sizeX, posY + sizeY, 0xff00ff, TRUE);
			DrawBox(mec_.LEFT_POSITION_MARGIN + (selected.x * mec_.IMAGE_PX_SIZE),
				mec_.TOP_POSITION_MARGIN + (selected.y * mec_.IMAGE_PX_SIZE),
				mec_.LEFT_POSITION_MARGIN + (selected.x * mec_.IMAGE_PX_SIZE + mec_.IMAGE_PX_SIZE),
				mec_.TOP_POSITION_MARGIN + (selected.y * mec_.IMAGE_PX_SIZE + mec_.IMAGE_PX_SIZE),
				0x00ffff, true, 2);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(posX, posY,
				posX + sizeX,
				posY + sizeY,
				0x11ffdf, FALSE, 3);


		for (int i = 1; i < mec_.AREA_PX_SIZE.x / mec_.IMAGE_PX_SIZE; i++)
		{
			DrawLine(mec_.LEFT_POSITION_MARGIN + i * mec_.IMAGE_PX_SIZE, mec_.TOP_POSITION_MARGIN,
						mec_.LEFT_POSITION_MARGIN + i * mec_.IMAGE_PX_SIZE, mec_.TOP_POSITION_MARGIN + mec_.AREA_PX_SIZE.y,
						0xffeedd, 1);
		}

		for (int i = 1; i < mec_.AREA_PX_SIZE.y / mec_.IMAGE_PX_SIZE; i++)
		{
			DrawLine(mec_.LEFT_POSITION_MARGIN, mec_.TOP_POSITION_MARGIN + i * mec_.IMAGE_PX_SIZE,
					mec_.LEFT_POSITION_MARGIN + mec_.AREA_PX_SIZE.x, mec_.TOP_POSITION_MARGIN + i * mec_.IMAGE_PX_SIZE,
						0xffeedd, 1);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (canDelete_)
	{
		DrawExtendGraph(0, 200, Screen::WIDTH, 600, hDelMessage_, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)(255.f * fabs(sinf(deleteTimer_))));
		DrawExtendGraph(0, 0, 300, 50, hAlert_, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void MapEdit::SaveMapData()
{
	OPENFILENAME openFile{0};
	TCHAR filePath[255] = "";

	openFile.lStructSize = sizeof(openFile);
	openFile.hwndOwner = GetMainWindowHandle();
	openFile.lpstrFilter = "csvファイル (*.csv)\0*.csv\0";
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = 255;
	openFile.Flags = OFN_OVERWRITEPROMPT;

	if(not(GetSaveFileName(&openFile)))
	{
		MessageBox(NULL, "キャンセルされました。", "情報", MB_OK);
	}

	int colCount = 0;
	printfDx("saved!");
	std::ofstream file(filePath);
	MapChip* mc = FindGameObject<MapChip>();

	file << "#HEAD,\t" << "\n";
	file << "WIDTH,\t" << mec_.IMAGE_TILE_NUM.x << "," << "\n";
	file << "HEIGHT,\t" << mec_.IMAGE_TILE_NUM.y << "," << "\n";
	file << ",\n";
	file << "#DATA, " << "\n";
	for (auto& mapData : myMap_)
	{
		colCount++;
		file << mc->GetChipIndex(mapData) << ",\t";
		if (colCount == mec_.IMAGE_TILE_NUM.x)
		{
			file << "\n";
			colCount = 0;
		}
	}
	file.close();
}

void MapEdit::OpenMapData()
{
	OPENFILENAME openFile{ 0 };
	TCHAR filePath[255] = "";

	openFile.lStructSize = sizeof(openFile);
	openFile.hwndOwner = GetMainWindowHandle();
	openFile.lpstrFilter = "csvファイル (*.csv)\0*.csv\0";
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = 255;
	openFile.Flags = OFN_OVERWRITEPROMPT;

	if (GetOpenFileName(&openFile))
	{
		MessageBox(NULL, filePath, "情報", MB_OK);
	}
	else
	{
		MessageBox(NULL, "キャンセルされました。", "情報", MB_OK);
	}

	int colCount = 0;

	if (not(std::filesystem::exists(filePath)))
	{
		printfDx("ファイルが存在していない！");
		return;
	}
	std::ifstream file(filePath);
	MapChip* mc = FindGameObject<MapChip>();

	CsvReader csv(filePath);
	
	{
		int i{};
		int dataStartLine{};
		while (true)
		{
			if (csv.GetString(i, 0) == "#DATA")
			{
				dataStartLine = i + 1;
				break;
			}
			i++;
		}


		// myMapに、CSVの中身を入れる
		// GetInt(line, col)
		// そのまま入れられるんだけど、インデックスをハンドルにする必要がある
		// あと、ヘッダを読み飛ばすので、その分のオフセットは必要。
		//
		for (int y = 0; y < mec_.IMAGE_TILE_NUM.y; y++)
		{
			for (int x = 0; x < mec_.IMAGE_TILE_NUM.x; x++)
			{
				myMap_[y * mec_.IMAGE_TILE_NUM.x + x] = mc->GetImageHandle(csv.GetInt(y + dataStartLine, x));
			}
		}
	}
}

void MapEdit::DeleteMapData()
{
	for (int i = myMap_.size() - eraseIndex_; i < myMap_.size(); i++)
	{
		if (myMap_[i] == -1) // 空じゃなくなるまで飛ばす
		{
			eraseIndex_ -= 1;
			continue;
		}

		myMap_[i] = -1;
		eraseIndex_ -= 1;
		break;
	}

	int emptyCount = 0;
	for (auto& map : myMap_)
	{
		if (map == -1)
		{
			emptyCount++;
		}
	}

	if (emptyCount >= myMap_.size())
	{
		myMapIsEmpty_ = true;
	}

	if (myMapIsEmpty_)
	{
		canDelete_ = false;
	}
}

void MapEdit::FillTile(const int& _hChoseImage, const int& _hFillImage, int _choseMapIndex)
{
	// 範囲の限定
	if (_choseMapIndex < 0 || myMap_.size() <= _choseMapIndex)
	{
		return;
	}

	// 塗りつぶしに関係のないタイルを巻き込まない
	// のと、塗りつぶしを範囲内に収める
	if (myMap_[_choseMapIndex] != _hChoseImage || 
		myMap_[_choseMapIndex] == _hFillImage)
	{
		return;
	}

	myMap_[_choseMapIndex] = _hFillImage;

	int upIndex = _choseMapIndex - mec_.IMAGE_TILE_NUM.x;
	int downIndex = _choseMapIndex + mec_.IMAGE_TILE_NUM.x;
	
	// forでできそう
	// 上方向
	FillTile(_hChoseImage, _hFillImage, ToSafeNeighbor(_choseMapIndex, -mec_.IMAGE_TILE_NUM.x));

	// 下方向
	FillTile(_hChoseImage, _hFillImage, ToSafeNeighbor(_choseMapIndex, mec_.IMAGE_TILE_NUM.x));

	// 左方向
	FillTile(_hChoseImage, _hFillImage, ToSafeNeighbor(_choseMapIndex, -1));

	// 右方向
	FillTile(_hChoseImage, _hFillImage, ToSafeNeighbor(_choseMapIndex, 1));
}

int MapEdit::ToSafeNeighbor(const int _from, const int _to)
{
	VECTOR2INT linerBaseMap{_from % mec_.IMAGE_TILE_NUM.x, _from / mec_.IMAGE_TILE_NUM.y};
	VECTOR2INT nextPos{(_from + _to) % mec_.IMAGE_TILE_NUM.x,(_from + _to) / mec_.IMAGE_TILE_NUM.y};

	// 左方向のフチ右方向のフチ
	if ((linerBaseMap.x - nextPos.x) * (linerBaseMap.x - nextPos.x) > 1)
	{
		return -1;
	}

	return _from + _to;
}
