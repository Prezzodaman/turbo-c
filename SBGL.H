typedef unsigned char byte; // we can now refer to "unsigned char" as simply "byte"
typedef unsigned short word;

void sbgl_write_buffer();
void sbgl_flood_fill_full(byte i);
struct sbgl_gfx{
	byte *data;
	byte width;
	byte height;
	byte transparent;
	byte tint;
	byte opaque;
	byte erase;
	byte mask;
};
struct sbgl_gfx sbgl_load_gfx(char *filename);
void sbgl_draw_gfx(sbgl_gfx gfx,int x,int y);
void sbgl_draw_gfx_rotate(sbgl_gfx gfx,int x,int y,byte angle);
void sbgl_draw_gfx_scale(sbgl_gfx gfx,int x,int y,float w,float h,byte centre);
void sbgl_wait_retrace();
void sbgl_init();