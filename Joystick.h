#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "mbed.h"

// this value can be tuned to alter tolerance of joystick movement
#define TOLERANCE 0.05f
#define RAD2DEG 57.2957795131f

enum Direction {
    CENTER,  // 0
    N,       // 1
    NE,      // 2
    E,       // 3
    SE,      // 4
    S,       // 5
    SW,      // 6
    W,       // 7
    NW       // 8
};

struct Vector2D {
    float x;
    float y;
};

struct Polar {
    float mag;
    float angle;
};

/** Joystick Class
@author Dr Craig A. Evans, University of Leeds
@brief  Library for interfacing with analogue joystick

Example:

@code

#include "Joystick.h"
#include "mbed.h"

//                y      x
Joystick joystick(PTB10, PTB11);

int main() {
    joystick.init();

    while(1) {
        Vector2D coord = joystick.get_coord();
        printf("Coord = %f,%f\n", coord.x, coord.y);

        Vector2D mapped_coord = joystick.get_mapped_coord();
        printf("Mapped coord = %f,%f\n", mapped_coord.x, mapped_coord.y);

        float mag = joystick.get_mag();
        float angle = joystick.get_angle();
        printf("Mag = %f Angle = %f\n", mag, angle);

        Direction d = joystick.get_direction();
        printf("Direction = %i\n",d);

        wait(0.5);
    }
}

* @endcode
*/
class Joystick {
   public:
    //               y-pot            x-pot
    Joystick(PinName vertPin, PinName horizPin);

    // needs to be called at start with joystick centered
    void init(float x_offset=0.0f, float y_offset=0.0f);
    float get_mag();                // polar
    float get_angle();              // polar
    Vector2D get_coord();           // cartesian co-ordinates x,y
    Vector2D get_mapped_coord();    // x,y mapped to circle
    Direction get_direction();      // N,NE,E,SE etc.
    Polar get_polar();              // mag and angle in struct form

   private:
    AnalogIn* vert;
    AnalogIn* horiz;

    // centered x,y values
    float _x0;
    float _y0;
};

#endif
