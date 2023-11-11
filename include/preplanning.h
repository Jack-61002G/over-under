#ifndef PREPLANNING_H
#define PREPLANNING_H

#include "LUT.h"
#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>

template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }

class Pose {
public:
  double x;
  double y;
  double theta;
  Pose(double xe, double ye, double thetae) {
    x = xe;
    y = ye;
    theta = thetae;
  }
  std::pair<double, double> point() { return std::make_pair(x, y); }
};

class trajectory {
private:
  LUT table;
  double curvature(double t){
    double x = -3 * pow(1 - t, 2) * P1.first + 3 * pow(1 - t, 2) * P2.first -
               6 * t * (1 - t) * P1.first - 3 * pow(t, 2) * P3.first +
               6 * t * (1 - t) * P3.first + 3 * pow(t, 2) * P4.first;
    double y = -3 * pow(1 - t, 2) * P1.second + 3 * pow(1 - t, 2) * P2.second -
               6 * t * (1 - t) * P1.second - 3 * pow(t, 2) * P3.second +
               6 * t * (1 - t) * P3.second + 3 * pow(t, 2) * P4.second;
    double x2 = P1.first*(-6*t+6)+P2.first*(18*t-12)+P3.first*(-18*t+6)+P4.first*6*t;
    double y2 = P1.second*(-6*t+6)+P2.second*(18*t-12)+P3.second*(-18*t+6)+P4.second*6*t;
    x = x/12;
    y = y/12;
    x2 = x2/12;
    y2 = y2/12;
    double determinate = x*y2-y*x2;
    double magnitude = sqrt(pow(x,2)+pow(y,2));
    return determinate/pow(magnitude,3); 
  }
  double generateVelocity(double accel, double deaccel, double maxSpeed,
                          double t, double direction) {
    double curvaturer = 1/fabs(curvature(t));
    double curvatureVel = sqrt(curvaturer * accel);
    double distance = table.lookUpDistance(t) / 12;
    double distanceEnd = table.distance/12-distance;
    double accelVel = sqrt(0.1 * 0.1 + accel * 2 * distance);
    double deaccelVel = sqrt(0.1 * 0.1 + deaccel * 2 * distanceEnd);
    return std::min({accelVel, deaccelVel, maxSpeed, curvatureVel}) * direction;
  }
  double anglewrap(double target, double actual) {
    double error = target - actual;
    while (error > 180) {
      error -= 360;
    }
    while (error < -180) {
      error += 360;
    }
    return error;
  }
  double anglewrap(double target) {
    while (target > 360) {
      target -= 180;
    }
    return target;
  }
  double quadrant(double x, double y) {
    if (x >= 0) {
      if (y >= 0) {
        return 0;
      } else {
        return 180;
      }
    } else {
      if (y >= 0) {
        return 360;
      } else {
        return 180;
      }
    }
  }

public:
  std::vector<std::pair<double, double>> points;
  int robot_direction = 1;
  std::deque<double> targetLinearVelocity;
  std::deque<double> targetAngle;
  std::deque<double> targetAngularVelocity;
  std::pair<double, double> P1;
  std::pair<double, double> P2;
  std::pair<double, double> P3;
  std::pair<double, double> P4;
  Pose start;
  Pose end;
  double stretchB, stretchEND, acceleration, deacceleration, robotspeed;
  trajectory(Pose starting, Pose ending, double stretchS, double stretchE,
             double accel, double deaccel, double max_speed, double direction)
      : table(
            starting.point(),
            std::make_pair(
                stretchS * sin(((direction > 0)
                                    ? starting.theta
                                    : anglewrap(starting.theta + 180)) *
                               M_PI / 180) +
                    starting.x,
                stretchS * cos(((direction > 0)
                                    ? starting.theta
                                    : anglewrap(starting.theta + 180)) *
                               M_PI / 180) +
                    starting.y),
            std::make_pair(stretchE * sin(((direction > 0) ? ending.theta + 180
                                                           : ending.theta) *
                                          M_PI / 180) +
                               ending.x,
                           stretchE * cos(((direction > 0) ? ending.theta + 180
                                                           : ending.theta) *
                                          M_PI / 180) +
                               ending.y),
            ending.point()), start(starting.x, starting.y, starting.theta), end(ending.x, ending.y, ending.theta) {
    robot_direction = direction;
    stretchB = stretchS;
    stretchEND = stretchE;
    acceleration = accel;
    deacceleration = deaccel;
    robotspeed = max_speed;
    bool twice = false;
    std::pair<double, double> re = starting.point();
    P1 = re;
    P4 = ending.point();
    float t = 0;
    std::vector<std::pair<double, double>> path = {};
    path.push_back(re);
    std::pair<double, double> waypoint1 = std::make_pair(
        stretchS * sin(((direction > 0) ? starting.theta
                                        : anglewrap(starting.theta + 180)) *
                       M_PI / 180) +
            starting.x,
        stretchS * cos(((direction > 0) ? starting.theta
                                        : anglewrap(starting.theta + 180)) *
                       M_PI / 180) +
            starting.y);
    std::pair<double, double> waypoint2 = std::make_pair(
        stretchE * sin(((direction > 0) ? ending.theta + 180 : ending.theta) *
                       M_PI / 180) +
            ending.x,
        stretchE * cos(((direction > 0) ? ending.theta + 180 : ending.theta) *
                       M_PI / 180) +
            ending.y);
    P2 = waypoint1;
    P3 = waypoint2;
    if (direction == 1) {
      targetAngle.push_back(starting.theta);

    } else {
      targetAngle.push_back(anglewrap(starting.theta + 180));
    }
    // w = c
    while (true) {
      if (t == 1 && twice) {
        break;
      }
      double velocity =
          generateVelocity(accel, deaccel, max_speed, t, direction);
      if (t != 0) {
        if (direction > 0) {
          if (velocity - targetLinearVelocity[targetLinearVelocity.size() - 1] >
              accel * 0.01) {
            velocity = targetLinearVelocity[targetLinearVelocity.size() - 1] +
                       accel * 0.01;
          }
        } else {
          if (velocity - targetLinearVelocity[targetLinearVelocity.size() - 1] <
              -accel * 0.01) {
            velocity = targetLinearVelocity[targetLinearVelocity.size() - 1] -
                       accel * 0.01;
          }
        }
      }
      targetLinearVelocity.push_back(velocity);
      double distance = table.lookUpDistance(t) / 12 + velocity * 0.01;
      if (direction < 0) {
        distance = table.lookUpDistance(t) / -12 + velocity * 0.01;
      }
      double newT = table.lookUpT(fabs(distance) * 12);
      double x = pow(1 - t, 3) * P1.first + 3 * pow(1 - t, 2) * t * P2.first +
                 3 * (1 - t) * pow(t, 2) * P3.first + pow(t, 3) * P4.first;
      double y = pow(1 - t, 3) * P1.second + 3 * pow(1 - t, 2) * t * P2.second +
                 3 * (1 - t) * pow(t, 2) * P3.second + pow(t, 3) * P4.second;
      if (t == 1 && !twice) {
        twice = true;
        targetAngularVelocity.push_back(0);
        targetAngle.push_back(ending.theta);
      } else {
        double x1 = pow(1 - newT, 3) * P1.first +
                    3 * pow(1 - newT, 2) * newT * P2.first +
                    3 * (1 - newT) * pow(newT, 2) * P3.first +
                    pow(newT, 3) * P4.first;
        double y1 = pow(1 - newT, 3) * P1.second +
                    3 * pow(1 - newT, 2) * newT * P2.second +
                    3 * (1 - newT) * pow(newT, 2) * P3.second +
                    pow(newT, 3) * P4.second;
        double angle =
            atan((x1 - x) / (y1 - y)) + quadrant(x1 - x, y1 - y) * M_PI / 180;
        if (direction < 0) {
          angle = atan((x1 - x) / (y1 - y)) +
                  quadrant(-(x1 - x), -(y1 - y)) * M_PI / 180;
        }
        targetAngularVelocity.push_back(velocity/(1/curvature(t)));
        targetAngle.push_back(angle * 180 / M_PI);
      }
      points.push_back(std::make_pair(x, y));
      t = newT;
    }
    targetAngle.pop_front();
  }
};
#endif