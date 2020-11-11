#pragma once

enum class DIR
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
	MAX
};

DIR begin(DIR);
DIR end(DIR);
DIR operator*(DIR dir);
DIR operator++(DIR& dir);