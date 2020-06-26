#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "MouseCtl.h"
#include "CardServer.h"

using MapInt = std::map<int, int>;
using VecInt = std::vector<int>;
using sharedMouse = std::shared_ptr<MouseCtl>;

#define lpTicketMachine TicketMachine::GetInstance()

enum class PayType
{
	CASH,	// ����
	CARD,	// IC�J�[�h
	MAX		// ���ݒ�
};

class TicketMachine
{
public:
	static TicketMachine& GetInstance()
	{
		static TicketMachine s_Instance;
		return s_Instance;
	}
	void Run(void);							// ���C������
	bool InsertCash(int cash);				// �����������������m�F
	bool InsertCard(void);					// �L���b�V���J�[�h
	void Draw(void);
	VecInt& GetMoneyType(void);
	bool Init(sharedMouse mouse);

private:
	bool PayCash(void);						// ������������
	bool PayCard(void);						// �J�[�h��������
	void Clear(void);
	void DrawBtn(void);
	bool paySuccess;						// �x��������

	sharedMouse mouse;						// ���炤���

	std::string btnKey;						// �\������{�^���ւ̃L�[
	Vector2 btnpos;							// �{�^���̍��W

	std::vector<int> moneyType;				// �x������@
	PayType payType;						// �x�������@
	MapInt cashData;						// ����
	MapInt cashDataChange;					// ���ނ�p
	PairInt cardData;						// �J�[�h�f�[�^

	std::map<std::string, int> images;		// �摜�ǂݍ��ݗp

	const int pay_btn_sizeX;				// �x�����{�^���̉��T�C�Y
	const int pay_btn_sizeY;
	const int comment_offsetY;
	const int draw_offsetX;					// �����`��̃I�t�Z�b�g
	const int draw_offsetY;
	const int price_card;					// �����̏ꍇ�̉��i
	const int price_cash;					// �d�q�}�l�[�̏ꍇ�̉��i

//---------------
	const int screen_sizeX;
	const int screen_sizeY;
	const int money_sizeX;					// �摜�T�C�Y
	const int money_sizeY;					// �摜�T�C�Y
	const int font_size;
//---------------


	TicketMachine();
	~TicketMachine();
	static TicketMachine s_Instance;
};

