#include <DxLib.h>
#include "InputState.h"

const TrgData& InputState::GetTrgData(void)
{
	return _state;
}

const bool InputState::GetTrgOnePull(INPUT_ID id)
{
	// ó£Ç≥ÇÍÇΩèuä‘
	return !_state.at(id)[static_cast<int>(Trg::Now)] && _state.at(id)[static_cast<int>(Trg::Old)];
}

const bool InputState::GetTrgOnePush(INPUT_ID id)
{
	return _state[id][static_cast<int>(Trg::Now)] && !_state.at(id)[static_cast<int>(Trg::Old)];
}

const bool InputState::GetTrgPush(INPUT_ID id)
{
	return _state[id][static_cast<int>(Trg::Now)];
}

const bool InputState::GetTrgPull(INPUT_ID id)
{
	return !_state.at(id)[static_cast<int>(Trg::Now)] && !_state.at(id)[static_cast<int>(Trg::Old)];
}
