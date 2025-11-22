#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <print>
#include <rerror/error.hpp>
#include <rerror/error_formatter.hpp>
#include <unders_helpers/types.hpp>

#include "core/window.hpp"
#include "game.hpp"

int main() {
  auto game = Game::create("SDL Test", 720, 480, Window::Flags::Resizable);
  if (!game) {
    std::println("{:#?}", game.error());
    return 1;
  }

  auto result = game->run();
  if (!result) {
    std::println("{:#?}", result.error());
    return 1;
  }

  return 0;
}
