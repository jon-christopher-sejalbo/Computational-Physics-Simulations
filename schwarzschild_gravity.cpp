#include <iostream>
#include <cmath>

// Note: Calculating on the equitorial plane; THETA = PI/2.

// UNITS: Standard SI

double const mass_M = 3.302e23;
double const mass_S = 1.988e30;
double const G = 6.67e-11;
double const c = 2.99e8;
double const schwarzschild_M = 2.0*G*mass_M/(c*c);
double const schwarzschild_S = 2.0*G*mass_S/(c*c);
double const perihelion_Speed = 5.8949e4;
double const perihelion_Distance = 4.6e10;
double const aphelion_Distance = 6.98e10;
double const l = 2.711e15;

// Step Variables
double const dt = 10;
double const t_tot = 10000000;
int const step = 100;

// Exaggeration Variable
double const exaggeration = 1000000;

struct Vector3 {
    double coordinates[3];

    // Default Constructor
    Vector3() {
        for(int i=0;i<3;i++) {
            this->coordinates[i] = 0;
        }
    }

    // Constructor w/ Parameters
    Vector3(double x, double y, double z) {
        this->coordinates[0] = x;
        this->coordinates[1] = y;
        this->coordinates[2] = z;
    }

    // Copy Constructor
    Vector3(const Vector3 &other) {
        for(int i=0;i<3;i++) {
            this->coordinates[i] = other.coordinates[i];
        }
    }    

    // Math Functions

    // Adds two vectors together
    Vector3 add_Vectors(const Vector3 &other) {
        Vector3 resultant = Vector3();
        for(int i=0;i<3;i++) {
            resultant.coordinates[i] = this->coordinates[i] + other.coordinates[i];
        }
        return resultant;
    }

    // Multiplies a vector by a scalar
    Vector3 multiply_Scalar(double scalar) {
        Vector3 resultant = Vector3();
        for(int i=0;i<3;i++) {
            resultant.coordinates[i] = this->coordinates[i]*scalar;
        }
        return resultant;
    }

    // Subtracts two vectors
    Vector3 subtract_Vectors(const Vector3 &other) {
        Vector3 resultant = multiply_Scalar(-1);
        resultant = resultant.add_Vectors(other);
        resultant = resultant.multiply_Scalar(-1);
        return resultant;
    }

    // Prints out in <...> notation
    void print_Vector(std::string title) {
        std::cout<<title<<": <";
        for(int i=0;i<3;i++) {
            std::cout<<this->coordinates[i];
            if(i!=2) {
                std::cout<<",";
            }
        }
        std::cout<<">"<<std::endl;
    }
};

struct Orbital_Mechanics {
    // Calculates the radial acceleration
    double calculate_Acceleration(double r) {
        return (-1.0*(((G*mass_S)/((r*r)))-((l*l)/(r*r*r))+exaggeration*((3*G*mass_S*l*l)/((c*c)*(r*r*r*r)))));
    }

    // Calculates the angular velocity
    double calculate_Velocity(double r) {
        return (l/(r*r));
    }
};

Vector3 polar_Cartesian(const Vector3 &polar) {
    Vector3 resultant = Vector3();

    resultant.coordinates[0] = polar.coordinates[0]*cos(polar.coordinates[1]);
    resultant.coordinates[1] = polar.coordinates[0]*sin(polar.coordinates[1]);
    resultant.coordinates[2] = 0;

    return resultant;
}

int main() {
    // Stepping Variable
    int k = 0;

    // Initializing Orbital_Mechanics
    Orbital_Mechanics mechanics = Orbital_Mechanics();

    // Mercury's Vectors
    Vector3 mercury_Position = Vector3(perihelion_Distance, 0, 0);
    Vector3 mercury_Velocity = Vector3(0, perihelion_Speed, 0);
    Vector3 mercury_Acceleration = Vector3(mechanics.calculate_Acceleration(perihelion_Distance), 0, 0);

    // Sun's Vectors; Will account for sun's wiggle eventually, but as of right now, seems trivial.
    Vector3 sun_Position = Vector3(0,0,0);
    Vector3 sun_Velocity = Vector3(0,0,0);
    Vector3 sun_Acceleration = Vector3(0,0,0);

    for(double t=0;t<t_tot;t+=dt,k++) {
        mercury_Velocity.coordinates[0] += mechanics.calculate_Acceleration(mercury_Position.coordinates[0])*dt;
        mercury_Position.coordinates[0] += mercury_Velocity.coordinates[0]*dt;
        mercury_Velocity.coordinates[1] = mechanics.calculate_Velocity(mercury_Position.coordinates[0]);
        mercury_Position.coordinates[1] += mercury_Velocity.coordinates[1]*dt;
        
        if(k%step==0) {
            Vector3 mercury_Cartesian = polar_Cartesian(mercury_Position);
            Vector3 sun_Cartesian = polar_Cartesian(sun_Position);
            std::cout<<mercury_Cartesian.coordinates[0]<<","<<mercury_Cartesian.coordinates[1]<<","<<sun_Cartesian.coordinates[0]<<","<<sun_Cartesian.coordinates[1]<<std::endl;
        }
    }

    return 0;
}