#pragma once

enum class DIR
{
	LEFT,
	UP,
	RIGHT,
	DOWN,
//	Deth,
	MAX
};

DIR begin(DIR);
DIR end(DIR);
DIR operator*(DIR dir);
DIR operator++(DIR& dir);