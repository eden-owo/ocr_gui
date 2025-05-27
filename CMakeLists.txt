cmake_minimum_required(VERSION 3.16)

project(ocr_gui VERSION 0.1 LANGUAGES CXX)

# 自动处理 moc/uic/rcc
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 设置 OpenCV_DIR 路径（你的路径貌似是 install 版）
set(OpenCV_DIR "C:/opencv-4.10.0/build-qt/install/x64/vc17/lib")  # 改成有 OpenCVConfig.cmake 的文件夹
find_package(OpenCV REQUIRED)

# 所有源文件
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ocr_module.cpp
    ocr_module.h
    folder.cpp
    folder.h
)

# 可执行文件生成
add_executable(ocr_gui ${PROJECT_SOURCES})

# 包含路径
target_include_directories(ocr_gui PRIVATE ${OpenCV_INCLUDE_DIRS})

# 链接 Qt 和 OpenCV 库
target_link_libraries(ocr_gui
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        ${OpenCV_LIBS}
)

# macOS/iOS 的 Bundle 设置（非必须）
set_target_properties(ocr_gui PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
