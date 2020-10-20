#pragma once
#include <memory>
#include <string>

#define lpTileMap TileMap::GetInstance()

class TileMap
{
public:
	static TileMap& GetInstance(void)
	{
		return *sInstance_;
	}

	bool LoadTileMap(std::string TileName);

private:

	//	�J�X�^���f���[�^�[
	struct TileMapDeleter
	{
		void operator () (TileMap* tileMap)const
		{
			delete tileMap;
		}
	};

	TileMap();
	~TileMap();

	static std::unique_ptr<TileMap,TileMapDeleter> sInstance_;

	int version;
	int tiledversion;
	int orientation;
	int renderorder;					// enum�Ƃ��ɕς�����
	int width;							// �^�C���̃}�X�̐�(��)
	int height;							// �@�@�@ "�@�@�@�@(�c)
	int tileWidth;						// 1�^�C���̃T�C�Y(��)
	int tileHeight;						//�@�@�@ "�@�@�@�@(�c)
	int infinite;
	int nextLayerID;					// layer�̐�
	int nextObjectID;
};

