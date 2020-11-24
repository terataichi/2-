#pragma once
#include <memory>
#include <list>
#include <chrono>
#include "BaseScene.h"
#include "../common/TileMap.h"
#include "../Object.h"

using sharedObj = std::shared_ptr<Object>;

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();

    uniqueBase Update(uniqueBase scene)override;
    void DrawOwnScene(void)override;
    void Init(void)override;
    bool SetBomb(int& ownerID, int& myID, Vector2& pos, chronoTime& time, bool flg);
    Object& GetObject(int id);
    void FlameGenerate(int& length, Vector2& pos);
private:

    TileMap tileMap_;                                       // タイルマップ情報格納

    std::chrono::system_clock::time_point  now_;
    std::chrono::system_clock::time_point  end_;

    int fpsCount_;
    int fps_;

    int averageCount_;


    std::list<sharedObj> objList_;                          // プレイヤー
};

