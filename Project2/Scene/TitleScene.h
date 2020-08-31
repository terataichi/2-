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
    bool Init(void);
    bool MenuFlg_;
    Vector2 logoPos_;
    int count_;
};
