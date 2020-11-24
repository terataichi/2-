#pragma once
#include <vector>
#include "common/Vector2.h"


/// <summary>
/// ���̔��������N���X
/// </summary>
class FlameGenerator
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="length">�����̕�</param>
	/// <param name="pos">�������钆�S�̍��W</param>
	FlameGenerator(int length, Vector2& pos);
	~FlameGenerator();

	bool Update(void);

private:

	int length_;
	Vector2 pos_;

	int time_;
	int count_;
};

