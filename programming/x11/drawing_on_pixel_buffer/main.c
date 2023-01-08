#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

#define WIDTH  800
#define HEIGHT 600 

int pixels[WIDTH*HEIGHT];

void draw() {
  for (int y=0; y<=HEIGHT; y++) {
    for (int x=0; x<=WIDTH; x++) {
      pixels[y*WIDTH + x] = 0x000000ff;
    }
  }
}

int main(){
  Display *display = XOpenDisplay(NULL);
  if(display == NULL){
    fprintf(stderr,"ERROR: Could not open the display\n");
    exit(1);
  }
  Window window = XCreateSimpleWindow(display,XDefaultRootWindow(display),
      0,0,
      WIDTH,HEIGHT,
      0,0,
      0xff);
  GC gc = XCreateGC(display, window, 0, NULL);

  // Doing some weird WM magic
  Atom window_delete = XInternAtom(display,"WM_DELETE_WINDOW",1);
  XSetWMProtocols(display,window,&window_delete,1);

  printf("The atom is %d\n",(int)window_delete);

  XSelectInput(display, window, ExposureMask);
  XMapWindow(display,window);

  // Getting the windows attributes
  XWindowAttributes wa = {0};
  XGetWindowAttributes(display,window,&wa);


  // Creating the image from the buffer
  memset(pixels,0x99,HEIGHT*WIDTH*4);
  XImage  *image = XCreateImage(display, wa.visual, wa.depth, ZPixmap, 0, (char*)pixels, WIDTH, HEIGHT, 32, WIDTH*4);

  if(image == NULL){
    fprintf(stderr,"ERROR: Could not create the image\n");
    exit(1);
  }


  XEvent event;
  int quit = 0;
  while(!quit){
  void draw();
  image = XCreateImage(display, wa.visual, wa.depth, ZPixmap, 0, (char*)pixels, WIDTH, HEIGHT, 32, WIDTH*4);
    int r = XPutImage(display, window, gc, image,
        0, 0,
        0, 0,
        WIDTH,HEIGHT);
    if (XPending(display) <= 0) continue;
    XNextEvent(display,&event);
    switch(event.type){

      case Expose:
        {
          //void draw();
          //image = XCreateImage(display, wa.visual, wa.depth, ZPixmap, 0, (char*)pixels, WIDTH, HEIGHT, 32, WIDTH*4);
          if (event.xexpose.count == 0) {
            printf("we are exposed!!\n");
            // Display the image
            r = XPutImage(display, window, gc, image,
                0, 0,
                0, 0,
                WIDTH,HEIGHT);
            printf("the result is: %d\n",r);

          }
        } break;

      case ClientMessage:
        {
          if((Atom)event.xclient.data.l[0] == window_delete){
            quit = 1;
          }
        }
        break;
      default:
        printf("Unknown event!!\n");
    }
  }
  return 0;
}
