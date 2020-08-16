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
private:
    int newKey_;            // 後でインプットでできるようにしたい
    int oldKey_;
};

