#pragma once
#include "puyo.h"

class OjamaPuyo :
    public puyo
{
public:
    OjamaPuyo(Vector2&& pos, PuyoID id);
    ~OjamaPuyo();
    void seiretuCnt(int cnt);
    bool UpDate(void) override;
private:
    int seiretuCnt_;
    bool seiretu_;
};

