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

namespace
{
	constexpr int MAP_CHIP_NUM_WIDTH{ 20 };  // マップエディタ横方向の最大チップ数
	constexpr int MAP_CHIP_NUM_HEIGHT{ 20 }; // マップエディタ縦方向の最大チップ数
	constexpr int IMAGE_SIZE{ 32 }; // チップの画素数（正方形前提）
	constexpr int MAP_EDITOR_WIDTH{ MAP_CHIP_NUM_WIDTH * IMAGE_SIZE };
	constexpr int MAP_EDITOR_HEIGHT{ MAP_CHIP_NUM_HEIGHT * IMAGE_SIZE };
	constexpr int MAP_EDITOR_TOP_MARGIN{ (Screen::HEIGHT - MAP_CHIP_NUM_HEIGHT * IMAGE_SIZE) / 2 };
	constexpr int MAP_EDITOR_LEFT_MARGIN{ 240 };
}

MapEdit::MapEdit() :
	GameObject(),
	myMap_(MAP_CHIP_NUM_WIDTH* MAP_CHIP_NUM_HEIGHT, -1),
	mapEditRect_({ MAP_EDITOR_LEFT_MARGIN, MAP_EDITOR_TOP_MARGIN }, { MAP_EDITOR_WIDTH, MAP_EDITOR_HEIGHT }),
	isOnMapEdit_(false),
	myMapIsEmpty_(true),
	canDelete_(false),
	eraseIndex_(0),
	deleteTimer_(0.f),
	hAlert_(-1),
	hDelMessage_(-1)
{
	eraseIndex_ = myMap_.size();
	hAlert_ = LoadGraph("Assets/img/mapDelAlert.png");
	hDelMessage_ = LoadGraph("Assets/img/mapDelMessage.png");
}

MapEdit::~MapEdit()
{	
}

void MapEdit::SetMap(VECTOR2INT _pos, int _value)
{
	assert(_pos.x >= 0 && _pos.x < MAP_CHIP_NUM_WIDTH);
	assert(_pos.y >= 0 && _pos.y < MAP_CHIP_NUM_HEIGHT);

	myMap_[_pos.y * MAP_CHIP_NUM_WIDTH + _pos.x] = _value;
}

int MapEdit::GetMap(VECTOR2INT _pos) const
{
	assert(_pos.x >= 0 && _pos.x < MAP_CHIP_NUM_WIDTH);
	assert(_pos.y >= 0 && _pos.y < MAP_CHIP_NUM_HEIGHT);
	return myMap_[_pos.y * MAP_CHIP_NUM_WIDTH + _pos.x];
}

void MapEdit::Update()
{
	GetMousePoint(&mousePosition_.x, &mousePosition_.y);

	selected.x = (mousePosition_.x - MAP_EDITOR_LEFT_MARGIN) / IMAGE_SIZE;
	selected.y = (mousePosition_.y - MAP_EDITOR_TOP_MARGIN) / IMAGE_SIZE;

	if (not(canDelete_))
	{
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
				MapChip* mapChip_ = FindGameObject<MapChip>();
				isOnMapEdit_ = true;
				if (Input::IsMouseHold(MOUSE_INPUT_LEFT))
				{
					if (Input::IsKeyHold(KEY_INPUT_F))
					{
						FillTile(GetMap({static_cast<int>(selected.x), static_cast<int>(selected.y)}), mapChip_->GetHoldImage(), selected.y * MAP_CHIP_NUM_WIDTH + selected.x);
					}
					if (mapChip_ && mapChip_->GetIsHold())
					{
						SetMap({ static_cast<int>(selected.x), static_cast<int>(selected.y) }, mapChip_->GetHoldImage());
					}
				}
				if (Input::IsMouseHold(MOUSE_INPUT_MIDDLE))
				{
					SetMap({ static_cast<int>(selected.x), static_cast<int>(selected.y) }, -1);
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
			if (IDOK == MessageBox(nullptr, "マップデータの解散を行いますか？", "マップデータの全削除", MB_OK | MB_OKCANCEL | MB_ICONWARNING));
			{
				deleteTimer_ = 0.f;
				canDelete_ = true;
				myMapIsEmpty_ = false;
				eraseIndex_ = myMap_.size();
			}
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

		for (int y = 0; y < MAP_CHIP_NUM_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_CHIP_NUM_WIDTH; x++)
			{
				int hImage = GetMap({ x, y });
				if (hImage > -1)
				{
					DrawGraph(MAP_EDITOR_LEFT_MARGIN + x * IMAGE_SIZE,
						MAP_EDITOR_TOP_MARGIN + y * IMAGE_SIZE,
						hImage, TRUE);
				}
			}
		}

		if (isOnMapEdit_)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
			//DrawBox(posX, posY, posX + sizeX, posY + sizeY, 0xff00ff, TRUE);
			DrawBox(MAP_EDITOR_LEFT_MARGIN + (selected.x * IMAGE_SIZE),
				MAP_EDITOR_TOP_MARGIN + (selected.y * IMAGE_SIZE),
				MAP_EDITOR_LEFT_MARGIN + (selected.x * IMAGE_SIZE + IMAGE_SIZE),
				MAP_EDITOR_TOP_MARGIN + (selected.y * IMAGE_SIZE + IMAGE_SIZE),
				0x00ffff, true, 2);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(posX, posY,
				posX + sizeX,
				posY + sizeY,
				0x11ffdf, FALSE, 3);


		for (int i = 1; i < MAP_EDITOR_WIDTH / IMAGE_SIZE; i++)
		{
			DrawLine(MAP_EDITOR_LEFT_MARGIN + i * IMAGE_SIZE, MAP_EDITOR_TOP_MARGIN,
						MAP_EDITOR_LEFT_MARGIN + i * IMAGE_SIZE, MAP_EDITOR_TOP_MARGIN + MAP_EDITOR_HEIGHT,
						0xffeedd, 1);
		}

		for (int i = 1; i < MAP_EDITOR_HEIGHT / IMAGE_SIZE; i++)
		{
			DrawLine(MAP_EDITOR_LEFT_MARGIN, MAP_EDITOR_TOP_MARGIN + i * IMAGE_SIZE,
					MAP_EDITOR_LEFT_MARGIN + MAP_EDITOR_WIDTH, MAP_EDITOR_TOP_MARGIN + i * IMAGE_SIZE,
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
	file << "WIDTH,\t" << MAP_CHIP_NUM_WIDTH << "," << "\n";
	file << "HEIGHT,\t" << MAP_CHIP_NUM_HEIGHT << "," << "\n";
	file << ",\n";
	file << "#DATA, " << "\n";
	for (auto& mapData : myMap_)
	{
		colCount++;
		file << mc->GetChipIndex(mapData) << ",\t";
		if (colCount == MAP_CHIP_NUM_WIDTH)
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
		for (int y = 0; y < MAP_CHIP_NUM_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_CHIP_NUM_WIDTH; x++)
			{
				myMap_[y * MAP_CHIP_NUM_WIDTH + x] = mc->GetImageHandle(csv.GetInt(y + dataStartLine, x));
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

void MapEdit::FillTile(const int _hChoseImage, const int _hFillImage, const int _choseMapIndex)
{

	if (_choseMapIndex < 0 || myMap_.size() <= _choseMapIndex)
	{
		return;
	}

	if (myMap_[_choseMapIndex] != _hChoseImage
		|| myMap_[_choseMapIndex] == _hChoseImage)
	{
		return;
	}

	int upIndex = _choseMapIndex - MAP_CHIP_NUM_WIDTH;
	int downIndex = _choseMapIndex + MAP_CHIP_NUM_WIDTH;
	int leftIndex = _choseMapIndex - 1;
	int rightIndex = _choseMapIndex + 1;

	
	// forでできそう
	// 上方向
	FillTile(_hChoseImage, _hFillImage, upIndex);

	// 下方向
	FillTile(_hChoseImage, _hFillImage, downIndex);

	// 左方向
	FillTile(_hChoseImage, _hFillImage, leftIndex);

	// 右方向
	FillTile(_hChoseImage, _hFillImage, rightIndex);
}
