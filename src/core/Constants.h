#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>  

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Physical Constants
const double MU_EARTH = 398600.4418;  // km^3/s^2
const double EARTH_RADIUS = 6378.137; // km

// Rendering Constants
const float SCALE = 0.001f;  // 1 unit = 1000 km

// Sun Constants
const double SUN_ANGULAR_RADIUS = 0.267 * M_PI / 180.0; // radians

// Grid Constants
const float GRID_SPACING = 5.0f;
const int GRID_LINES = 35;

// Altitude Thresholds for Orbit Classification
const double LEO_MAX_ALTITUDE = 2000.0;    // km
const double MEO_MAX_ALTITUDE = 35000.0;   // km
const double GEO_ALTITUDE = 35786.0;       // km

// Solar Panel Constants
const double SOLAR_EFFICIENCY_PENUMBRA = 0.5;
const double SOLAR_EFFICIENCY_HIGH_BETA = 0.7;
const double HIGH_BETA_THRESHOLD = 75.0;

// Camera Constants
const float CAMERA_TRANSITION_SPEED = 5.0f;
const float CAMERA_MIN_DISTANCE = 10.0f;
const float CAMERA_MAX_DISTANCE = 200.0f;

// Animation Constants
const float MAX_ANIMATION_SPEED = 10.0f;
const float MIN_ANIMATION_SPEED = 0.05f;

// Satellite Trail
const size_t TRAIL_LENGTH = 25;

#endif // CONSTANTS_H