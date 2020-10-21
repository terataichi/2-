#pragma once
#include <memory>
#include <string>

#define lpTileMap TileMap::GetInstance()

#define TMX_VERSION "1.4"

class TileMap
{
public:
	TileMap();
	~TileMap();

	bool LoadTmx(std::string file);

private:
	
	std::string version;
	std::string tiledversion;
	std::string orientation;
	std::string renderorder;			// enum�Ƃ��ɕς�����
	int width;							// �^�C���̃}�X�̐�(��)
	int height;							// �@�@�@ "�@�@�@�@(�c)
	int tileWidth;						// 1�^�C���̃T�C�Y(��)
	int tileHeight;						//�@�@�@ "�@�@�@�@(�c)
	int infinite;
	int nextLayerID;					// layer�̐�
	int nextObjectID;
};

