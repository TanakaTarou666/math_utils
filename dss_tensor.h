#ifndef __DSDTENSOR__
#define __DSDTENSOR__

class DSSTensor {
   private:
    int rows_;          // 行数
    int cols_;          // 列数
    int depth_;         // 深さ（スライス数）
    int nnz_;           // 非ゼロ要素数
    int* row_pointers_; // 行ポインタ配列
    int* col_indices_;  // 列インデックス配列
    SparseVector* elements_; // 非ゼロ要素の配列

   public:
    DSSTensor(SparseMatrix &arg, int depth);            // コンストラクタ
    DSSTensor(SparseMatrix &arg, int depth, SparseVector* elements); // コンストラクタ（要素指定）
    DSSTensor();                                        // デフォルトコンストラクタ
    ~DSSTensor();                                       // デストラクタ
    SparseVector& operator()(int row, int col);         // 要素アクセス演算子（非const版）
    SparseVector operator()(int row, int col) const;    // 要素アクセス演算子（const版）
    int& operator()(int row, int index, const char* s); // インデックスアクセス演算子（非const版）
    int operator()(int row, int index, const char* s) const; // インデックスアクセス演算子（const版）
    int operator()(int row, const char* s) const;       // 行の要素数を返す演算子
    int& dense_index(int row, int index);               // 非ゼロ要素のインデックスを返す
    DSSTensor& operator=(const DSSTensor& arg);         // コピー代入演算子
    DSSTensor& operator=(DSSTensor&& arg);              // ムーブ代入演算子
    int rows() const;                                   // 行数を返す
    int cols() const;                                   // 列数を返す
    int depth() const;                                  // 深さを返す
    int nnz() const;                                    // 非ゼロ要素数を返す
    int nnz(int row) const;                             // 特定の行の非ゼロ要素数を返す
    SparseVector* get_elements();                       // 非ゼロ要素の配列を返す
    int* get_row_pointers();                            // 行ポインタ配列を返す
    int* get_col_indices();                             // 列インデックス配列を返す
};

#endif