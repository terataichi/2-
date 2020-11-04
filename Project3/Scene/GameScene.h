#pragma once
#include "BaseScene.h"
#include "../common/TileMap.h"

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

    TileMap tileMap_;                                   // タイルマップ情報格納
};

