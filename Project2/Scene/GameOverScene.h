#pragma once
#include "BaseScene.h"

class GameOverScene :
    public BaseScene
{
public:
    GameOverScene(int& mask);
    ~GameOverScene();
    uniqueBase Update(uniqueBase own) override;
    void Draw(void)override;
private:
    int mask_;
    bool MenuFlg_;
};

