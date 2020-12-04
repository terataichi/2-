#pragma once
#include <functional>
#include <map>
#include "Object.h"
#include "../common/Vector2.h"

enum class ItemType
{
    FireUp,                     // 火力アップ
    AddBomb,                    // ボム追加
    RemoCon,                    // リモコン
    SpeedUp                     // スピードアップ
};

class Item :
    public Object
{
public:
    Item(ItemType type, Vector2& pos);
    ~Item();

    bool Update(void)override;
    void Draw(void)override;

    bool UpdateDef();
    ObjectType ObjType() { return ObjectType::Item; };
private:
    void InitFunc(void);


    std::map<STATE, std::function<void(void)>>stateUpdate_;
    ItemType itemType_;

    int moveCnt_;
    int sinPosY_;
};

