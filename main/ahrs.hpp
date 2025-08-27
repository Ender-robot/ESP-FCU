#ifndef AHRS_HPP
#define AHRS_HPP

#include <cmath>
#include "esp_log.h"
#include "struct.hpp"

// 可选的位姿估计算法
struct CF{}; // 互补滤波
struct MadgwickFilter{}; // 基于梯度下降的优化滤波器

/**
 * @brief 用于姿态角估计的类
 * 
 * @param gyroBias 陀螺仪零偏数据（可选）
 * @param accelBias 加速度计零偏数据（可选）
 * @param accelGain 加速度计增益数据（可选）
 */
class AHRS {
    public:
        AHRS(const Vec3f gyroBias = {}, const Vec3f accelBias = {}, const Vec3f accelGain = {});
        ~AHRS();

        Vec3f attiEst(const Vec3f& gyroData, const Vec3f& accelData, float dt, CF); // 互补滤波
        Vec3f attiEst(const Vec3f& gyroData, const Vec3f& accelData, float dt, MadgwickFilter); // 基于梯度下降的优化滤波器
    private:
        Vec3f gyroBias, accelBias, accelGain; // 传感器校准数据
        Vec3f lastAtti; // 姿态角数据
};

#endif
