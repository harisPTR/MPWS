// The cpp only version of MPWS (Multi Platform MPWS_WINDOW System) offering
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
		#ifndef u8
			#define u8 unsigned char
		#endif
		#ifndef i8
			#define i8 char
		#endif
		#ifndef u16
			#define u16 unsigned short
		#endif
		#ifndef i16
			#define i16 short
		#endif
		#ifndef u32
			#define u32 unsigned int
		#endif
		#ifndef i32
			#define i32 int
		#endif
		#ifndef i64
			#define i64 uint64_t
		#endif
		#ifndef f32
			#define f32 float
		#endif
		#ifndef f64
			#define f64 double
		#endif	
	#endif // MPWS_UTIL_H
	
	struct Point2D {
		i32 x,y;
		
		Point2D(i32 x, i32 y): x(x), y(y) {}
		
		Point2D operator+(const Point2D& other) const { return Point2D(x + other.x, y + other.y); }
		Point2D operator-(const Point2D& other) const { return Point2D(x - other.x, y - other.y); }
	}; typedef struct Point2D Point2D;
	
	struct Floint2D {
		f32 x,y;
		
		Floint2D() : x(0), y(0) {}
		Floint2D(f32 x, f32 y): x(x), y(y) {}
		
		Floint2D operator+(const Floint2D& other) const { return Floint2D(x + other.x, y + other.y); }
		Floint2D operator-(const Floint2D& other) const { return Floint2D(x - other.x, y - other.y); }
		
		operator Point2D() const {
			return Point2D(static_cast<i32>(x),static_cast<i32>(y)); // Cast the f32 to i32
		}
	}; typedef struct Floint2D Floint2D;
	
	struct Line2D {
		Point2D p0,p1;
		
		Line2D(Point2D p0, Point2D p1): p0(p0), p1(p1) {}
	}; typedef struct Line2D Line2D;
	
	struct Triangle2D {
		Point2D p0, p1, p2;
	}; typedef struct Triangle2D Triangle2D;
	
	struct Square2D {
		Point2D p0, p1, p2, p4;
	}; typedef struct Square2D Square2D;
	
	
	
	struct Point3D {
		i32 x,y,z;
		
		Point3D(i32 x, i32 y, i32 z): x(x), y(y), z(z) {}
		
		Point3D operator+(const Point3D& other) const { return Point3D(x + other.x, y + other.y, z + other.z); }
		Point3D operator-(const Point3D& other) const { return Point3D(x - other.x, y - other.y, z - other.z); }
	}; typedef struct Point3D Point3D;
	
	struct Floint3D {
		f32 x,y,z;
		
		Floint3D(f32 x, f32 y, f32 z): x(x), y(y), z(z) {}
		
		Floint3D operator+(const Floint3D& other) const { return Floint3D(x + other.x, y + other.y, z + other.z); }
		Floint3D operator-(const Floint3D& other) const { return Floint3D(x - other.x, y - other.y, z - other.z); }
		
		operator Point3D() const {
			return Point3D(static_cast<i32>(x),static_cast<i32>(y),static_cast<i32>(z)); // Cast the f32 to i32
		}
	}; typedef struct Floint3D Floint3D;
	
	struct Line3D {
		Point3D p0,p1;
	}; typedef struct Line3D Line3D;
	
	
	struct Color {
		u8 r,g,b,a;
		
		Color(u8 r, u8 g, u8 b):r(r), g(g), b(g) {a = 255;}
		Color(u8 r, u8 g, u8 b, u8 a):r(r), g(g), b(g), a(a) {}
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

	enum KeyCharacter {
		KEY_UNKNOWN,
		Key_A, Key_B, Key_C,
		Key_D, Key_E, Key_F,
		Key_G, Key_H, Key_I,
		Key_J, Key_K, Key_L,
		Key_M, Key_N, Key_O,
		Key_P, Key_Q, Key_R,
		Key_S, Key_T, Key_U,
		Key_V, Key_W, Key_X,
		Key_Y, Key_Z,

		Key_0, Key_1, Key_2, Key_3, Key_4,
		Key_5, Key_6, Key_7, Key_8, Key_9,

		Key_ESCAPE, Key_ENTER, Key_BACKSPACE, Key_TAB,
		Key_SPACE, Key_LEFT, Key_RIGHT, Key_UP,
		Key_DOWN, Key_CTRL, Key_SHIFT, Key_ALT
	};
	
	i32 cmp_floats(const void *a, const void *b) {
		f32 f_a = *(f32 *)a;
		f32 f_b = *(f32 *)b;

		if (f_a < f_b) return -1;
		else if (f_a > f_b) return 1;

		return 0;
	}
	
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef MPWS_IMPLEMENTATION
	
	#include <iostream>
	#include <thread>
	#include <atomic>
	#include <vector>
	#include <cstring>
	#include <unordered_map>
//file reading
	#define READ_BE16(mem) ((((u8*)(mem))[0] << 8) | (((u8*)(mem))[1]))
	#define READ_BE32(mem) ((((u8*)(mem))[0] << 24) | (((u8*)(mem))[1] << 16) | (((u8*)(mem))[2] << 8) | (((u8*)(mem))[3]))
	#define P_MOVE(mem, a) ((mem) += (a))

	#define READ_BE16_MOVE(mem) (READ_BE16((mem))); (P_MOVE((mem), 2))
	#define READ_BE32_MOVE(mem) (READ_BE32((mem))); (P_MOVE((mem), 4))

	#define FONT_TAG(a, b, c, d) (a<<24|b<<16|c<<8|d)
	#define GLYF_TAG FONT_TAG('g', 'l', 'y', 'f')
	#define LOCA_TAG FONT_TAG('l', 'o', 'c', 'a')
	#define HEAD_TAG FONT_TAG('h', 'e', 'a', 'd')
	#define CMAP_TAG FONT_TAG('c', 'm', 'a', 'p')
	#define OS2_TAG FONT_TAG('O', 'S', '/', '2')
//structs
	typedef struct {
		u32	scaler_type;
		u16	numTables;
		u16	searchRange;
		u16	entrySelector;
		u16	rangeShift;
	} offset_subtable;

	typedef struct {
		u16 platformID;
		u16 platformSpecificID;
		u32 offset;
	} cmap_encoding_subtable;

	typedef struct {
		u16 version;
		u16 numberSubtables;
		cmap_encoding_subtable* subtables;
	} cmap;

	typedef struct {
		u16  format;
	 	u16  length;
	 	u16  language;
	 	u16  segCountX2;
	 	u16  searchRange;
	 	u16  entrySelector;
	 	u16  rangeShift;
		u16  reservedPad;
		u16  *endCode;
		u16  *startCode;
		u16  *idDelta;
		u16  *idRangeOffset;
		u16  *glyphIdArray;
	} format4;

	typedef struct {
		union { 
			char tag_c[4];
			u32	tag;
		};
		u32	checkSum;
		u32	offset;
		u32	length;
	} table_directory;

	typedef struct  {
		offset_subtable off_sub;
		table_directory* tbl_dir;
		format4* f4;
		cmap* cmap;
		char* glyf;
		char* loca;
		char* head;
		char* os2;
	} font_directory; 

	typedef union {
		struct {
			u8 on_curve: 1;
			
			u8 x_short: 1;
			u8 y_short: 1;
			
			u8 repeat: 1;
			
			u8 x_short_pos: 1;
			u8 y_short_pos: 1;
			
			u8 reserved1: 1;
			u8 reserved2: 1;
		};
		u8 flag;
	} glyph_flag;

	typedef struct {
		u16 numberOfContours;
		i16 xMin;
		i16 yMin;
		i16 xMax;
		i16 yMax;
		u16 instructionLength;
		u8* instructions;
		glyph_flag* flags;
		i16* xCoordinates;
		i16* yCoordinates;
		u16* endPtsOfContours;
	} glyph_outline;

	typedef struct {
	    u16 majorVersion, minorVersion;
	    i16 fontRevisionMajor, fontRevisionMinor;
	    u32 checkSumAdjustment, magicNumber;
	    u16 flags;
	    u16 unitsPerEm;
	    i64 created, modified;
	    i16 xMin, yMin, xMax, yMax;
	    u16 macStyle, lowestRecPPEM;
	    i16 fontDirectionHint, indexToLocFormat, glyphDataFormat;
	} head_table;

	typedef struct {
	    u16 version;
	    /* … many fields … */
	    i16 sTypoAscender;
	    i16 sTypoDescender;
	    i16 sTypoLineGap;
	    u16 usWinAscent;
	    u16 usWinDescent;
	    /* … */
	} os2_table;

	#ifndef MPWS_MANUAL_OS_SUPPORT
		#ifdef UTIL_WIN32
				#define MPWS_WIN32
			#endif
			#ifdef UTIL_LINUX
				#define MPWS_LINUX
			#endif
	#endif // MPWS_MANUAL_OS_SUPPORT
	
	//Events
	typedef enum {
		EV_NONE,
		EV_KEY_DOWN,
		EV_KEY_UP,
		EV_MOVE,
		EV_RESIZE,
		EV_CLOSE
	} EventType;
	
	typedef struct {
		EventType type;
		KeyCharacter keycode;
		i32 modifiers;
		i32 moveX;
		i32 moveY;
		i32 resizeWidth;
		i32 resizeHeight;
	} Event;
//classes

	class Raster {
	// raster only needs to store 3 values for each pixel because it represents
	// the final color being displayed. there is no need to store A
	// but for compatibility with linux 4 values will be used
	// the Alpha value gets set to 0 during initialization and it never changes
	private:
		u8* raster;
		i32 width, height;
		i32 valPerPix = 4;
	public:
		
		Raster(i32 width, i32 height): width(width), height(height) {
			createRaster();
		}
		
		~Raster() {free(raster);}
		
		void createRaster() {
			i32 size = width * height * valPerPix;
			raster = (u8*) calloc(size, sizeof(u8));
		}
		
		void resize(i32 nw, i32 nh) {
			free(raster);
			width = nw;
			height = nh;
			createRaster();
		}
		
		void setColor(i32 x, i32 y, u8 r,u8 g, u8 b) {
			i32 i = (y * width + x) * valPerPix;
			if (x >= 0 && x < width && y >= 0 && y < height){
				raster[i] = b;
				raster[i+1] = g;
				raster[i+2] = r;
			}
		}void setColor(i32 x, i32 y, Color c) {setColor(x, y, c.r, c.g, c.b);}
		
		void clearChunk(i32 yStart, i32 yEnd, Color c) {
			const i32 rowSize = width * valPerPix;
			for (i32 y = yStart; y < yEnd; ++y) {
				i32 rowStart = y * rowSize;
				for (i32 x = 0; x < width; ++x) {
					i32 index = rowStart + x * valPerPix;
					raster[index + 0] = c.r;
					raster[index + 1] = c.g;
					raster[index + 2] = c.b;
				}
			}
		}
		
		void clear(Color c) {
			i32 numThreads = std::thread::hardware_concurrency();
			std::vector<std::thread> threads;

			for (i32 t = 0; t < numThreads; ++t) {
				i32 yStart = (height * t) / numThreads;
				i32 yEnd = (height * (t + 1)) / numThreads;
				threads.emplace_back([=]() {
					this->clearChunk(yStart, yEnd, c);
				});
			}

			for (auto& thread : threads) thread.join();
		}void clear() {clear(Color(0,0,0));}
		
		
		void drawLine(i32 x, i32 y, i32 xx, i32 yy, u8 r,u8 g, u8 b) {
			i32 dx = abs(xx - x), sx = x < xx ? 1 : -1;
			i32 dy = -abs(yy - y), sy = y < yy ? 1 : -1; 
			i32 err = dx + dy, e2; // error value e_xy
			
			while (true) {
				if (x >= 0 && x < width && y >= 0 && y < height) {
					setColor(x, y, r, g, b);
				}
				if (x == xx && y == yy) break;
				e2 = 2 * err;
				if (e2 >= dy) { err += dy; x += sx; }
				if (e2 <= dx) { err += dx; y += sy; }
			}
		}void drawLine(i32 x, i32 y, i32 xx, i32 yy, Color c) {drawLine(x,y,xx,yy,c.r,c.g,c.b);}


		void fillTriangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, u8 r,u8 g, u8 b) {
			//sort the vertices by y-coordinates
			auto sortVerticesByY = [](i32& x1, i32& y1, i32& x2, i32& y2, i32& x3, i32& y3) {
				if(y1 > y2) { std::swap(x1, x2); std::swap(y1,y2); }
				if(y1 > y3) { std::swap(x1, x3); std::swap(y1,y1); }
				if(y2 > y3) { std::swap(x2, x3); std::swap(y2,y3); }
			};

			sortVerticesByY(x0,y0,x1,y1,x2,y2);

			i32 xTop = x0, yTop = y0;
			i32 xMid = x1, yMid = y1;
			i32 xBottom = x2, yBottom = y2;

			//Function to interpolate x-coordinates on a scanline
			auto interpolateX = [](i32 x1, i32 y1, i32 x2, i32 y2, i32 y) -> i32 {
				if(y1 == y2) return x1;
				return x1 + (x2 - x1) * (y - y1) / (y2 - y1);
			};

			//Draw the filled triangle
			for(i32 y = yTop; y <= yBottom; y++) {
				i32 x1,x2;

				//Determmine x-coordinates of the intersection points
				if(y < yMid) {
					x1 = interpolateX(xTop, yTop, xBottom, yBottom, y);
					x2 = interpolateX(xTop, yTop, xMid, yMid, y);
				} else {
					x1 = interpolateX(xMid, yMid, xBottom, yBottom, y);
					x2 = interpolateX(xTop, yTop, xBottom, yBottom, y);
				}

				//Ensure x1 is always the leftmost x-coordinate
				if(x1 > x2) std::swap(x1,x2);

				//Draw horizontal line between x1 and x2
				for(i32 x = x1; x <= x2; ++x) {
					setColor(x, y, r, g, b);
				}
			}
		}void fillTriangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, Color c) {fillTriangle(x0,y0,x1,y1,x2,y2,c.r,c.g,c.b);}

		
		i32 size() {return width * height * valPerPix;}
		u8* getRaster() {return raster;}
	};



	class Window_common {
	public:
		i32 width;
		i32 height;
		i32 posX = 0;
		i32 posY = 0;
		const i8* name;
		Raster r = Raster(0,0);
		
		i32 getWidht() {return width;}
		i32 getHeight() {return height;}
		i32 getPosX() {return posX;}
		i32 getPosY() {return posY;}
		
	//font logic
		// Parsed TrueType tables
		font_directory  fontDir_;
		head_table *head_ = nullptr;
		os2_table *os2_ = nullptr;
		i8* loca_ = nullptr;
		
		u16 cmapSegCount_ = 0;
		const u16 *cmapEndCode_ = nullptr;
		const u16 *cmapStartCode_ = nullptr;

		// Scaling
		f32 scale_ = 1.0f;
		f32 baselinePx_ = 0.0f;
		u16 unitsPerEm_ = 0;
		
		// Cached glyph outlines
		std::unordered_map<u8, glyph_outline> glyphMap_;
		
		// Cached letter bitmaps based of font size and character index
		std::unordered_map<int, std::unordered_map<i32, u8*>> letterBitmaps;
		
		
		i8* read_file(const i8 *file_name, i32* file_size) { 
			if(strlen(file_name) > 0) {
				FILE* file = fopen(file_name, "rb");
				if(file) {
					fseek(file, 0, SEEK_END);
					i32 size = ftell(file);
					fseek(file, 0, SEEK_SET);

					if(file_size) { *file_size = size; }
					i8 *file_content = (i8*)malloc(size+1);
					i32 read_amount = fread(file_content, size, 1, file);
					file_content[size] = '\0';
					if(read_amount) {
						fclose(file);
						return file_content;
					}
					free(file_content);
					fclose(file);
					return NULL;
				}
			}
			return NULL;
		}
		
		void read_cmap(i8* mem, cmap* c) {
			i8 *m = mem;
			c->version = READ_BE16_MOVE(m);
			c->numberSubtables = READ_BE16_MOVE(m);

			c->subtables = (cmap_encoding_subtable*) calloc(1, sizeof(cmap_encoding_subtable)*c->numberSubtables);
			for(i32 i = 0; i < c->numberSubtables; ++i) {
				cmap_encoding_subtable* est = c->subtables + i;
				est->platformID = READ_BE16_MOVE(m);
				est->platformSpecificID = READ_BE16_MOVE(m);
				est->offset = READ_BE32_MOVE(m);
			}
		}
		
		void read_format4(i8* mem, format4** format) {
			i8* m = mem;

			u16 length = READ_BE16(m + 2);

			format4* f = NULL;

			f = (format4*) calloc(1, length + sizeof(u16*)*5);
			f->format = READ_BE16_MOVE(m);
			f->length = READ_BE16_MOVE(m);
			f->language = READ_BE16_MOVE(m);
			f->segCountX2 = READ_BE16_MOVE(m);
			f->searchRange = READ_BE16_MOVE(m);
			f->entrySelector = READ_BE16_MOVE(m);
			f->rangeShift = READ_BE16_MOVE(m);

			f->endCode = (u16*) ((u8*)f  + sizeof(format4));
			f->startCode = f->endCode + f->segCountX2/2;
			f->idDelta = f->startCode + f->segCountX2/2;
			f->idRangeOffset = f->idDelta + f->segCountX2/2;
			f->glyphIdArray = f->idRangeOffset + f->segCountX2/2;

			i8* start_code_start = m + f->segCountX2 + 2;
			i8* id_delta_start = m + f->segCountX2*2 + 2;
			i8* id_range_start = m + f->segCountX2*3 + 2;

			for(i32 i = 0; i < f->segCountX2/2; ++i) {
				f->endCode[i] = READ_BE16(m + i*2);
				f->startCode[i] = READ_BE16(start_code_start + i*2);
				f->idDelta[i] = READ_BE16(id_delta_start + i*2);
				f->idRangeOffset[i] = READ_BE16(id_range_start + i*2);
			}

			P_MOVE(m, f->segCountX2*4 + 2);	

			i32 remaining_bytes = f->length - (m - mem);
			for(i32 i = 0; i < remaining_bytes/2; ++i) {
				f->glyphIdArray[i] = READ_BE16_MOVE(m);
			}

			*format = f;
		}
		
		void read_offset_subtable(i8** mem, offset_subtable* off_sub) {
			i8* m = *mem;
			off_sub->scaler_type = READ_BE32_MOVE(m);
			off_sub->numTables = READ_BE16_MOVE(m);
			off_sub->searchRange = READ_BE16_MOVE(m);
			off_sub->entrySelector = READ_BE16_MOVE(m);
			off_sub->rangeShift = READ_BE16_MOVE(m);

			*mem = m;
		}
		
		void read_table_directory(i8* file_start, i8** mem, font_directory* ft) {
			i8* m = *mem;
			ft->tbl_dir = (table_directory*)calloc(1, sizeof(table_directory)*ft->off_sub.numTables);

			for(i32 i = 0; i < ft->off_sub.numTables; ++i) {
				table_directory* t = ft->tbl_dir + i;
				t->tag = READ_BE32_MOVE(m);
				t->checkSum = READ_BE32_MOVE(m);
				t->offset = READ_BE32_MOVE(m);
				t->length = READ_BE32_MOVE(m);

				switch(t->tag) {
					case GLYF_TAG: ft->glyf = t->offset + file_start; break;
					case LOCA_TAG: ft->loca = t->offset + file_start; break;
					case HEAD_TAG: ft->head = t->offset + file_start; break;
					case CMAP_TAG: {
						ft->cmap = (cmap*) calloc(1, sizeof(cmap));
						read_cmap(file_start + t->offset, ft->cmap);
						read_format4(file_start + t->offset + ft->cmap->subtables[0].offset, &ft->f4);
					} break;
					case OS2_TAG:  ft->os2  = t->offset + file_start; break;
				}
			}

			*mem = m;
		}
		
		void read_font_directory(i8* file_start, i8** mem, font_directory* ft) {
			read_offset_subtable(mem, &ft->off_sub); 
			read_table_directory(file_start, mem, ft);
		}
		
		i32 read_loca_type(font_directory* ft) {
			return READ_BE16(ft->head + 50);
		}
		
		void parseTables() {
			// Head table must have been set by read_font_directory
			if (!fontDir_.head)
				throw std::runtime_error("No head table found");
			head_ = reinterpret_cast<head_table*>(fontDir_.head);

			// OS/2 table pointer
			if (!fontDir_.os2)
				throw std::runtime_error("No OS/2 table found");
			os2_ = reinterpret_cast<os2_table*>(fontDir_.os2);
			
			// OS/2 table pointer
			if (!fontDir_.loca)
				throw std::runtime_error("No loca table found");
			loca_ = reinterpret_cast<char*>(fontDir_.loca);

			// cmap format4 data prepared in read_font_directory
			if (!fontDir_.cmap || !fontDir_.f4) {
				cmapSegCount_   = 0;
				cmapEndCode_    = nullptr;
				cmapStartCode_  = nullptr;
				return;
			}

			format4* f4 = fontDir_.f4;
			cmapSegCount_   = f4->segCountX2 / 2;
			cmapEndCode_    = f4->endCode;
			cmapStartCode_  = f4->startCode;
		}
		
		i32 get_glyph_index(font_directory* ft, u16 code_point) {
			format4 *f = ft->f4;
			i32 index = -1;
			u16 *ptr = NULL;
			for(i32 i = 0; i < f->segCountX2/2; i++) {
				if(f->endCode[i] > code_point) {index = i; break;};
			}
			
			if(index == -1) return 0;

			if(f->startCode[index] < code_point) {
				if(f->idRangeOffset[index] != 0) {
					ptr = f->idRangeOffset + index + f->idRangeOffset[index]/2;
					ptr += code_point - f->startCode[index];
					if(*ptr == 0) return 0;
					return (i16) (*ptr + f->idDelta[index]);
				} else {
					return (i16) (code_point + f->idDelta[index]);
				}
			}

			return 0;
		}
		
		u32 get_glyph_offset(font_directory *ft, u32 glyph_index) {
			u32 offset = 0;
			if(read_loca_type(ft)) {
				//32 bit
				offset = READ_BE32((u32*)ft->loca + glyph_index);
			} else {
				offset =  READ_BE16((u16*)ft->loca + glyph_index)*2;
			}
			return offset;
		}
		
		glyph_outline deep_copy_glyph_outline(const glyph_outline& src) {
			glyph_outline dst = src;

			i32 pointCount = src.endPtsOfContours[src.numberOfContours - 1] + 1;

			dst.endPtsOfContours = (u16*)calloc(src.numberOfContours, sizeof(u16));
			memcpy(dst.endPtsOfContours, src.endPtsOfContours, src.numberOfContours * sizeof(u16));

			dst.instructions = (u8*)calloc(src.instructionLength, 1);
			memcpy(dst.instructions, src.instructions, src.instructionLength);

			dst.flags = (glyph_flag*)calloc(pointCount, sizeof(glyph_flag));
			memcpy(dst.flags, src.flags, pointCount * sizeof(glyph_flag));

			dst.xCoordinates = (i16*)calloc(pointCount, sizeof(i16));
			memcpy(dst.xCoordinates, src.xCoordinates, pointCount * sizeof(i16));

			dst.yCoordinates = (i16*)calloc(pointCount, sizeof(i16));
			memcpy(dst.yCoordinates, src.yCoordinates, pointCount * sizeof(i16));

			return dst;
		}

		
		glyph_outline get_glyph_outline(font_directory* ft, u32 glyph_index) {
			u32 offset = get_glyph_offset(ft, glyph_index);
			unsigned char* glyph_start = (unsigned char*)(ft->glyf + offset);
			glyph_outline outline = {0};
			outline.numberOfContours = READ_BE16_MOVE(glyph_start);
			outline.xMin = READ_BE16_MOVE(glyph_start);
			outline.yMin = READ_BE16_MOVE(glyph_start);
			outline.xMax = READ_BE16_MOVE(glyph_start);
			outline.yMax = READ_BE16_MOVE(glyph_start);

			outline.endPtsOfContours = (u16*) calloc(1, outline.numberOfContours*sizeof(u16));
			for(i32 i = 0; i < outline.numberOfContours; ++i) {
				outline.endPtsOfContours[i] = READ_BE16_MOVE(glyph_start);
			}

			outline.instructionLength = READ_BE16_MOVE(glyph_start);
			outline.instructions = (u8*)calloc(1, outline.instructionLength);
			memcpy(outline.instructions, glyph_start, outline.instructionLength);
			P_MOVE(glyph_start, outline.instructionLength);

			i32 last_index = outline.endPtsOfContours[outline.numberOfContours-1];
			outline.flags = (glyph_flag*) calloc(1, last_index + 1);

			for(i32 i = 0; i < (last_index + 1); ++i) {
				outline.flags[i].flag = *glyph_start;
				glyph_start++;
				if(outline.flags[i].repeat) {
					u8 repeat_count = *glyph_start;
					while(repeat_count-- > 0) {
						i++;
						outline.flags[i] = outline.flags[i-1];
					}
					glyph_start++;
				}
			}


			outline.xCoordinates = (i16*) calloc(1, (last_index+1)*2);
			i16 prev_coordinate = 0;
			i16 current_coordinate = 0;
			for(i32 i = 0; i < (last_index+1); ++i) {
				i32 flag_combined = outline.flags[i].x_short << 1 | outline.flags[i].x_short_pos;
				switch(flag_combined) {
					case 0: {
						current_coordinate = READ_BE16_MOVE(glyph_start);
					} break;
					case 1: { current_coordinate = 0; }break;
					case 2: { current_coordinate = (*glyph_start++)*-1; }break;
					case 3: { current_coordinate = (*glyph_start++); } break;
				}

				outline.xCoordinates[i] = current_coordinate + prev_coordinate;
				prev_coordinate = outline.xCoordinates[i];
			}

			outline.yCoordinates = (i16*) calloc(1, (last_index+1)*2);
			current_coordinate = 0;
			prev_coordinate = 0;
			for(i32 i = 0; i < (last_index+1); ++i) {
				i32 flag_combined = outline.flags[i].y_short << 1 | outline.flags[i].y_short_pos;
				switch(flag_combined) {
					case 0: {
						current_coordinate = READ_BE16_MOVE(glyph_start);
					} break;
					case 1: { current_coordinate = 0; }break;
					case 2: { current_coordinate = (*glyph_start++)*-1; }break;
					case 3: { current_coordinate = (*glyph_start++); } break;
				}

				outline.yCoordinates[i] = current_coordinate + prev_coordinate;
				prev_coordinate = outline.yCoordinates[i];
			}

			return outline;
		}
		
		void tessellate_bezier(Floint2D *output, i32 *output_size, Floint2D p0, Floint2D p1, Floint2D p2) {
			i32 subdiv_into = 5;
			f32 step_per_iter = 1.0/subdiv_into;
			i32 out_size = 0;
			output[out_size++] = p0;
			for(int i = 1; i <= subdiv_into; i++) {
				f32 t = i*step_per_iter;
				f32 t1 = (1.0 - t);
				f32 t2 = t*t;
				f32 x = t1*t1*p0.x + 2*t1*t*p1.x + t2*p2.x;
				f32 y = t1*t1*p0.y + 2*t1*t*p1.y + t2*p2.y;
				output[out_size].x = x;
				output[out_size].y = y;
				out_size++;
			}
			
			*output_size = out_size;
		}

		void generate_points(glyph_outline *outline, Floint2D *generated_points, i32 *point_count, i32 *gen_pts_end_indices) {
			i32 j = 0;      // global index for the outline points
			i32 index = 0;  // index for generated_points

			// Process each contour
			for (i32 i = 0; i < outline->numberOfContours; i++) {
				i32 contour_start_index = j;
				i32 generated_points_start_index = index;
				bool contour_started_off = false;
				// The endPtsOfContours array holds the last valid index (inclusive) for the contour.
				i32 contour_end = outline->endPtsOfContours[i];

				// Process points in the current contour
				// Loop through inclusive indices of this contour.
				for (; j <= contour_end; j++) {
					// Compute next index with wrap–around within the current contour.
					// If we are at the end, wrap back to the start of the contour.
					i32 next_index = (j < contour_end) ? (j + 1) : contour_start_index;

					// Get current point data.
					glyph_flag flag = outline->flags[j];
					i32 x = outline->xCoordinates[j];
					i32 y = outline->yCoordinates[j];

					if (flag.on_curve) {
						// On–curve: simply add the current point.
						generated_points[index].x = (f32)x;
						generated_points[index].y = (f32)y;
						index++;
					} else {
						// Off–curve: need to handle differently.
						if (j == contour_start_index) {
							// If the very first point of the contour is off–curve,
							// we mark that this contour started off and check the next point.
							contour_started_off = true;
							if (outline->flags[next_index].on_curve) {
								// If next point is on curve, add that point directly and skip it.
								generated_points[index].x = (f32)outline->xCoordinates[next_index];
								generated_points[index].y = (f32)outline->yCoordinates[next_index];
								index++;
								j++; // Skip the next point since it was already processed.
								continue;
							} else {
								// Otherwise, compute an implicit on–curve point at the midpoint.
								x = x + (outline->xCoordinates[next_index] - x) / 2.0;
								y = y + (outline->yCoordinates[next_index] - y) / 2.0;
								generated_points[index].x = (f32)x;
								generated_points[index].y = (f32)y;
								index++;
							}
						} else {
							// For off–curve points after the first point in the contour,
							// tessellate a quadratic Bézier between:
							//   p0: the last generated (on–curve) point,
							//   p1: the current off–curve point, and
							//   p2: the next point (or its midpoint if it is off–curve)
							Floint2D p0 = generated_points[index - 1];
							Floint2D p1 = { (f32)x, (f32)y };
							Floint2D p2 = { (f32)outline->xCoordinates[next_index], (f32)outline->yCoordinates[next_index] };

							if (!outline->flags[next_index].on_curve) {
								// Next point is also off–curve; compute its midpoint with p1.
								p2.x = p1.x + (p2.x - p1.x) / 2.0;
								p2.y = p1.y + (p2.y - p1.y) / 2.0;
							} else {
								// Next point is on–curve; advance the point index as it will be handled here.
								j++;
							}
							i32 out_size = 0;
							// Generate points along the Bézier from p0 to p2 with control point p1.
							tessellate_bezier(generated_points + index, &out_size, p0, p1, p2);
							index += out_size;
						}
					}
				}

				// Close the contour by connecting the final generated point with the start.
				if (outline->flags[contour_end].on_curve) {
					generated_points[index] = generated_points[generated_points_start_index];
					index++;
				}

				// If the contour started with an off–curve point,
				// perform an extra tessellation between the last generated point,
				// the contour’s first point, and the initial generated point.
				if (contour_started_off) {
					Floint2D p0 = generated_points[index - 1];
					Floint2D p1 = { (f32)outline->xCoordinates[contour_start_index], (f32)outline->yCoordinates[contour_start_index] };
					Floint2D p2 = generated_points[generated_points_start_index];
					i32 out_size = 0;
					tessellate_bezier(generated_points + index, &out_size, p0, p1, p2);
					index += out_size;
				}

				// Record the end index for generated points of this contour.
				gen_pts_end_indices[i] = index;
			}

			*point_count = index; // Return the total count of generated points.
		}
		
		Line2D* generate_edges(Floint2D *pts_gen, i32 *edge_count, i32 *contour_ends, i32 contour_count) {
			i32 j = 0;
			i32 edge_index = 0;
			Line2D *edges = (Line2D*) malloc(sizeof(Line2D)*contour_ends[contour_count-1]);
			for(i32 i = 0; i < contour_count; i++) {
				for(; j < contour_ends[i]-1; j++) {
					Line2D *edge = edges + edge_index;
					edge->p0.x = pts_gen[j].x;
					edge->p0.y = pts_gen[j].y;
					
					edge->p1.x = pts_gen[j+1].x;
					edge->p1.y = pts_gen[j+1].y;
					edge_index++;
				}
				j++; //we are the end of the contour, skip over this point to the start of the next contour
			}
			
			*edge_count = edge_index;

			return edges;
		}

		void rasterize_glyph(Line2D *edges, i32 edge_count, u8 *bitmap, i32 bitmap_height, i32 bitmap_width) {
			f32 intersections[32] = {0};
			i32 intersection_count = 0;
			i32 scanline_subdiv = 5;
			f32 alpha_weight = 255.0/scanline_subdiv;
			f32 step_per_scanline = 1.0/5;

			for(i32 i = 0; i < bitmap_height; i++) {

				for(i32 x = 0; x < scanline_subdiv; x++) {
					intersection_count = 0;

					f32 scanline = i + x*step_per_scanline;

					for(i32 j = 0; j < edge_count; j++) {
						Line2D *edge = edges + j;
						
						f32 bigger_y =  edge->p0.y;
						f32 smaller_y = edge->p1.y;
						
						
						if(edge->p0.y < edge->p1.y) {
							bigger_y =  edge->p1.y;
							smaller_y = edge->p0.y;
						}

						if(scanline <= smaller_y) continue;
						if(scanline > bigger_y) continue;

						f32 dx = edge->p1.x - edge->p0.x;
						f32 dy = edge->p1.y - edge->p0.y;

						if(dy == 0) continue;


						f32 intersection = -1;
						if(dx == 0) {
							intersection = edge->p0.x;
						} else {
							intersection = (scanline - edge->p0.y)*(dx/dy) + edge->p0.x;
							if(intersection < 0) intersection = edge->p0.x;
						}

						intersections[intersection_count] = intersection;
						intersection_count++;
					}
					
					qsort(intersections, intersection_count, sizeof(f32), cmp_floats);


					if(intersection_count > 1) {
						for(i32 m = 0; m < intersection_count; m += 2) {
							f32 start_intersection = intersections[m];
							i32 start_index = intersections[m];
							f32 start_covered = (start_index + 1) - start_intersection ;

							f32 end_intersection = intersections[m+1];
							i32 end_index = intersections[m+1];
							f32 end_covered = end_intersection - (end_index);

							if(start_index == end_index) {
								bitmap[start_index + i*bitmap_width] += alpha_weight*start_covered;
							} else {
								bitmap[start_index + i*bitmap_width] += alpha_weight*start_covered;
								bitmap[end_index + i*bitmap_width] += alpha_weight*end_covered;
							}

							for(i32 j = start_index+1; j < end_index; j++) {
								bitmap[j + i*bitmap_width] += alpha_weight;
							}
						}
					}
				}
			}
		}
		
		void loadFont(const i8* path) {
			i32 file_size = 0;
			i8* file = read_file(path, &file_size);
			if (file == NULL) return;
			i8* mem_ptr = file;
			
			fontDir_ = {0};
			read_font_directory(file, &mem_ptr, &fontDir_);
			parseTables();
			
			f32 pixelSize = 64;
			// Compute scale and baseline shift
			unitsPerEm_ = head_->unitsPerEm;
			scale_ = pixelSize / static_cast<float>(unitsPerEm_);
			baselinePx_ = os2_->sTypoAscender * scale_;
			
			const std::string letterList =
				"abcdefghijklmnopqrstuvwxyz"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"1234567890";
				
			for (u8 c : letterList) {
				glyph_outline go = get_glyph_outline(&fontDir_, get_glyph_index(&fontDir_, static_cast<u8>(c)) );

				// Cache transformed outline
				glyphMap_[c] = deep_copy_glyph_outline(go);
			}
			
			std::cout<< "font loaded\n";
		}
		
		void drawLetter(i32 cc, Point2D pos, i16 fontSize, Point2D wp) {
			u8 c = static_cast<u8>(cc);
			
			if(glyphMap_.find(c) == glyphMap_.end()){
				// std::cout << "letter not found\n";
				return;
			}
			if(c >= 'a') {
				fontSize = fontSize * 0.80;
			}
			
			Floint2D temp_points[512] = {Floint2D(0,0)};
			
			f32 scale = (f32)fontSize/(f32)(glyphMap_[c].yMax - glyphMap_[c].yMin);
			
			i32 index = 0;
			i32 *contour_end_pts = (i32 *) malloc(sizeof(i32)*glyphMap_[c].numberOfContours);
			generate_points(&glyphMap_[c], temp_points, &index, contour_end_pts);
			
			for(int i = 0; i < index; i++) {
				temp_points[i].x = (temp_points[i].x  - glyphMap_[c].xMin)*scale;
				temp_points[i].y = (temp_points[i].y  - glyphMap_[c].yMin)*scale;
			}
			
			//lines
			i32 edge_count = 0;
			Line2D* edges = generate_edges(temp_points, &edge_count, contour_end_pts, glyphMap_[c].numberOfContours);
			
			u8 temp_texture[fontSize*fontSize] = {0};
			
			rasterize_glyph(edges, edge_count, temp_texture,fontSize,fontSize);
			
			for (int j = 0; j < fontSize; j++) {
				for (int i = 0; i < fontSize; i++) {
					if(temp_texture[j*fontSize+i] >  0)
						r.setColor(wp.x+pos.x+i, wp.y+pos.y-j ,
									0-temp_texture[j*fontSize+i], 0-temp_texture[j*fontSize+i], 0-temp_texture[j*fontSize+i]);
				}
			}
			free(edges);
			free(contour_end_pts);
		}
		
	//draw logic

		void clear() { r.clear();}
		void clear(Color c) { r.clear(c);}

		void draw(Point2D p, Color c) {
			r.setColor(p.x, p.y, c.r, c.g, c.b);
		}void draw(Point2D p) {draw(p, Color(255,0,0));}
		
		void draw(Line2D l, Color c) {
			r.drawLine(l.p0.x, l.p0.y, l.p1.x, l.p1.y, c.r, c.g, c.b);
		}void draw(Line2D l) {draw(l,Color(255,0,0));}
		
		void draw(Triangle2D t, Color c) {
			r.drawLine(t.p0.x, t.p0.y, t.p1.x, t.p1.y, c.r, c.g, c.b);
			r.drawLine(t.p1.x, t.p1.y, t.p2.x, t.p2.y, c.r, c.g, c.b);
			r.drawLine(t.p2.x, t.p2.y, t.p0.x, t.p0.y, c.r, c.g, c.b);
		}void draw(Triangle2D t) {draw(t,Color(255,0,0));}

		void fillTriangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2, Color c) {
			r.fillTriangle(x0,y0,x1,y1,x2,y2, c.r, c.g, c.b);
		}void fillTriangle(i32 x0, i32 y0, i32 x1, i32 y1, i32 x2, i32 y2){fillTriangle(x0,y0,x1,y1,x2,y2,Color(255,0,0));}
		void fillTriangle(Point2D p0, Point2D p1, Point2D p2, u8 r, u8 g, u8 b) {fillTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, Color(r, g, b));}
		void fillTriangle(Point2D p0, Point2D p1, Point2D p2, Color c) {fillTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, c);}
		void fillTriangle(Triangle2D t, Color c) {fillTriangle(t.p0.x, t.p0.y, t.p1.x, t.p1.y, t.p2.x, t.p2.y, c);}
		void fillTriangle(Triangle2D t) {fillTriangle(t.p0.x, t.p0.y, t.p1.x, t.p1.y, t.p2.x, t.p2.y, Color(255,0,0));}
		
		void parallelMemcpy(void* dst, const void* src, size_t size) {
			i32 numThreads = std::thread::hardware_concurrency();
			if (numThreads <= 0) numThreads = 4;
			std::vector<std::thread> threads;

			size_t chunkSize = size / numThreads;

			for (i32 t = 0; t < numThreads; ++t) {
				size_t offset = t * chunkSize;
				size_t thisChunkSize = (t == numThreads - 1) ? (size - offset) : chunkSize;

				threads.emplace_back([=]() {
					std::memcpy(static_cast<char*>(dst) + offset,
								static_cast<const char*>(src) + offset,
								thisChunkSize);
				});
			}

			for (auto& thread : threads)
				thread.join();
		}
	};
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef MPWS_WIN32
		#include <windows.h>
		#include <Windowsx.h>//for mouse
		
		#include <queue>
		#include <mutex>

		enum CustomEventType {
			CE_NONE,
			CE_KEY_DOWN,
			CE_KEY_UP,
			CE_MOVE,
			CE_RESIZE,
			CE_CLOSE
		};
		
		struct CustomEvent {
			CustomEventType type;
			union {
				struct { i32 width, height; } resize;
				struct { i32 keycode; } key;
				struct { i32 x, y; } move;
			};
		};

		EXTERN_C IMAGE_DOS_HEADER __ImageBase;
		HINSTANCE get_hinstance(void) {
			return ((HINSTANCE)&__ImageBase);
		}
		
		typedef struct {
			const char* className;
			WNDCLASS wc_;
		} MPWS_WINDOWClass;
		MPWS_WINDOWClass globalClass;
		
		class MPWS_WINDOW : public Window_common {
		private:

			HWND hwnd;
			HDC memDC;
			BITMAPINFO bmi;
			void* pBits = NULL;
			HBITMAP hBuffer;
			
			std::thread update;
			std::queue<CustomEvent> customEventQueue;
			std::mutex queueMutex;
			
			
			void create_W32_WindowClass(void);
			static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
			void resize(i32 nw, i32 nh);
			void setupBuffer();
			
			std::thread renderThread;
			std::atomic<bool> renderAgain;
			
			void setupMPWS_WINDOW();
			void pushEvent(const CustomEvent& event);
			KeyCharacter translateInput(i32 vk);
			
			std::atomic<bool> isLogicRunning;
			void setUp(i32 w, i32 h, const char* n) {
				width= w;
				height = h;
				name = n;
				isRunning = true;
				isLogicRunning = true;
				renderAgain.store(true);
				
				update = std::thread([this]() {
					setupMPWS_WINDOW();
					
					MSG msg;
					while(GetMessage(&msg, nullptr, 0, 0)) {
						TranslateMessage(&msg);
						DispatchMessage(&msg);
						if(!isLogicRunning)
							return;
					}
				});
			}
			
			void renderRaster();
			
		public:
			bool isRunning;
			
			MPWS_WINDOW() {
				setUp(400, 400, "default");
			}
			
			MPWS_WINDOW(i32 w, i32 h, const char* n) {
				setUp(w,h,n);
			}
			
			~MPWS_WINDOW() {
				renderThread.join();
				update.join();
			};
			
			void display();
			
			bool handleEvents(Event* event);
			
			bool isOpen() {return isRunning;}
		};
		
		
		
		void MPWS_WINDOW::resize(i32 nw, i32 nh) {
			r.resize(nw, nh);
			setupBuffer();
		}

		void MPWS_WINDOW::setupBuffer() {
			if (memDC) {
				DeleteDC(memDC);
				memDC = nullptr;
			}
			if (hBuffer) {
				DeleteObject(hBuffer);
				hBuffer = nullptr;
			}

			bmi.bmiHeader.biWidth = width;
			bmi.bmiHeader.biHeight = -height;
			
			HDC screenDC = GetDC(hwnd);
			memDC = CreateCompatibleDC(screenDC);
			
			hBuffer = CreateDIBSection(screenDC, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
			SelectObject(memDC, hBuffer);
			
			ReleaseDC(hwnd, screenDC);
			
		}
		
		void MPWS_WINDOW::display() {
			if(globalClass.className == NULL || !isRunning) return;
			
			while(!renderAgain) {std::this_thread::sleep_for(std::chrono::milliseconds(1));}
			
			renderAgain.store(false);
			std::thread([=]{
				this->parallelMemcpy(pBits, r.getRaster(), r.size());
				HDC hdcLocal = GetDC(hwnd);
				BitBlt(hdcLocal, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
				ReleaseDC(hwnd, hdcLocal);
				renderAgain.store(true);
			}).detach();
		}
		
		void MPWS_WINDOW::create_W32_WindowClass(void) {
			globalClass.className = "MPWS_WINDOWClass_";
			
			globalClass.wc_ = {0};
			globalClass.wc_.lpfnWndProc   = WindowProc;
			globalClass.wc_.hInstance     = get_hinstance();
			globalClass.wc_.lpszClassName = globalClass.className;
			globalClass.wc_.hCursor       = LoadCursor(NULL, IDC_ARROW);
			globalClass.wc_.hbrBackground = NULL;
			RegisterClass(&globalClass.wc_);
		}
		
		void MPWS_WINDOW::setupMPWS_WINDOW() {
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
				this);
			
			i32 sW = GetSystemMetrics(SM_CXSCREEN);
			i32 sH = GetSystemMetrics(SM_CYSCREEN);
			i32 xPos = (sW - width) / 2;
			i32 yPos = (sH - height) / 2;
			SetWindowPos(hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
			
			ZeroMemory(&bmi, sizeof(BITMAPINFO));
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = width;
			bmi.bmiHeader.biHeight = -height; // Negative to indicate a top-down DIB
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32; // 32-bit bitmap
			bmi.bmiHeader.biCompression = BI_RGB;
			
			resize(width, height);
			
			ShowWindow(hwnd, SW_SHOWNORMAL);
			UpdateWindow(hwnd);
		}
		
		void MPWS_WINDOW::pushEvent(const CustomEvent& ev) {
			std::lock_guard<std::mutex> lock(queueMutex);
			customEventQueue.push(ev);
		}
		
		LRESULT CALLBACK MPWS_WINDOW::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			MPWS_WINDOW* w = reinterpret_cast<MPWS_WINDOW*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			switch (uMsg) {
				case WM_CREATE: {
					CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
					w = reinterpret_cast<MPWS_WINDOW*>(cs->lpCreateParams);
					SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)w);
					return 0;
				}
				case WM_SIZE: {
					if(w) {
						CustomEvent ev;
						ev.type = CE_RESIZE;
						ev.resize.width = LOWORD(lParam);
						ev.resize.height = HIWORD(lParam);
						w->pushEvent(ev);
					}
					return 0;
				}
				case WM_MOVE: {
					if(w) {
						CustomEvent ev;
						ev.type = CE_MOVE;
						ev.move.x = LOWORD(lParam);
						ev.move.y = HIWORD(lParam);
						w->pushEvent(ev);
					}
					return 0;
				}
				case WM_KEYDOWN:
				case WM_SYSKEYDOWN: {
					if(w) {
						CustomEvent ev;
						ev.type = CE_KEY_DOWN;
						ev.key.keycode = (i32)wParam;
						w->pushEvent(ev);
					}
					return 0;	
				}
				case WM_KEYUP:
				case WM_SYSKEYUP: {
					if(w) {
						CustomEvent ev;
						ev.type = CE_KEY_UP;
						ev.key.keycode = (i32)wParam;
						w->pushEvent(ev);
					}
					return 0;	
				}
				case WM_ERASEBKGND:
					return 1;
				case WM_CLOSE:
				case WM_DESTROY: {
					if(w) {
						CustomEvent ev;
						ev.type = CE_CLOSE;
						
						w->pushEvent(ev);
						w->isLogicRunning = false;
						
						DestroyWindow(hwnd);
						PostQuitMessage(0);
					}
					return 0;
				}
			}
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		
		bool MPWS_WINDOW::handleEvents(Event* event) {
			if(globalClass.className == NULL || !isRunning) return false;
			
			{
				std::lock_guard<std::mutex> lock(queueMutex);
				if (customEventQueue.empty()) return false;

				//TODO: while resize should happen once, move should give continous updates
				CustomEvent ev = customEventQueue.front();
				customEventQueue.pop();
				//remove duplicate resize calls
				if(ev.type == CE_RESIZE) {
					CustomEvent nextEvent = customEventQueue.front();
					while(nextEvent.type == CE_RESIZE && !customEventQueue.empty()) {
						ev = nextEvent;
						customEventQueue.pop();
						nextEvent = customEventQueue.front();
					}
				}

				switch (ev.type) {
				case CE_KEY_DOWN:
					event->type = EV_KEY_DOWN;
					event->keycode = translateInput(ev.key.keycode);
					event->modifiers = 
						(GetKeyState(VK_SHIFT)   < 0 ?1:0) |
						(GetKeyState(VK_CONTROL) < 0 ?2:0) |
						(GetKeyState(VK_MENU)    < 0 ?4:0) |
						((GetKeyState(VK_CAPITAL) & 0x0001) ? 8:0);
					return true;
				case CE_KEY_UP:
					event->type     = EV_KEY_UP;
					event->keycode  = translateInput(ev.key.keycode);
					event->modifiers = 0;
					return true;
				case CE_MOVE:
					event->type = EV_MOVE;
					event->moveX = ev.move.x;
					event->moveY = ev.move.y;
					posX = event->moveX;
					posY = event->moveY;
					return true;
				case CE_RESIZE:
					event->type = EV_RESIZE;
					event->resizeWidth = ev.resize.width;
					event->resizeHeight = ev.resize.height;
					width = event->resizeWidth;
					height = event->resizeHeight;
					resize(width,height);
					return true;
				case CE_CLOSE:
					event->type = EV_CLOSE;
					isRunning = false;
					// update.join();
					// std::cout << "Window Closed";
					return true;
				default:
					return false;
				}
			}
			
			return false;
		}

		KeyCharacter MPWS_WINDOW::translateInput(i32 vk) {
			switch(vk) {
				case 'A': return Key_A;
				case 'B': return Key_B;
				case 'C': return Key_C;
				case 'D': return Key_D;
				case 'E': return Key_E;
				case 'F': return Key_F;
				case 'G': return Key_G;
				case 'H': return Key_H;
				case 'I': return Key_I;
				case 'J': return Key_J;
				case 'K': return Key_K;
				case 'L': return Key_L;
				case 'M': return Key_M;
				case 'N': return Key_N;
				case 'O': return Key_O;
				case 'P': return Key_P;
				case 'Q': return Key_Q;
				case 'R': return Key_R;
				case 'S': return Key_S;
				case 'T': return Key_T;
				case 'U': return Key_U;
				case 'V': return Key_V;
				case 'W': return Key_W;
				case 'X': return Key_X;
				case 'Y': return Key_Y;
				case 'Z': return Key_Z;

				case '0': return Key_0;
				case '1': return Key_1;
				case '2': return Key_2;
				case '3': return Key_3;
				case '4': return Key_4;
				case '5': return Key_5;
				case '6': return Key_6;
				case '7': return Key_7;
				case '8': return Key_8;
				case '9': return Key_9;

				case VK_ESCAPE: return Key_ESCAPE;
				case VK_RETURN: return Key_ENTER;
				case VK_BACK: return Key_BACKSPACE;
				case VK_TAB: return Key_TAB;
				case VK_SPACE: return Key_SPACE;

				case VK_LEFT: return Key_LEFT;
				case VK_RIGHT: return Key_RIGHT;
				case VK_UP: return Key_UP;
				case VK_DOWN: return Key_DOWN;

				case VK_CONTROL: return Key_CTRL;
				case VK_SHIFT: return Key_SHIFT;
				case VK_MENU: return Key_ALT;

				default: return KEY_UNKNOWN;
			}
		}
	#endif // WIN32
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef MPWS_LINUX

		#include <string>

		static const std::string session_type = []() -> std::string {
			if (std::getenv("WAYLAND_DISPLAY")) {
				return "wayland";
			} else if (std::getenv("DISPLAY")) {
				return "x11";
			} else {
			return "unknown";
			}
		}();

		static const bool is_wayland = (session_type == "wayland");

		//manual
		#if(is_wayland)
			#define MPWS_WAYLAND
		#endif

		#ifdef MPWS_WAYLAND

			#include <wayland-client.h>

			class MPWS_WINDOW : public Window_common{
			public:
				MPWS_WINDOW() {
					std::cout << "Hello Wayland!";
				}

				~MPWS_WINDOW() {};
			};

		#else

			#include <X11/Xlib.h>
			#include <X11/keysym.h>

			typedef struct {
				Display* display;
			} MPWS_LINUX_display;
			MPWS_LINUX_display globalDisplay;

			
			class MPWS_WINDOW : public Window_common{
			private:
				Atom wmDeleteMessage;
				Window window;
				GC gc;
				XEvent event;
				i32 screen;
				XImage* image;

				void resize(i32 nw, i32 nh);
				void setupBuffer();
				void setupMPWS_WINDOW();

				std::atomic<bool> renderAgain;

				KeyCharacter translateInput(KeySym sym);

			public:
				bool isRunning;

				MPWS_WINDOW() {
					width = 600;
					height = 400;
					name = "default";
					setupMPWS_WINDOW();
					isRunning = true;
				}

				MPWS_WINDOW(i32 w, i32 h, const char* n) {
					width = w;
					height = h;
					name = n;
					setupMPWS_WINDOW();
					isRunning = true;
				}

				~MPWS_WINDOW() {
					// XDestroyWindow(globalDisplay.display, window);
					// XDestroyImage(image);
				}

				void display();

				bool handleEvents(Event* event);
				
				bool isOpen() {return isRunning;}
			};



			void MPWS_WINDOW::setupBuffer() {
				if(image) {
					image = nullptr;
				}
				image = XCreateImage(globalDisplay.display,
                                 DefaultVisual(globalDisplay.display, screen),
                                 DefaultDepth(globalDisplay.display, screen),
                                 ZPixmap, 0,
                                 reinterpret_cast<char*>(r.getRaster()),
                                 width, height,
                                 32, 0);
			}

			void MPWS_WINDOW::resize(i32 nw, i32 nh) {
				r.resize(nw,nh);
				setupBuffer();
			}

			void MPWS_WINDOW::display() {
				if(!isRunning) return;

				while(!renderAgain) {std::this_thread::sleep_for(std::chrono::milliseconds(1));}
				// image->data = reinterpret_cast<char*>(r.getRaster());
				// XPutImage(globalDisplay.display, window, gc, image, 0, 0, 0, 0, width, height);
				renderAgain.store(false);
				std::thread([=]{
					// memcpy(pBits, r.getRaster(), r.size());
					parallelMemcpy(image->data, r.getRaster(), r.size());

					XPutImage(globalDisplay.display, window, gc, image, 0, 0, 0, 0, width, height);
					
					renderAgain.store(true);
				}).detach();
			}

			void MPWS_WINDOW::setupMPWS_WINDOW() {
				if (globalDisplay.display == NULL) {
					globalDisplay.display = XOpenDisplay(NULL);
					if (globalDisplay.display == NULL) {
						std::cout << "Cannot open display\n";
					}
				}

				screen = DefaultScreen(globalDisplay.display);

				window = XCreateSimpleWindow(
					globalDisplay.display,
					RootWindow(globalDisplay.display, screen),
					100, 100, width, height,
					1,
					BlackPixel(globalDisplay.display, screen),
					WhitePixel(globalDisplay.display, screen)
				);

				XSelectInput(globalDisplay.display, window, StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask);

				XMapWindow(globalDisplay.display, window);

				gc = DefaultGC(globalDisplay.display, screen);

				resize(width, height);

				wmDeleteMessage = XInternAtom(globalDisplay.display, "WM_DELETE_WINDOW", False);
				XSetWMProtocols(globalDisplay.display, window, &wmDeleteMessage, 1);
			}

			bool MPWS_WINDOW::handleEvents(Event* event) {

				if (XPending(globalDisplay.display) == 0)
					return false;  // Nothing to process — return immediately

				XEvent xev;
				XNextEvent(globalDisplay.display, &xev);  // This removes the event from the queue

				switch (xev.type) {
				case KeyPress:
					event->type = EV_KEY_DOWN;
					// event->keycode = xev.xkey.keycode;
					event->keycode = translateInput(XLookupKeysym(&xev.xkey, 0));
					event->modifiers = 
					((xev.xkey.state & ShiftMask)   ? 1 : 0) |
					((xev.xkey.state & ControlMask) ? 2 : 0) |
					((xev.xkey.state & Mod1Mask)    ? 4 : 0) | // Alt (usually Mod1)
					((xev.xkey.state & LockMask)    ? 8 : 0);
					break;
				case KeyRelease:
					event->type = EV_KEY_UP;
					event->keycode = translateInput(XLookupKeysym(&xev.xkey, 0));
					event->modifiers = 0;
					break;
				case ConfigureNotify: {
						i32 newX = xev.xconfigure.x;
						i32 newY = xev.xconfigure.y;
						i32 newW = xev.xconfigure.width;
						i32 newH = xev.xconfigure.height;

						bool moved = (newX != event->moveX) || (newY != event->moveY);
						bool resized = (newW != event->resizeWidth) || (newH != event->resizeHeight);

						if (moved) {
							event->type = EV_MOVE;
							event->moveX = newX;
							event->moveY = newY;
							posX = event->moveX;
							posY = event->moveY;
						}

						if (resized) {
							event->type = EV_RESIZE;
							event->resizeWidth = newW;
							event->resizeHeight = newH;
							width = newW;
							height = newH;
							resize(width,height);
						}
						break;
					}
				case ClientMessage:
					if ((Atom)xev.xclient.data.l[0] == wmDeleteMessage) {
						event->type = EV_CLOSE;
						isRunning = false;
						XDestroyWindow(globalDisplay.display, window);
						break;
					}
				// fallthrough to default
				default:
					// Dispatch unhandled events to the default handler
					// or ignore them
					// For X11, you might forward Expose/Resize/etc here
					return false;
    			}

				return true;
			}

			KeyCharacter MPWS_WINDOW::translateInput(KeySym sym) {
				switch(sym) {
				case XK_a: return Key_A;
				case XK_b: return Key_B;
				case XK_c: return Key_C;
				case XK_d: return Key_D;
				case XK_e: return Key_E;
				case XK_f: return Key_F;
				case XK_g: return Key_G;
				case XK_h: return Key_H;
				case XK_i: return Key_I;
				case XK_j: return Key_J;
				case XK_k: return Key_K;
				case XK_l: return Key_L;
				case XK_m: return Key_M;
				case XK_n: return Key_N;
				case XK_o: return Key_O;
				case XK_p: return Key_P;
				case XK_q: return Key_Q;
				case XK_r: return Key_R;
				case XK_s: return Key_S;
				case XK_t: return Key_T;
				case XK_u: return Key_U;
				case XK_v: return Key_V;
				case XK_w: return Key_W;
				case XK_x: return Key_X;
				case XK_y: return Key_Y;
				case XK_z: return Key_Z;

				case XK_0: return Key_0;
				case XK_1: return Key_1;
				case XK_2: return Key_2;
				case XK_3: return Key_3;
				case XK_4: return Key_4;
				case XK_5: return Key_5;
				case XK_6: return Key_6;
				case XK_7: return Key_7;
				case XK_8: return Key_8;
				case XK_9: return Key_9;

				case XK_Escape: return Key_ESCAPE;
				case XK_Return: return Key_ENTER;
				case XK_BackSpace: return Key_BACKSPACE;
				case XK_Tab: return Key_TAB;
				case XK_space: return Key_SPACE;

				case XK_Left: return Key_LEFT;
				case XK_Right: return Key_RIGHT;
				case XK_Up: return Key_UP;
				case XK_Down: return Key_DOWN;

				case XK_Control_L: return Key_CTRL;
				case XK_Shift_L: return Key_SHIFT;
				case XK_Alt_L: return Key_ALT;

				default: return KEY_UNKNOWN;
				}
			}

		#endif
	#endif // Linux
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
#endif //implementation
