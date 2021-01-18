#pragma once
#include "../common/Vector2.h"
#include <map>

#define COLOR_MAX 6

class Star
{
public:
	Star(Vector2 pos, Vector2 speed,int num);
	~Star();

	void Update();
	void Draw();

	const Vector2& pos()const;
	const int colorNum()const;
	const int colorCode();
private:

	Vector2 pos_;
	float rad_;
	Vector2 speed_;
	int colorNum_;

	std::map<int, int> colorCode_;
};

