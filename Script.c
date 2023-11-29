#include <iostream>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <string>
#include <unistd.h>

void sendKeys(const std::string& keys) {
    Display* display = XOpenDisplay(nullptr);
    if (!display) {
        std::cerr << "Unable to open display." << std::endl;
        return;
    }

    for (char key : keys) {
        if (key == '^') {
            XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), True, 0);
            XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Control_L), False, 0);
        } else if (key == '+') {
            XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), True, 0);
            XTestFakeKeyEvent(display, XKeysymToKeycode(display, XK_Shift_L), False, 0);
        } else if (key == '{') {
            size_t endPos = keys.find('}', 1);
            if (endPos != std::string::npos) {
                std::string specialKey = keys.substr(1, endPos - 1);
                KeySym keysym = XStringToKeysym(specialKey.c_str());
                if (keysym != NoSymbol) {
                    KeyCode keycode = XKeysymToKeycode(display, keysym);
                    XTestFakeKeyEvent(display, keycode, True, 0);
                    XTestFakeKeyEvent(display, keycode, False, 0);
                } else {
                    std::cerr << "Invalid special key: " << specialKey << std::endl;
                }
            }
        } else {
            KeySym keysym = XStringToKeysym(&key);
            if (keysym != NoSymbol) {
                KeyCode keycode = XKeysymToKeycode(display, keysym);
                XTestFakeKeyEvent(display, keycode, True, 0);
                XTestFakeKeyEvent(display, keycode, False, 0);
            } else {
                std::cerr << "Invalid key: " << key << std::endl;
            }
        }
        XFlush(display);
        usleep(10000); // Add delay for each key, adjust as needed
    }

    XCloseDisplay(display);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Please provide keys to send." << std::endl;
        return 1;
    }

    std::string keysToSend = argv[1];
    sendKeys(keysToSend);

    return 0;
}
