#ifndef HANDLER_SHM_HPP
#define HANDLER_SHM_HPP

#include "common/log/log.hpp"
#include "common/shm/shm.hpp"

namespace handler {
	struct with_shm {
		inline with_shm(shm::shm_t &&shm):
			_shm{std::move(shm)},
			_shm_offset{0}
		{}
		
		inline with_shm(handler::with_shm &&other):
			_shm{std::move(other._shm)},
			_shm_offset{other._shm_offset}
		{}

		inline char *shm() const {
			return reinterpret_cast<char *>(_shm.memory());
		}

		inline size_t shm_offset() const {
			return _shm_offset;
		}

		inline void shm_push(size_t bytes) {
			LOG_TRACE("Pushing " << bytes << " B of shm space");
			_shm_offset += bytes;
		}

		inline void shm_pop(size_t bytes) {
			LOG_TRACE("Popping " << bytes << " B of shm space");
			_shm_offset -= bytes;	
		}

	protected:
		shm::shm_t _shm;
		size_t _shm_offset;
	};
}

#endif
