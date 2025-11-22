#pragma once

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_render.h>

#include <expected>
#include <rerror/error.hpp>
#include <string>

#include "core/window.hpp"

class Renderer {
 protected:
  SDL_Renderer* _renderer;

  /* Constructor (Protected, use functional constructors instead) */
  Renderer(SDL_Renderer* renderer) : _renderer(renderer) {};

 public:
  /* Errors */
  enum class Error {
    Creation,
    UnknownDriver
  };

  enum class Driver {
    Default,
    Direct3d,
    Direct3d11,
    Direct3d12,
    OpenGl,
    OpenGlEs,
    OpenGlEs2,
    Metal,
    Vulkan,
    Gpu,
    Software
  };

  /* Special constructors */
  // No copy
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  // Moveable
  Renderer(Renderer&& other) noexcept
      : _renderer(other._renderer) {
    other._renderer = nullptr;
  }
  Renderer& operator=(Renderer&& other) noexcept {
    _renderer = other._renderer;
    other._renderer = nullptr;
    return *this;
  }

  /* Destructor */
  ~Renderer() {
    if (_renderer != nullptr)
      SDL_DestroyRenderer(_renderer);
  }

  /* Functional Contructor */
  [[nodiscard]]
  static std::expected<Renderer, re::Error<Renderer::Error>> create(Window& window, Driver driver = Driver::Default) {
    // Map driver to name
    std::expected<const char*, re::Error<Error>> driver_name_result;
    if (driver_name_result = get_driver_name(driver); !driver_name_result)
      return std::unexpected(re::error(Error::Creation, "Failed to get driver name", std::move(driver_name_result.error())));

    // Create render
    SDL_Renderer* renderer = SDL_CreateRenderer(window.get_raw(), *driver_name_result);

    // Check for failure
    if (renderer == nullptr)
      return std::unexpected(re::error(Error::Creation, std::string(SDL_GetError())));

    return Renderer(renderer);
  }

  /* Member functions */
  [[nodiscard]]
  SDL_Renderer* get_raw() const;
  void clear(u8 r, u8 g, u8 b, u8 a = 255) const;
  void present() const;

 private:
  constexpr static std::expected<const char*, re::Error<Error>> get_driver_name(Driver driver) {
    using driver_type = std::underlying_type_t<Driver>;

    switch (driver) {
      case Driver::Default: return (const char*)NULL;
      case Driver::Direct3d: return "direct3d";
      case Driver::Direct3d11: return "direct3d11";
      case Driver::Direct3d12: return "direct3d12";
      case Driver::OpenGl: return "opengl";
      case Driver::OpenGlEs: return "opengles";
      case Driver::OpenGlEs2: return "opengles2";
      case Driver::Metal: return "metal";
      case Driver::Vulkan: return "vulkan";
      case Driver::Gpu: return "gpu";
      case Driver::Software: return "software";
      default:
        return std::unexpected(re::error(Error::UnknownDriver, std::format("The provided driver [{}] is not known", static_cast<driver_type>(driver))));
    }
  }
};
