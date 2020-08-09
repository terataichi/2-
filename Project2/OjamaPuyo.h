#pragma once
#include "puyo.h"

class OjamaPuyo :
    public puyo
{
public:
    OjamaPuyo(Vector2&& pos, PuyoID id);
    ~OjamaPuyo();
    void seiretuCnt(int cnt);
    bool UpDate(int no) override;
    bool seiretu(void) override;
    void SetFall(bool flg)override;
private:
    int seiretuCnt_;
    bool seiretu_;
    bool fall_;
    static int ojamaCnt_;                   // 何個目か
    int id_;                                // カウント格納
    int interval_;                          // 何秒待つか
    int intervalCnt_;                        // 何秒待つか
};

