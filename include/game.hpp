#pragma once

#include <SDL3/SDL_render.h>

#include <expected>

#include "core/application.hpp"
#include "core/window.hpp"
#include "rerror/error.hpp"
#include "unders_helpers/types.hpp"

class Game : public Application {
 protected:
  Game(Application&& app) : Application(std::move(app)) {};

 public:
  enum class Error {
    Application
  };

  static std::expected<Game, re::AnyError> create(std::string title, u32 width, u32 height, Window::Flags flags = Window::Flags::None) {
    auto app = Application::create(title, width, height, flags);
    if (!app) [[unlikely]]
      return std::unexpected(re::anyError(Error::Application, "Failed to create game's base application", std::move(app.error())));

    return Game(std::move(*app));
  }

  re::expected<re::AnyError> setup() noexcept override;
  re::expected<re::AnyError> input(const SDL_Event& event) noexcept override;
  re::expected<re::AnyError> update(double delta_time) noexcept override;
  re::expected<re::AnyError> draw() const noexcept override;
};
