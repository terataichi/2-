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
    int newKey_;            // ��ŃC���v�b�g�łł���悤�ɂ�����
    int oldKey_;
};

