#pragma once
#include <vector>
#include <map>
#include "BaseScene.h"

class TitleScene :
    public BaseScene
{
public:
    TitleScene();
    ~TitleScene();
    uniqueBase Update(uniqueBase own) override;
    void Draw(void)override;
    Scene scene()override;
    void SetMenuFlg(bool set)override;
private:
    bool Init(void); 
    Scene scene_;
    Vector2 logoPos_;
    int count_;
    bool menuFlg_;

    //std::vector<bool> imageNumBase;
    //std::vector<bool*> imageNum;
};
