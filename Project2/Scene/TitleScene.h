#pragma once
#include "BaseScene.h"

class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();
    uniqueBase Update(uniqueBase own) override;
    void Draw(void)override;
private:
    int newKey_;            // 後でインプットでできるようにしたい
    int oldKey_;
};
