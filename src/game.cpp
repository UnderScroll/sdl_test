#include "game.hpp"

re::expected<re::AnyError> Game::setup() noexcept {
  return re::expected<re::AnyError>();
}

re::expected<re::AnyError> Game::input(const SDL_Event& event) noexcept {
  switch (event.type) {
    default:
      break;
  }

  return re::expected<re::AnyError>();
}

re::expected<re::AnyError> Game::update(double UNUSED(delta_time)) noexcept {
  return re::expected<re::AnyError>();
}

re::expected<re::AnyError> Game::draw() const noexcept {
  _renderer.clear(20, 20, 20);

  return re::expected<re::AnyError>();
}
