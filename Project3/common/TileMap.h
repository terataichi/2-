#pragma once
#include <vector>
#include <list>
#include <map>
#include <string>
#include "Vector2.h"

#include "../TmxLoader.h"

#define TMX_VERSION "1.4"

class TileMap;

/// <summary>
/// ���̔��������p
/// </summary>
struct FlameGenerator
{
public:
	FlameGenerator(int length, Vector2& pos, TileMap& map);
	~FlameGenerator();
	bool Update(void);
private:
	bool CheckNextMap(void);
	int length_;
	Vector2 chipPos_;
	int count_;
	int dirCount_[4];
	TileMap& tileMap_;
};

using sharedFlame = std::shared_ptr<FlameGenerator>;

class TileMap
{
public:
	TileMap();
	TileMap(std::string fileName);
	~TileMap();

	bool LoadTmx(std::string fileName);					// TMX�t�@�C���ǂݍ���
	bool SendTmxSizeData(void);							// TMX�t�@�C���̃T�C�Y���擾���đ��M
	bool SendTmxData(void);								// TMX�t�@�C���̃f�[�^���M
	void Update(void);									// �`��̍X�V
	LayerVec& GetLayerVec(void);
	LayerData& GetLayerData(std::string name);			// ���C���[�̏��擾
	MapData& GetMapData(void);

	std::vector<Vector2> GetCharChipPos();				// �L�����N�^�[�̏����z�u�擾

	bool SetFlameMap_(Vector2 size,int count);				// ������񏑂����ݗp

	void AddFlameGenerate(int& length, Vector2& pos);	// ��������
private:
	bool DrawMap(LayerData layerData);					// �}�b�v�̕`��

	// �ǂݍ���TMX�t�@�C���̏��ۑ��p
	TmxLoader loader_;
	LayerVec layerVec_;
	MapData mapData_;
	std::string chipImageName_;

	std::vector<int> flameMap_;							// ���������ݗp�}�b�v

	std::list<sharedFlame> flameList_;					// �������X�g
};

