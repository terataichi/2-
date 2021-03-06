#pragma once
#include "puyo.h"

class OjamaPuyo :
    public puyo
{
public:
    OjamaPuyo(Vector2&& pos, int no);
    ~OjamaPuyo();
    void Init(int no) override;
    void seiretuCnt(int cnt);
    bool UpDate(int no) override;
    bool seiretu(void) override;
    void SetFall(bool flg)override;
    bool CheckFall()override;
    void SetDrop()override;
private:
    int seiretuCnt_;
    bool seiretu_;
    bool fall_;
    bool drop_ = false;
    int id_;                                // カウント格納
    int interval_;                          // 何秒待つか
    int intervalCnt_;                       // 何秒待つか
    Vector2 initPos_;                       // 再整列しなおす時用
};

