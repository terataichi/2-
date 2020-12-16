#include "ResultScene.h"
#include "CrossOverScene.h"
#include "LoginScene.h"
#include "../NetWork/NetWork.h"
#include <DxLib.h>
#include <string>
#include <sstream>

ResultScene::ResultScene(UnionVec& resultData)
{
    resultData_ = resultData;
    lpNetWork.EndNetWork();
    DrawOwnScene();
}

ResultScene::~ResultScene()
{
}

uniqueBase ResultScene::Update(uniqueBase scene)
{
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        return std::make_unique<CrossOverScene>(std::move(scene), std::make_unique<LoginScene>());
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
