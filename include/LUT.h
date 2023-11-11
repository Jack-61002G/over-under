#ifndef LUT_H
#define LUT_H

#include <vector>
#include <cmath>


class slope{
    private:
        void slopeCalculator(std::pair<double, double> point1,std::pair<double, double> point2){
            slope1 = (point2.second-point1.second)/(point2.first-point1.first);
            slope2 = 1/slope1;
            intercept1 = point1.second - slope1*point1.first;
            intercept2 = point2.first - slope2*point2.second;
        }
    public:
        double slope1, slope2, intercept1, intercept2;
        slope(std::pair<double, double> point1, std::pair<double, double> point2){
            slopeCalculator(point1, point2);
        }
        double point1(double x){
            return (slope1*x)+intercept1;
        }
        double point2(double x){
            return (slope2*x)+intercept2;
        }
};

// binary search algorithm, given a sorted list and number
int closestBinary(std::vector<double> list, double number);

// a look up table of bezier curves and get return new information
class LUT{
  private:
    std::vector<double> lt;
    std::vector<double> distances;
    std::vector<slope> slopes;
    double prevX, prevY;
    double t = 0;
  public:
    double distance = 0;
    LUT(std::pair<double, double> p1,std::pair<double, double> p2,std::pair<double, double> p3,std::pair<double, double> p4){
        prevX = p1.first;
        prevY = p1.second;
        for(int i=0; i<=249; i++){
            double x = pow(1 - t, 3) * p1.first +
               3 * pow(1 - t, 2) * t * p2.first +
               3 * (1 - t) * pow(t, 2) * p3.first +
               pow(t, 3) * p4.first;
            double y = pow(1 - t, 3) * p1.second +
               3 * pow(1 - t, 2) * t * p2.second +
               3 * (1 - t) * pow(t, 2) * p3.second +
               pow(t, 3) * p4.second;
            distance += sqrt(pow(x-prevX, 2)+pow(y-prevY,2));
            distances.push_back(distance);
            lt.push_back(t);
            t+=0.004;
            prevX = x;
            prevY = y;
        }
        for(int i=0; i<distances.size()-1; i++){
            std::pair<double, double> point1 = std::make_pair(lt[i], distances[i]);
            std::pair<double, double> point2 = std::make_pair(lt[i+1], distances[i+1]);
            slope sloped(point1, point2);
            slopes.push_back(sloped);
        }
        slopes.push_back(slope(std::make_pair(lt[lt.size()-1],distances[distances.size()-1]), std::make_pair(lt[lt.size()-2],distances[distances.size()-2])));
    }
    double lookUpDistance(double t){
        if(t>=1){      
            return distance;
        }
        return slopes[closestBinary(lt, t)].point1(t);
    }
    double lookUpDistanceToEnd(double t){
        if(t>=1){
            return 0.00;
        }
        return distance-slopes[closestBinary(lt, t)].point1(t);
    }
    double lookUpT(double distanceF){
        if(distanceF>distance){
            return 1.00;
        }
        return slopes[closestBinary(distances, distanceF)].point2(distanceF);
    }
};
#endif
