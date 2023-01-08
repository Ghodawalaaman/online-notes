.TL
X11 cheatsheet
.AU
Ghodawala Aman
.NH
How to create a simple window
.PP
First of all you have to open a connection to X server through X11 protocol. To achive that you have to use the folowing function. \fBXOpenDisplay(char* display_name)\fP this function takes display name as argument if you don't have multiple display you can safely pass \fBNULL\fP as argument, It will tell X server to connect to whatever display is available. This fucntion returns a pointer to structure \fBDisplay\fP. It has all the metadata to talk to X server.

After establishing connection to X server, now we can create a simple window. To create a simple window you have to use the following fuction :

\fBXCreateSimpleWindow(Display *display, Window parent, int x, int y, unsigned  int  width,  unsigned  int  height,  unsigned  int  border_width, int depth, unsigned int class,  Visual  *visual,  unsigned long valuemask, XSetWindowAttributes *attributes)\fP 
