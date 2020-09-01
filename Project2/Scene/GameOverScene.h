#pragma once
#include "BaseScene.h"

class GameOverScene :
    public BaseScene
{
public:
    GameOverScene();
    ~GameOverScene();
    uniqueBase Update(uniqueBase own) override;
    void Draw(void)override;
    Scene scene()override;
    void SetMenuFlg(bool set)override;
private:
    Scene scene_;
    bool menuFlg_;
};

