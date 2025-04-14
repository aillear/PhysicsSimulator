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
 *      ------   *   10'000 cm^2  ->   10'000 g -> 10 kg
 *       cm^2
 */



constexpr float TUAreaFactor = 0.0001f; // from cm^2 -> m^2
constexpr float TUMassFactor = 10.0f;   // density(g/cm^2) * area(m^2) = mass()
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