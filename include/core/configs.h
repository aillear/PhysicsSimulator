# pragma once

/**
 * @brief 
 * 
 *  each pxiel represen 1 cm.
 *  inside the system, all the longitude data will be saved and calculated as pxiel unit.
 *
 *  position unit is cm, I'll call it gm (game meter)
 *  area unit is m ^ 2.
 *  density unit is g / cm ^ 2 (it is a 2D simulator)
 *  mass unit is Kg.
 *  
 *  force unit is Kg * cm / s ^ 2, I'll call it gN (game N)
 *
 *         g
 *      ------   *   10'000 cm^2  ->   10'000 g -> 10 kg xxxxx
 *       cm^2
 */



#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_stdinc.h"
#include <glm/ext/vector_float2.hpp>
#include <numbers>
constexpr float TUAreaFactor = 0.0001f; // from cm^2 -> m^2
constexpr float TUMassFactor = 1.0f;   // density(g/cm^2) * area(m^2) = mass()
// this is using meter ^ 2
constexpr float MinBodySize = 0.01f * 0.01f;
constexpr float MaxBodySize = 64.0f * 64.0f;

constexpr float MinDensity = 0.25f;
constexpr float MaxDensity = 21.4f;

// transmit game unit to universal unit.
constexpr float TUForceFactor = 0.01f;        // 1 game N = 0.01 N
constexpr float TULongitudeFactor = 0.01f;     // 1 game m = 0.01 m

// transmit universal unit to game unit.
constexpr float TGForceFactor = 100.0f;     // 1 N = 100 game N
constexpr float TGLongitudeFactor = 100.0f; // 1 m = 100 game m

// set the value bound of iteration
constexpr int MinIteration = 1;
constexpr int MaxIteration = 64;

constexpr float MaxRadian = std::numbers::pi * 2.0f;    // 360 degree
constexpr float MinRadian = 0.0f;                       // 0 degree

constexpr glm::vec2 WorldToScreenPosFactor = {100.0f, -100.0f}; // 1 m = 100 pxiel, y is reversed
constexpr glm::vec2 ScreenToWorldPosFactor = {0.01f, -0.01f}; // 1 pxiel = 0.01 m, y is reversed    

constexpr glm::vec2 ZeroVec2 = {0.0f, 0.0f};
constexpr glm::vec2 OneVec2 = {1.0f, 1.0f};
constexpr glm::vec2 UpVec2 = {0.0f, 1.0f};
constexpr glm::vec2 DownVec2 = {0.0f, -1.0f};
constexpr glm::vec2 LeftVec2 = {-1.0f, 0.0f};
constexpr glm::vec2 RightVec2 = {1.0f, 0.0f};


// Rope color
constexpr SDL_FColor Rope_Relaxed {1.0f, 1.0f, 1.0f, 1.0f}; 
constexpr SDL_FColor Rope_Tight   {1.0f, 0.0f, 0.0f, 1.0f};  

// Link color
constexpr SDL_FColor Link_Default {0.0f, 0.0f, 0.5f, 1.0f}; 

// Cord color 
constexpr SDL_FColor Cord_Relaxed{0.0f, 1.0f, 1.0f, 1.0f};  
constexpr SDL_FColor Cord_Tight  {1.0f, 0.0f, 1.0f, 1.0f}; 

// Spring color
constexpr SDL_FColor Spring_Relaxed{0.0f, 1.0f, 0.0f, 1.0f};
constexpr SDL_FColor Spring_Tight{1.0f, 1.0f, 0.0f, 1.0f}; 


/**
 * @brief event here
 * 
 */

 // this is slow update which is triggered by every 500ms.
extern Uint32 USER_EVENT_SLOW_UPDATE;

 


class Configer {
    public:
        static Configer &Instance();
        void Init();
        void Destroy();
};

#define GET_Configer Configer::Instance()