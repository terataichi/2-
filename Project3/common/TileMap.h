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

	//	カスタムデリーター
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
	int renderorder;					// enumとかに変えたい
	int width;							// タイルのマスの数(横)
	int height;							// 　　　 "　　　　(縦)
	int tileWidth;						// 1タイルのサイズ(横)
	int tileHeight;						//　　　 "　　　　(縦)
	int infinite;
	int nextLayerID;					// layerの数
	int nextObjectID;
};

