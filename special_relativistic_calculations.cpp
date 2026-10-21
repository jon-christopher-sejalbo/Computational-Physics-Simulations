#include <iostream>
#include <cmath>

// For now, let's use the 1D transform to keep things simple

// Keep in GR Units
const double t_tot = 1000;
const double dt = 0.1;

/* 
What do I need: Spacetime interval calculation, matrix multiplication for lorentz transformation
    How I'm going to do it: Instead of single coordinate variables, I should use arrays and 2D arrays, spacetime interval calculation is easy.
*/

struct Vector4 {
    double speed;
    double coordinates[4];

    Vector4() {
        this->speed = 0;

        for(int i=0;i<4;i++) {
            this->coordinates[i] = 0;
        }
    }

    Vector4(double speed, const double coordinates[4]) {
        this->speed = speed;

        for (int i=0;i<4;i++) {
            this->coordinates[i] = coordinates[i];
        }
    }

    Vector4(const Vector4 &other) {
        this->speed = other.speed;

        for (int i=0;i<4;i++) {
            this->coordinates[i] = other.coordinates[i];
        }
    }

    double add_Speed(double speed) {
        return (this->speed + speed)/(1.0 + ((this->speed)*(speed)));
    }

    void move_Forward(double t, int direction) {
        this->coordinates[0] -= t;
        this->coordinates[direction] = this->coordinates[direction] + speed*t;
    }

    void print_pos(std::string title) {
        std::cout<<title<<": "<<"<";
        for(int i=0;i<4;i++) {
            std::cout<<this->coordinates[i];
            if(i!=3) {
                std::cout<<",";
            }
        }
        std::cout<<">"<<std::endl;
    }
};

// 1D Lorentz Boost

struct Lorentz_Transformation {
    double boost;
    double lorentz_Factor;
    double Lorentz_Matrix[4][4];

    double calculate_Lorentz_Factor() {
        if(boost>=1) {
            std::cout<<"ERROR: Null/Spacelike Interval."<<std::endl;
            return 0;
        }
        return (1)/pow((1-boost*boost), 0.5);
    }

    Lorentz_Transformation(double boost) {
        this->boost = boost;
        this->lorentz_Factor = calculate_Lorentz_Factor();

        for(int i=0;i<4;i++) {
            for(int j=0;j<4;j++) {
                Lorentz_Matrix[i][j] = 0;
            }
        }

        Lorentz_Matrix[0][0] = this->lorentz_Factor;
        Lorentz_Matrix[1][0] = -1.0*this->lorentz_Factor*this->boost;
        Lorentz_Matrix[1][1] = this->lorentz_Factor;
        Lorentz_Matrix[0][1] = -1.0*this->lorentz_Factor*this->boost;
        Lorentz_Matrix[2][2] = 1.0;
        Lorentz_Matrix[3][3] = 1.0;
    }

    Vector4 transform_Vector(const Vector4 &boosted_vector) {
        Vector4 resultant = boosted_vector;
        for(int i=0;i<4;i++) {
            double result = 0;
            for(int j=0;j<4;j++) {
                result+=Lorentz_Matrix[i][j]*boosted_vector.coordinates[j];
            }
            resultant.coordinates[i] = result;
        }
        return resultant;
    }
};

// Metric Struct
struct Minkowski_Metric {
    double min_Metric[4][4];

    Minkowski_Metric() {
        for(int i=0;i<4;i++) {
            for(int j=0;j<4;j++) {
                min_Metric[i][j] = 0;
            }
        }

        min_Metric[0][0] = -1.0;
        min_Metric[1][1] = 1.0;
        min_Metric[2][2] = 1.0;
        min_Metric[3][3] = 1.0;
    }

    // Preconditions: Must be an contravariant vector.
    Vector4 contract_Vector(Vector4 &position_vector) {
        Vector4 resultant;
        for(int i=0;i<4;i++) {
            double result = 0;
            for(int j=0;j<4;j++) {
                result += min_Metric[i][j] * position_vector.coordinates[j];
            }
            resultant.coordinates[i] = result;
        }
        return resultant;
    }

    // Preconditions: Must be a position vector; will add contraction compatibility with other vectors later.
    double calculate_interval(const Vector4 &position_vector) {
        Vector4 contracted_vector = position_vector;
        contracted_vector = contract_Vector(contracted_vector);

        double result = 0;
        for(int i=0;i<4;i++) {
            result += position_vector.coordinates[i]*contracted_vector.coordinates[i];
        }

        return pow(-1.0*result, 0.5);
    }
};

int main() {
    // Will test out later...
    double acceleration = 0;

    // Boost value
    double boost = 0.5;

    // Initializing Minkowski Metric
    Minkowski_Metric metric;

    // Particle positions as observed by a stationary frame
    double particle_pos[4] = {0,0,0,0};
    Vector4 moving_particle_pos(boost,particle_pos);

    for(double t=0;t<t_tot;t+=dt) {
        moving_particle_pos.move_Forward(dt, 1);
    }

    moving_particle_pos.print_pos("PARTICLE POSITION ACCORDING TO OBSERVER");

    // Setting up Lorentz Transformation
    Lorentz_Transformation observer_particle(boost);
    Vector4 lorentz_particle_pos = observer_particle.transform_Vector(moving_particle_pos);

    lorentz_particle_pos.print_pos("PARTICLE POSITION ACCORDING TO PARTICLE");

    // Proving invariance of the interval
    double unprimed_interval = metric.calculate_interval(moving_particle_pos);
    double primed_interval = metric.calculate_interval(lorentz_particle_pos);

    std::cout<<"PARTICLE INTERVAL ACCORDING TO OBSERVER: "<<unprimed_interval<<std::endl;
    std::cout<<"PARTICLE INTERVAL ACCORDING TO PARTICLE: "<<primed_interval<<std::endl;

    return 0;
}