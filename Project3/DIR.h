#pragma once

enum class DIR
{
	DOWN,
	LEFT,
	UP,
	RIGHT,
//	Deth,
	MAX
};

DIR begin(DIR);
DIR end(DIR);
DIR operator*(DIR dir);
DIR operator++(DIR& dir);