#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h> // Most hated graphic library!!!
#include <X11/Xutil.h>

#define WIDTH  800
#define HEIGHT 600

int pixel[WIDTH*HEIGHT];

// NOTE: The ExposeMask is must to be able to put image into the window otherwise the image won't be visible

int main() {
  printf("Hello, World\n");
  memset(pixel, 0xaa, WIDTH*HEIGHT*4); // filling the pixels with 0xAAAAAA
  int screen;
  XWMHints wmh;
  XTextProperty WindowName, IconName;
  char *window_name = "mywindow";
  char *icon_name   = "something";

  Display *dpy = XOpenDisplay(NULL);
  screen = XDefaultScreen(dpy);

  // Setting some attributes for window
  XSetWindowAttributes wa = {0};
  wa.background_pixel = XWhitePixel(dpy,screen);
  wa.border_pixel = XBlackPixel(dpy,screen);
  wa.event_mask = KeyPressMask | ButtonPressMask | ExposureMask;
  wa.save_under = True;

  Window win = XCreateWindow(dpy, XRootWindow(dpy,screen), 0, 0, WIDTH, HEIGHT, 0, 
                             XDefaultDepth(dpy,screen), InputOutput, XDefaultVisual(dpy,screen), 
                             CWBorderPixel | CWBackPixel | CWEventMask | CWSaveUnder, &wa);
  if (win == 0) {
    fprintf(stderr,"ERROR: Could not create window\n");
    exit(1);
  }

  // Give the window manager hint about the dimention of window
  XSizeHints ws;

  ws.flags           =  USPosition | USSize;
  wmh.initial_state  =  NormalState;
  wmh.flags          =  StateHint;

  XSetWMNormalHints(dpy,win,&ws);
  XSetWMHints(dpy, win, &wmh);
  XStringListToTextProperty(&window_name,1,&WindowName);
  XStringListToTextProperty(&icon_name,1,&IconName);
  XSetWMName(dpy, win, &WindowName);
  XSetWMIconName(dpy, win, &WindowName);

  XMapWindow(dpy,win);

  GC gc = XCreateGC(dpy, win, 0, NULL);
  XImage *image =   XCreateImage(dpy, XDefaultVisual(dpy,screen),   XDefaultDepth(dpy,screen), ZPixmap, 0, (char*)pixel, WIDTH, HEIGHT, 32,  WIDTH*4);

  // Making the program quit on closing the window
  Atom wm_quit = XInternAtom(dpy,"WM_DELETE_WINDOW", 0);
  XSetWMProtocols(dpy, win, &wm_quit, 1);

  XEvent e;
  int run = 1;
  while(run) {
    XNextEvent(dpy,&e);  

    switch (e.type) 
      {
        case KeyPress:
          {
            printf("Some key has pressed\n");
            XBell(dpy, 0);
          } break; 

        case ButtonPress:
          {
            printf("Mouse button has pressed\n");
            XSetWindowBackground(dpy, win, 0xff00ff);
            XClearWindow(dpy, win);
          } break;

        case ClientMessage:
          {
            if ((Atom)e.xclient.data.l[0] == wm_quit) {
              run = 0;
            }
          } break;

        case MapNotify:
          {
            printf("Window Created Succesfully\n");
          } break;

        case Expose:
          {
            printf("We are exposed\n");
            XPutImage(dpy, win, gc, image, 0, 0, 0, 0, WIDTH,  HEIGHT);
          } break;

        default:
          {
            printf("Unknown Event\n");
          }
      }
  }

  return 0;
}
