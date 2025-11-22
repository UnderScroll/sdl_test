#include "core/window.hpp"

[[nodiscard]]
Window::Flags Window::get_flags() noexcept {
  return (Flags)SDL_WindowFlags(_window);
}

re::expected<re::Error<Window::Error>> Window::maximize() noexcept {
  if (!SDL_MaximizeWindow(_window)) {
    return std::unexpected(re::error(Error::Maximization, std::string(SDL_GetError())));
  }
  return re::expected<re::Error<Window::Error>>();
}

re::expected<re::Error<Window::Error>> Window::minimize() noexcept {
  if (!SDL_MinimizeWindow(_window)) {
    return std::unexpected(re::error(Error::Minimization, std::string(SDL_GetError())));
  }
  return re::expected<re::Error<Error>>();
}

re::expected<re::Error<Window::Error>> Window::restore() noexcept {
  if (!SDL_RestoreWindow(_window)) {
    return std::unexpected(re::error(Error::Restoration, std::string(SDL_GetError())));
  }
  return re::expected<re::Error<Error>>();
}

re::expected<re::Error<Window::Error>> Window::set_fullscreen(bool isEnabled) noexcept {
  if (!SDL_SetWindowFullscreen(_window, isEnabled)) {
    return std::unexpected(re::error(Error::Fullscreen, std::string(SDL_GetError())));
  }
  return re::expected<re::Error<Error>>();
}

re::expected<re::Error<Window::Error>> Window::show() noexcept {
  if (!SDL_ShowWindow(_window)) {
    return std::unexpected(re::error(Error::Show, std::string(SDL_GetError())));
  }
  return re::expected<re::Error<Error>>();
}

re::expected<re::Error<Window::Error>> Window::hide() noexcept {
  if (!SDL_HideWindow(_window)) {
    return std::unexpected(re::error(Error::Hide, std::string(SDL_GetError())));
  }
  return re::expected<re::Error<Error>>();
}

SDL_Window* Window::get_raw() const noexcept {
  return _window;
}
