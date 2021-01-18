#include <DxLib.h>
#include <string>
#include <sstream>
#include <algorithm>
#include "../_debug/_DebugConOut.h"
#include "ResultScene.h"
#include "CrossOverScene.h"
#include "LoginScene.h"
#include "../NetWork/NetWork.h"

ResultScene::ResultScene(std::vector<int>& resultData)
{
    // ID範囲チェック(-1 はハイフンの為許容)
    //for (auto id = resultData.begin(); id != resultData.end(); id++)
    //{
    //    if ((*id) > (lpNetWork.GetPlayerMax() * 5) ||
    //        (*id) < -1 ||
    //        (*id) % 5 != 0)
    //    {
    //        TRACE("Result 不正なデータを発見したので削除します ID : %d\n", (*id));
    //        (*id) = -1;
    //        resultData.erase(id);
    //        cnt++;
    //    }
    //}

    auto check = [&]()
    {
        int cnt = -1;
        for (auto id = resultData.begin(); id != resultData.end(); id++)
        {
            if ((*id) > (lpNetWork.GetPlayerMax() * 5) ||
                (*id) < -1 ||
                (*id) % 5 != 0)
            {
                //cnt++;
                return cnt;
            }
            cnt++;
        }
        return -1;
    };

    int cnt = 0;
    int checkCnt = check();
    while (checkCnt != -1)
    {
        resultData.erase(resultData.begin() + checkCnt);
        checkCnt = check();
        cnt++;
    }

    for (int i = 0; i < cnt; i++)
    {
        TRACE("Result 不正なデータを発見したので削除します\n");
        resultData.push_back(-1);
    }

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
        oss << (ranking / 5 + 1);
        auto data = oss.str();
        std::string str;
        if (ranking != -1)
        {
            str = "%d位：" + data;
        }
        else
        {
            str = "%d位：-";
        }
        DrawFormatString(100, 100 + 30 * i, 0xffff, str.c_str(), i);
        i++;
    }
}

void ResultScene::Init(void)
{
}
