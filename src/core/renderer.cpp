#include "core/renderer.hpp"

SDL_Renderer* Renderer::get_raw() const {
  return _renderer;
}

void Renderer::clear(u8 r, u8 g, u8 b, u8 a) const {
  SDL_SetRenderDrawColor(_renderer, r, g, b, a);
  SDL_RenderClear(_renderer);
}

void Renderer::present() const {
  SDL_RenderPresent(_renderer);
}
