#pragma once
#include <map>
#include <functional>
#include <string>
#include "Vector2.h"

// �����̃p�^�[�������߂Ă���
enum class MovePattern
{
	Non,						// �����Ȃ�
	UpDown,						// �㉺�ɓ���
	LeftRight,					// ���E�ɓ���
	Scale,						// �g��k��
};

using ButtonMoveFunc = std::function<void(Vector2& pos, float& angle,int& exRate, int count)>;

class Button
{
public:
	//			�摜�t�@�C�����@���W�@�@�@�@�T�C�Y�@�@�@�@�g�嗦�@�@�@�p�x
	Button(std::string& name, Vector2&& pos, Vector2&& size,int rate, float angle);
	~Button();

	void Update();
	bool CheckHitButton(Vector2 pos);				// �{�^���Ƃ̓����蔻��

	// ----- Set
	void pos(Vector2 pos);
	void angle(float angle);
	void movePattern(MovePattern pattern);
	void exRate(int exRate);
	// ----- Get
	const Vector2& pos(void)const;
	const float& angle(void)const;
private:
	bool Init(std::string name,Vector2 pos, Vector2 size, int rate, float angle);
	void Draw();

	Vector2 pos_;											// button�̍��W
	Vector2 size_;											// button�̃T�C�Y
	float angle_;											// button�̊p�x
	int exRate_;											// �g�嗦

	Vector2 stdPos_;										// ���W�̊�ʒu
	float stdAngle_;										// �p�x�̊�ʒu
	int stdExRate_;											// �g�嗦�̊

	std::string id_;										// button�ɂȂ�摜��ID�i�[�p

	MovePattern oldPattern_;								// �O���i�[�p
	MovePattern movePattern_;								// �����̃p�^�[�����ʗp
	int moveCnt_;											// ������p�J�E���g

	std::map<MovePattern, ButtonMoveFunc> moveFunc;			// �e�����Ή��̊֐��I�u�W�F�N�g
};

