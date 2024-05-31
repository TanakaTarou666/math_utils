#include "vector.h"

#ifndef __MATRIX__
#define __MATRIX__

class Matrix {
   private:
    int rows_;       // 行数
    int cols_;       // 列数
    double *values_; // 値を保持する配列

   public:
    Matrix(int rows, int cols);            // 行数と列数を指定するコンストラクタ
    Matrix(int rows, int cols, double arg); // 行数、列数、および初期値を指定するコンストラクタ
    Matrix(void);                           // デフォルトコンストラクタ
    Matrix(const Matrix &arg);              // コピーコンストラクタ
    Matrix &operator=(const Matrix &rhs);   // 代入演算子
    ~Matrix(void);                          // デストラクタ
    int rows(void) const;                   // 行数を取得するメソッド
    int cols(void) const;                   // 列数を取得するメソッド
    double &operator()(int row, int col);   // 要素にアクセスする演算子（非const版）
    double operator()(int row, int col) const; // 要素にアクセスする演算子（const版）
    Vector operator[](int row);             // 行にアクセスする演算子
    Matrix operator+(void) const;           // 単項プラス演算子
    Matrix operator-(void) const;           // 単項マイナス演算子
    Matrix &operator+=(const Matrix &rhs);  // 加算代入演算子
    Matrix &operator-=(const Matrix &rhs);  // 減算代入演算子
    std::ostream &print(std::ostream &lhs) const; // 行列を出力するメソッド
    double *get_values();                   // データへのポインタを取得するメソッド
};

// 非メンバー関数の宣言
std::ostream &operator<<(std::ostream &lhs, const Matrix &rhs); // 行列の出力演算子
Matrix operator+(const Matrix &lhs, const Matrix &rhs); // 行列の加算演算子
Matrix operator-(const Matrix &lhs, const Matrix &rhs); // 行列の減算演算子
Vector operator*(const Matrix &lhs, const Vector &rhs); // 行列とベクトルの乗算演算子
Matrix operator*(Matrix &lhs, Matrix &rhs);             // 行列同士の乗算演算子
bool operator==(const Matrix &lhs, const Matrix &rhs);  // 行列の等価比較演算子
bool operator!=(const Matrix &lhs, const Matrix &rhs);  // 行列の不等価比較演算子
Matrix operator*(double factor, const Matrix &rhs);     // スカラー倍の行列演算子
double squared_sum(const Matrix &arg);                  // 行列の平方和を計算する関数
double frobenius_norm(const Matrix &arg);               // フロベニウスノルムを計算する関数
Matrix transpose(Matrix &arg);                          // 行列の転置を計算する関数

#endif