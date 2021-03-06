#include "client/array_buffer.hpp"
#include "client/callbacks.hpp"
#include "client/plugin_t.hpp"
#include "common/log/log.hpp"
#include "common/msg/type.hpp"
#include "pluginterfaces/vst2.x/aeffectx.h"

plugin_t &get_plugin(AEffect* effect) {
	return *reinterpret_cast<plugin_t *>(effect->user);
}

client_t &get_client(AEffect* effect) {
	return get_plugin(effect).get_client();
}

VstIntPtr VSTCALLBACK aeffect_dispatcher_proc(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
	msg::dispatcher_request request{index, value, ptr, opt};
	array_buffer buf;
	LOG_TRACE("aeffect_dispatcher_proc called, effect=" << reinterpret_cast<void*>(effect)
		<< " opcode=" << opcode
		<< " request=" << request);

	// ignore vendor specific calls (for now)
	if (opcode == effVendorSpecific) {
		LOG_TRACE("Request is vendor specific, ignoring");
		return 0;
	}

	// get the plugin & a client
	auto &plugin = get_plugin(effect);
	auto &client = plugin.get_client();
	auto result = msg::send_dispatcher(client, buf, opcode, request);
	LOG_TRACE("aeffect_dispatcher_proc finished with result " << result);

	// If we got an effMainsChanged message, we can clean up allocated chunks.
	if (opcode == effMainsChanged) {
		plugin.shared.clear_chunks();
	}

	// If we got an effClose message, we should first close all server handlers
	// by sending them a return and then close the primary handler by sending it
	// a return as well.
	if (opcode == effClose) {
		LOG_TRACE("effClose received, shutting down " << plugin.tid_to_client.size() << " client");

		std::lock_guard lock{plugin.primary_client_mutex};

		for (auto &i : plugin.tid_to_client) {
			LOG_TRACE("Stopping server handler for TID " << i.first);

			msg::send_return(*i.second, buf);
			i.second.reset();
		}

		LOG_TRACE("Stopping server handler for primary client");
		msg::send_return(plugin.primary_client, buf);

		LOG_TRACE("Deleting plugin");
		delete &plugin;
		LOG_TRACE("Cleaned up");
	}

	return result;
}

void VSTCALLBACK aeffect_process_proc(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames) {
	auto &client = get_client(effect);
	msg::process_request<float> request{sampleFrames, effect->numInputs, effect->numOutputs, inputs, outputs};
	LOG_TRACE("aeffect_process_proc called, effect=" << reinterpret_cast<void *>(effect) << " request=" << request);
	array_buffer buf;
	msg::send_process(client, buf, request);
}

void VSTCALLBACK aeffect_process_replacing_proc(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames) {
	auto &client = get_client(effect);
	msg::process_request<float> request{sampleFrames, effect->numInputs, effect->numOutputs, inputs, outputs};
	LOG_TRACE("aeffect_process_replacing_proc called, effect=" << reinterpret_cast<void *>(effect) << " request=" << request);
	array_buffer buf;
	msg::send_process_replacing(client, buf, request);
}

void VSTCALLBACK aeffect_process_double_replacing_proc(AEffect* effect, double** inputs, double** outputs, VstInt32 sampleFrames) {
	auto &client = get_client(effect);
	msg::process_request<double> request{sampleFrames, effect->numInputs, effect->numOutputs, inputs, outputs};
	LOG_TRACE("aeffect_process_double_replacing_proc called, effect=" << reinterpret_cast<void *>(effect) << " request=" << request);
	array_buffer buf;
	msg::send_process_double_replacing(client, buf, request);
}

void VSTCALLBACK aeffect_set_parameter_proc(AEffect* effect, VstInt32 index, float parameter) {
	auto &client = reinterpret_cast<plugin_t*>(effect->user)->get_client();
	msg::set_parameter_request request{index, parameter};
	array_buffer buf;
	LOG_TRACE("aeffect_set_parameter_proc called, effect: " << reinterpret_cast<void *>(effect)
		<< "request: " << request);
	msg::send_set_parameter(client, buf, request);
}

float VSTCALLBACK aeffect_get_parameter_proc(AEffect* effect, VstInt32 index) {
	auto &client = reinterpret_cast<plugin_t*>(effect->user)->get_client();
	msg::get_parameter_request request{index};
	array_buffer buf;
	LOG_TRACE("aeffect_get_parameter_proc called, effect: " << reinterpret_cast<void *>(effect)
		<< "request: " << request);
	return msg::send_get_parameter(client, buf, request);
}
