#pragma once
#include "puyo.h"

class OjamaPuyo :
    public puyo
{
public:
    OjamaPuyo(Vector2&& pos, int no);
    ~OjamaPuyo();
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
    int id_;                                // ƒJƒEƒ“ƒgŠi”[
    int interval_;                          // ‰½•b‘Ò‚Â‚©
    int intervalCnt_;                       // ‰½•b‘Ò‚Â‚©
};

