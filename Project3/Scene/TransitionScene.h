#pragma once
#include "BaseScene.h"

/// <summary>
/// �p���p�Ȃ̂Œ��ڎg��Ȃ���
/// </summary>
class TransitionScene :
    public BaseScene
{
public:
    TransitionScene();
    ~TransitionScene();
protected:

    uniqueBase prevScene_;
    uniqueBase nextScene_;
};

