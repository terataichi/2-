#pragma once
#include <vector>
#include <map>
#include <string>

#include "../TmxLoader.h"

#define TMX_VERSION "1.4"

class TileMap
{
public:
	TileMap();
	TileMap(std::string fileName);
	~TileMap();

	bool LoadTmx(std::string fileName);
	void DrawUpdate(void);								// �`��̍X�V
private:
	bool DrawMap(LayerData layerData);					// �}�b�v�̕`��
	TmxLoader loader_;
	LayerVec layerData_;
	MapData mapData_;
	std::string ImageName_;
};

