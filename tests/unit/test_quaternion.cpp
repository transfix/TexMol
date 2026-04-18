// TexMol Unit Tests — UsefulMath::Quaternion
#include <gtest/gtest.h>
#include <UsefulMath/Quaternion.h>
#include <cmath>

using CCVOpenGLMath::Quaternion;
using CCVOpenGLMath::Vector;

TEST(QuaternionTest, DefaultConstructor) {
    // Layout: q[0]=w, q[1]=x, q[2]=y, q[3]=z
    Quaternion q;
    EXPECT_FLOAT_EQ(q[0], 1.0f);  // w = 1 (identity)
    EXPECT_FLOAT_EQ(q[1], 0.0f);
    EXPECT_FLOAT_EQ(q[2], 0.0f);
    EXPECT_FLOAT_EQ(q[3], 0.0f);
}

TEST(QuaternionTest, ParameterizedConstructor) {
    Quaternion q(1.0f, 0.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(q[0], 1.0f);
    EXPECT_FLOAT_EQ(q[1], 0.0f);
    EXPECT_FLOAT_EQ(q[2], 0.0f);
    EXPECT_FLOAT_EQ(q[3], 0.0f);
}

TEST(QuaternionTest, IdentityNorm) {
    // Identity quaternion (w=1, x=y=z=0)
    // norm() returns squared magnitude
    Quaternion q(1.0f, 0.0f, 0.0f, 0.0f);
    EXPECT_NEAR(q.norm(), 1.0f, 1e-6f);  // 1² = 1
}

TEST(QuaternionTest, Normalize) {
    // Note: norm() returns squared magnitude, and normalize() divides by norm()
    // so normalize() does NOT produce a unit quaternion in the traditional sense.
    // This test documents the actual behavior.
    Quaternion q(2.0f, 0.0f, 0.0f, 0.0f);
    // norm() = 4.0, after normalize: q[0] = 2/4 = 0.5
    q.normalize();
    EXPECT_NEAR(q[0], 0.5f, 1e-6f);
    EXPECT_NEAR(q[1], 0.0f, 1e-6f);
}

TEST(QuaternionTest, Conjugate) {
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion conj = q.conjugate();
    // Conjugate negates the vector part (x,y,z) but keeps w
    EXPECT_FLOAT_EQ(conj[0], q[0]);   // w unchanged
    EXPECT_FLOAT_EQ(conj[1], -q[1]);  // x negated
    EXPECT_FLOAT_EQ(conj[2], -q[2]);  // y negated
    EXPECT_FLOAT_EQ(conj[3], -q[3]);  // z negated
}

TEST(QuaternionTest, CopyConstructor) {
    Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion b(a);
    EXPECT_FLOAT_EQ(b[0], 1.0f);
    EXPECT_FLOAT_EQ(b[1], 2.0f);
    EXPECT_FLOAT_EQ(b[2], 3.0f);
    EXPECT_FLOAT_EQ(b[3], 4.0f);
}

TEST(QuaternionTest, Rotation90DegreesAroundZ) {
    // Rotate a vector (1,0,0) by 90 degrees around Z axis
    Quaternion q = Quaternion::rotation(M_PI / 2.0f, 0.0f, 0.0f, 1.0f);
    Vector v(1.0f, 0.0f, 0.0f, 0.0f);
    Vector result = q.applyRotation(v);
    EXPECT_NEAR(result[0], 0.0f, 1e-5f);
    EXPECT_NEAR(result[1], 1.0f, 1e-5f);
    EXPECT_NEAR(result[2], 0.0f, 1e-5f);
}

TEST(QuaternionTest, Rotation180DegreesAroundY) {
    Quaternion q = Quaternion::rotation(M_PI, 0.0f, 1.0f, 0.0f);
    Vector v(1.0f, 0.0f, 0.0f, 0.0f);
    Vector result = q.applyRotation(v);
    EXPECT_NEAR(result[0], -1.0f, 1e-5f);
    EXPECT_NEAR(result[1], 0.0f, 1e-5f);
    EXPECT_NEAR(result[2], 0.0f, 1e-5f);
}

TEST(QuaternionTest, MultiplyIdentity) {
    Quaternion identity(1.0f, 0.0f, 0.0f, 0.0f);
    Quaternion q(0.5f, 0.5f, 0.5f, 0.5f);
    Quaternion result = identity * q;
    EXPECT_NEAR(result[0], q[0], 1e-6f);
    EXPECT_NEAR(result[1], q[1], 1e-6f);
    EXPECT_NEAR(result[2], q[2], 1e-6f);
    EXPECT_NEAR(result[3], q[3], 1e-6f);
}

TEST(QuaternionTest, InverseMultiplyGivesIdentity) {
    Quaternion q = Quaternion::rotation(1.0f, 1.0f, 0.0f, 0.0f);
    Quaternion inv = q.inverse();
    Quaternion result = q * inv;
    // Should be identity: (1, 0, 0, 0) or close
    EXPECT_NEAR(result[0], 1.0f, 1e-5f);
    EXPECT_NEAR(result[1], 0.0f, 1e-5f);
    EXPECT_NEAR(result[2], 0.0f, 1e-5f);
    EXPECT_NEAR(result[3], 0.0f, 1e-5f);
}

TEST(QuaternionTest, ScalarMultiply) {
    Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
    Quaternion r = q * 2.0f;
    EXPECT_FLOAT_EQ(r[0], 2.0f);
    EXPECT_FLOAT_EQ(r[1], 4.0f);
    EXPECT_FLOAT_EQ(r[2], 6.0f);
    EXPECT_FLOAT_EQ(r[3], 8.0f);
}
