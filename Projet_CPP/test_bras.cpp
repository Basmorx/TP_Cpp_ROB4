#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "CBras.h"

// 3.a. Bras vide retourne I4
TEST(BrasTest, EmptyFKReturnsIdentity) { // [cite: 79]
    CBras bras;
    Mat4 T = bras.computeFK();
    Mat4 I = Mat4::Identity(); // [cite: 79]
    
    EXPECT_TRUE(T.isApprox(I, 1e-10));
}

// 3.b. 1 joint rotatif, extraction translation
TEST(BrasTest, RevoluteFKTranslationExtraction) { // [cite: 80]
    CBras bras;
    // theta = 0, dx = 0.5m
    bras.addJoint(std::make_unique<CJointRevolute>(-M_PI, M_PI, 0.5)); // [cite: 80]
    bras.getJoint(0)->setQ(0.0); // [cite: 80]
    
    Mat4 T = bras.computeFK();
    
    // Extraction avec T.block<3, 1>(0,3)
    Eigen::Vector3d translation = T.block<3, 1>(0, 3); // [cite: 80]
    Eigen::Vector3d expected(0.5, 0.0, 0.0); // [cite: 80]
    
    EXPECT_TRUE(translation.isApprox(expected, 1e-10));
}

// 3.c. getJoint lève une exception
TEST(BrasTest, GetJointOutOfBoundsThrows) { // [cite: 81]
    CBras bras;
    bras.addJoint(std::make_unique<CJointRevolute>(-M_PI, M_PI, 0.5));
    
    EXPECT_NO_THROW(bras.getJoint(0));
    EXPECT_THROW(bras.getJoint(1), std::out_of_range); // [cite: 81]
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}