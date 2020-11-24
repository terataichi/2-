#pragma once
#include <vector>
#include <list>
#include <map>
#include <string>
#include "Vector2.h"

#include "../TmxLoader.h"

#define TMX_VERSION "1.4"

class FlameGenerator;

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
	void Update(void);								// �`��̍X�V
	LayerVec& GetLayerVec(void);
	LayerData& GetLayerData(std::string name);			// ���C���[�̏��擾
	MapData& GetMapData(void);

	std::vector<Vector2> GetCharChipPos();				// �L�����N�^�[�̏����z�u�擾

	void AddFlameGenerate(int& length, Vector2& pos);		// ��������
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

