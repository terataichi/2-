#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include <DxLib.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <istream>

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::sInstance_(new NetWork);

ArrayIP NetWork::GetMyIP()
{
	GetMyIPAddress(&ip_[0],ip_.size());
	return ip_;
}

bool NetWork::SetNetWorkMode(NetWorkMode mode)
{
	switch (mode)
	{
	case NetWorkMode::OFFLINE:
		state_ = std::make_unique<NetWorkState>();
		break;
	case NetWorkMode::HOST:
		state_ = std::make_unique<HostState>();
		RunUpDate();
		break;
	case NetWorkMode::GUEST:
		state_ = std::make_unique<GuestState>();
		RunUpDate();
		break;
	default:
		TRACE("例外：オフラインに設定します。\n");
		state_ = std::make_unique<NetWorkState>();
		return false;
		break;
	}
	return false;
}

NetWorkMode NetWork::GetNetWorkMode(void)
{
	return state_->GetMode();
}

bool NetWork::CheckConnect(void)
{
	return state_->CheckConnect();
}

bool NetWork::Update(void)
{
	while (ProcessMessage() == 0)
	{

		if (!state_->Update())
		{
			// 使ったものをリセットする
			recvStanby_ = false;
			continue;
		}
		auto handle = state_->GetNetHandle();
		if (handle == -1)
		{
			continue;
		}

		// ヘッダー部の受信
		if (GetNetWorkDataLength(state_->GetNetHandle()) >= sizeof(MesH))
		{
			MesH data;
			NetWorkRecv(state_->GetNetHandle(), &data, sizeof(MesH));

			// データの大きさ分送られてくる

			if (state_->GetMode() == NetWorkMode::HOST)
			{
				if (hostRevMap_.find(data.type) != hostRevMap_.end())
				{
					hostRevMap_[data.type](data);
				}
			}
			else if (state_->GetMode() == NetWorkMode::GUEST)
			{
				if (guestRevMap_.find(data.type) != guestRevMap_.end())
				{
					guestRevMap_[data.type](data);
				}
			}
			TRACE("type : %d\n", data.type);
		}


		//if (guestRevMap_.find(data.type) != guestRevMap_.end())
		//{
		//	guestRevMap_[data.type](data, SendVec);
		//}
	}

	// オフライン以外だったら切断するよ
	if (state_->GetMode() != NetWorkMode::OFFLINE)
	{
		CloseNetWork();
	}

	return true;
}

bool NetWork::CloseNetWork(void)
{
	DxLib::CloseNetWork(state_->GetNetHandle());
	return true;
}

ActiveState NetWork::GetActive(void)
{
	return state_->GetActive();
}

void NetWork::SendStanby(void)
{
	// 初期化情報の送信
	UnionVec vecData;
	UnionHeader mData{ MesType::STANBY,0, 0,0 };

	SetHeader(mData, vecData);
	state_->SetActive(ActiveState::Stanby);

	if (NetWorkSend(state_->GetNetHandle(), &vecData[0], vecData.size() * sizeof(UnionData)) == -1)
	{
		TRACE("Stanby : 送信失敗\n");
	}
}

void NetWork::SendStart(void)
{
	// 初期化情報の送信
	UnionVec vecData;

	UnionHeader mData{ MesType::GAME_START,0, 0,0 };

	SetHeader(mData, vecData);
	if (NetWorkSend(state_->GetNetHandle(), &vecData[0], vecData.size() * sizeof(UnionData)) == -1)
	{
		TRACE("Start : 送信失敗\n");
	}
}

void NetWork::RunUpDate(void)
{
	update=std::thread(&NetWork::Update, this);
	update.detach();
}

void NetWork::SetHeader(UnionHeader header, UnionVec& vec)
{
	vec.insert(vec.begin(), { header.iData[1] });
	vec.insert(vec.begin(), { header.iData[0] });
}

bool NetWork::SendMes(UnionVec& data)
{
	if (NetWorkSend(state_->GetNetHandle(), &data[0], data.size() * sizeof(UnionData)) == -1)
	{
		TRACE("ヘッダー部の送信失敗\n");
		return false;
	}
	return true;
}

bool NetWork::ConnectHost(IPDATA hostIP)
{
	if (state_)
	{
		return state_->ConnectHost(hostIP);
	}
	TRACE("モードが設定されていません\n");
	return false;
}

NetWork::NetWork()
{
	recvStanby_ = false;
	tmp_ = 0;
	tmxSize_ = 0;
	ip_ = ArrayIP{};
	InitFunc();
}

NetWork::~NetWork()
{
	if (update.joinable())
	{
		update.join();
	}
}

void NetWork::InitFunc(void)
{
	// ---- ホスト ---
	auto hostStanby = [&](MesH& data)
	{
		// ゲームスタートを受信時
		if (state_->GetActive() == ActiveState::Stanby)
		{
			// ゲームスタートを受信時
			if (static_cast<MesType>(data.type) == MesType::GAME_START)
			{
				TRACE("ゲストから通知を確認、ゲームを開始します\n");
				state_->SetActive(ActiveState::Play);
				return true;
			}
		}
		return false;
	};


	// ---- ゲスト ---
	auto guestStanby = [&](MesH& data)
	{
		std::ofstream ofs("TileMap/SendData.tmx", std::ios::out);			// 書き込み用

		std::ifstream ifs("TileMap/Stage01_FileData.dat", std::ios::in | std::ios::binary);					// ヘッダー読み込み用
		if (ifs.fail())
		{
			TRACE("読み込み用ファイルが開けません\n");
			return false;
		}

		if (ofs.fail())
		{
			TRACE("書き込み用のファイルが開けません\n");
			return false;
		}

		// ヘッダー部分の書き込み用ラムダ式
		std::string str;
		auto getlineHeader = [&]()
		{
			do
			{
				getline(ifs, str);
				ofs << str;

				// ヘッダーの読み込み完了チェック
				if (ifs.eof())
				{
					TRACE("初期化情報の確認、ゲームを開始の合図をします\n");
					recvStanby_ = true;
					state_->SetActive(ActiveState::Play);
					return true;
				}
			} while (str.find("data encoding") == std::string::npos);

			return false;
		};


		// 最初に一回読み込んでおく
		getlineHeader();

		// データを書き込む
		int count = 0;
		for (auto box : revBox_)
		{
			for (int i = 0; i < 8; i++)
			{
				// 文字の挿入
				ofs << (box.cData[(i / 2) % 4] >> (4 * (i % 2)) & 0x0f);

				count++;

				// 文字を入れるときに最後の行以外にコロンを入れる
				if (count % 21 != 0)
				{
					ofs << ',';
					continue;
				}

				// 行終わりチェック
				if (count % (21 * 17) != 0)
				{
					ofs << ',';
					ofs << std::endl;
					continue;
				}

				ofs << std::endl;

				// 読み込みが終わったので終了
				if (getlineHeader())
				{
					end_ = std::chrono::system_clock::now();
					// 秒に変換
					auto time = std::chrono::duration_cast<std::chrono::microseconds>(end_ - start_).count();
					//TRACE("受け取りから送るまでかかった時間は : %d\n", time);
					std::cout << time << std::endl;
					break;
				}
			}
		}
		ifs.close();
		return true;
	};

	auto tmx_Size = [&](MesH& data)
	{
		if (data.length > 0)
		{
			if (GetNetWorkDataLength(state_->GetNetHandle()) <= static_cast<int>(data.length))
			{
				TRACE("データがLengthより小さいです\n");
			}
			UnionVec SendVec;
			SendVec.resize(data.length);
			NetWorkRecv(state_->GetNetHandle(), &SendVec[0], sizeof(int) * data.length);

			tmxSize_ = SendVec[0].iData;
			revBox_.resize(tmxSize_);
		}

		TRACE("TMXデータサイズは：%d\n", tmxSize_);
		start_ = std::chrono::system_clock::now();
		return true;
	};

	auto  tmx_Data = [&](MesH& data)
	{
		if (data.length > 0)
		{
			if (GetNetWorkDataLength(state_->GetNetHandle()) <= static_cast<int>(data.length))
			{
				TRACE("データがLengthより小さいです\n");
			}
			NetWorkRecv(state_->GetNetHandle(), &revBox_[0], data.length * sizeof(UnionData));
		}
		return true;
	};

	hostRevMap_.try_emplace(MesType::GAME_START, hostStanby );

	guestRevMap_.try_emplace(MesType::STANBY, guestStanby);
	guestRevMap_.try_emplace(MesType::TMX_DATA, tmx_Data);
	guestRevMap_.try_emplace(MesType::TMX_SIZE, tmx_Size);

}
