#pragma once
#include <functional>
#include <map>
#include "Object.h"
#include "../common/Vector2.h"
#include "../Scene/BaseScene.h"

class Item :
    public Object
{
public:
    Item(ItemType type, Vector2& pos, BaseScene& baseScene);
    ~Item();

    bool Update(void)override;
    void Draw(void)override;

    bool UpdateDef();
    ObjectType ObjType(void) { return ObjectType::Item; };
    ItemType GetItemType(void) { return itemType_; };
private:
    void InitFunc(void);


    std::map<STATE, std::function<void(void)>>stateUpdate_;
    ItemType itemType_;

    int moveCnt_;
    int sinPosY_;

    BaseScene& scene_;												// ゲームシーンアクセス用
};

