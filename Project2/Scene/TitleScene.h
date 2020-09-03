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

    std::vector<std::vector<bool>> imageNum_;                           // フェードインアウト管理用
    std::vector<int>randomNum_;                                         // ランダムに数字を入れて画像を表示させる順番を変える
    int fadeScreen_;                                                    // キューに投げたらすごいことになるので一旦こっちに描画する
    int fadeCnt_;

    std::map<FadeMode,std::function<FadeMode(void)>> fadeMap_;          // フェードの管理
    FadeMode fadeMode_;                                                 // フェード管理用
    int waitCnt_;                                                       // 待ち時間管理用

    // ここでインスタンスしてゲームシーンに渡す
    std::vector<std::unique_ptr<Stage>> stage_;		               	    // ステージをあらかじめ作っておく
};
