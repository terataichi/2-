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
	bool DrawUpdate(void);								// �`��̍X�V
	bool DrawMap(LayerData layerData);					// �}�b�v�̕`��
private:
	void Init(void);									// ������
	TmxLoader loader_;
	LayerVec layerData_;
	MapData mapData_;
};

