#include "tensor.h"
#include <cmath>
#include <iostream>

// コンストラクタ（高さ、行数、列数を指定）
Tensor::Tensor(int heights, int rows, int cols) : heights_(heights), rows_(rows), cols_(cols) {
    matrices_ = new Matrix[heights];
    for (int i = 0; i < heights; i++) {
        matrices_[i] = Matrix(rows, cols);
    }
}

// コンストラクタ（高さ、行数、列数、初期値を指定）
Tensor::Tensor(int heights, int rows, int cols, double arg) : heights_(heights), rows_(rows), cols_(cols) {
    matrices_ = new Matrix[heights];
    for (int i = 0; i < heights; i++) {
        matrices_[i] = Matrix(rows, cols, arg);
    }
}

// コピーコンストラクタ（const版）
Tensor::Tensor(const Tensor& arg) : heights_(arg.heights_), rows_(arg.rows_), cols_(arg.cols_) {
    matrices_ = new Matrix[heights_];
    for (int h = 0; h < heights_; ++h) {
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                (*this)[h](i, j) = arg[h](i, j);
            }
        }
    }
}

// コピーコンストラクタ（非const版）
Tensor::Tensor(Tensor& arg) : heights_(arg.heights_), rows_(arg.rows_), cols_(arg.cols_) {
    matrices_ = new Matrix[heights_];
    for (int h = 0; h < heights_; ++h) {
        for (int i = 0; i < rows_; ++i) {
            for (int j = 0; j < cols_; ++j) {
                (*this)[h](i, j) = arg[h](i, j);
            }
        }
    }
}

// デフォルトコンストラクタ
Tensor::Tensor() { matrices_ = nullptr; }

// デストラクタ
Tensor::~Tensor(void) { delete[] matrices_; }

// 高さを返す
int Tensor::heights(void) const { return heights_; }

// 行数を返す
int Tensor::rows(void) const { return rows_; }

// 列数を返す
int Tensor::cols(void) const { return cols_; }

// 要素アクセス演算子（const版）
Matrix Tensor::operator[](int height) const { return matrices_[height]; }

// 要素アクセス演算子（非const版）
Matrix& Tensor::operator[](int height) { return matrices_[height]; }

// コピー代入演算子
Tensor& Tensor::operator=(const Tensor& arg) {
    if (this == &arg) {
        return *this;  // 自己代入の場合、何もしない
    }

    // メンバー変数をコピー
    heights_ = arg.heights_;
    rows_ = arg.rows_;
    cols_ = arg.cols_;

    // 既存のリソースを解放
    delete[] matrices_;

    // 新しいリソースを確保
    matrices_ = new Matrix[heights_];

    // メンバー変数をコピー
    for (int i = 0; i < heights_; i++) {
        matrices_[i] = arg.matrices_[i];
    }

    return *this;
}

// ムーブ代入演算子
Tensor& Tensor::operator=(Tensor&& arg) {
    if (this == &arg) {
        return *this;  // 自己代入の場合、何もしない
    }

    // 既存のリソースを解放
    delete[] matrices_;

    // メンバー変数をムーブ
    heights_ = arg.heights_;
    rows_ = arg.rows_;
    cols_ = arg.cols_;
    matrices_ = arg.matrices_;

    // 右辺値のリソースを無効化
    arg.heights_ = 0;
    arg.rows_ = 0;
    arg.cols_ = 0;
    arg.matrices_ = nullptr;

    return *this;
}

// テンソルの加算演算子
Tensor operator+(Tensor& lhs, Tensor& rhs) {
    int heights = lhs.heights();
    int rows = lhs.rows();
    int cols = lhs.cols();

    Tensor result(heights, rows, cols);
    for (int h = 0; h < heights; h++) {
        double* values_A = lhs[h].get_values();
        double* values_B = rhs[h].get_values();
        double* values_Result = result[h].get_values();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                *values_Result++ = *values_A++ + *values_B++;
            }
        }
    }
    return result;
}

// テンソルの減算演算子
Tensor operator-(Tensor& lhs, Tensor& rhs) {
    int heights = lhs.heights();
    int rows = lhs.rows();
    int cols = lhs.cols();

    Tensor result(heights, rows, cols);
    for (int h = 0; h < heights; h++) {
        double* values_A = lhs[h].get_values();
        double* values_B = rhs[h].get_values();
        double* values_Result = result[h].get_values();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                *values_Result++ = *values_A++ - *values_B++;
            }
        }
    }
    return result;
}

// テンソルの要素の平方和を計算する関数
double squared_sum(const Tensor& arg) {
    double result = 0.0;
    for (int i = 0; i < arg.heights(); i++) {
        result += squared_sum(arg[i]);
    }
    return result;
}

// フロベニウスノルムを計算する関数
double frobenius_norm(const Tensor& arg) {
    return sqrt(squared_sum(arg));
}