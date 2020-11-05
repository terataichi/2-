#pragma once
#include "BaseScene.h"

/// <summary>
/// 継承用なので直接使わないで
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

