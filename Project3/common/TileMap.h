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

	bool LoadTmx(std::string fileName);					// TMX�t�@�C���ǂݍ���
	bool SendTmxSizeData(void);							// TMX�t�@�C���̃T�C�Y���擾���đ��M
	bool SendTmxData(void);								// TMX�t�@�C���̃f�[�^���M
	void DrawUpdate(void);								// �`��̍X�V
	LayerVec GetLayerData(void);
	MapData GetMapData(void);
private:
	bool DrawMap(LayerData layerData);					// �}�b�v�̕`��



	// �ǂݍ���TMX�t�@�C���̏��ۑ��p
	TmxLoader loader_;
	LayerVec layerData_;
	MapData mapData_;
	std::string ImageName_;
};

