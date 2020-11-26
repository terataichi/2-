#include "TileMap.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "Vector2.h"
#include "ImageMng.h"
#include "../NetWork/NetWork.h"
// rappidXml
#include "../TileMap/rapidxml.hpp"
#include "../TileMap/rapidxml_utils.hpp"

#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

TileMap::TileMap()
{
	mapData_ = MapData{};
	layerVec_ = LayerVec{};
}

TileMap::TileMap(std::string fileName)
{
	LoadTmx(fileName);
}

TileMap::~TileMap()
{
}

bool TileMap::LoadTmx(std::string fileName)
{
	if (!loader_.LoadTmx(fileName))
	{
		TRACE("失敗\n");
		return false;
	}

	layerVec_ = loader_.GetLayerData();
	mapData_ = loader_.GetMapData();
	chipImageName_ = loader_.GetImageName();

	flameMap_.resize(mapData_.height * mapData_.width);
	return true;
}

bool TileMap::SendTmxSizeData(void)
{
	std::ifstream ifs{ loader_.GetTmxFileName() };
	ifs.seekg(0, std::ios_base::end);

	UnionVec vecData;
	vecData.resize(1);
	// データ情報の追加
	vecData[0].cData[0] = mapData_.width;
	vecData[0].cData[1] = mapData_.height;
	vecData[0].cData[2] = static_cast<char>(layerVec_.size() - 1);

	//UnionHeader mData{ MesType::TMX_SIZE, 1,0,1 };
	//lpNetWork.SetHeader(mData, vecData);
	return lpNetWork.SendMes(MesType::TMX_SIZE, vecData);
}

bool TileMap::SendTmxData(void)
{
	// 数字データは持ってるけどファイル操作の練習
	std::ifstream ifs{ loader_.GetTmxFileName() };

	std::string fileData;
	std::stringstream lineData;
	int count = 0;

	UnionData unionData{};
	UnionVec packetData;

	auto SetLineData = [&]()
	{
		lineData.str("");
		lineData.clear();
		getline(ifs, fileData);
		lineData << fileData;
	};

	auto ChangeInt = [&](std::string str)
	{
		return atoi(str.c_str());
	};

	int sendID = 0;

	while (!ifs.eof())
	{
		// 欲しいデータが来るまで空うち
		do 
		{
			getline(ifs, fileData);
			if (ifs.eof())
			{
				break;
			}
		} while (fileData.find("data encoding") == std::string::npos);

		// 抜けてきたらからここから数字を取り出す
		if (!ifs.eof())
		{
			SetLineData();
			while (fileData.find("/data") == std::string::npos)
			{
				std::string numData;
				getline(lineData, numData, ',');
				if (numData.size())
				{
					if (count % 2 == 1)
					{
						unionData.cData[(count / 2) % 4] |= ChangeInt(numData) << 4;
					}
					else
					{
						unionData.cData[(count / 2) % 4] = ChangeInt(numData);
					}

					count++;
					if (count % 8 == 0)
					{ 
						packetData.emplace_back(unionData);
						sendID++;
					}
				}

				// 読み終わったら次の行をもらってくる
				if (lineData.eof())
				{
					SetLineData();
				}
			}
		}
	}
	if (count % 8 != 0)
	{
		packetData.emplace_back(unionData);
		sendID++;
	}
	std::cout << sendID << std::endl;

	// TMXSIZEの送信
	SendTmxSizeData();

	// 今作ったDATAの送信
	//UnionHeader mData{ MesType::TMX_DATA,0,0,vecData.size() };
	//lpNetWork.SetHeader(mData, vecData);
	lpNetWork.SendMes(MesType::TMX_DATA, packetData);

	return true;
}

void TileMap::Update(void)
{
	for (auto &data : layerVec_)
	{
		if (data.name != "Char")
		{
			DrawMap(data);
		}
	}

	// 爆発終わったらfalse で帰ってくる
	flameList_.erase(std::remove_if(flameList_.begin(), flameList_.end(),
		[](sharedFlame& flame) {return !flame->Update(); }), flameList_.end());

}

LayerVec& TileMap::GetLayerVec(void)
{
	return layerVec_;
}

void TileMap::AddFlameGenerate(int& length, Vector2& pos)
{
	flameList_.emplace_back(std::make_shared<FlameGenerator>(length, pos,*this));
}

LayerData& TileMap::GetLayerData(std::string name)
{
	for (auto& data : layerVec_)
	{
		if (data.name == name)
		{
			return data;
		}
	}
	LayerData tmp{};
	return tmp;
}

MapData& TileMap::GetMapData(void)
{
	return mapData_;
}

const FlameMapVec& TileMap::GetFlameMap(void) const
{
	return flameMap_;
}

std::vector<Vector2> TileMap::GetCharChipPos()
{
	std::vector<Vector2> chipPos{};
	for (auto &layer : layerVec_)
	{
		if (layer.name == "Char")
		{
			int cnt = 0;
			Vector2 div{ layer.width ,layer.heigth };

			for (auto data : layer.chipData)
			{
				if (data == 4)
				{
					chipPos.emplace_back(Vector2{cnt % div.x, cnt / div.x });
				}
				cnt++;
			}
		}
	}
	return chipPos;
}

bool TileMap::SetFlameMap(dirBit dir,Vector2 size,bool next,chronoTime time)
{
	auto& data = GetLayerData("Obj");
	if (data.chipData[size.y * 21 + size.x] == 0 ||data.chipData[size.y * 21 + size.x] == 8)
	{
		if (data.chipData[size.y * 21 + size.x] == 8)
		{
			data.chipData[size.y * 21 + size.x] = 0;
		}
		flameMap_[size.y * 21 + size.x].startTime = time;
		flameMap_[size.y * 21 + size.x].next = next;
		flameMap_[size.y * 21 + size.x].dir.down |= dir.down;
		flameMap_[size.y * 21 + size.x].dir.left |= dir.left;
		flameMap_[size.y * 21 + size.x].dir.right |= dir.right;
		flameMap_[size.y * 21 + size.x].dir.up |= dir.up;

		return true;
	}
	return false;
}

bool TileMap::DrawMap(LayerData layerData)
{
	// 長いのでローカル変数に格納
	Vector2 div{ layerData.width ,layerData.heigth };
	Vector2 size{ loader_.GetMapData().tileWidth,loader_.GetMapData().tileHeight };

	int i = 0;
	for (auto chip : layerData.chipData)
	{
		//int chip = data.second.chipData[i];
		Vector2 chipPos{ size.x * (i % div.x),size.y * (i / div.x)};
		if (chip != 0)
		{
			auto image = lpImageMng.GetHandle(chipImageName_.c_str(), { 4,3 }, size)[chip - 1];
			DrawGraph(chipPos.x, chipPos.y, image, true);
		}
		i++;
	}


	int j = 0;
	for (auto& chip : flameMap_)
	{
		Vector2 chipPos{ size.x * (j % 21),size.y * (j / 21) };
		auto now = lpSceneMng.GetTime();
		auto difference = std::chrono::duration_cast<std::chrono::milliseconds>(now - chip.startTime).count();
		//DrawFormatString(chipPos.x, chipPos.y, 0xffffff, "%d", chip.startTime.time_since_epoch().count());
		if (difference < 1200)
		{
			int offset = 1 + static_cast<int>(!chip.next);
			double angle = 0;
			if ((chip.dir.down || chip.dir.up) && (chip.dir.left || chip.dir.right))
			{
				offset = 0;
			}

			if (chip.dir.down || chip.dir.up || chip.dir.left)
			{
				angle = -DX_PI_F / 2;
				if (chip.dir.down)
				{
					angle = DX_PI_F / 2;
				}
				if (chip.dir.left)
				{
					angle = DX_PI_F;
				}
			}

			int tmp = (static_cast<int>(difference) / 3 / 60);
			int num = abs(tmp % 4 - tmp / 4 % 2 * 2);
			auto image = lpImageMng.GetHandle("Image/fire.png", { 3,4 }, size)[num * 3 + offset];
			DrawRotaGraph(chipPos.x + mapData_.tileWidth / 2, chipPos.y + mapData_.tileHeight / 2, 1, angle, image, true);
			DrawFormatString(chipPos.x, chipPos.y, 0xffffff, "%d", num * 3 + offset);
		}
		else
		{
			chronoTime time{};
			chip.startTime = time;
			dirBit dir{false};
			chip.dir = dir;
			chip.next = false;
		}
		oldTime_ = now;
		j++;
	}
    return true;
}

FlameGenerator::FlameGenerator(int length, Vector2& pos, TileMap& map) :tileMap_(map)
{
	length_ = length;
	chipPos_ = { pos.x / 32,pos.y / 32 };
	count_ = 60;
	time_ = lpSceneMng.GetTime();
	for (auto dir : DIR())
	{
		dirCount_[static_cast<int>(dir)] = 0;
	}
}

FlameGenerator::~FlameGenerator()
{
}

bool FlameGenerator::Update(void)
{
	// カウント0以下で終わるようにしたい
	if (count_ < 0)
	{
		return false;
	}

	// 次のマスを見る
	CheckNextMap();
	count_--;
	return true;
}

bool FlameGenerator::CheckNextMap(void)
{
	auto checkNext = [&](DIR dir, int cnt)
	{
		auto& chip = tileMap_.GetLayerData("Obj").chipData;

		if (dir == DIR::DOWN)
		{
			int chipPos = (chipPos_.y + cnt) * 21 + chipPos_.x;
			Vector2 tmp{ chipPos_.x,chipPos_.y + cnt };
			dirBit bit{ 0,0,0,1 };
			if (chip[chipPos] == 0)
			{
				tileMap_.SetFlameMap(bit, tmp, (cnt + 1 < length_), lpSceneMng.GetTime());
				// まだいけるのでtrueを返す
				return true;
			}
			else if (chip[chipPos] == 8)
			{
				// 壊せるブロックなのでここで止める
				tileMap_.SetFlameMap(bit,tmp, false,lpSceneMng.GetTime());
				dirCount_[static_cast<int>(dir)] = length_;
				return false;
			}
		}
		else if (dir == DIR::UP)
		{
			int chipPos = (chipPos_.y - cnt) * 21 + chipPos_.x;
			Vector2 setPos{ chipPos_.x,chipPos_.y - cnt };
			dirBit bit{ 0,0,1,0 };
			if (chip[chipPos] == 0)
			{
				tileMap_.SetFlameMap(bit,setPos, (cnt + 1 < length_),lpSceneMng.GetTime());
				return true;
			}
			else if (chip[chipPos] == 8)
			{
				tileMap_.SetFlameMap(bit,setPos, false,lpSceneMng.GetTime());
				dirCount_[static_cast<int>(dir)] = length_;
				return false;
			}
		}
		else if (dir == DIR::LEFT)
		{
			int chipPos = (chipPos_.y  * 21) + chipPos_.x - cnt;
			Vector2 setPos{ chipPos_.x - cnt,chipPos_.y};
			dirBit bit{ 1,0,0,0 };
			if (chip[chipPos] == 0)
			{
				tileMap_.SetFlameMap(bit,setPos, (cnt + 1 < length_),lpSceneMng.GetTime());
				return true;
			}
			else if (chip[chipPos] == 8)
			{
				tileMap_.SetFlameMap(bit,setPos, false,lpSceneMng.GetTime());
				dirCount_[static_cast<int>(dir)] = length_;
				return false;
			}
		}
		else if (dir == DIR::RIGHT)
		{
			int chipPos = (chipPos_.y * 21)  + chipPos_.x + cnt;
			Vector2 setPos{ chipPos_.x + cnt,chipPos_.y};
			dirBit bit{ 0,1,0,0 };
			if (chip[chipPos] == 0)
			{
				tileMap_.SetFlameMap(bit,setPos, (cnt + 1 < length_),lpSceneMng.GetTime());
				return true;
			}
			else if (chip[chipPos] == 8)
			{
				tileMap_.SetFlameMap(bit,setPos, false,lpSceneMng.GetTime());
				dirCount_[static_cast<int>(dir)] = length_;
				return false;
			}
		}
		dirCount_[static_cast<int>(dir)] = length_;
		return false;
	};

	for (auto dir : DIR())
	{
		if (count_ % 6 == 0)
		{
			if (dirCount_[static_cast<int>(dir)] < length_)
			{
				int d = static_cast<int>(dir);
				checkNext(dir, dirCount_[d]);
				dirCount_[static_cast<int>(dir)]++;
			}
		}
	}

	return false;
}
