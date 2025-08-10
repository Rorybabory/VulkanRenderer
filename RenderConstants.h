#pragma once

#include <stdint.h>

const uint32_t WIDTH = 1920;
const uint32_t HEIGHT = 1080;

const int MAX_FRAMES_IN_FLIGHT = 2;

const uint32_t POOL_MAX_UBO = 128;
const uint32_t POOL_MAX_SAMPLERS = 64;
const uint32_t POOL_MAX_SETS = 128;

const uint32_t MAX_ENTITIES = 128;

const float CLEAR_COLOR[3] = { 0.95f, 0.95f, 0.95f};