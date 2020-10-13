#include "NetWork.h"

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::sInstance_(new NetWork);

IPDATA NetWork::GetIP()
{
	GetMyIPAddress();
	return  
}

NetWork::~NetWork()
{
}
