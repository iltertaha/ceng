#pragma once

#include <vector>

namespace fst{
    class Matrix
    {
        public:
            double m_matrix[4][4];
            Matrix();
            Matrix operator*(const Matrix &rhs);
            void makeIdentity();
            void makeTranslation(double x, double y, double z);
            void makeScaling(double sx, double sy, double sz);
            void makeRotation(double angle);
            Matrix transpose();
            Matrix inverse();

    };

}