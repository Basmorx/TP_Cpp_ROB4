#include <gtest/gtest.h>
#include "classe.h"
#include <pinocchio/fwd.hpp>
#include <pinocchio/multibody/model.hpp>
#include <pinocchio/parsers/urdf.hpp>

// --- a. setQ() hors bornes ---
TEST(JointTest, SetQOutOfRange) {
    CJointPrismatic j(0.0, 1.0);
    EXPECT_THROW(j.setQ(1.5), std::out_of_range);
    EXPECT_THROW(j.setQ(-0.5), std::out_of_range);
}

// --- b. CJointRevolute::getTransform() ---
TEST(JointTest, RevoluteIdentity) {
    CJointRevolute j(-M_PI, M_PI, 0.0); 
    j.setQ(0.0);
    
    Mat4 res = j.getTransform();
    EXPECT_TRUE(res.isApprox(Mat4::Identity(), 1e-10));
}

// --- c. CJointPrismatic::getTransform() ---
TEST(JointTest, PrismaticTranslation) {
    CJointPrismatic j(0.0, 2.0);
    j.setQ(1.0);
    
    Mat4 res = j.getTransform();
    Mat4 expected = Mat4::Identity();
    expected(2, 3) = 1.0; 
    
    EXPECT_TRUE(res.isApprox(expected, 1e-10));
}

// --- d. Bonus Pinocchio UR5 ---
TEST(JointTest, BonusUR5) {
    pinocchio::Model model;
    std::string urdf = "/opt/openrobots/share/ur5_description/urdf/ur5_robot.urdf";
    
    if (std::ifstream(urdf)) {
        pinocchio::urdf::buildModel(urdf, model);
        
        CJointRevolute monJoint(-M_PI, M_PI, 0.0);
        double theta = M_PI / 4.0;
        monJoint.setQ(theta);
        
        pinocchio::SE3 se3_ref;
        se3_ref.rotation(Eigen::AngleAxisd(theta, Eigen::Vector3d::UnitZ()).toRotationMatrix());
        se3_ref.translation(Eigen::Vector3d::Zero());
        
        Mat4 T_ref = se3_ref.toHomogeneousMatrix();
        EXPECT_TRUE(monJoint.getTransform().isApprox(T_ref, 1e-10));
        EXPECT_TRUE(monJoint.getTransform().isApprox(T_ref, 1e-10));
    }
}