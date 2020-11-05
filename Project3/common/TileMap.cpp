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

	MesH data{ MesType::TMX_SIZE, 0,0,static_cast<int>(ifs.tellg()),0 };
	
	return lpNetWork.SendMes(data);
}

bool TileMap::SendTmxData(void)
{
	// 数字データは持ってるけどファイル操作の練習
	std::ifstream ifs{ loader_.GetTmxFileName() };

	UnionData unionData{ 0 };
	std::string fileData;
	std::stringstream lineData;
	int count = 0;


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
						unionData.cData[(count / 2) % 8] |= ChangeInt(numData) << 4;
					}
					else
					{
						unionData.cData[(count / 2) % 8] = ChangeInt(numData);
					}

					count++;
					if (count % 16 == 0)
					{
						MesH data{ MesType::TMX_DATA, sendID,0,unionData.iData[0],unionData.iData[1] };
						lpNetWork.SendMes(data);
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
	std::cout << sendID << std::endl;

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
		Vector2 chipPos{ size.x * (i % div.x),size.y * (i / div.x)};
		if (chip != 0)
		{
			auto image = lpImageMng.GetHandle(ImageName_.c_str(), { 4,3 }, size)[chip - 1];
			DrawGraph(chipPos.x, chipPos.y, image, true);
		}
		i++;
	}
    return true;
}
