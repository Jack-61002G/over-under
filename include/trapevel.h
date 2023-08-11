#pragma once
#ifndef TRAPEVEL_H
#define TRAPEVEL_H

class trapevel{
    
    private:
        double k_UP, k_DOWN, k_VEL, kP;
        double prevVelocity = 0; 
    
    public:
        trapevel(double k_UP = 0, double k_DOWN = 0, double k_VEL = 0, double kp = 0);

        double update(double velocity, double actual);
        
        void reset();

};



#endif