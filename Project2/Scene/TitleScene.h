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
    int newKey_;            // ��ŃC���v�b�g�łł���悤�ɂ�����
    int oldKey_;
};
