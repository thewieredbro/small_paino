#include <Windows.h>

bool running = true;

void* buffer_memory;
int buffer_height;
int buffer_width;
BITMAPINFO buffer_bitmap_info;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    LRESULT result = 0;

    switch (uMsg) {
    case WM_CLOSE:
    case WM_DESTROY: {
        running = false;
    } break;
    
    case WM_SIZE: {
        RECT rect;
        GetClientRect(hwnd, &rect);
        buffer_width = rect.right - rect.left;
        buffer_height = rect.bottom - rect.top;

        int buffer_size = buffer_width * buffer_height * sizeof(unsigned int);

        if (buffer_memory) VirtualFree(buffer_memory, 0, MEM_RELEASE);
        buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info.bmiHeader);
        buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
        buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
        buffer_bitmap_info.bmiHeader.biPlanes = 1;
        buffer_bitmap_info.bmiHeader.biBitCount = 32;
        buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;

    }break;

    default: {
        result = DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "small piano";
    window_class.lpfnWndProc = window_callback;

    RegisterClass(&window_class);

    HWND window = CreateWindow(window_class.lpszClassName, "paino", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1600, 900, 0, 0, hInstance, 0);
    HDC hdc = GetDC(window);

        while (running) {
        MSG message;
        while (PeekMessageA(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
    }
}