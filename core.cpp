#include <iostream>
#include <windows.h>
#include <cctype> // Include for the toupper function
#include <map>
#include <string>
#include <limits>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <cmath> // Include the cmath library for mathematical functions


std::map<COLORREF, std::string> colorNames = {
    {RGB(255, 0, 0), "Red"},
    {RGB(0, 255, 0), "Green"},
    {RGB(0, 0, 255), "Blue"},
    // Add more color names as needed
};

std::map<std::string, int> keyMap = {
    {"0", '0'}, {"1", '1'}, {"2", '2'}, {"3", '3'}, {"4", '4'},
    {"5", '5'}, {"6", '6'}, {"7", '7'}, {"8", '8'}, {"9", '9'},
    {"a", 'A'}, {"b", 'B'}, {"c", 'C'}, {"d", 'D'}, {"e", 'E'},
    {"f", 'F'}, {"g", 'G'}, {"h", 'H'}, {"i", 'I'}, {"j", 'J'},
    {"k", 'K'}, {"l", 'L'}, {"m", 'M'}, {"n", 'N'}, {"o", 'O'},
    {"p", 'P'}, {"q", 'Q'}, {"r", 'R'}, {"s", 'S'}, {"t", 'T'},
    {"u", 'U'}, {"v", 'V'}, {"w", 'W'}, {"x", 'X'}, {"y", 'Y'},
    {"z", 'Z'}, {"m4", VK_XBUTTON1}, {"m5", VK_XBUTTON2},
};

COLORREF targetColor = RGB(255, 255, 255); // White

void performClick() {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));

    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

COLORREF pickColor() {
    system("cls");
    std::cout << "*+*+*+*+*+*+* Color Picker *+*+*+*+*+*+*\n";
    std::cout << "Place the mouse cursor over the desired color and press 'K' to capture it.\n";

    while (true) {
        if (GetAsyncKeyState('K')) {
            POINT cursorPos;
            GetCursorPos(&cursorPos);

            HDC hdc = GetDC(NULL);
            COLORREF color = GetPixel(hdc, cursorPos.x, cursorPos.y);
            ReleaseDC(NULL, hdc);

            std::string colorName = "Unknown";
            if (colorNames.find(color) != colorNames.end()) {
                colorName = colorNames[color];
            }
            std::cout << "Color set to: " << colorName << " (" << std::hex << color << std::dec << ")\n";
            return color;
        }
        Sleep(10);
    }
}

void trigger() {
    system("cls");
    std::cout << "*+*+*+*+*+*+* Trigger Settings *+*+*+*+*+*+*\n";

    std::cout << "Enter the key to activate the trigger (0-9, a-z, m4 for Mouse 4, m5 for Mouse 5): ";
    std::string keyInput;
    std::cin >> keyInput;

    if (keyMap.find(keyInput) == keyMap.end()) {
        std::cout << "Invalid key. Returning to the menu.\n";
        return;
    }
    int triggerKey = keyMap[keyInput];

    std::cout << "Enter the trigger delay in milliseconds: ";
    int delay;
    std::cin >> delay;

    std::cout << "Enter the key to exit back to the menu: ";
    char exitKey;
    std::cin >> exitKey;

    while (true) {
        if (GetAsyncKeyState(exitKey)) {
            break;
        }
        if (GetAsyncKeyState(triggerKey)) {
            POINT cursorPos;
            GetCursorPos(&cursorPos);

            HDC hdc = GetDC(NULL);
            COLORREF color = GetPixel(hdc, cursorPos.x, cursorPos.y);
            ReleaseDC(NULL, hdc);

            if (color == targetColor) {
                performClick();
            }
            Sleep(delay);
        }
        else {
            Sleep(10);
        }
    }
}



/*

void colorBot() {
    system("cls");
    std::cout << "============== Color Bot Settings ==============\n";

    std::cout << "Enter the pixel radius to search for the target color: ";
    int pixelRadius;
    std::cin >> pixelRadius;

    std::cout << "Enter the key to activate the color bot (0-9, a-z, m4 for Mouse 4, m5 for Mouse 5): ";
    std::string keyInput;
    std::cin >> keyInput;

    char ch;
    while (std::cin.get(ch) && ch != '\n');

    if (keyMap.find(keyInput) == keyMap.end()) {
        std::cout << "Invalid key. Returning to the menu.\n";
        return;
    }
    int activationKey = keyMap[keyInput];

    std::cout << "Enter the key to exit back to the menu: ";
    char exitKey;
    std::cin >> exitKey;

    while (std::cin.get(ch) && ch != '\n');

    std::cout << "Enter the smoothness level (0-100, 0 snaps instantly, 100 moves slowly): ";
    int smoothness;
    std::cin >> smoothness;

    while (true) {
        if (GetAsyncKeyState(exitKey)) {
            std::cout << "Exiting Color Bot.\n";
            break;
        }
        if (GetAsyncKeyState(activationKey)) {
            std::cout << "Color Bot is ON.\n";
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            std::cout << "Current Cursor Position: (" << cursorPos.x << ", " << cursorPos.y << ")\n";

            HDC hdc = GetDC(NULL);
            POINT targetPos = { -1, -1 };

            for (int x = cursorPos.x - pixelRadius; x <= cursorPos.x + pixelRadius; x++) {
                for (int y = cursorPos.y - pixelRadius; y <= cursorPos.y + pixelRadius; y++) {
                    COLORREF color = GetPixel(hdc, x, y);
                    if (color == targetColor) {
                        targetPos = { x, y };
                        break;
                    }
                }
                if (targetPos.x != -1 && targetPos.y != -1) {
                    break;
                }
            }

            if (targetPos.x != -1 && targetPos.y != -1) {
                std::cout << "Target Detected at: (" << targetPos.x << ", " << targetPos.y << ")\n";
                POINT direction = { targetPos.x - cursorPos.x, targetPos.y - cursorPos.y };
                float length = sqrt(direction.x * direction.x + direction.y * direction.y);
                float scaleFactor = smoothness / 100.0f;
                POINT moveVector = {
                    static_cast<int>(direction.x * scaleFactor),
                    static_cast<int>(direction.y * scaleFactor)
                };
                SetCursorPos(cursorPos.x + moveVector.x, cursorPos.y + moveVector.y);
            }
            else {
                std::cout << "Target Not Detected.\n";
            }

            ReleaseDC(NULL, hdc);
            Sleep(10);
        }
        else {
            std::cout << "Color Bot is OFF.\n";
            Sleep(10);
        }
    }
}
*/

int colorDistance(COLORREF color1, COLORREF color2) {
    int r1 = GetRValue(color1), g1 = GetGValue(color1), b1 = GetBValue(color1);
    int r2 = GetRValue(color2), g2 = GetGValue(color2), b2 = GetBValue(color2);
    return abs(r1 - r2) + abs(g1 - g2) + abs(b1 - b2);
}

void colorBot() {
    system("cls");
    std::cout << "*+*+*+*+*+*+* Color Bot Settings *+*+*+*+*+*+*\n";

    std::cout << "Enter the pixel radius to search for the target color: ";
    int pixelRadius;
    std::cin >> pixelRadius;

    std::cout << "Enter the key to activate the color bot (0-9, a-z, m4 for Mouse 4, m5 for Mouse 5): ";
    std::string keyInput;
    std::cin >> keyInput;

    char ch;
    while (std::cin.get(ch) && ch != '\n');

    if (keyMap.find(keyInput) == keyMap.end()) {
        std::cout << "Invalid key. Returning to the menu.\n";
        return;
    }
    int activationKey = keyMap[keyInput];

    std::cout << "Enter the key to exit back to the menu: ";
    char exitKey;
    std::cin >> exitKey;

    while (std::cin.get(ch) && ch != '\n');

    std::cout << "Enter the smoothness level (0-100, 0 snaps instantly, 100 moves slowly): ";
    int smoothness;
    std::cin >> smoothness;

    std::cout << "Enter color tolerance (0-35): ";
    int colorTolerance;
    std::cin >> colorTolerance;
    colorTolerance = colorTolerance < 0 ? 0 : (colorTolerance > 35 ? 35 : colorTolerance);

    while (true) {
        if (GetAsyncKeyState(exitKey)) {
            std::cout << "Exiting Color Bot.\n";
            break;
        }
        if (GetAsyncKeyState(activationKey)) {
            std::cout << "Color Bot is ON.\n";
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            std::cout << "Current Cursor Position: (" << cursorPos.x << ", " << cursorPos.y << ")\n";

            HDC hdc = GetDC(NULL);
            POINT targetPos = { -1, -1 };

            for (int x = cursorPos.x - pixelRadius; x <= cursorPos.x + pixelRadius; x++) {
                for (int y = cursorPos.y - pixelRadius; y <= cursorPos.y + pixelRadius; y++) {
                    COLORREF color = GetPixel(hdc, x, y);
                    if (colorDistance(color, targetColor) <= colorTolerance) {
                        targetPos = { x, y };
                        break;
                    }
                }
                if (targetPos.x != -1 && targetPos.y != -1) {
                    break;
                }
            }

            if (targetPos.x != -1 && targetPos.y != -1) {
                std::cout << "Target Detected at: (" << targetPos.x << ", " << targetPos.y << ")\n";
                POINT direction = { targetPos.x - cursorPos.x, targetPos.y - cursorPos.y };
                float length = sqrt(direction.x * direction.x + direction.y * direction.y);
                float scaleFactor = smoothness / 100.0f;
                POINT moveVector = {
                    static_cast<int>(direction.x * scaleFactor),
                    static_cast<int>(direction.y * scaleFactor)
                };
                SetCursorPos(cursorPos.x + moveVector.x, cursorPos.y + moveVector.y);
            }
            else {
                std::cout << "Target Not Detected.\n";
            }

            ReleaseDC(NULL, hdc);
            Sleep(10);
        }
        else {
            std::cout << "Color Bot is OFF.\n";
            Sleep(10);
        }
    }
}


int main() {
    while (true) {
        system("cls");
        std::cout << "*+*+*+*+*+*+* Menu *+*+*+*+*+*+*\n";
        std::cout << "1. Color Picker\n";
        std::cout << "2. Trigger\n";
        std::cout << "3. Color Bot\n";
        std::cout << "*+*+*+*+*+*+**+*+*+*+*+*+**+*+*+\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear the newline character from the input buffer

        switch (choice) {
        case 1:
            targetColor = pickColor();
            break;
        case 2:
            trigger();
            break;
        case 3:
            colorBot();
            break;
        default:
            std::cout << "Invalid option. Please choose again.\n";
            break;
        }
    }

    return 0;
}

