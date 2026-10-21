#include <iostream>
#include <cmath>

const int time_interval = 100;

// standard SI units
const double dt = 0.01;
const double G = 6.67e-11;
const double planet_mass = 2.0e30;
const double planet_radius = 7.0e3;
const double particle_mass = 2.0e15;
const double particle_radius = 2.0e3;
const double t_tot = 500;
/* const double MAX_VELOCITY = 3.4e7;
const double MAX_ACCELERATION = 1e7;
const double MAX_PLANET_VELOCITY = 10e3;
const double MAX_PLANET_ACCELERATION = 10e3; */

// 3D vector struct
struct Vector3 {
    double x;
    double y;
    double z;

    Vector3(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3(const Vector3 &other) {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }

    double magnitude() {
        return pow(this->x * this->x + this->y * this->y + this->z * this->z, 0.5);
    }

    void mult(double scalar) {
        this->x*=scalar;
        this->y*=scalar;
        this->z*=scalar;
    }

    Vector3 add(Vector3 other) {
        double x = this->x + other.x;
        double y = this->y + other.y;
        double z = this->z + other.z;

        Vector3 resultant(x,y,z);
        return resultant;
    }

    Vector3 sub(Vector3 other) {
        double x = this->x - other.x;
        double y = this->y - other.y;
        double z = this->z - other.z;

        Vector3 resultant(x,y,z);
        return resultant;
    }

    Vector3 unit() {
        double x = this->x/magnitude();
        double y = this->y/magnitude();
        double z = this->z/magnitude();

        Vector3 resultant(x,y,z);
        return resultant;
    }

    bool check_equality(const Vector3 &other) {
        Vector3 radius(this->x,this->y,this->z);
        radius = radius.sub(other);
        if(radius.magnitude()<planet_radius+particle_radius) {
            return true;
        }
        return false;
    }

    void print(std::string name) {
        std::cout<<name<<": "<<"<"<<x<<","<<y<<","<<z<<">"<<std::endl;
    }
};

void change_Gravitational_Acceleration(const Vector3 &mass_eff, const Vector3 &mass_applying, Vector3 &mass_eff_accel, double applying_mass) {
    Vector3 radius = mass_eff;
    radius = radius.sub(mass_applying);
    Vector3 accel = radius.unit();
    double magnitude_rad = radius.magnitude();

    double accel_mag = -(G*applying_mass/(magnitude_rad*magnitude_rad));
    accel.mult(accel_mag);
    mass_eff_accel = accel;
}

void change_Velocity(Vector3 &mass_vel, Vector3 &mass_accel, double t) {
    Vector3 eff_accel = mass_accel;
    eff_accel.mult(t);
    mass_vel = mass_vel.add(eff_accel);
}

void change_Position(Vector3 &mass_pos, Vector3 &mass_vel, Vector3 &mass_accel, double t) {
    Vector3 eff_accel = mass_accel;
    eff_accel.mult((1.0/2.0)*t*t);
    Vector3 eff_vel = mass_vel;
    eff_vel.mult(t);

    mass_pos = mass_pos.add(eff_vel.add(eff_accel));
}

// NOTE: Make sure to check boundary case s.t. masses collide.

int main() {
    // step amt counter
    int step = 0;

    //planet vectors
    Vector3 planet_position(0, 0, 0);
    Vector3 planet_velocity(9.43044e-10, 0, 0);
    Vector3 planet_acceleration(0, 0, 0);

    //particle vectors
    Vector3 particle_position(0, 0, 0.75e8);
    Vector3 particle_velocity(1.333666e6, 0, 0);
    Vector3 particle_acceleration(0, 0, 0);

    for(double t=0;t<t_tot;t+=dt) {
        change_Gravitational_Acceleration(particle_position, planet_position, particle_acceleration, planet_mass);
        change_Gravitational_Acceleration(planet_position, particle_position, planet_acceleration, particle_mass);
        change_Velocity(particle_velocity, particle_acceleration, dt);
        change_Velocity(planet_velocity, planet_acceleration, dt);

        /* if(particle_velocity.magnitude()>MAX_VELOCITY) {
            Vector3 new_velocity = particle_velocity.unit();
            new_velocity.mult(MAX_VELOCITY);
            particle_velocity = new_velocity;
        }

        if(particle_acceleration.magnitude()>MAX_ACCELERATION) {
            Vector3 new_acceleration = particle_acceleration.unit();
            new_acceleration.mult(MAX_ACCELERATION);
            particle_acceleration = new_acceleration;
        }

        if(planet_velocity.magnitude()>MAX_PLANET_VELOCITY) {
            Vector3 new_velocity = planet_velocity.unit();
            new_velocity.mult(MAX_PLANET_VELOCITY);
            planet_velocity = new_velocity;
        }

        if(planet_acceleration.magnitude()>MAX_PLANET_ACCELERATION) {
            Vector3 new_acceleration = planet_acceleration.unit();
            new_acceleration.mult(MAX_PLANET_ACCELERATION);
            planet_acceleration = new_acceleration;
        } */

        change_Position(particle_position, particle_velocity, particle_acceleration, dt);
        change_Position(planet_position, planet_velocity, planet_acceleration, dt);

        if(step%time_interval==0) {
            std::cout<<particle_position.x<<","<<particle_position.z<<","<<planet_position.x<<","<<planet_position.z<<"\n";
        }

        step++;
    }

    return 0;
}