#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
     
#include "graphic.h"
     
/*
Voir :
- http://math.msu.su/~vvb/2course/Borisenko/CppProjects/GWindow/xintro.html
- http://www.dis.uniroma1.it/~liberato/screensaver/double.html
*/
     
    static Display *dis;
    static int screen;
    static Window win;
    static GC gc;
    static char XColorNames[G_NB_COLORS][10]={ "red", "green", "blue", "yellow" };
     
    static XColor colors[G_NB_COLORS];
    static Pixmap db;
     
    static unsigned long foreColor,backColor;
     
     
    void g_init() {
    	int i;
    	Colormap screen_colormap; /* color map to use for allocating colors. */
     
    	/* use the information from the environment variable DISPLAY 
    	   to create the X connection:
    	 */	
    	dis=XOpenDisplay((char *)0);
     
    	screen=DefaultScreen(dis);
    	foreColor=BlackPixel(dis, screen);  /* get color black */
    	backColor=WhitePixel(dis, screen);  /* get color white */
     
     
    	/* once the display is initialized, create the window.
    	   It will have the foreground white and background black
    	 */
    	win=XCreateSimpleWindow(dis,DefaultRootWindow(dis),0,0,	
    			G_WIDTH, G_HEIGHT, 5, foreColor, backColor);
     
    	/* get attributes of the root window */
    	XWindowAttributes wa;
    	XGetWindowAttributes(dis, win, &wa);
     
    	db=XCreatePixmap(dis, win, wa.width, wa.height, wa.depth);
     
    	/* here is where some properties of the window can be set.
    	   The third and fourth items indicate the name which appears
    	   at the top of the window and the name of the minimized window
    	   respectively.
    	 */
    	XSetStandardProperties(dis,win,"My Window","HI!",None,NULL,0,NULL);
     
    	/* this routine determines which types of input are allowed in
    	   the input.  see the appropriate section for details...
    	 */
    	XSelectInput(dis, win, ExposureMask|ButtonPressMask|KeyPressMask);
     
    	/* create the Graphics Context */
    	gc=XCreateGC(dis, win, 0,0);        
     
    	/* here is another routine to set the foreground and background
    	   colors _currently_ in use in the window.
    	 */
    	XSetForeground(dis,gc,foreColor);
    	XSetBackground(dis,gc,backColor);
     
    	XSizeHints * size_hints=XAllocSizeHints();
     
    	if(size_hints==NULL)
    	{
    		fprintf(stderr,"XMallocSizeHints() failed\n");
     
    		exit(1);
    	}
     
    	size_hints->flags=PMinSize | PMaxSize;
    	size_hints->min_width=G_WIDTH;
    	size_hints->min_height=G_HEIGHT;
    	size_hints->max_width=G_WIDTH;
    	size_hints->max_height=G_HEIGHT;
     
    	XSetWMNormalHints(dis,win,size_hints);
     
    	/* colors */
     
    	/* get access to the screen's color map. */
     
    	screen_colormap = DefaultColormap(dis, DefaultScreen(dis));
    	for(i=0; i< G_NB_COLORS; ++i) {
    		if( ! XAllocNamedColor(dis, screen_colormap, \
    		   XColorNames[i], &colors[i], &colors[i]) )
    			fprintf(stderr, "XAllocNamedColor - "
    			"failed to allocated %s color.\n", XColorNames[i]),
    			exit(1);
    	}
     
     
    	/* clear the window and bring it on top of the other windows */
    	XClearWindow(dis, win);
    	XMapRaised(dis, win);
    }
     
    void g_close(void) {
    	XFreeGC(dis, gc);
    	XDestroyWindow(dis,win);
    	XCloseDisplay(dis);	
    	printf("Graphic closed ! Bye !\n");
    }
     
    void g_draw(int x, int y, int color) {
    	XSetForeground(dis, gc, colors[color].pixel);
    	XFillArc(dis, db, gc, x, y, 10, 10, 0, 360*64); 
    }
     
    void g_clear(void) {
    	XSetForeground(dis,gc,backColor);
    	XFillRectangle (dis, db, gc, 0, 0, G_WIDTH, G_HEIGHT);
    }
     
    void g_flush(void) {
    	XFlush(dis);
    	XCopyArea(dis, db, win, gc, 0, 0, G_WIDTH, G_HEIGHT, 0, 0);
    }
     
    void g_msleep(int ms) {
    	long nano=ms*1000000L;
    	struct timespec req = { nano/1000000000L, nano%1000000000L };
    	nanosleep(&req, NULL);
    }
     
    void g_srandom(void) {
    	struct timeval tv;
     
     
    	if (gettimeofday(&tv, NULL)) {
    		perror("gettimeofday");
    		exit(1);
    	}
     
    	srandom(tv.tv_usec);
    }
     
    int g_random(void) {
    	return random();
    }

