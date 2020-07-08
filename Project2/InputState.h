#pragma once
#include <vector>
#include <map>
#include "INPUT_ID.h"

using KeyPair = std::pair<int, int>;
using KeyMap = std::map<INPUT_ID, KeyPair>;

using KeyTuple = std::tuple<int, int, int, int, int>;

class InputState
{
public:
	InputState();
	~InputState();
	void UpDate(void);
	const KeyPair& state(INPUT_ID id)const;		// �L�[����Ԃ�
	bool state(INPUT_ID id, int data);			// new�Z�b�g�֐�
	void SetOld(void);							// old�Z�b�g�֐�
private:
	char _buf[256];								// �L�[�{�[�h���擾�p
	std::vector<int> _keyCon;					// �L�[�̓o�^
	std::vector<int> _keyConDef;				//

	KeyMap _keyState;							// ���ꂼ���key��new �� old�̏��i�[

	static int playCnt;							// �����l�����ꍇ�l���ŷ���ς����悤��
};

