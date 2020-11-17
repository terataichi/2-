#pragma once
#include <memory>
#include <chrono>
#include "BaseScene.h"
#include "../common/TileMap.h"

class Player;

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();

    uniqueBase Update(uniqueBase scene)override;
    void DrawOwnScene(void)override;
    void Init(void)override;
private:

    TileMap tileMap_;                                       // タイルマップ情報格納

    std::chrono::system_clock::time_point  now_;
    std::chrono::system_clock::time_point  end_;

    int fpsCount_;
    int fps_;

    int averageCount_;


    std::vector<std::shared_ptr<Player>> player_;           // プレイヤー
};

