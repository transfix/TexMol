// TexMol Unit Tests — UsefulMath::Matrix
#include <gtest/gtest.h>
#include <UsefulMath/Matrix.h>
#include <UsefulMath/Vector.h>
#include <cmath>

using CCVOpenGLMath::Matrix;
using CCVOpenGLMath::Vector;

TEST(MatrixTest, DefaultIsIdentity) {
    Matrix m;
    EXPECT_FLOAT_EQ(m.get(0, 0), 1.0f);
    EXPECT_FLOAT_EQ(m.get(1, 1), 1.0f);
    EXPECT_FLOAT_EQ(m.get(2, 2), 1.0f);
    EXPECT_FLOAT_EQ(m.get(3, 3), 1.0f);
    EXPECT_FLOAT_EQ(m.get(0, 1), 0.0f);
    EXPECT_FLOAT_EQ(m.get(1, 0), 0.0f);
}

TEST(MatrixTest, Reset) {
    Matrix m(
        2, 0, 0, 0,
        0, 3, 0, 0,
        0, 0, 4, 0,
        0, 0, 0, 5
    );
    m.reset();
    EXPECT_FLOAT_EQ(m.get(0, 0), 1.0f);
    EXPECT_FLOAT_EQ(m.get(1, 1), 1.0f);
    EXPECT_FLOAT_EQ(m.get(2, 2), 1.0f);
    EXPECT_FLOAT_EQ(m.get(3, 3), 1.0f);
}

TEST(MatrixTest, IdentityDeterminant) {
    Matrix m;
    EXPECT_NEAR(m.determinant(), 1.0f, 1e-6f);
}

TEST(MatrixTest, ScaleDeterminant) {
    Matrix m = Matrix::scale(2.0f, 3.0f, 4.0f);
    EXPECT_NEAR(m.determinant(), 24.0f, 1e-5f);
}

TEST(MatrixTest, IdentityTimesVector) {
    Matrix m;
    Vector v(1.0f, 2.0f, 3.0f, 1.0f);
    Vector r = m * v;
    EXPECT_FLOAT_EQ(r[0], 1.0f);
    EXPECT_FLOAT_EQ(r[1], 2.0f);
    EXPECT_FLOAT_EQ(r[2], 3.0f);
    EXPECT_FLOAT_EQ(r[3], 1.0f);
}

TEST(MatrixTest, TranslationTimesPoint) {
    Matrix m = Matrix::translation(10.0f, 20.0f, 30.0f);
    Vector v(1.0f, 2.0f, 3.0f, 1.0f);
    Vector r = m * v;
    EXPECT_FLOAT_EQ(r[0], 11.0f);
    EXPECT_FLOAT_EQ(r[1], 22.0f);
    EXPECT_FLOAT_EQ(r[2], 33.0f);
}

TEST(MatrixTest, ScaleTimesVector) {
    Matrix m = Matrix::scale(2.0f, 3.0f, 4.0f);
    Vector v(1.0f, 1.0f, 1.0f, 1.0f);
    Vector r = m * v;
    EXPECT_FLOAT_EQ(r[0], 2.0f);
    EXPECT_FLOAT_EQ(r[1], 3.0f);
    EXPECT_FLOAT_EQ(r[2], 4.0f);
}

TEST(MatrixTest, Transpose) {
    Matrix m(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Matrix t = m.transpose();
    EXPECT_FLOAT_EQ(t.get(0, 1), m.get(1, 0));
    EXPECT_FLOAT_EQ(t.get(1, 0), m.get(0, 1));
    EXPECT_FLOAT_EQ(t.get(2, 3), m.get(3, 2));
}

TEST(MatrixTest, InverseOfIdentity) {
    Matrix m;
    Matrix inv = m.inverse();
    EXPECT_NEAR(inv.get(0, 0), 1.0f, 1e-6f);
    EXPECT_NEAR(inv.get(1, 1), 1.0f, 1e-6f);
    EXPECT_NEAR(inv.get(0, 1), 0.0f, 1e-6f);
}

TEST(MatrixTest, InverseOfTranslation) {
    Matrix m = Matrix::translation(5.0f, 10.0f, 15.0f);
    Matrix inv = m.inverse();
    Matrix product = m * inv;
    // Should be identity
    EXPECT_NEAR(product.get(0, 0), 1.0f, 1e-5f);
    EXPECT_NEAR(product.get(0, 3), 0.0f, 1e-5f);
    EXPECT_NEAR(product.get(1, 1), 1.0f, 1e-5f);
    EXPECT_NEAR(product.get(2, 2), 1.0f, 1e-5f);
    EXPECT_NEAR(product.get(3, 3), 1.0f, 1e-5f);
}

TEST(MatrixTest, RotationZ90) {
    Matrix m = Matrix::rotationZ(M_PI / 2.0f);
    Vector v(1.0f, 0.0f, 0.0f, 1.0f);
    Vector r = m * v;
    EXPECT_NEAR(r[0], 0.0f, 1e-5f);
    EXPECT_NEAR(r[1], -1.0f, 1e-5f);  // rotationZ uses clockwise convention
    EXPECT_NEAR(r[2], 0.0f, 1e-5f);
}

TEST(MatrixTest, MatrixMultiplication) {
    Matrix a = Matrix::scale(2.0f, 2.0f, 2.0f);
    Matrix b = Matrix::translation(1.0f, 0.0f, 0.0f);
    // scale * translate: first translate, then scale
    Matrix c = a * b;
    Vector v(0.0f, 0.0f, 0.0f, 1.0f);
    Vector r = c * v;
    EXPECT_NEAR(r[0], 2.0f, 1e-5f);  // translated to 1, then scaled to 2
}

TEST(MatrixTest, CopyConstructor) {
    Matrix a(
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    );
    Matrix b(a);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            EXPECT_FLOAT_EQ(a.get(i, j), b.get(i, j));
}
