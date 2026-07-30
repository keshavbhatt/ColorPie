# Bundled Qt-Color-Widgets by Mattia Basaglia (LGPL-3.0)
# https://gitlab.com/mattbas/Qt-Color-Widgets
add_library(colorwidgets STATIC
    src/QtColorWidgets/abstract_widget_list.cpp
    src/QtColorWidgets/bound_color_selector.cpp
    src/QtColorWidgets/color_2d_slider.cpp
    src/QtColorWidgets/color_delegate.cpp
    src/QtColorWidgets/color_dialog.cpp
    src/QtColorWidgets/color_dialog.ui
    src/QtColorWidgets/color_line_edit.cpp
    src/QtColorWidgets/color_list_widget.cpp
    src/QtColorWidgets/color_names.cpp
    src/QtColorWidgets/color_palette.cpp
    src/QtColorWidgets/color_palette_model.cpp
    src/QtColorWidgets/color_palette_widget.cpp
    src/QtColorWidgets/color_palette_widget.ui
    src/QtColorWidgets/color_preview.cpp
    src/QtColorWidgets/color_selector.cpp
    src/QtColorWidgets/color_utils.cpp
    src/QtColorWidgets/color_wheel.cpp
    src/QtColorWidgets/gradient_editor.cpp
    src/QtColorWidgets/gradient_list_model.cpp
    src/QtColorWidgets/gradient_slider.cpp
    src/QtColorWidgets/harmony_color_wheel.cpp
    src/QtColorWidgets/hue_slider.cpp
    src/QtColorWidgets/swatch.cpp
    include/QtColorWidgets/abstract_widget_list.hpp
    include/QtColorWidgets/bound_color_selector.hpp
    include/QtColorWidgets/color_2d_slider.hpp
    include/QtColorWidgets/color_delegate.hpp
    include/QtColorWidgets/color_dialog.hpp
    include/QtColorWidgets/color_line_edit.hpp
    include/QtColorWidgets/color_list_widget.hpp
    include/QtColorWidgets/color_names.hpp
    include/QtColorWidgets/color_palette.hpp
    include/QtColorWidgets/color_palette_model.hpp
    include/QtColorWidgets/color_palette_widget.hpp
    include/QtColorWidgets/color_preview.hpp
    include/QtColorWidgets/color_selector.hpp
    include/QtColorWidgets/color_utils.hpp
    include/QtColorWidgets/color_wheel.hpp
    include/QtColorWidgets/color_wheel_private.hpp
    include/QtColorWidgets/colorwidgets_global.hpp
    include/QtColorWidgets/gradient_delegate.hpp
    include/QtColorWidgets/gradient_editor.hpp
    include/QtColorWidgets/gradient_helper.hpp
    include/QtColorWidgets/gradient_list_model.hpp
    include/QtColorWidgets/gradient_slider.hpp
    include/QtColorWidgets/harmony_color_wheel.hpp
    include/QtColorWidgets/hue_slider.hpp
    include/QtColorWidgets/swatch.hpp
    resources/QtColorWidgets/color_widgets.qrc
)

target_include_directories(colorwidgets PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(colorwidgets PUBLIC QTCOLORWIDGETS_STATICALLY_LINKED)

target_link_libraries(colorwidgets PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
