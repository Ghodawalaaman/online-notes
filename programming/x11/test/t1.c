#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h> // Most hated graphic library!!!
#include <X11/Xutil.h>

static char backing_bits[]={
  0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,
  0xff,0x00,0xff,0x00,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,
  0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff
};
//typedef struct {
//    Pixmap background_pixmap;	/* background or None or ParentRelative */
//    unsigned long background_pixel;	/* background pixel */
//    Pixmap border_pixmap;	/* border of the window */
//    unsigned long border_pixel;	/* border pixel value */
//    int bit_gravity;		/* one of bit gravity values */
//    int win_gravity;		/* one of the window gravity values */
//    int backing_store;		/* NotUseful, WhenMapped, Always */
//    unsigned long backing_planes;/* planes to be preserved if possible */
//    unsigned long backing_pixel;/* value to use in restoring planes */
//    Bool save_under;		/* should bits under be saved? (popups) */
//    long event_mask;		/* set of events that should be saved */
//    long do_not_propagate_mask;	/* set of events that should not propagate */
//    Bool override_redirect;	/* boolean value for override-redirect */
//    Colormap colormap;		/* color map to be associated with window */
//    Cursor cursor;		/* cursor to be displayed (or None) */
//} XSetWindowAttributes;

int main() {
  printf("Hello, World\n");
  int screen;
  XWMHints wmh;
  XTextProperty WindowName, IconName;
  char *window_name = "mywindow";
  char *icon_name   = "something";
  Pixmap back;

  Display *dpy = XOpenDisplay(NULL);
  screen = XDefaultScreen(dpy);
  // Setting some attributes for window
  XSetWindowAttributes wa = {0};
  wa.background_pixel = XWhitePixel(dpy,screen);
  wa.border_pixel = XBlackPixel(dpy,screen);
  wa.event_mask = KeyPressMask | ButtonPressMask;

  Window win = XCreateWindow(dpy, XRootWindow(dpy,screen), 0, 0, 800, 600, 0, 
      XDefaultDepth(dpy,screen), InputOutput, XDefaultVisual(dpy,screen), 
      CWBorderPixel | CWBackPixel | CWEventMask, &wa);
  if (win == 0) {
    fprintf(stderr,"ERROR: Could not create window\n");
    exit(1);
  }

  back = XCreatePixmapFromBitmapData(dpy, win,
      (char*)backing_bits, 16, 16,
      //BlackPixel(dpy, screen),
      0xff00ff,
      WhitePixel(dpy, screen),
      XDefaultDepth(dpy, screen));
  XSetWindowBackgroundPixmap(dpy, win, back);
  GC gc = XCreateGC(dpy, win, 0, NULL);

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
          printf("Some key has been pressed\n");
          XBell(dpy, 0);
        } break; 

      case ButtonPress:
        {
          printf("Mouse button has pressed\n");
        } break;

      case ClientMessage:
        {
          if (e.xclient.data.l[0] == wm_quit) {
            run = 0;
          }
        } break;

      case MapNotify:
        {
          printf("Window Created Succesfully\n");
        } break;

      default:
        {
          printf("Unknown Event\n");
        }
    }

  }

  return 0;
}
