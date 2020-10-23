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
		TRACE("Ž¸”s\n")
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

	MesData data{ MesType::TMX_SIZE, ifs.tellg() };
	
	return lpNetWork.SendMes(data);
}

bool TileMap::SendTmxData(void)
{
	//std::ifstream ifs{ loader_.GetTmxFileName() };

	//char ch;
	//while (ifs.get(ch))
	//{
	//	std::cout << ch;
	//}

	//MesData data{MesType::TMX_DATA, }
	//return lpNetWork.SendMes();
	return false;
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
	// ’·‚¢‚Ì‚Åƒ[ƒJƒ‹•Ï”‚ÉŠi”[
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
