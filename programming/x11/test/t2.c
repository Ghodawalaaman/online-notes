#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(void) {
  Display *dpy;
  Screen *scr;
  int scr_num;

  dpy = XOpenDisplay(NULL);
  scr_num = XDefaultScreen(dpy);
  scr = XScreenOfDisplay(dpy, scr_num);
  
  if (DoesSaveUnders(scr)) {
    printf("Does screen unders\n");
  }
  else {
    printf("Does not provide screen unders\n");
  }

  switch (DoesBackingStore(scr))
  {
    case WhenMapped:
      {
        printf("Backing store provided when window is mapped\n");
        break;
      }

    case Always:
      {
        printf("Backing store is always provided\n");
        break;
      }

    case NotUseful:
      {
        printf("Does not provide backing store\n");
        break;
      }
  }
  printf("Default depth of screen: %d\n",XDefaultDepth(dpy,scr_num));
  return 0;
}
