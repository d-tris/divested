#ifndef MSG_GENERAL_MESSAGES_HPP
#define MSG_GENERAL_MESSAGES_HPP

#include "common/msg/base.hpp"

#define MSG_GENERAL_MESSAGES(F) \
	F(get_parameter) \
	F(set_parameter) \
	F(instantiate_handler)

namespace msg::general {
	struct get_parameter : index, plain_ret {};
	struct set_parameter : index, opt {};
	struct instantiate_handler : index {};
};

#endif
