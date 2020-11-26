#pragma once

enum class DIR
{
	DOWN = 0,
	LEFT,
	RIGHT,
	UP,
//	Deth,
	MAX
};

DIR begin(DIR);
DIR end(DIR);
DIR operator*(DIR dir);
DIR operator++(DIR& dir);