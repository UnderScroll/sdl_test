#pragma once

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_video.h>

#include <expected>
#include <rerror/error.hpp>
#include <unders_helpers/bitmasks.hpp>
#include <unders_helpers/types.hpp>

class Window {
 protected:
  SDL_Window* _window = nullptr;

  /* Constructor (Protected, use functional constructors instead) */
  Window(SDL_Window* window) : _window(window) {}

 public:
  /* Errors */
  enum class Error {
    Size,
    Creation,
    Maximization,
    Minimization,
    Fullscreen,
    Restoration,
    Show,
    Hide
  };

  // Flags
  enum class Flags : u64 {
    None = 0,
    Fullscreen = SDL_WINDOW_FULLSCREEN,
    OpenGL = SDL_WINDOW_OPENGL,
    Occluded = SDL_WINDOW_OCCLUDED,
    Hidden = SDL_WINDOW_HIDDEN,
    Borderless = SDL_WINDOW_BORDERLESS,
    Resizable = SDL_WINDOW_RESIZABLE,
    Minimized = SDL_WINDOW_MINIMIZED,
    Maximized = SDL_WINDOW_MAXIMIZED,
    Grabbed = SDL_WINDOW_MOUSE_GRABBED,
    InputFocus = SDL_WINDOW_INPUT_FOCUS,
    MouseFocus = SDL_WINDOW_MOUSE_FOCUS,
    External = SDL_WINDOW_EXTERNAL,
    Modal = SDL_WINDOW_MODAL,
    HighPixelDensity = SDL_WINDOW_HIGH_PIXEL_DENSITY,
    MouseCapture = SDL_WINDOW_MOUSE_CAPTURE,
    MouseRelativeMode = SDL_WINDOW_MOUSE_RELATIVE_MODE,
    AlwaysOnTop = SDL_WINDOW_ALWAYS_ON_TOP,
    Utility = SDL_WINDOW_UTILITY,
    Tooltip = SDL_WINDOW_TOOLTIP,
    PopupMenu = SDL_WINDOW_POPUP_MENU,
    KeyboardGrabbed = SDL_WINDOW_KEYBOARD_GRABBED,
    Vulkan = SDL_WINDOW_VULKAN,
    Metal = SDL_WINDOW_METAL,
    Transparent = SDL_WINDOW_TRANSPARENT,
    NotFocusable = SDL_WINDOW_NOT_FOCUSABLE,
  };

  /* Special Contructors */
  // No copy
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;

  // Moveable
  Window(Window&& other) noexcept
      : _window(other._window) {
    other._window = nullptr;
  }
  Window& operator=(Window&& other) noexcept {
    _window = other._window;
    other._window = nullptr;
    return *this;
  }

  /* Destructor */
  ~Window() {
    if (_window != nullptr) // Avoid moved window destruction
      SDL_DestroyWindow(_window);
  };

  /* Functional Constructors */
  [[nodiscard]]
  static std::expected<Window, re::Error<Error>> create(std::string title, u32 width, u32 height, Flags flags = Flags::None) noexcept {
    /* Guards */
    if (width >> 31)
      return std::unexpected(re::error(Error::Size, "Can't create a window with a width superior to 2^31"));
    if (height >> 31)
      return std::unexpected(re::error(Error::Size, "Can't create a window with a height superior to 2^31"));

    if (width == 0)
      return std::unexpected(re::error(Error::Size, "Can't create a window with no width"));
    if (height == 0)
      return std::unexpected(re::error(Error::Size, "Can't create a window with no height"));

    /* Creation */
    Window window = Window{SDL_CreateWindow(title.c_str(), width, height, (u64)flags)};
    if (window._window == nullptr) {
      return std::unexpected(re::error(Error::Creation, std::string(SDL_GetError())));
    }

    return window;
  }

  /* Member functions */
  [[nodiscard]] SDL_Window* get_raw() const noexcept; // Use with care
  [[nodiscard]] Flags get_flags() noexcept;
  re::expected<re::Error<Error>> set_fullscreen(bool isEnabled) noexcept;
  re::expected<re::Error<Error>> maximize() noexcept;
  re::expected<re::Error<Error>> minimize() noexcept;
  re::expected<re::Error<Error>> restore() noexcept;
  re::expected<re::Error<Error>> show() noexcept;
  re::expected<re::Error<Error>> hide() noexcept;
};

// Enable Window::Flags bitmask operations
template <>
struct enable_bitmask_operators<Window::Flags> {
  static constexpr bool enable = true;
};
