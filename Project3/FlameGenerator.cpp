#include "FlameGenerator.h"
#include "_debug/_DebugConOut.h"

FlameGenerator::FlameGenerator(int length, Vector2& pos)
{
	TRACE("flameGenerator¶¬\n");
	length_ = length;
	pos_ = { pos.x / 32,pos.y / 32 };
	time_ = 60;
	count_ = 0;
}

FlameGenerator::~FlameGenerator()
{
	TRACE("flameGenerator”jŠü\n");
}

bool FlameGenerator::Update(void)
{

	if (count_ > time_)
	{
		return false;
	}
	count_++;
	return true;
}
