// TexMol Unit Tests — UsefulMath::Vector
#include <gtest/gtest.h>
#include <UsefulMath/Vector.h>
#include <cmath>

using CCVOpenGLMath::Vector;

TEST(VectorTest, DefaultConstructor) {
    Vector v;
    EXPECT_FLOAT_EQ(v[0], 0.0f);
    EXPECT_FLOAT_EQ(v[1], 0.0f);
    EXPECT_FLOAT_EQ(v[2], 0.0f);
    EXPECT_FLOAT_EQ(v[3], 0.0f);  // Tuple default is all zeros
}

TEST(VectorTest, ParameterizedConstructor) {
    Vector v(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v[0], 1.0f);
    EXPECT_FLOAT_EQ(v[1], 2.0f);
    EXPECT_FLOAT_EQ(v[2], 3.0f);
    EXPECT_FLOAT_EQ(v[3], 4.0f);
}

TEST(VectorTest, CopyConstructor) {
    Vector a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector b(a);
    EXPECT_FLOAT_EQ(b[0], 1.0f);
    EXPECT_FLOAT_EQ(b[1], 2.0f);
    EXPECT_FLOAT_EQ(b[2], 3.0f);
    EXPECT_FLOAT_EQ(b[3], 4.0f);
}

TEST(VectorTest, Assignment) {
    Vector a(1.0f, 2.0f, 3.0f, 4.0f);
    Vector b;
    b = a;
    EXPECT_FLOAT_EQ(b[0], 1.0f);
    EXPECT_FLOAT_EQ(b[1], 2.0f);
}

TEST(VectorTest, DotProduct) {
    Vector a(1.0f, 0.0f, 0.0f, 0.0f);
    Vector b(0.0f, 1.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(a.dot(b), 0.0f);

    Vector c(1.0f, 2.0f, 3.0f, 0.0f);
    Vector d(4.0f, 5.0f, 6.0f, 0.0f);
    EXPECT_FLOAT_EQ(c.dot(d), 32.0f);  // 1*4 + 2*5 + 3*6
}

TEST(VectorTest, CrossProduct) {
    Vector x(1.0f, 0.0f, 0.0f, 0.0f);
    Vector y(0.0f, 1.0f, 0.0f, 0.0f);
    Vector z = x.cross(y);
    EXPECT_FLOAT_EQ(z[0], 0.0f);
    EXPECT_FLOAT_EQ(z[1], 0.0f);
    EXPECT_FLOAT_EQ(z[2], 1.0f);
}

TEST(VectorTest, Norm) {
    Vector v(3.0f, 4.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(v.norm(), 5.0f);
}

TEST(VectorTest, Normalize) {
    Vector v(3.0f, 4.0f, 0.0f, 0.0f);
    v.normalize();
    EXPECT_NEAR(v.norm(), 1.0f, 1e-6f);
    EXPECT_NEAR(v[0], 0.6f, 1e-6f);
    EXPECT_NEAR(v[1], 0.8f, 1e-6f);
}

TEST(VectorTest, Addition) {
    Vector a(1.0f, 2.0f, 3.0f, 0.0f);
    Vector b(4.0f, 5.0f, 6.0f, 0.0f);
    Vector c = a + b;
    EXPECT_FLOAT_EQ(c[0], 5.0f);
    EXPECT_FLOAT_EQ(c[1], 7.0f);
    EXPECT_FLOAT_EQ(c[2], 9.0f);
}

TEST(VectorTest, Subtraction) {
    Vector a(4.0f, 5.0f, 6.0f, 0.0f);
    Vector b(1.0f, 2.0f, 3.0f, 0.0f);
    Vector c = a - b;
    EXPECT_FLOAT_EQ(c[0], 3.0f);
    EXPECT_FLOAT_EQ(c[1], 3.0f);
    EXPECT_FLOAT_EQ(c[2], 3.0f);
}

TEST(VectorTest, ScalarMultiply) {
    Vector v(1.0f, 2.0f, 3.0f, 0.0f);
    Vector r = v * 2.0f;
    EXPECT_FLOAT_EQ(r[0], 2.0f);
    EXPECT_FLOAT_EQ(r[1], 4.0f);
    EXPECT_FLOAT_EQ(r[2], 6.0f);
}

TEST(VectorTest, Negation) {
    Vector v(1.0f, 2.0f, 3.0f, 4.0f);
    Vector neg = -v;
    EXPECT_FLOAT_EQ(neg[0], -1.0f);
    EXPECT_FLOAT_EQ(neg[1], -2.0f);
    EXPECT_FLOAT_EQ(neg[2], -3.0f);
    EXPECT_FLOAT_EQ(neg[3], 4.0f);   // negation preserves w
}

TEST(VectorTest, Set) {
    Vector v;
    v.set(10.0f, 20.0f, 30.0f, 40.0f);
    EXPECT_FLOAT_EQ(v[0], 10.0f);
    EXPECT_FLOAT_EQ(v[1], 20.0f);
    EXPECT_FLOAT_EQ(v[2], 30.0f);
    EXPECT_FLOAT_EQ(v[3], 40.0f);
}
