#pragma once
#include <memory>
#include <map>
#include <functional>
#include <list>
#include <chrono>
#include "BaseScene.h"
#include "../common/TileMap.h"
#include "../Object.h"

using sharedObj = std::shared_ptr<Object>;

// 状態管理用
enum class GameState
{
    Wait,
    Play
};

class GameScene :
    public BaseScene
{
public:
    GameScene();
    ~GameScene();

    uniqueBase Update(uniqueBase scene)override;
    void DrawOwnScene(void)override;
    void Init(void)override;
    bool SetBomb(int& ownerID, int& myID, Vector2& pos, chronoTime& time,int length, bool flg);
    sharedObj GetObjectList(int id);                                                             // オブジェリストから要素の取り出し
    void FlameGenerate(int& length, Vector2& pos);                                               // フレーム生成
    bool CheckHitFlame(int chipPos);                                                             // 爆風との当たり判定用

    void SetBombMap(int chipPos, bool flg);
    const std::vector<bool> GetBombMap(void);
private:
    void InitFunc();
    TileMap tileMap_;                                                                            // タイルマップ情報格納

    std::chrono::system_clock::time_point  now_;
    std::chrono::system_clock::time_point  end_;

    std::map<GameState, std::function<void(void)>> stateUpdate_;                                // 状態別アップデート
    GameState gameState_;

    int fpsCount_;
    int fps_;

    int averageCount_;

    std::list<sharedObj> objList_;                                                            // ｵﾌﾞｼﾞｪｸﾄリスト
};

