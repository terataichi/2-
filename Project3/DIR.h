#pragma once

enum class DIR
{
	DOWN,
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