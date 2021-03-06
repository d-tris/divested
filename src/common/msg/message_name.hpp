#ifndef MSG_MESSAGE_NAME_HPP
#define MSG_MESSAGE_NAME_HPP

#include "common/msg/effect/messages.hpp"
#include "common/msg/general/messages.hpp"
#include "common/msg/master/messages.hpp"

namespace msg {
	template<typename T>
	constexpr const char *message_name;

	#define EFFECT_MESSAGE_TO_NAME(MSG) \
		template <> \
		inline constexpr const char *message_name<effect::MSG> = #MSG;
	MSG_EFFECT_MESSAGES(EFFECT_MESSAGE_TO_NAME)
	#undef EFFECT_MESSAGE_TO_NAME

	#define MASTER_MESSAGE_TO_NAME(MSG) \
		template <> \
		inline constexpr const char *message_name<master::MSG> = #MSG;
	MSG_MASTER_MESSAGES(MASTER_MESSAGE_TO_NAME)
	#undef MASTER_MESSAGE_TO_NAME

	#define GENERAL_MESSAGE_TO_NAME(MSG) \
		template <> \
		inline constexpr const char *message_name<general::MSG> = #MSG;
	MSG_GENERAL_MESSAGES(GENERAL_MESSAGE_TO_NAME)
	#undef MASTER_MESSAGE_TO_NAME
}

#endif
