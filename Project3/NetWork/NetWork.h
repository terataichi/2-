#pragma once
#include <memory>
#include <Dxlib.h>

#define lpNetWork NetWork::GetInstance()

class NetWork
{
public:
	static NetWork& GetInstance(void)
	{
		return *sInstance_;
	}

	IPDATA GetIP();
private:

	struct NetWorkDeleter
	{
		void operator() (NetWork* network)const
		{
			delete network;
		}
	};

	NetWork();
	~NetWork();

	static std::unique_ptr<NetWork, NetWorkDeleter> sInstance_;

};

