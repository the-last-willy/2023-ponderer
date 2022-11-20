#pragma once

#include "ponderer/core/windowing/inspecting/texture.hpp"

#include <memory>
#include <vector>

namespace ponderer::windowing {

class Context {
public:
	std::vector<std::unique_ptr<TextureInspector>> textureInspectors_;
};

inline void removeClosedTextureInspectors(Context& c) {
	for(auto it = begin(c.textureInspectors_); it != end(c.textureInspectors_); (void)0) {
		auto& inspector = **it;
		if(inspector.isOpen()) {
			++it;
		} else {
			it = c.textureInspectors_.erase(it);
		}
	}
}

inline void updateTextureInspectors(Context& c) {
	for(auto&& inspector : c.textureInspectors_) {
		update(*inspector);
	}
}

inline void update(Context& context) {
	removeClosedTextureInspectors(context);
	updateTextureInspectors(context);
}

inline void openInspector(Context& c, gpu::TextureId texture) {
	c.textureInspectors_.push_back(std::make_unique<TextureInspector>(texture));
}

}