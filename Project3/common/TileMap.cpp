#include "TileMap.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "Vector2.h"
#include "ImageMng.h"
#include "../Scene/SceneMng.h"
#include "../NetWork/NetWork.h"

// rappidXml
#include "../TileMap/rapidxml.hpp"
#include "../TileMap/rapidxml_utils.hpp"

#include "../_debug/_DebugConOut.h"

TileMap::TileMap()
{
	mapData_ = MapData{};
	layerData_ = LayerVec{};
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
		TRACE("失敗\n")
		return false;
	}

	layerData_ = loader_.GetLayerData();
	mapData_ = loader_.GetMapData();
	ImageName_ = loader_.GetImageName();
	return true;
}

bool TileMap::SendTmxSizeData(void)
{
	std::ifstream ifs{ loader_.GetTmxFileName() };
	ifs.seekg(0, std::ios_base::end);

	MesData data{ MesType::TMX_SIZE, static_cast<int>(ifs.tellg()) };
	
	return lpNetWork.SendMes(data);
}

bool TileMap::SendTmxData(void)
{
	// 数字データは持ってるけどファイル操作の練習
	std::ifstream ifs{ loader_.GetTmxFileName() };

	UnionData unionData{ 0 };
	std::string fileData;
	std::string numData;
	std::stringstream lineData;

	auto SetLineData = [&]()
	{
		lineData.clear();
		getline(ifs, fileData);
		lineData << fileData;
	};

	auto CInt = [&](std::string str)
	{
		std::stringstream ss;
		char ch;
		ss << str;
		ss >> ch;
		return static_cast<int>(ch);
	};

	int sousin = 0;

	while (!ifs.eof())
	{
		// 欲しいデータが来るまで空うち
		do 
		{
			if (ifs.eof())
			{
				break;
			}
			getline(ifs, fileData);
		} while (fileData.find("data encoding") == std::string::npos);

		// 抜けてきたらからここから数字を取り出す
		int count = 0;
		SetLineData();
		do
		{
			if (ifs.eof())
			{
				break;
			}
			// 読み終わったら次の行をもらってくる
			if (!lineData)
			{
				SetLineData();
			}


			getline(lineData, numData, ',');
			unionData.cData[count % 8] = CInt(numData);
			unionData.cData[count % 8] <<= 4;

			count++;
			if (count % 16 == 0)
			{
				MesData data{ MesType::TMX_DATA, sousin,0,unionData.iData[0],unionData.iData[1] };
				//lpNetWork.SendMes(data);
				sousin++;
			}

		} while (fileData.find("/data") == std::string::npos);
	}
	std::cout << sousin << std::endl;

	//for (auto layer : layerData_)
	//{
	//	int num = 0;
	//	int dataNum = 0;
	//	UnionData unionData{ 0 };
	//	for (auto chipData : layer.chipData)
	//	{
	//		unionData.cData[num] = (chipData << 4);

	//		num++;
	//		if (num % 8 == 0)
	//		{

	//			break;
	//		}
	//	}
	//}

	//int cnt = 0;
	//int j = 0;
	//char *ch = reinterpret_cast<char*>(&j);
	//while (!ifs.eof())
	//{
	//	for (int i = 0; i < sizeof(j); i++)
	//	{
	//		if (!ifs.get(ch[i]))
	//		{
	//			//ch[i] = 0;
	//		}
	//		std::cout << ch[i];
	//	}

	//	MesData data{ MesType::TMX_DATA, cnt, j };
	//	lpNetWork.SendMes(data);
	//	cnt++;
	//}

	return true;
}

void TileMap::DrawUpdate(void)
{
	for (auto data : layerData_)
	{
		DrawMap(data);
	}
}

LayerVec TileMap::GetLayerData(void)
{
	return layerData_;
}

MapData TileMap::GetMapData(void)
{
	return mapData_;
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
		Vector2 chipPos{ size.x * (i % div.x) + size.x / 2 ,size.y * (i / div.x) + size.y / 2 };
		if (chip != 0)
		{
			lpSceneMng.AddDrawQue({ lpImageMng.GetHandle(ImageName_.c_str(),{4,3},size)[chip - 1], chipPos.x,chipPos.y,1,0.0f,1 });
		}
		i++;
	}
    return true;
}
