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
		
		#ifdef __cplusplus // see if the program runs in c or c++
			#define UTIL_CPP_EXTERN_START extern "C" {
			#define UTIL_CPP_EXTERN_END   }
		#else
			#define UTIL_CPP_EXTERN_START
			#define UTIL_CPP_EXTERN_END
		#endif
		
		UTIL_CPP_EXTERN_START
			#if !defined(int8_t)   || !defined(uint8_t)  || \
				!defined(int16_t)  || !defined(uint16_t) || \
				!defined(int32_t)  || !defined(uint32_t) || \
				!defined(int64_t)  || !defined(uint64_t) || \
				!defined(imax)
					
				#include <stdint.h>
					
			#endif 
			
			// stddef.h dependencies
			#if !defined(size_t)
				#include <stddef.h>
			#endif //stddef.h
		
			//stdbool.h dependencies
			#if !defined(bool) || !defined(true) || !defined(false)
				#include <stdbool.h>
			#endif // stdbool.h
			
			//structs are made differently in c and c++
			#ifndef ZERO_STRUCT
				#ifdef __cplusplus
					#define ZERO_STRUCT(s) {}
				#else
					#define ZERO_STRUCT(s) (s){0}
				#endif
			#endif

			#ifndef ZERO_STRUCT_CONST
				#ifdef __cplusplus
					#define ZERO_STRUCT_CONST(s) {}
				#else
			#define ZERO_STRUCT_CONST(s) {0}
				#endif
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
			
		UTIL_CPP_EXTERN_END
	#endif // UTIL_H
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	UTIL_CPP_EXTERN_START
		#if !defined(malloc) || !defined (free) || !defined(realloc)
			#include <stdlib.h>
		#endif
		
		struct Point {
			int32_t x,y;
			
			Point(uint32_t x, uint32_t y) {
				this->x = x;
				this->y = y;
			}
		};
		typedef struct Point Point;
		struct Point3D {
			int32_t x,y,z;
		};
		typedef struct Point3d Point3d;
		
		struct Line {
			Point p1,p2;
		};
		typedef struct Line Line;
		struct Line3D {
			Point3D p1,p2;
		};
		typedef struct Line3D Line3D;
		
		struct Triangle {
			Point p1,p2,p3;
			Line l1,yl2,l3;
		};
		typedef struct Triangle Triangle;
		struct Triangle3D {
			Point3D p1,p2,p3;
			Line3D l1,yl2,l3;
		};
		typedef struct Triangle3D Triangle3D;
		
		struct RGB {
			uint8_t r,g,b;
		};
		typedef struct RGB RGB;
		struct RGBA {
			uint8_t r,g,b,a;
		};
		typedef struct RGBA RGBA;
		
		enum SHAPE {
			MPWS_POINT,
			MPWS_POINT3D,
			MPWS_LINE,
			MPWS_LINE3D,
			MPWS_TRIANGLE,
			MPWS_TRIANGLE3D,
			MPWS_RGB,
			MPWS_RGBA};
		
		enum RENDERTYPE {
			SINGLE
		};
		
			
	UTIL_CPP_EXTERN_END
#endif // MPWS_H


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef MPWS_IMPLEMENTATION
	UTIL_CPP_EXTERN_START
			
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
				
			#endif /* MPWS_MANUAL_OS_SUPPORT */
			
			#include <cstdio>
			
			
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
			
			//Raster
			struct Raster {
				uint32_t width;
				uint32_t height;
				
				uint8_t* raster;
				uint32_t valperpix; // how many values represent a pixel
				
				void setColor(int x, int y, uint8_t r,uint8_t g, uint8_t b) {
					int i = (y * width + x) * valperpix;
					if (x >= 0 && x < width && y >= 0 && y < height){
						raster[i] = b;
						raster[i+1] = g;
						raster[i+2] = r;
					}
				}
				
				void createRaster(uint32_t w, uint32_t h, uint32_t d) {
					width = w;
					height = h;
					valperpix = d;
					raster = (uint8_t*) malloc((w*h*d) * sizeof(uint8_t));
					for(int i = 0; i < w*h*d; i++) {
						raster[i] = 0;
					}
				}
				
				void clear() {
					for(int i = 0; i < width; i++) {
						for(int j = 0; j < height; j++) {
							setColor(i,j,99,99,99);
						}
					}
				}
				
				void resize(int x, int y) {
					free(raster);
					width = x;
					height = y;
					createRaster(width, height, valperpix);
				}
				
				void drawLine(int x, int y, int xx, int yy, uint8_t r,uint8_t g, uint8_t b){
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
				}
				
				
			};
			typedef struct Raster Raster;
			
			typedef struct {
				uint32_t width;
				uint32_t height;
				const char* name;
				bool isRunning;
				
				void* osSpeciffic;
			} Window;
			
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			#ifdef MPWS_WIN32
				#include <windows.h>
				#include <Windowsx.h>
				
				#define UNICODE
				#define _UNICODE
				#define NUM_KEYS 256
				
				// Hinstance retriever (thank you Raymond Chen)
				EXTERN_C IMAGE_DOS_HEADER __ImageBase;
				HINSTANCE get_hinstance(void) {
					return ((HINSTANCE)&__ImageBase);
				}
				
				typedef struct {
					const char* className;
					WNDCLASS wc_;
				} WindowClass;
				
				WindowClass globalClass;
				
				typedef struct {
					HWND hwnd;
					Raster r;
					BITMAPINFO bmi;
				} W32info;
				
				LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
				
				void create_W32_WindowClass(void) {
					globalClass.className = "WindowClass_";
					
					globalClass.wc_ = {0};
					globalClass.wc_.lpfnWndProc   = WindowProc;
					globalClass.wc_.hInstance     = get_hinstance();
					globalClass.wc_.lpszClassName = globalClass.className;
					globalClass.wc_.hCursor       = LoadCursor(NULL, IDC_ARROW);
					globalClass.wc_.hbrBackground = NULL; // we'll paint ourselves
					RegisterClass(&globalClass.wc_);
				}
				
				void setupWindow(Window* w, uint32_t width, uint32_t height, const char* name) {
					if(globalClass.className == NULL) {
						create_W32_WindowClass();
					}
					w->width = width;
					w->height = height;
					w->name = name;
					w->osSpeciffic = (void*)malloc(sizeof(W32info*));
					W32info* wi = (W32info*)malloc(sizeof(W32info));
					
					wi->hwnd = CreateWindowEx(
						0,
						globalClass.className,
						w->name,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT, CW_USEDEFAULT,
						w->width, w->height,
						NULL, NULL,
						get_hinstance(), w);
					
					wi->r.createRaster(w->width, w->height, 3); // change to 3 for rgbA
					ZeroMemory(&wi->bmi, sizeof(BITMAPINFO));
					wi->bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					wi->bmi.bmiHeader.biWidth = w->width;
					wi->bmi.bmiHeader.biHeight = -w->height; // Negative to indicate a top-down DIB
					wi->bmi.bmiHeader.biPlanes = 1;
					wi->bmi.bmiHeader.biBitCount = 24; // 24-bit bitmap (8r,8g,8b)
					wi->bmi.bmiHeader.biCompression = BI_RGB;
					
					ShowWindow(wi->hwnd, SW_SHOW);
					UpdateWindow(wi->hwnd);
					
					w->osSpeciffic = wi;
					w->isRunning = true;
				}
				
				LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
					Window* w = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

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
				
				bool handleEvents_W32(Window* w, Event* event) {
					if(globalClass.className == NULL) return false;
					W32info* info = (W32info*)w->osSpeciffic;
					
					MSG msg;
					if (!PeekMessage(&msg, info->hwnd, 0,0, PM_REMOVE))  // non-blocking peek :contentReference[oaicite:1]{index=1}
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
				
				void render_W32(Window* w) {
					if(globalClass.className == NULL) return;
					W32info* info = (W32info*)w->osSpeciffic;
					
					HDC hdc = GetDC(info->hwnd);
					HDC memDC = CreateCompatibleDC(hdc);
					
					//load to bitmap
					HBITMAP hBitmap = CreateDIBitmap(
						hdc,
						&info->bmi.bmiHeader,
						CBM_INIT,
						info->r.raster,
						&info->bmi,
						DIB_RGB_COLORS
					);
					
					HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
					BitBlt(hdc, 0, 0, w->width, w->height, memDC, 0, 0, SRCCOPY);
					SelectObject(memDC, oldBitmap); // Restore the old bitmap
					
					DeleteDC(memDC);
					ReleaseDC(info->hwnd, hdc);
				}
				
				void draw_W32(Window* w, int SHAPE, void* ptr) {
					if(globalClass.className == NULL) return;
					W32info* info = (W32info*)w->osSpeciffic;
					
					switch(SHAPE) {
					case MPWS_LINE:
						Line* l = (Line*)ptr;
						info->r.drawLine(l->p1.x, l->p1.y, l->p2.x, l->p2.y, 255, 0, 0);
						break;
					}
				}
				
			#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
			#ifdef MPWS_LINUX
				
				typedef struct {
					
				} Linfo;
				
				
				void create_L_Window() {
					printf("Hi Linux!\n");
				}
				
			#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
			DEF Window createWindow(uint32_t width, uint32_t height, const char* name) {
				#ifdef MPWS_WIN32
					Window w = {0};
					w.width = width;
					setupWindow(&w, width, height, name);
					return w;
				#endif
				#ifdef MPWS_LINUX
				
				#endif
			}
			
			DEF bool handleEvents(Window* w, Event* ev) {
				#ifdef MPWS_WIN32
					return handleEvents_W32(w, ev);
				#endif
				#ifdef MPWS_LINUX
					
				#endif
			}
			
			DEF void display(Window* w) {
				#ifdef MPWS_WIN32
					render_W32(w);
				#endif
				#ifdef MPWS_LINUX
					
				#endif
			}
			
			DEF void draw(Window* w, int SHAPE, void* ptr) {
				#ifdef MPWS_WIN32
					draw_W32(w, SHAPE, ptr);
				#endif
				#ifdef MPWS_LINUX
					
				#endif
			}
			
			DEF void clear(Window* w) {
				#ifdef MPWS_WIN32
					W32info* info = (W32info*)w->osSpeciffic;
					info->r.clear();
				#endif
				#ifdef MPWS_LINUX
					
				#endif
			}
			
			
			DEF bool isOpen(Window w) {
				 return w.isRunning;
			}
			
		UTIL_CPP_EXTERN_END
#endif
