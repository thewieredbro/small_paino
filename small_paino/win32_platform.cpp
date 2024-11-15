#include <Windows.h>
#include "utiles.cpp"

global_variable bool running = true;

struct Render_State {
    int height, width;
    void* memory;
    BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;

#include "platform_commoncpp.cpp"
#include "renderer.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (uMsg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        running = false;
    } break;

    case WM_SIZE: {
        RECT rect;
        GetClientRect(hwnd, &rect);
        render_state.width = rect.right - rect.left;
        render_state.height = rect.bottom - rect.top;

        int size = render_state.width * render_state.height * sizeof(unsigned int);

        if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
        render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
        render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
        render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
        render_state.bitmap_info.bmiHeader.biPlanes = 1;
        render_state.bitmap_info.bmiHeader.biBitCount = 32;
        render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
    } break;

    default: {
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    WNDCLASS window_class = { sizeof(WNDCLASS) };
    window_class.hCursor = LoadCursor(0, IDC_ARROW);
    window_class.hInstance = hInstance;
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "small piano";
    window_class.lpfnWndProc = window_callback;

    RegisterClass(&window_class);

    HWND window = CreateWindow(window_class.lpszClassName, "paino", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1600, 900, 0, 0, hInstance, 0);
    HDC hdc = GetDC(window);

    input input = {};

    while (running) {
        MSG message;

        for (int i = 0; i < BUTTON_COUNT; i++) {
            input.buttons.changed = false;
        }

        while (PeekMessageA(&message, window, 0, 0, PM_REMOVE)) {

            switch (message.message) {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    unsigned int* vk_code = (unsigned int*)message.wParam;
                    bool is_down = ((message.lParam & (1 << 31)) == 0);

                    switch (vk_code) {
                        case VK_UP: {
                            input.buttons[BUTTON_UP].is_down = is_down;
                            input.buttons[BUTTON_UP].changed = true;
                        }break;
                    }
                }break;
                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }
        }
        if (input.buttons[BUTTON_UP.is_down])
            clear_screen(16777215);

        for (int i = 1; i < 7; ++i) {
            draw_rect(i + (-1600 + 3200.0 / 7 * i), 0, 10, 3200, 000000);
        }

        for (int i = 1; i < 7; ++i) {
            if (i != 3) {
                draw_rect(i + (-1600 + 3200.0 / 7 * i), 820, 50, 800, 000000);
            }
        }

        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
    }

}
