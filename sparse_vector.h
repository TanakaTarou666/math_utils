#include <iostream> 

#include "vector.h"

#ifndef __SPARSEVECTOR__
#define __SPARSEVECTOR__

class Vector;

class SparseVector {
   private:
    int size_;      // ベクトルの全体のサイズ
    int nnz_;       // 非ゼロ成分の数
    int *indices_;  // 非ゼロ成分のインデックス配列
    double *values_; // 非ゼロ成分の値配列

   public:
    SparseVector(int size_ = 0, int nnz_ = 0);  // コンストラクタ
    SparseVector(const SparseVector &arg);      // コピーコンストラクタ
    SparseVector(SparseVector &&arg);           // ムーブコンストラクタ
    ~SparseVector(void);                        // デストラクタ
    SparseVector &operator=(const SparseVector &arg); // コピー代入演算子
    SparseVector &operator=(SparseVector &&arg); // ムーブ代入演算子
    int size(void) const;                       // サイズを返すメソッド
    int nnz(void) const;                        // 非ゼロ成分の数を返すメソッド
    double &operator()(int index);              // 非ゼロ成分の値にアクセスする演算子
    double operator()(int index) const;         // 非ゼロ成分の値にアクセスする演算子（const版）
    int &operator()(int index, const char *s);  // 非ゼロ成分のインデックスにアクセスする演算子
    int operator()(int index, const char *s) const; // 非ゼロ成分のインデックスにアクセスする演算子（const版）
    double& value(int index);                   // 非ゼロ成分の値を返すメソッド
    int& dense_index(int index);                // 非ゼロ成分のインデックスを返すメソッド
    SparseVector operator+(void) const;         // 単項プラス演算子
    SparseVector operator-(void) const;         // 単項マイナス演算子
    bool operator==(const SparseVector &rhs) const; // 等価比較演算子
    bool operator!=(const SparseVector &rhs) const; // 不等価比較演算子
    void modifyvalues_(int n, int index, double value); // 値を変更するメソッド
};

std::ostream &operator<<(std::ostream &os, const SparseVector &rhs); // 出力演算子
double max_norm(const SparseVector &arg);       // 最大ノルムを計算する関数
double squared_norm(const SparseVector &arg);   // 2ノルムを計算する関数
double norm_square(const SparseVector &arg);    // 2ノルムの二乗を計算する関数
double operator*(const SparseVector &lhs, const Vector &rhs); // スパースベクトルとベクトルの内積演算子
double operator*(const Vector &lhs, const SparseVector &rhs); // ベクトルとスパースベクトルの内積演算子

#endif