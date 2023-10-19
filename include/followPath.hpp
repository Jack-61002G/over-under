#include "main.h"
namespace meow {
    class Trapevel {
    public:
        Trapevel(double max_vel, double max_acc, double distance) :
            max_vel_(max_vel), max_acc_(max_acc), distance_(distance) {}

        double getDuration() const {
            double acc_time = max_vel_ / max_acc_;
            double acc_distance = 0.5 * max_acc_ * acc_time * acc_time;
            double const_distance = distance_ - 2 * acc_distance;
            double const_time = const_distance / max_vel_;
            return 2 * acc_time + const_time;
        }

    private:
        double max_vel_;
        double max_acc_;
        double distance_;
    };
    
    void follow2d(std::vector<std::vector<double>> path) {


        
    };
}
