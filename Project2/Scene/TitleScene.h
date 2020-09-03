#pragma once
#include <vector>
#include <map>
#include "BaseScene.h"

#define FADE_NUM 10

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
    bool nextUpdate(void)override;
private:
    bool Init(void); 
    bool InitFunc(void);
    Vector2 logoPos_;
    int count_;

    std::vector<std::vector<bool>> imageNum_;                           // �t�F�[�h�C���A�E�g�Ǘ��p
    std::vector<int>randomNum_;                                         // �����_���ɐ��������ĉ摜��\�������鏇�Ԃ�ς���
    int fadeScreen_;                                                    // �L���[�ɓ������炷�������ƂɂȂ�̂ň�U�������ɕ`�悷��
    int fadeCnt_;

    std::map<FadeMode,std::function<FadeMode(void)>> fadeMap_;          // �t�F�[�h�̊Ǘ�
    FadeMode fadeMode_;                                                 // �t�F�[�h�Ǘ��p
    int waitCnt_;                                                       // �҂����ԊǗ��p

    // �����ŃC���X�^���X���ăQ�[���V�[���ɓn��
    std::vector<std::unique_ptr<Stage>> stage_;		               	    // �X�e�[�W�����炩���ߍ���Ă���
};
