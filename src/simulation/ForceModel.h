#ifndef FORCE_MODEL_H
#define FORCE_MODEL_H

// Force model options
struct ForceModel {
    bool pointMass;           // Central body gravity (always on)
    bool j2Perturbation;      // Earth oblateness
    bool j3Perturbation;      // Pear-shaped Earth (future)
    bool j4Perturbation;      // Higher-order (future)
    bool atmosphericDrag;     // Drag model (future)
    bool solarRadiation;      // SRP (future)
    bool thirdBodyMoon;       // Lunar gravity (future)
    bool thirdBodySun;        // Solar gravity (future)
    
    ForceModel() 
        : pointMass(true),
          j2Perturbation(false),
          j3Perturbation(false),
          j4Perturbation(false),
          atmosphericDrag(false),
          solarRadiation(false),
          thirdBodyMoon(false),
          thirdBodySun(false) {}
};

#endif // FORCE_MODEL_H