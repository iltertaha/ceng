#include "matrix.h"
#include <iostream>
#include <math.h>
#include "matrixInverse.h"

#define PI 3.14159265

namespace fst
{
    Matrix::Matrix()
    {
        for(int i=0; i < 4; i++){
            m_matrix[0][i] = 0;
            m_matrix[1][i] = 0;  
            m_matrix[2][i] = 0;  
            m_matrix[3][i] = 0;  
        }
    }

    void Matrix::makeIdentity(){
        m_matrix[0][0] = 1.0;
        m_matrix[1][1] = 1.0;
        m_matrix[2][2] = 1.0;
        m_matrix[3][3] = 1.0;
    }

    void Matrix::makeTranslation(double x, double y, double z){
        this->makeIdentity();
        m_matrix[0][3] = x;
        m_matrix[1][3] = y;
        m_matrix[2][3] = z;
    }

    void Matrix::makeScaling(double sx, double sy, double sz){
        m_matrix[0][0] = sx;
        m_matrix[1][1] = sy;
        m_matrix[2][2] = sz;
        m_matrix[3][3] = 1;
    }

    void Matrix::makeRotation(double angle){
        makeIdentity();
        m_matrix[1][1] = cos(angle* PI / 180.0);
        m_matrix[2][2] = cos(angle* PI / 180.0);
        m_matrix[1][2] = -sin(angle * PI / 180.0);
        m_matrix[2][1] = sin(angle * PI / 180.0);
        return;
    }

    Matrix Matrix::operator*(const Matrix &rhs){
        
        double temp;
        Matrix res;

        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                temp = 0;
                for(int m=0; m<4; m++){
                    temp +=  this->m_matrix[i][m] * rhs.m_matrix[m][j];
                }
                res.m_matrix[i][j] = temp;
                
            }

        }
        return res;
    }

    Matrix Matrix::inverse(){
        double temp[16];
        double inv[16];
        Matrix result = Matrix();

        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                temp[i*4+j] = m_matrix[i][j];
            }
        }

        invert(temp, inv);

        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                result.m_matrix[i][j] = inv[i*4+j];
            }
        }
        return result;
    }

    Matrix Matrix::transpose(){
        Matrix result = Matrix();

        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                m_matrix[i][j] = result.m_matrix[j][i];
            }
        }
        return result;
    }
}