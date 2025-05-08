// The cpp only version of MPWS (Multi Platform Window System) offering
// more flexibility through the use of classes and FINALLY function overwriting
// getting rid of the need for a function wrapper

#ifndef MPWS_H
#define MPWS_H
	
	#ifndef UTIL_H
	#define UTIL_H
		
		#ifndef DEF
			#ifdef STATIC
				#define DEF static
			#else
				#define DEF extern
			#endif
		#endif
		
		// c++ zero struct implementation
		#ifndef ZERO_STRUCT
			#define ZERO_STRUCT(s) {}
		#endif
		#ifndef ZERO_STRUCT_CONST
				#define ZERO_STRUCT_CONST(s) {}
		#endif
		
		// OS recognition
		#if !defined(UTIL_WIN32) && !defined(UTIL_LINUX)
			#if defined(WIN32) || defined(_WIN32)
				#define UTIL_WIN32
			#endif
			#if defined(__linux__)
				#define UTIL_LINUX
			#endif
		#endif
		
		// enums
		#define UTIL_ENUM(name, ...) enum _##name{__VA_ARGS__};typedef enum _##name _##name;typedef size_t name;
		
		
		// Custom types
		#ifndef  ui8
			#define ui8 unsigned char
		#endif
		
	#endif // MPWS_UTIL_H
	
	struct Point2D {
		int x,y;
		
		Point2D(int x, int y): x(x), y(y) {}
		
	}; typedef struct Point2D Point2D;
	
	
	struct Line2D {
		Point2D p1,p2;
		
		Line2D(Point2D p1, Point2D p2): p1(p1), p2(p2) {}
	}; typedef struct Line2D Line2D;
	
	struct Triangle2D {
		Point2D p1, p2, p3;
	}; typedef struct Triangle2D Triangle2D;
	
	struct Square2D {
		Point2D p1, p2, p3, p4;
	}; typedef struct Square2D Square2D;
	
	
	
	struct Point3D {
		int x,y,z;
		
		Point3D(int x, int y, int z): x(x), y(y), z(z) {}
		
	}; typedef struct Point3D Point3D;
	
	struct Line3D {
		Point3D p1,p2;
	}; typedef struct Line3D Line3D;
	
	
	struct Color {
		ui8 r,g,b,a;
		
		Color(ui8 r, ui8 g, ui8 b):r(r), g(g), b(g) {a = 255;}
		Color(ui8 r, ui8 g, ui8 b, ui8 a):r(r), g(g), b(g), a(a) {}
	}; typedef struct Color Color;
	
	enum SHAPE {
			MPWS_POINT,
			MPWS_LINE,
			MPWS_TRIANGLE,
			MPWS_FILL_TRIANGLE,
			MPWS_SQUARE,
			MPWS_FILL_SQUARE,
			MPWS_POLYGON,
			MPWS_FILL_POLYGON};
	
	
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef MPWS_IMPLEMENTATION
	
	#include <iostream>
	
	#ifndef MPWS_MANUAL_OS_SUPPORT
		#if defined(UTIL_WIN32) || defined(UTIL_LINUX)
			#ifdef UTIL_WIN32
				#define MPWS_WIN32
			#endif
			#ifdef UTIL_LINUX
				#define MPWS_LINUX
			#endif
			#define MPWS_WRAP_CALL(...) __VA_ARGS__
		#else
			#define MPWS_WRAP_CALL(...)
		#endif
	#endif // MPWS_MANUAL_OS_SUPPORT
	
	//Events
	typedef enum {
		EV_NONE,
		EV_KEY_DOWN,
		EV_KEY_UP,
		EV_CLOSE
	} EventType;
	
	typedef struct {
		EventType type;
		int keycode;
		int modifiers;
	} Event;
	
	class Raster {
	// raster only needs to store 3 values for each pixel because it represents
	// the final color being displayed. there is no need to store A
	private:
		ui8* raster;
		int width, height;
	public:
		
		Raster(int width, int height): width(width), height(height) {
			createRaster();
		}
		
		~Raster() {free(raster);}
		
		void createRaster() {
			raster = (ui8*) malloc((width*height*3) * sizeof(ui8));
			for(int i = 0; i < width*height*3; i++) {
				raster[i] = 0;
			}
		}
		
		void resize(int nw, int nh) {
			free(raster);
			width = nw;
			height = nh;
			createRaster();
		}
		
		void setColor(int x, int y, Color c) {
			setColor(x, y, c.r, c.g, c.b);
		}
		
		void setColor(int x, int y, ui8 r,ui8 g, ui8 b) {
			int i = (y * width + x) * 3;
			if (x >= 0 && x < width && y >= 0 && y < height){
				raster[i] = b;
				raster[i+1] = g;
				raster[i+2] = r;
			}
		}
		
		void clear(Color c) {
			for(int i=0; i<width; i++) {
				for(int j=0; j<height; j++) {
					setColor(i, j, c.r, c.g, c.b);
				}
			}
		}void clear() {clear(Color(0,0,0));}
		
		
		void drawLine(int x, int y, int xx, int yy, uint8_t r,uint8_t g, uint8_t b) {
			int dx = abs(xx - x), sx = x < xx ? 1 : -1;
			int dy = -abs(yy - y), sy = y < yy ? 1 : -1; 
			int err = dx + dy, e2; // error value e_xy
			
			while (true) {
				if (x >= 0 && x < width && y >= 0 && y < height) {
					setColor(x, y, r, g, b);
				}
				if (x == xx && y == yy) break;
				e2 = 2 * err;
				if (e2 >= dy) { err += dy; x += sx; }
				if (e2 <= dx) { err += dx; y += sy; }
			}
		}void drawLine(int x, int y, int xx, int yy, Color c) {drawLine(x,y,xx,yy,c.r,c.g,c.b);}
		
		
		ui8* getRaster() {return raster;}
	};
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef MPWS_WIN32
		#include <windows.h>
		#include <Windowsx.h>//for mouse
		
		EXTERN_C IMAGE_DOS_HEADER __ImageBase;
		HINSTANCE get_hinstance(void) {
			return ((HINSTANCE)&__ImageBase);
		}
		
		typedef struct {
			const char* className;
			WNDCLASS wc_;
		} WindowClass;
		WindowClass globalClass;
		
		class Window {
		private:
			int width;
			int height;
			const char* name;
			HWND hwnd;
			Raster r = Raster(0,0);
			BITMAPINFO bmi;
			
			void create_W32_WindowClass(void);
			static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			void setupWindow();
		public:
			bool isRunning;
			
			Window() {
				// std::cout << "Hello Windows!";
				width= 400;
				height = 400;
				name = "default";
				setupWindow();
				isRunning = true;
			}
			
			Window(int w, int h, const char* name): width(w), height(h), name(name) {
				// std::cout << "Hello Windows!";
				setupWindow();
				isRunning = true;
			}
			~Window() {};
			
			void clear() { r.clear();}
			void clear(Color c) { r.clear(c);}
			// void draw(int SHAPE, void* ptr, Color c);
			// void draw(int SHAPE, void* ptr);
			void draw(Point2D p, Color c);
			void draw(Point2D p);
			
			void draw(Line2D l, Color c);
			void draw(Line2D l);
			
			void draw(Triangle2D t, Color c);
			void draw(Triangle2D t);
			
			// void draw();
			// void draw();
			
			// void draw();
			// void draw();
			
			
			void display();
			
			bool handleEvents(Event* event);
			
			bool isOpen() {return isRunning;}
		};
		
		
		void Window::display() {
			if(globalClass.className == NULL) return;
			
			HDC hdc = GetDC(hwnd);
			HDC memDC = CreateCompatibleDC(hdc);
			
			//load to bitmap
			HBITMAP hBitmap = CreateDIBitmap(
				hdc,
				&bmi.bmiHeader,
				CBM_INIT,
				r.getRaster(),
				&bmi,
				DIB_RGB_COLORS
			);
			
			HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
			BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
			SelectObject(memDC, oldBitmap); // Restore the old bitmap
			
			DeleteDC(memDC);
			ReleaseDC(hwnd, hdc);
		}
		
		LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			Window* w = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			switch (uMsg) {
				case WM_CREATE: {
					SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)w);
					return 0;
				}
				case WM_ERASEBKGND:
					return 1;
				case WM_DESTROY:
					w->isRunning = false;
					return 0;
				}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		
		void Window::create_W32_WindowClass(void) {
			globalClass.className = "WindowClass_";
			
			globalClass.wc_ = {0};
			globalClass.wc_.lpfnWndProc   = WindowProc;
			globalClass.wc_.hInstance     = get_hinstance();
			globalClass.wc_.lpszClassName = globalClass.className;
			globalClass.wc_.hCursor       = LoadCursor(NULL, IDC_ARROW);
			globalClass.wc_.hbrBackground = NULL; // we'll paint ourselves
			RegisterClass(&globalClass.wc_);
			// std::cout << "\nClass registered\n";
		}
		
		void Window::setupWindow() {
			if(globalClass.className == NULL) {
				create_W32_WindowClass();
			}
			
			hwnd = CreateWindowEx(
				0,
				globalClass.className,
				name,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				width, height,
				NULL, NULL,
				get_hinstance(),
				nullptr);
			
			int sW = GetSystemMetrics(SM_CXSCREEN);
			int sH = GetSystemMetrics(SM_CYSCREEN);
			int xPos = (sW - width) / 2;
			int yPos = (sH - height) / 2;
			SetWindowPos(hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			
			r.resize(width, height);
			ZeroMemory(&bmi, sizeof(BITMAPINFO));
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = width;
			bmi.bmiHeader.biHeight = -height; // Negative to indicate a top-down DIB
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 24; // 24-bit bitmap
			bmi.bmiHeader.biCompression = BI_RGB;
			
			ShowWindow(hwnd, SW_SHOWNORMAL);
			UpdateWindow(hwnd);
		}
		
		
		bool Window::handleEvents(Event* event) {
			if(globalClass.className == NULL) return false;
			
			MSG msg;
			if (!PeekMessage(&msg, hwnd, 0,0, PM_REMOVE))  // non-blocking peek :contentReference[oaicite:1]{index=1}
				return false;
			// TranslateMessage(&msg);
			switch (msg.message) {
				case WM_KEYDOWN:
				case WM_SYSKEYDOWN:
					event->type     = EV_KEY_DOWN;
					event->keycode  = (int)msg.wParam;
					event->modifiers = 
						(GetKeyState(VK_SHIFT)   < 0 ?1:0) |
						(GetKeyState(VK_CONTROL) < 0 ?2:0) |
						(GetKeyState(VK_MENU)    < 0 ?4:0);
					break;
				
				case WM_KEYUP:
				case WM_SYSKEYUP:
					event->type     = EV_KEY_UP;
					event->keycode  = (int)msg.wParam;
					event->modifiers = 0;
					break;

				case WM_CLOSE:
				case WM_QUIT:
					event->type = EV_CLOSE;
					break;
				default:
					//ignore other messages
					// return handleEvents_W32(index,event);
					DispatchMessage(&msg);
			}
			return true;
		}
		
		// void Window::draw(int SHAPE, void* ptr, Color c) {
		// 	switch(SHAPE) {
		// 	case MPWS_LINE: {
		// 		Line* l = (Line*)ptr;
		// 		r.drawLine(l->p1.x, l->p1.y, l->p2.x, l->p2.y, c.r, c.g, c.b);
		// 	} break;
		// 	case MPWS_POINT: {
		// 		Point2D* p = (Point2D*)ptr;
		// 		r.setColor(p->x, p->y, c.r, c.g, c.b);
		// 	} break;
		// 	}
		// } void Window::draw(int SHAPE, void* ptr) {draw(SHAPE, ptr, Color(255,0,0));}
		
		void Window::draw(Point2D p, Color c) {
			r.setColor(p.x, p.y, c.r, c.g, c.b);
		}void Window::draw(Point2D p) {draw(p, Color(255,0,0));}
		
		void Window::draw(Line2D l, Color c) {
			r.drawLine(l.p1.x, l.p1.y, l.p2.x, l.p2.y, c.r, c.g, c.b);
		}void Window::draw(Line2D l) {draw(l,Color(255,0,0));}
		
		void Window::draw(Triangle2D t, Color c) {
			r.drawLine(t.p1.x, t.p1.y, t.p2.x, t.p2.y, c.r, c.g, c.b);
			r.drawLine(t.p2.x, t.p2.y, t.p3.x, t.p3.y, c.r, c.g, c.b);
			r.drawLine(t.p3.x, t.p3.y, t.p1.x, t.p1.y, c.r, c.g, c.b);
		}void Window::draw(Triangle2D t) {draw(t,Color(255,0,0));}
		
	#endif // WIN32
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef MPWS_LINUX
		
		class Window {
		public:
			
			Window() {
				std::cout << "Hello Linux!";
			};
			~Window() {};
		};
		
	#endif // Linux
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
#endif //implementation
