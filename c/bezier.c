#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define P_SIZE 10
#define ITER 1000

static size_t running = 1;

typedef struct Vec2 {
  float x;
  float y;
} Vec2;

const Vec2 p1 = { .x = 50.0, .y = 400.0 };
const Vec2 p2 = { .x = 200.0, .y = 50.0 };
const Vec2 p3 = { .x = 500.0, .y = 50.0 };
const Vec2 p4 = { .x = 700.0, .y = 400.0 };

struct Vec2 points[4] = { p1, p2, p3, p4 };

int selected = -1;

void poll_events() {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      running = 0;
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        for (size_t i = 0; i < sizeof(points); i++) {
          if (event.button.x >= points[i].x && event.button.x <= points[i].x + P_SIZE
              && event.button.y >= points[i].y && event.button.y <= points[i].y + P_SIZE) {
            selected = i;
          }
        }
      }
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        selected = -1;
      }
    }

    if (event.type == SDL_MOUSEMOTION) {
      Vec2 mouse_pos = { .x = event.motion.x, .y = event.motion.y};
      if (selected >= 0) {
        points[selected] = mouse_pos;
      }
    }
  }
}

Vec2 scaleVec2(Vec2 a, float p) {
  return (Vec2){ .x = a.x * p, .y = a.y * p };
}

Vec2 subVec2(Vec2 a, Vec2 b) {
  return (Vec2){ .x = a.x - b.x, .y = a.y - b.y };
}

Vec2 addVec2(Vec2 a, Vec2 b) {
  return (Vec2){ .x = a.x + b.x, .y = a.y + b.y };
}

Vec2 lerpVec2(Vec2 a, Vec2 b, float p) {
  return addVec2(a, scaleVec2(subVec2(b, a), p));
}

float s(float t) {
  return (sin(t) + 1) * .5;
}

int main(void) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("Bézier", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  while (running) {
    poll_events();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < ITER; i++) {
      Vec2 p12 = lerpVec2(points[0], points[1], s(i));
      Vec2 p23 = lerpVec2(points[1], points[2], s(i));
      Vec2 p34 = lerpVec2(points[2], points[3], s(i));
      Vec2 pA = lerpVec2(p12, p23, s(i));
      Vec2 pB = lerpVec2(p23, p34, s(i));
      Vec2 pC = lerpVec2(pA, pB, s(i));

      SDL_Rect rectC = { pC.x, pC.y, P_SIZE, P_SIZE };
      size_t r = (pC.x / WIDTH) * 255;
      size_t g = (pC.y / HEIGHT) * 255;
      size_t b = (pC.x / WIDTH) * 255;
      SDL_SetRenderDrawColor(renderer, r, g, b, 255);

      SDL_RenderFillRect(renderer, &rectC);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (size_t i = 0; i < 4; i++) {
      SDL_Rect r = { points[i].x, points[i].y, P_SIZE, P_SIZE };
      SDL_RenderFillRect(renderer, &r);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(
      renderer,
      points[0].x + P_SIZE * .5,
      points[0].y + P_SIZE * .5,
      points[1].x + P_SIZE * .5,
      points[1].y + P_SIZE * .5
    );

    SDL_RenderDrawLine(
      renderer,
      points[2].x + P_SIZE * .5,
      points[2].y + P_SIZE * .5,
      points[3].x + P_SIZE * .5,
      points[3].y + P_SIZE * .5
    );

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
