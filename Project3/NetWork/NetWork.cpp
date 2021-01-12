#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include <DxLib.h>
#include <chrono>
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
	MesH revHeader{};
	UnionVec tmpPacket{};
	int writePos = 0;
	while (ProcessMessage() == 0)
	{
		auto lostHandle = GetLostNetWork();

		for (auto list = handlelist_.begin(); list != handlelist_.end(); list++)
		{
			if (lostHandle == list->handle_)
			{
				TRACE("切断されたハンドル削除\n");
				handlelist_.erase(list);
				playerMax_--;
				TRACE("-max :%d,", playerMax_);
				break;
			}
		}

		if (!state_->Update() || !handlelist_.size())
		{
			// 使ったものをリセットする
			//state_.reset();
			recvStanby_ = false;
		}
		else
		{
			break;
		}
	}

	while (ProcessMessage() == 0 && handlelist_.size() && !endFlg_)
	{
		auto lostHandle = GetLostNetWork();
		int lengthCnt = 0;
		// データの長さチェック
		for (auto list = handlelist_.begin(); list != handlelist_.end(); list++)
		{
			if (lostHandle == list->handle_)
			{
				TRACE("切断されたハンドル削除\n");

				if (revDataList_.size())
				{
					std::lock_guard<std::mutex> lock(revDataList_[list->id_ / 5].first);
					UnionData data;
					data.iData = list->id_;
					UnionVec packet{ data };
					SendMesAll(MesType::LOST, packet, 0);
					revDataList_[list->id_ / 5].second.emplace_back(MesH{ MesType::LOST,0,0,0 }, packet);
				}
				handlelist_.erase(list);
				playerMax_--;
				break;
			}

			if (GetNetWorkDataLength(list->handle_) >= sizeof(MesH))
			{
				// これ以上データがないとき
				// ※ヘッダー部を受信する前情報
				if (!revHeader.next)
				{
					tmpPacket.resize(0);
					writePos = 0;
				}

				// ヘッダー部の受信
				NetWorkRecv(list->handle_, &revHeader, sizeof(MesH));

				// データがある場合
				if (revHeader.length)
				{
					if (tmpPacket.max_size() > static_cast<size_t>(tmpPacket.size() + revHeader.length))
					{
						tmpPacket.resize(tmpPacket.size() + revHeader.length);

						// データの受け取り
						NetWorkRecv(list->handle_, &tmpPacket[writePos], sizeof(int) * revHeader.length);
						writePos = tmpPacket.size();
					}
					else
					{
						TRACE("データサイズが大きすぎます\n");
					}
				}
				// まだデータが残ってる場合
				if (revHeader.next)
				{
					lengthCnt++;
					// TRACE("%d\n", lengthCnt);
					continue;
				}
				lengthCnt = 0;

				// 

				if (revHeader.length < 500 && revHeader.length >= 0)
				{

					if (static_cast<int>(revHeader.type) >= static_cast<int>(MesType::NON) &&
						static_cast<int>(revHeader.type) <= static_cast<int>(MesType::MAX))
					{
						if (revHeader.type == MesType::POS)
						{
							if (revHeader.length == 6)
							{
								SendMesAll(revHeader.type, tmpPacket, list->handle_);

								unsigned int type = static_cast<int>(revHeader.type) - static_cast<int>(MesType::NON);
								revUpdate_[type](revHeader, tmpPacket);
							}
							else
							{
								TRACE("pos\n");
							}
						}
						else
						{
							SendMesAll(revHeader.type, tmpPacket, list->handle_);

							unsigned int type = static_cast<int>(revHeader.type) - static_cast<int>(MesType::NON);
							revUpdate_[type](revHeader, tmpPacket);
						}

					}
					else
					{
						TRACE("メッセージタイプが変 ID : %d\n", list->id_);
					}
				}
			}
		}
	}

	// オフライン以外だったら切断するよ
	if (state_->GetMode() != NetWorkMode::OFFLINE)
	{
		TRACE("オンラインを終了します\n");
		if (handlelist_.size())
		{
			CloseNetWork();
			Init();
			InitFunc();
		}
	}

	return true;
}

bool NetWork::CloseNetWork(void)
{
	for (auto list = handlelist_.begin(); list != handlelist_.end(); list++)
	{
		DxLib::CloseNetWork(list->handle_);
	}	return true;
}

ActiveState NetWork::GetActive(void)
{
	return state_->GetActive();
}

void NetWork::SendStanby(void)
{
	// 初期化情報の送信
	state_->SetActive(ActiveState::Stanby);
	SendMesAll(MesType::STANBY_HOST);
}

void NetWork::SendStart(void)
{
	// 初期化情報の送信
	SendMes(MesType::STANBY_GUEST);
}

bool NetWork::CheckNetWork()
{
	return state_->CheckNetWork();
}

bool NetWork::EndNetWork(void)
{
	endFlg_ = true;
	return endFlg_;
}

chronoTime NetWork::GetStartTime(void)
{
	return countDownTime_;
}

void NetWork::SetStartTime(chronoTime time)
{
	countDownTime_ = time;
}

bool NetWork::GetCountDownFlg(void)
{
	return countDownFlg_;
}

void NetWork::SetCountDownFlg(bool flg)
{
	countDownFlg_ = flg;
}

void NetWork::SetStartCntFlg(bool flg)
{
	startCntFlg_ = flg;
}

bool NetWork::GetStartCntFlg(void)
{
	return startCntFlg_;
}

void NetWork::AddPlayerMax()
{
	playerMax_++;
}

const int NetWork::GetPlayerMax(void) const
{
	return playerMax_;
}

const int NetWork::GetPlayerID(void) const
{
	return playerID_;
}

const UnionVec& NetWork::GetResultData(void) const
{
	return resultData_;
}

void NetWork::AddHandleList(PlayerHandle intp)
{
	handlelist_.emplace_front(intp);
}

listIntP& NetWork::GetHandleList(void)
{
	return handlelist_;
}

bool NetWork::AddRevList(std::mutex& mtx, RevDataListP& data)
{
	// 追加
	revDataList_.emplace_back(std::pair<std::mutex&, RevDataListP&>{ mtx, data });
	return true;
}

void NetWork::RunUpDate(void)
{
	update_ = std::thread(&NetWork::Update, this);
	update_.detach();
}

void NetWork::SetHeader(UnionHeader& header, UnionVec& packet)
{
	packet.insert(packet.begin(), { header.iData[1] });
	packet.insert(packet.begin(), { header.iData[0] });
}

bool NetWork::SendMes(MesType type,UnionVec packet)
{
	if (!handlelist_.size())
	{
		return false;
	}

	SendMes(type, packet, handlelist_.front().handle_);

	return true;
}

bool NetWork::SendMes(MesType type, UnionVec packet, int handle)
{
	// 受け取ったMesTypeでヘッダーを生成して、MesPacketの先頭に挿入する。
	UnionHeader header{ type,0,0 };
	SetHeader(header, packet);

	// 求めた送信データ長からヘッダーサイズを除いた分をヘッダーのlengthに入れる。
	do
	{
		// 送信データ長を求める
		int sendCount = (packet.size() < 500 / sizeof(UnionData)) ? packet.size() : 500 / sizeof(UnionData);

		// 求めた送信データ長からヘッダーサイズを除いた分をヘッダーのlengthに入れる
		header.mesH.length = sendCount - HEADER_COUNT;

		// 分割しないといけないかどうか
		if (packet.size() == sendCount)
		{
			//TRACE("分割しません\n");
			header.mesH.next = 0;
		}
		else
		{
			//TRACE("分割します\n");
			//TRACE("%d回目\n", header.mesH.sendID);
			header.mesH.next = 1;
		}

		// ヘッダー情報の更新
		packet[0].iData = header.iData[0];
		packet[1].iData = header.iData[1];


		// データの送信
		NetWorkSend(handle, packet.data(), sendCount * sizeof(UnionData));

		// 送った要素のみ削除
		packet.erase(packet.begin() + HEADER_COUNT, packet.begin() + sendCount);

		// 送る度にカウント++
		header.mesH.sendID++;

		// 一度は送ってほしいからdo
		// 分割データが送り終わるまでループ
	} while (packet.size() > HEADER_COUNT);
	return false;
}

bool NetWork::SendMesAll(MesType type, UnionVec packet, int handle)
{
	//if (state_->GetNetHandle().size())
	//{
	//	if (state_->GetNetHandle().front().first == -1)
	//	{
	//		return false;
	//	}
	//}

	for (auto list = handlelist_.begin(); list != handlelist_.end(); list++)
	{
		if (list->handle_ == -1)
		{
			continue;
		}

		if (list->handle_ != handle)
		{
			// 受け取ったMesTypeでヘッダーを生成して、MesPacketの先頭に挿入する。
			UnionHeader header{ type,0,0 };
			UnionVec tmpPacket = packet;
			SetHeader(header, tmpPacket);
			// 求めた送信データ長からヘッダーサイズを除いた分をヘッダーのlengthに入れる。
			do
			{
				// 送信データ長を求める
				int sendCount = (tmpPacket.size() < 500 / sizeof(UnionData)) ? tmpPacket.size() : 500 / sizeof(UnionData);

				// 求めた送信データ長からヘッダーサイズを除いた分をヘッダーのlengthに入れる
				header.mesH.length = sendCount - HEADER_COUNT;

				// 分割しないといけないかどうか
				if (tmpPacket.size() == sendCount)
				{
					//TRACE("分割しません\n");
					header.mesH.next = 0;
				}
				else
				{
					//TRACE("分割します\n");
					//TRACE("%d回目\n", header.mesH.sendID);
					header.mesH.next = 1;
				}

				// ヘッダー情報の更新
				tmpPacket[0].iData = header.iData[0];
				tmpPacket[1].iData = header.iData[1];


				// データの送信
				if (handlelist_.size())
				{
					NetWorkSend(list->handle_, tmpPacket.data(), sendCount * sizeof(UnionData));
				}

				// 送った要素のみ削除
				tmpPacket.erase(tmpPacket.begin() + HEADER_COUNT, tmpPacket.begin() + sendCount);

				// 送る度にカウント++
				header.mesH.sendID++;

				// 一度は送ってほしいからdo
				// 分割データが送り終わるまでループ
			} while (tmpPacket.size() > HEADER_COUNT);
		}
	}
	return false;
}

bool NetWork::SendMes(MesType type)
{
	UnionVec vec{};
	return SendMes(type, vec);
}

bool NetWork::SendMesAll(MesType type)
{
	UnionVec vec{};
	return SendMesAll(type, vec, 0);
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
	Init();
	InitFunc();
}

NetWork::~NetWork()
{
	if (update_.joinable())
	{
		update_.join();
	}
}

void NetWork::InitFunc(void)
{
	// ---- ホスト ---
	auto gameStart = [&](MesH& data,UnionVec& packet)
	{
		// ゲームスタートを受信時
		if (state_->GetActive() == ActiveState::Stanby)
		{
			// ゲームスタートを受信時
			if (static_cast<MesType>(data.type) == MesType::STANBY_GUEST)
			{
				TRACE("ゲストから通知を確認\n"); 
				stanbyCnt_++;
				// 自分の分を引いておく
				if (playerMax_ - 1  <= stanbyCnt_)
				{
					// カウントダウンの送信
					UnionData data[2];
					TimeData time{ lpSceneMng.GetTime() };

					data[0].iData = time.iData[0];
					data[1].iData = time.iData[1];

					lpNetWork.SetStartTime(time.time);
					lpNetWork.SetStartCntFlg(true);
					lpNetWork.SendMesAll(MesType::COUNT_DOWN_GAME, UnionVec{ data[0],data[1] }, 0);

					state_->SetActive(ActiveState::Play);
					playerID_ = 0;
				}
				return true;
			}
		}
		return false;
	};

	// ---- ゲスト ---
	auto stanby = [&](MesH& data, UnionVec& packet)
	{
		if (loadStage_ > 0)
		{
			TRACE("ｽﾃｰｼﾞ情報が%d回送られてきてます\n",loadStage_);
			return false;
		}
		loadStage_++;
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

		return true;
	};

	auto tmx_Size = [&](MesH& data, UnionVec& packet)
	{
		// 縦かけ横かけレイヤー数
		tmxSize_ = packet[0].cData[0] * packet[0].cData[1] * packet[0].cData[2];
		revBox_.resize(tmxSize_);

		TRACE("TMXデータサイズは：%d\n", tmxSize_);
		start_ = std::chrono::system_clock::now();
		return true;
	};

	auto  tmx_Data = [&](MesH& data, UnionVec& packet)
	{
		revBox_ = packet;
		return true;
	};

	auto addList = [&](MesH& data, UnionVec& packet)
	{
		if (packet.size())
		{
			if (data.type == MesType::LOST)
			{
				TRACE("誰かが切断\n");
			}

			if (mesTypeSize_[data.type] != packet.size())
			{
				TRACE("ボマー見つけた\n");
				return false;
			}

			int id = packet[0].iData / 5;

			if (id < static_cast<int>(revDataList_.size()))
			{
				if (id <= playerMax_)
				{
					// if (packet[0].iData > 0)
					{
						std::lock_guard<std::mutex> lock(revDataList_[id].first);
						auto a = (revDataList_[id].second.size() + packet.size());
						auto b = revDataList_[id].second.max_size();

						if (packet.size() > 5000)
						{
							int unko = 0;
						}

						if (b > a && a > 0)
						{
							revDataList_[id].second.emplace_back(data, packet);
						}
						else
						{
							int unchi = 0;
						}
					}
				}
			}
		}
		return true;
	};

	auto non = [&](MesH& data, UnionVec& packet)
	{
		TRACE("Non\n");
		return true;
	};

	auto countDown = [&](MesH& data, UnionVec& packet) 
	{
		TimeData timeData{};
		timeData.iData[0] = packet[0].iData;
		timeData.iData[1] = packet[1].iData;

		countDownFlg_ = true;
		countDownTime_ = timeData.time;
		return true;
	};

	auto id = [&](MesH& data, UnionVec& packet)
	{
		// 一回だけ設定
		if (packet[0].iData % 5 == 0)
		{
			if (playerID_ == -1 && packet[0].iData / 5 <= packet[1].iData)
			{
				playerID_ = packet[0].iData;
				playerMax_ = packet[1].iData;
				state_->SetPlayerID(playerID_);
			}
		}
		TRACE("id : %d", playerID_);
		TRACE("max : %d\n", playerMax_);

		return true;
	};

	auto startTime = [&](MesH& data, UnionVec& packet)
	{
		TimeData timeData{};
		timeData.iData[0] = packet[0].iData;
		timeData.iData[1] = packet[1].iData;

		startCntFlg_ = true;
		countDownTime_ = timeData.time;

		chronoTime now = std::chrono::system_clock::now();
		auto time = std::chrono::duration_cast<std::chrono::milliseconds>(now - countDownTime_).count();
		auto tmp = (COUNT_START_MAX - time) / 1000;
		if (tmp < 0)
		{
			countDownTime_ = lpSceneMng.GetTime();
		}
		return true;
	};

	auto result = [&](MesH& data, UnionVec& packet)
	{
		if (packet.size() == 5)
		{
			resultData_ = packet;
		}
		return true;
	};

	revUpdate_[0] = non;
	revUpdate_[1] = countDown;
	revUpdate_[2] = id;
	revUpdate_[3] = stanby;
	revUpdate_[4] = gameStart;
	revUpdate_[5] = startTime;
	revUpdate_[6] = tmx_Size;
	revUpdate_[7] = tmx_Data;
	revUpdate_[8] = addList;
	revUpdate_[9] = addList;
	revUpdate_[10] = addList;
	revUpdate_[11] = result;
	revUpdate_[12] = addList;
}

void NetWork::Init(void)
{
	recvStanby_ = false;
	tmp_ = 0;
	tmxSize_ = 0;
	ip_ = ArrayIP{};
	sendLength_ = 0;
	state_.reset();
	countDownFlg_ = false;
	startCntFlg_ = false;
	stanbyCnt_ = 0;
	playerID_ = -1;
	playerMax_ = 0;
	revDataList_.clear();
	resultData_.clear();
	revBox_.clear();
	endFlg_ = false;
	handlelist_.clear();
	loadStage_ = 0;
	// バイト長の読み込み
	std::ifstream ifs("init/setting.txt");
	if (ifs.fail())
	{
		TRACE("ファイルの読み込みに失敗しました。\n");
		return;
	}
	// ファイルの内容を格納
	std::string data;
	getline(ifs, data);
	if (data.find("byte"))
	{
		std::istringstream is{ data.substr(data.find_first_of("\"")+ 1, data.find_last_of("\"") - 1)};
		std::string length;
		getline(is, length);
		sendLength_ = atoi(length.c_str());
	}


	mesTypeSize_.try_emplace(MesType::NON, 0);
	mesTypeSize_.try_emplace(MesType::COUNT_DOWN_GAME, 2);
	mesTypeSize_.try_emplace(MesType::COUNT_DOWN_ROOM, 2);
	mesTypeSize_.try_emplace(MesType::DETH, 1);
	mesTypeSize_.try_emplace(MesType::ID, 2);
	mesTypeSize_.try_emplace(MesType::LOST, 1);
	mesTypeSize_.try_emplace(MesType::MAX, 0);
	mesTypeSize_.try_emplace(MesType::POS, 4);
	mesTypeSize_.try_emplace(MesType::SET_BOMB, 7);
	mesTypeSize_.try_emplace(MesType::STANBY_GUEST, 0);
	mesTypeSize_.try_emplace(MesType::STANBY_HOST, 0);
	mesTypeSize_.try_emplace(MesType::TMX_SIZE, 1);
	mesTypeSize_.try_emplace(MesType::TMX_DATA, 179);

}
