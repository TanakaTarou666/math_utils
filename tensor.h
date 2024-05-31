#include "matrix.h"
#ifndef __TENSOR__
#define __TENSOR__

class Tensor {
   private:
    int heights_; // 高さ（テンソルのスライス数）
    int rows_; // 行数
    int cols_; // 列数
    Matrix* matrices_; // 行列の配列

   public:
    Tensor(int heights, int rows, int cols); // コンストラクタ（高さ、行数、列数を指定）
    Tensor(int heights, int rows, int cols, double arg); // コンストラクタ（高さ、行数、列数、初期値を指定）
    Tensor(Tensor& arg);                    // コピーコンストラクタ（非const版）
    Tensor(const Tensor& arg);              // コピーコンストラクタ（const版）
    Tensor();                               // デフォルトコンストラクタ
    ~Tensor(void);                          // デストラクタ
    int heights(void) const;                // 高さを返す
    int rows(void) const;                   // 行数を返す
    int cols(void) const;                   // 列数を返す
    Matrix& operator[](int height);         // 要素アクセス演算子（非const版）
    Matrix operator[](int height) const;    // 要素アクセス演算子（const版）
    Tensor& operator=(const Tensor& arg);   // コピー代入演算子
    Tensor& operator=(Tensor&& arg);        // ムーブ代入演算子
};

Tensor operator+(Tensor& lhs, Tensor& rhs); // テンソルの加算演算子
Tensor operator-(Tensor& lhs, Tensor& rhs); // テンソルの減算演算子
double squared_sum(const Tensor& arg);      // テンソルの要素の平方和を計算する関数
double frobenius_norm(const Tensor& arg);   // フロベニウスノルムを計算する関数

#endif