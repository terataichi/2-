#include "NetWork.h"

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::sInstance_(new NetWork);

IPDATA NetWork::GetIP()
{
	IPDATA ip;
	GetMyIPAddress(&ip);
	return ip;
}

NetWork::NetWork()
{
}

NetWork::~NetWork()
{
}
