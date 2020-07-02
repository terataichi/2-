#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <functional>
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
	void Run(void);																	// ���C������
	void Draw(void);
	VecInt& GetMoneyType(void);
	bool Init(sharedMouse mouse);

	PayType& GetPayType(void);
	void SetPayType(PayType type);
	MapInt& GetCashData(void);
	PairInt& GetCardData(void);
	bool GetPaySuccess(void);
private:
	bool InitDraw(void);															// �`���o�^������
	bool InitPay(void);
	bool PayCash(void);																// ������������
	bool PayCard(void);																// �J�[�h��������
	bool PayMax(void);																// ��O����
	void Clear(void);
	void DrawBtn(void);
	bool paySuccess;																// �x��������

	sharedMouse mouse;																// ���炤���

	std::string btnKey;																// �\������{�^���ւ̃L�[
	std::string cancelKey;
	Vector2 btnpos;																	// �{�^���̍��W
		
	std::vector<int> moneyType;														// �x������@
	PayType payType;																// �x�������@
	MapInt cashData;																// ����
	MapInt cashDataChange;															// ���ނ�p
	PairInt cardData;																// �J�[�h�f�[�^

	std::map<std::string, int> images;												// �摜�ǂݍ��ݗp

	std::map<PayType, std::function<void(void)>>draw;								// �`�悷�邽�߂̃����_�����i�[����map
	std::map<PayType, bool (TicketMachine::*)()> pay;								// ���Ϗ����pmap

	const int pay_btn_sizeX;														// �x�����{�^���̉��T�C�Y
	const int pay_btn_sizeY;
	const int comment_offsetY;
	const int draw_offsetX;															// �����`��̃I�t�Z�b�g
	const int draw_offsetY;
	const int price_card;															// �����̏ꍇ�̉��i
	const int price_cash;															// �d�q�}�l�[�̏ꍇ�̉��i
	const int cancel_offSet;														// �L�����Z���p�I�t�Z�b�g�i�[�p
//---------------
	const int screen_sizeX;
	const int screen_sizeY;
	const int money_sizeX;															// �摜�T�C�Y
	const int money_sizeY;															// �摜�T�C�Y
	const int font_size;
//---------------


	TicketMachine();
	~TicketMachine();
	static TicketMachine s_Instance;
};

