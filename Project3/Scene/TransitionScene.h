#pragma once
#include "BaseScene.h"

/// <summary>
/// Œp³—p‚È‚Ì‚Å’¼Úg‚í‚È‚¢‚Å
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

