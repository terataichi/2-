#pragma once
class CSample
{
public:
	CSample();
	~CSample();
	void add(int w_pos, int num);		// 足し算
	void set(int w_pos, int num);		// ｾｯﾄ関数
	int get(int w_pos);					// ｹﾞｯﾄ関数
	int Count();
private :
	int *n_num;							// 値保持をする変数

	static int count;					// 関数が何回呼ばれたかのトータルを数えるための変数
};

