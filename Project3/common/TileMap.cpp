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
		TRACE("���s\n")
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

	UnionVec vecData;
	vecData.resize(1);
	// �f�[�^���̒ǉ�
	vecData[0].cData[0] = mapData_.width;
	vecData[0].cData[1] = mapData_.height;
	vecData[0].cData[2] = layerData_.size() - 1;

	//UnionHeader mData{ MesType::TMX_SIZE, 1,0,1 };
	//lpNetWork.SetHeader(mData, vecData);
	return lpNetWork.SendMes(MesType::TMX_SIZE, vecData);
}

bool TileMap::SendTmxData(void)
{
	// �����f�[�^�͎����Ă邯�ǃt�@�C������̗��K
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
		// �~�����f�[�^������܂ŋ󂤂�
		do 
		{
			getline(ifs, fileData);
			if (ifs.eof())
			{
				break;
			}
		} while (fileData.find("data encoding") == std::string::npos);

		// �����Ă����炩�炱�����琔�������o��
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

				// �ǂݏI������玟�̍s��������Ă���
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

	// TMXSIZE�̑��M
	SendTmxSizeData();

	// �������DATA�̑��M
	//UnionHeader mData{ MesType::TMX_DATA,0,0,vecData.size() };
	//lpNetWork.SetHeader(mData, vecData);
	lpNetWork.SendMes(MesType::TMX_DATA, packetData);

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

std::vector<Vector2> TileMap::GetCharChipPos()
{
	std::vector<Vector2> chipPos{};
	for (auto layer : layerData_)
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

bool TileMap::DrawMap(LayerData layerData)
{
	// �����̂Ń��[�J���ϐ��Ɋi�[
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
