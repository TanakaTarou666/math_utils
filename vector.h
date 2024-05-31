#include <cmath>     // 数学関数の標準ライブラリ
#include <cstdlib>   // 一般的な目的の関数の標準ライブラリ
#include <cstring>   // 文字列関数の標準ライブラリ
#include <iostream>  // 入出力ストリームの標準ライブラリ
#include <new>       // メモリ割り当ての標準ライブラリ

#ifndef __VECTOR__
#define __VECTOR__

class Vector {
   private:
    double* values_;       // ベクトルの値を保持するポインタ
    int size_;             // ベクトルのサイズを保持する整数
    bool part_of_matrix_;  // ベクトルが行列の一部であるかを示すブール値

   public:
    Vector(void);                               // デフォルトコンストラクタ
    explicit Vector(int n);                     // サイズを指定するコンストラクタ
    Vector(double* values, int size);           // 値とサイズを指定するコンストラクタ
    Vector(int n, double value, const char* flag); // サイズ、値、およびフラグを指定するコンストラクタ
    Vector(const Vector& arg);                  // コピーコンストラクタ
    Vector& operator=(const Vector& rhs);       // 代入演算子
    ~Vector(void);                              // デストラクタ

    int size(void) const;                       // サイズを取得するメソッド
    double operator[](int index) const;         // インデックスで要素にアクセスするためのconstメソッド
    double& operator[](int index);              // インデックスで要素にアクセスするための非constメソッド
    std::ostream& print(std::ostream& lhs) const; // ベクトルを出力するメソッド
    Vector& operator+=(const Vector& rhs);      // 加算代入演算子
    Vector& operator-=(const Vector& rhs);      // 減算代入演算子
    Vector operator+(void) const;               // 単項プラス演算子
    Vector operator-(void) const;               // 単項マイナス演算子
    double* get_values();                       // データへのポインタを取得するメソッド
};

// 非メンバー関数の宣言
std::ostream& operator<<(std::ostream& lhs, const Vector& rhs); // ベクトルの出力演算子
Vector operator+(const Vector& lhs, const Vector& rhs); // ベクトルの加算演算子
Vector operator-(const Vector& lhs, const Vector& rhs); // ベクトルの減算演算子
double operator*(const Vector& lhs, const Vector& rhs); // ベクトルの内積演算子
Vector operator*(double lhs, const Vector& rhs);        // ベクトルとスカラーの乗算演算子
Vector operator/(const Vector& lhs, double rhs);        // ベクトルとスカラーの除算演算子
bool operator==(const Vector& lhs, const Vector& rhs);  // ベクトルの等価比較演算子
bool operator!=(const Vector& lhs, const Vector& rhs);  // ベクトルの不等価比較演算子
double squared_sum(const Vector& arg);                  // ベクトルの平方和を計算する関数
double norm(const Vector& arg, int p);                  // ベクトルのpノルムを計算する関数
double max_norm(const Vector& arg);                     // ベクトルの最大ノルムを計算する関数
double squared_norm(const Vector& arg);                 // ベクトルの平方ノルムを計算する関数


#endif