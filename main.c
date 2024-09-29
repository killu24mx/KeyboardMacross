#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define EXIT_CODE 1


Window get_focus_window(Display* display){
    Window window;
    int focus_state = 0;
    XGetInputFocus(display,&window,&focus_state);
    return window;
}

int ReleaseEventsOnFocusWindow(Display *display){
    Window window = get_focus_window(display);
    XSelectInput(display,window,KeyPressMask|KeyReleaseMask);
    while(XPending(display)) { 
        XEvent event;
        XNextEvent(display,&event);
        if(event.type == KeyPress){
            uint32_t KeyEventCode = event.xkey.keycode;
            if(KeyEventCode == 24){
                printf("%s\n","exit");
                return EXIT_CODE;
            }
        }
    }
    return 0;
}


int press_button_for(Display* display,unsigned int keycode,unsigned int sec)
{
    XTestFakeKeyEvent(display, keycode, true, 0);
    XFlush(display);
    int time = clock();
    while((clock() - time)/CLOCKS_PER_SEC < sec){
        int exit = ReleaseEventsOnFocusWindow(display);
        if(exit){
            XTestFakeKeyEvent(display, keycode, false, 0);
            return exit;
        }
    }
    XTestFakeKeyEvent(display, keycode, false, 0);
    return 0;
}

int main(int argc,char** argv){

    
    Display *display = XOpenDisplay(NULL);
    Bool exit = false;


    unsigned int keycode_1 = XKeysymToKeycode(display, XK_A);
    unsigned int keycode_2 = XKeysymToKeycode(display, XK_D);
    unsigned int keycode_3 = XKeysymToKeycode(display, XK_3);
    
    while(!exit){
        exit = exit || press_button_for(display,keycode_1,3);
        exit = exit || press_button_for(display,keycode_2,3);
    }
    

    XCloseDisplay(display);
    printf("%s\n","finished");


    

    return 0;

}