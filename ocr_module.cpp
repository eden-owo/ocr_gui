#include "ocr_module.h"
#include <windows.h>
#include <iostream>

using namespace std;

typedef __int64(__cdecl* CreateOcrInitOptions_t)(__int64*);
typedef __int64(__cdecl* OcrInitOptionsSetUseModelDelayLoad_t)(__int64, char);
typedef __int64(__cdecl* CreateOcrPipeline_t)(__int64, __int64, __int64, __int64*);
typedef __int64(__cdecl* CreateOcrProcessOptions_t)(__int64*);
typedef __int64(__cdecl* OcrProcessOptionsSetMaxRecognitionLineCount_t)(__int64, __int64);
typedef __int64(__cdecl* RunOcrPipeline_t)(__int64, Img*, __int64, __int64*);
typedef __int64(__cdecl* GetOcrLineCount_t)(__int64, __int64*);
typedef __int64(__cdecl* GetOcrLine_t)(__int64, __int64, __int64*);
typedef __int64(__cdecl* GetOcrLineContent_t)(__int64, __int64*);

ocr_module::ocr_module() {}

void ocr_module::run_ocr(const Img& img) {
    HINSTANCE hDLL = LoadLibraryA("ocr.dll");
    if (!hDLL) {
        cerr << "Failed to load DLL\n";
        return;
    }

    auto CreateOcrInitOptions = (CreateOcrInitOptions_t)GetProcAddress(hDLL, "CreateOcrInitOptions");
    auto OcrInitOptionsSetUseModelDelayLoad = (OcrInitOptionsSetUseModelDelayLoad_t)GetProcAddress(hDLL, "OcrInitOptionsSetUseModelDelayLoad");
    auto CreateOcrPipeline = (CreateOcrPipeline_t)GetProcAddress(hDLL, "CreateOcrPipeline");
    auto CreateOcrProcessOptions = (CreateOcrProcessOptions_t)GetProcAddress(hDLL, "CreateOcrProcessOptions");
    auto OcrProcessOptionsSetMaxRecognitionLineCount = (OcrProcessOptionsSetMaxRecognitionLineCount_t)GetProcAddress(hDLL, "OcrProcessOptionsSetMaxRecognitionLineCount");
    auto RunOcrPipeline = (RunOcrPipeline_t)GetProcAddress(hDLL, "RunOcrPipeline");
    auto GetOcrLineCount = (GetOcrLineCount_t)GetProcAddress(hDLL, "GetOcrLineCount");
    auto GetOcrLine = (GetOcrLine_t)GetProcAddress(hDLL, "GetOcrLine");
    auto GetOcrLineContent = (GetOcrLineContent_t)GetProcAddress(hDLL, "GetOcrLineContent");

    __int64 ctx = 0, pipeline = 0, opt = 0, instance = 0;
    CreateOcrInitOptions(&ctx);
    OcrInitOptionsSetUseModelDelayLoad(ctx, 0);

    string key_str;
    cout << "Please input OCR key: ";
    cin >> key_str;
    const char* key = key_str.c_str();

    CreateOcrPipeline((__int64)"ocr.model", (__int64)key, ctx, &pipeline);
    CreateOcrProcessOptions(&opt);
    OcrProcessOptionsSetMaxRecognitionLineCount(opt, 1000);
    RunOcrPipeline(pipeline, const_cast<Img*>(&img), opt, &instance);

    __int64 lineCount;
    GetOcrLineCount(instance, &lineCount);
    printf("Recognized %lld lines:\n", lineCount);

    for (__int64 i = 0; i < lineCount; ++i) {
        __int64 line = 0, content = 0;
        GetOcrLine(instance, i, &line);
        GetOcrLineContent(line, &content);
        printf("%lld: %s\n", i, (char*)content);
    }

    FreeLibrary(hDLL);
}
