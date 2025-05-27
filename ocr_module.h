#ifndef OCR_MODULE_H
#define OCR_MODULE_H

#pragma once
#include <cstdint>
#include <opencv2/opencv.hpp> // 若你要處理 OpenCV 的 Mat

// 封裝影像資料的結構，供 OCR 模組使用
struct Img {
    std::int32_t t;           // type
    std::int32_t col;         // width
    std::int32_t row;         // height
    std::int32_t _unk;        // 保留欄位
    std::int64_t step;        // 每行位元組數
    std::int64_t data_ptr;    // 指向影像資料
};

// ocr_module 類別定義
class ocr_module {
public:
    ocr_module();           // 建構子
    void run_ocr(const Img&);   // 提供 run OCR 功能
};

#endif
