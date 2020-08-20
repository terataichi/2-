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
    int id_;                                // �J�E���g�i�[
    int interval_;                          // ���b�҂�
    int intervalCnt_;                       // ���b�҂�
    Vector2 initPos_;                       // �Đ��񂵂Ȃ������p
};

