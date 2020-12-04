#pragma once
#include <vector>
#include <list>
#include <map>
#include <string>
#include "Vector2.h"
#include "../DIR.h"
#include "../Scene/SceneMng.h"

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
	chronoTime time_;
	int count_;
	int dirCount_[4];
	TileMap& tileMap_;
};

struct dirBit
{
	int left : 8;
	int right : 8;
	int up : 8;
	int down : 8;
};

struct FlameMapData
{
	chronoTime startTime;
	dirBit dir{};
	bool next{};
};

using sharedFlame = std::shared_ptr<FlameGenerator>;

using FlameMapVec = std::vector<FlameMapData>;

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
	const FlameMapVec& GetFlameMap(void)const;			// �����蔻��Ɏg��
	std::vector<Vector2> GetCharChipPos();				// �L�����N�^�[�̏����z�u�擾
	std::vector<Vector2> GetItemChipPos();				// �L�����N�^�[�̏����z�u�擾

	bool SetFlameMap(dirBit dir,Vector2 size,bool next, chronoTime time);	// ������񏑂����ݗp

	void SetBombMap(int chipPos, bool flg);
	const std::vector<bool>GetBombMap(void)const;

	void AddFlameGenerate(int& length, Vector2& pos);	// ��������
private:
	bool DrawMap(LayerData layerData);					// �}�b�v�̕`��

	// �ǂݍ���TMX�t�@�C���̏��ۑ��p
	TmxLoader loader_;
	LayerVec layerVec_;
	MapData mapData_;
	std::string chipImageName_;

	std::vector<FlameMapData> flameMap_;				// ���������ݗp�}�b�v
	chronoTime oldTime_;
	std::list<sharedFlame> flameList_;					// �������X�g
	std::vector<bool>bombMap_;								// �{���pϯ��							
};

