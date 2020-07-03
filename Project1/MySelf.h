#pragma once
#include <mutex>
#include <map>
#include <vector>
#include <memory>
#include "TicketMachine.h"

#define lpMySelf MySelf::GetInstance()

using InsFnc = std::function<bool(PayType& , MapInt& , PairInt& , int )>;	// �֐��I�u�W�F�N�g�̐錾

class MouseCtl;

class MySelf
{
public:
	static MySelf& GetInstance()
	{
		std::once_flag once;
		std::call_once(once, Create);
		return *s_Instance;
	}

	static void Create()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new MySelf();
		}
	}

	static void Destroy()
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	bool Run(void);
	void Draw(void);
	bool MergeCash(MapInt& cash);
	void SetIns(InsFnc ins);				// �Z�b�g
private:
	MySelf(void);
	~MySelf(void);

	bool SysInit();
	bool MyInit();

	const int screen_sizeX;
	const int screen_sizeY;
	const int money_sizeX;					// �摜�T�C�Y
	const int money_sizeY;					// �摜�T�C�Y
	const int font_size;

	InsFnc insert;							// �C���T�[�g���֐��I�u�W�F�N�g�ŊǗ�	

	//std::map<PayType, InsFnc> insMap;		// �֐��I�u�W�F�N�g�Ǘ��p�ϐ�
	//std::map<std::string, int> images;	// �摜�ǂݍ��ݗp
	MapInt cash;							// �����Ă��邨��

	// unique_ptr �� unique_ptr �̃����o�[
	//   ��		  �� MouseCtl�̃}���o�[
	// mouse
	sharedMouse mouse;

	static MySelf* s_Instance;
};

