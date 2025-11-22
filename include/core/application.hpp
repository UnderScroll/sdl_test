#pragma once

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>

#include <expected>
#include <rerror/error.hpp>
#include <unders_helpers/unused.hpp>

#include "core/renderer.hpp"
#include "core/window.hpp"

class Application {
 private:
  bool _owned = true;

 protected:
  /* Members */
  Renderer _renderer; // /!\ Must be decalared before _window because of destruction order (see: https://wiki.libsdl.org/SDL3/SDL_DestroyRenderer)
  Window _window;

  bool _shouldContinue = true;

  /* Constructor */
  Application(Window&& window, Renderer&& renderer)
      : _renderer(std::move(renderer)), _window(std::move(window)) {};

 public:
  enum class Error {
    NotImplemented,
    Creation,
    SdlInitialization,
    WindowCreation,
    RendererCreation,
  };

  /* Special constructors */
  // No copy
  Application(const Renderer&) = delete;
  Application& operator=(const Renderer&) = delete;

  // Moveable
  Application(Application&& other) noexcept
      : _renderer(std::move(other._renderer)), _window(std::move(other._window)) {
    other._owned = false;
  }
  Application& operator=(Application&& other) noexcept {
    _renderer = std::move(other._renderer);
    _window = std::move(other._window);

    other._owned = false;
    return *this;
  }

  /* Destructor */
  virtual ~Application() {
    if (_owned)
      SDL_Quit();
  }

  /* Functional constructors */
  [[nodiscard]]
  static std::expected<Application, re::Error<Error>> create(std::string title, u32 width, u32 height, Window::Flags flags = Window::Flags::None) {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) [[unlikely]]
      return std::unexpected(re::error(Error::SdlInitialization, std::string(SDL_GetError())));

    // Window
    std::expected<Window, re::Error<Window::Error>> window = Window::create(title, width, height, flags);
    if (!window) [[unlikely]]
      return std::unexpected(re::error(Error::WindowCreation, "Failed to create window", std::move(window.error())));

    // Renderer
    std::expected<Renderer, re::Error<Renderer::Error>> renderer = Renderer::create(*window);
    if (!renderer) [[unlikely]]
      return std::unexpected(re::error(Error::RendererCreation, "Failed to create Renderer", std::move(renderer.error())));

    return Application(std::move(*window), std::move(*renderer));
  }

  /* Member functions */
  re::expected<re::AnyError> run();

  /* Virtual functions */
  virtual re::expected<re::AnyError> setup() noexcept { return std::unexpected(re::anyError(Error::NotImplemented, "The setup() function was not implemented")); }
  virtual re::expected<re::AnyError> input(const SDL_Event& UNUSED(event)) noexcept { return std::unexpected(re::anyError(Error::NotImplemented, "The input() function was not implemented")); }
  virtual re::expected<re::AnyError> update(double UNUSED(delta_time)) noexcept { return std::unexpected(re::anyError(Error::NotImplemented, "The update() function was not implemented")); }
  virtual re::expected<re::AnyError> draw() const noexcept { return std::unexpected(re::anyError(Error::NotImplemented, "The draw() function was not implemented")); }
};
