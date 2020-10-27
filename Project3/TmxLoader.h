#pragma once
#include <map>

// rappidXml
#include "../TileMap/rapidxml.hpp"
#include "../TileMap/rapidxml_utils.hpp"

#define TMX_VERSION "1.4"

struct LayerData
{
	std::string name;
	std::vector<int>chipData;
	int id,
		width,
		heigth;
};

struct MapData
{
	int width,
		height,
		tileWidth,
		tileHeight;
};

using LayerVec = std::vector<LayerData>;

class TmxLoader
{
public:
	TmxLoader();
	~TmxLoader();

	bool LoadTmx(std::string fileName);
	//rapidxml::xml_node<>& GetNode(std::string nodeName);
	LayerVec GetLayerData(void);
	MapData GetMapData(void);
	std::string GetImageName(void);
	std::string GetTmxFileName(void);
private:
	bool LoadTsx(std::string fileName);

	std::string version_;											// ���݂̃t�@�C����Version���`���Ċi�[���Ă���
	std::string tmxFileName_;										// Tmx�t�@�C�����ۑ�
	std::string tsxFileName_;										// Tsx�t�@�C�����ۑ�
	std::string ImageName_;											// �^�C���̉摜�擾
	LayerVec layerData_;
	MapData mapData_;


	rapidxml::xml_document<> doc;
};
