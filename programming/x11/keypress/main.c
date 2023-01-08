#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>

#include "./main.h"

#define WIDTH 128
#define HEIGHT 64

int hand(Display *d,XErrorEvent* e){
    fprintf(stderr,"ERROR CODE: %d\n",e->error_code);
    return 0;
}

int print_char(Display *display, Window window,GC gc,char chr,XImage* font){
    int y =  (((int)chr - 32) / 8);
    int x =  (((int)chr - 32) % 8);
    printf("x: %d    y: %d\n",x,y);
    // TODO: Make this accurate
    // I think the logic to put characters into the screen is not correct.
    int r = XPutImage(display, window,
            gc, font, 0, 0, 0, 0,
            WIDTH, HEIGHT);
    return r;
}

void handle_key(XKeyEvent* event){
    char buffer[64];
    KeySym keysym;
    XComposeStatus status;

    XLookupString(event,buffer,sizeof(buffer),&keysym,&status);

    char* key = XKeysymToString(keysym);
    printf("%s key is pressed\n",key);
}

int main(void){

    Display* display = XOpenDisplay(NULL);
    Window window = XCreateSimpleWindow(display, XDefaultRootWindow(display), 0, 0, 800, 600, 0, 0 , 0x00ffffff);
    GC gc = XCreateGC(display,window,0,NULL);

    Atom wm_delete_window = XInternAtom(display,"WM_DELETE_WINDOW",True);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    XSelectInput(display, window, KeyPressMask | ExposureMask);

    XMapWindow(display,window);

    XWindowAttributes attr;
    XGetWindowAttributes(display,window,&attr);

    // Setting up the GC
    unsigned long valuemask = 0;		/* which values in 'values' to  */
    /* check when creating the GC.  */
    XGCValues values;			/* initial values for the GC.   */
    unsigned int line_width = 2;		/* line width for the GC.       */
    int line_style = LineSolid;		/* style for lines drawing and  */
    int cap_style = CapButt;		/* style of the line's edje and */
    int join_style = JoinBevel;		/*  joined lines.		*/
    int screen_num = DefaultScreen(display);
    /* define the style of lines that will be drawn using this GC. */
    XSetLineAttributes(display, gc,
            line_width, line_style, cap_style, join_style);

    /* define the fill style for the GC. to be 'solid filling'. */
    XSetFillStyle(display, gc, FillSolid);

    // Creating Ximage from pixels
    char* pixels = (char*)FONT;
    XImage *ximage = XCreateImage(display, attr.visual, attr.depth,
            ZPixmap, 0,(char*) pixels,
            WIDTH, HEIGHT, 32, 0);

    // Finding the appropriate font
    int font_num;
    char **something = XListFonts( display, "*",
            1000,
            &font_num);
    printf("There is %d available fonts in system\n",font_num);
    // Setting up the font
    XFontStruct* font_info;
    char* font_name = *something;

    font_info = XLoadQueryFont(display, font_name);
    if(!font_info){
        fprintf(stderr, "XLoadQueryFont: failed loading font '%s'\n", font_name);
        exit(-1);
    }

    XSetFont(display, gc, font_info->fid);



    XEvent event;
    int run = 1;
    int r = print_char(display,window,gc,'a',ximage);
    printf("Result: %d\n",r);
    while(run){
        XNextEvent(display,&event);

        r = XDrawString(display, window,  gc, 0, 0, "Hwllo", 5);
        printf("Result: %d\n",r);
        if(event.type == Expose){
            int r = XPutImage(display, window,
                    gc, ximage, 0, 0, 0, 0,
                    WIDTH, HEIGHT);
            printf("RES: %i\n", r);
        }

        if(event.type == KeyPress){
            handle_key((XKeyEvent*)&event);
        }

        else if(event.type == ClientMessage){

            if(event.xclient.data.l[0] == wm_delete_window){
                printf("Quiting...\n");
                run = 0;
            }

        }

    }

    return 0;
}
