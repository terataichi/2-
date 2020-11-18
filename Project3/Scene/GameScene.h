#pragma once
#include <memory>
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
private:

    TileMap tileMap_;                                       // �^�C���}�b�v���i�[

    std::chrono::system_clock::time_point  now_;
    std::chrono::system_clock::time_point  end_;

    int fpsCount_;
    int fps_;

    int averageCount_;


    std::vector<sharedObj> objList_;           // �v���C���[
};

