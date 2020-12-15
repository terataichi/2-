#include "ResultScene.h"
#include "CrossOverScene.h"
#include <DxLib.h>
#include <string>
#include <sstream>

ResultScene::ResultScene(UnionVec& resultData)
{
    resultData_ = resultData;
    DrawOwnScene();
}

ResultScene::~ResultScene()
{
}

uniqueBase ResultScene::Update(uniqueBase scene)
{
    if (CheckHitKey(KEY_INPUT_SPACE))
    {

    }
    DrawOwnScene();
    return scene;
}

void ResultScene::DrawOwnScene(void)
{
    SetDrawScreen(drawScreen_);
    int i = 1;
    std::string str;
    for(auto& ranking : resultData_)
    {
        std::ostringstream oss;
        oss << (ranking.iData / 5 + 1);
        auto data = oss.str();
        std::string str;
        if (ranking.iData != -1)
        {
            str = "%dˆÊF" + data;
        }
        else
        {
            str = "%dˆÊF-";
        }
        DrawFormatString(100, 100 + 30 * i, 0xffff, str.c_str(), i);
        i++;
    }
}

void ResultScene::Init(void)
{
}
