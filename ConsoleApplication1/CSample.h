#pragma once
class CSample
{
public:
	CSample();
	~CSample();
	void add(int w_pos, int num);		// �����Z
	void set(int w_pos, int num);		// ��Ċ֐�
	int get(int w_pos);					// �ޯĊ֐�
	int Count();
private :
	int *n_num;							// �l�ێ�������ϐ�

	static int count;					// �֐�������Ă΂ꂽ���̃g�[�^���𐔂��邽�߂̕ϐ�
};

