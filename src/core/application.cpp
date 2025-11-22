#include "core/application.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>

#include <chrono>
#include <ratio>
#include <rerror/error.hpp>

[[nodiscard]]
auto Application::run() -> re::expected<re::AnyError> {
  if (!setup()) [[unlikely]]
    return std::unexpected(setup().error());

  auto start_time = std::chrono::high_resolution_clock().now();

  while (_shouldContinue) {
    /* Compute delta_time */
    auto end_time = std::chrono::high_resolution_clock().now();
    auto delta_time = std::chrono::duration<double, std::milli>(end_time - start_time).count(); // In ms
    start_time = std::chrono::high_resolution_clock().now();

    /* Input handling */
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) [[unlikely]]
        _shouldContinue = false;

      if (auto input_result = input(event); !input_result) [[unlikely]]
        return input_result;
    }

    /* Update state */
    if (auto update_result = update(delta_time); !update_result) [[unlikely]]
      return update_result;

    /* Draw current state */
    if (auto draw_result = draw(); !draw_result) [[unlikely]]
      return draw_result;

    _renderer.present();
  }

  return re::expected<re::AnyError>();
}
