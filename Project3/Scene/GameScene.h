#pragma once
#include <memory>
#include <map>
#include <functional>
#include <list>
#include <chrono>
#include "BaseScene.h"
#include "../common/TileMap.h"
#include "../Object.h"

using sharedObj = std::shared_ptr<Object>;

// ��ԊǗ��p
enum class GameState
{
    Wait,
    Play
};

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();

    uniqueBase Update(uniqueBase scene)override;
    void DrawOwnScene(void)override;
    void Init(void)override;
    bool SetBomb(int& ownerID, int& myID, Vector2& pos, chronoTime& time,int length, bool flg);
    sharedObj GetObjectList(int id);                                                             // �I�u�W�F���X�g����v�f�̎��o��
    void FlameGenerate(int& length, Vector2& pos);                                               // �t���[������
    bool CheckHitFlame(int chipPos);                                                             // �����Ƃ̓����蔻��p

    void SetBombMap(int chipPos, bool flg);
    const std::vector<bool> GetBombMap(void);
private:
    void InitFunc();
    TileMap tileMap_;                                                                            // �^�C���}�b�v���i�[

    std::chrono::system_clock::time_point  now_;
    std::chrono::system_clock::time_point  end_;

    std::map<GameState, std::function<void(void)>> stateUpdate_;                                // ��ԕʃA�b�v�f�[�g
    GameState gameState_;

    int fpsCount_;
    int fps_;

    int averageCount_;

    std::list<sharedObj> objList_;                                                            // ��޼ު�ă��X�g
};

