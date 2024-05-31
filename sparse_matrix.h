#include "matrix.h"
#ifndef __SPARSE_MATRIX__
#define __SPARSE_MATRIX__

//計算速度を向上させるために、直接アドレスを参照して計算を行っている
class SparseMatrix {
   private:
    int rows_;          // 行数
    int cols_;          // 列数
    int nnz_;           // 非ゼロ要素数
    int* row_pointers_; // 行ポインタ配列
    int* col_indices_;  // 列インデックス配列
    double* values_;    // 非ゼロ要素の値

   public:
    SparseMatrix(int rows, int cols);           // コンストラクタ
    SparseMatrix(int rows, int cols, int nnz);  // コンストラクタ
    SparseMatrix();                             // デフォルトコンストラクタ
    SparseMatrix(const SparseMatrix& arg);      // コピーコンストラクタ
    SparseMatrix(int size, const char* s);      // 特殊なコンストラクタ
    ~SparseMatrix();                            // デストラクタ
    double& operator()(int row, int index);     // 要素アクセス演算子（非const版）
    double operator()(int row, int index) const; // 要素アクセス演算子（const版）
    int& operator()(int row, int index, const char* s); // インデックスアクセス演算子（非const版）
    int operator()(int row, int index, const char* s) const; // インデックスアクセス演算子（const版）
    int operator()(int row, const char* s) const; // 行の要素数を返す演算子
    double& value(int row, int index);          // 非ゼロ要素の値を返す
    int& dense_index(int row, int index);       // 非ゼロ要素のインデックスを返す
    int rows() const;                           // 行数を返す
    int cols() const;                           // 列数を返す
    int nnz() const;                            // 非ゼロ要素数を返す
    int nnz(int row);                           // 特定の行の非ゼロ要素数を返す
    SparseMatrix remove_zeros();                // ゼロ要素を削除した疎行列を返す
    SparseMatrix& operator=(const SparseMatrix& arg); // コピー代入演算子
    SparseMatrix& operator=(SparseMatrix&& arg); // ムーブ代入演算子
    Matrix operator*(Matrix& arg);              // 行列の乗算演算子
    void print_values();                        // 行列の値を表示する
    double* get_values();                       // 値のポインタを取得する
    int* get_row_pointers();                    // 行ポインタのポインタを取得する
    int* get_col_indices();                     // 列インデックスのポインタを取得する
    void set_row_pointers(int* new_row_pointers); // 新しい行ポインタを設定する
    void set_col_indices(int* new_col_indices); // 新しい列インデックスを設定する
    void set_values(double* new_values);        // 新しい値を設定する
    void set_nnz(int nnz);                      // 非ゼロ要素数を設定する
    SparseMatrix transpose();                   // 転置行列を返す
    void product(Matrix& lhs, Matrix& rhs);     // 行列の積を計算する
    SparseMatrix one_hot_encode();              // ワンホットエンコードを行う(Factorization Machine用)
};

#endif // __SPARSE_MATRIX__