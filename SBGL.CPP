#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <mem.h>
#include <conio.h>
#include <dos.h>
#include <math.h>

#include "sbgl.h"

byte *vga=(byte*)0xa0000000;
byte *buffer;
byte sbgl_background_colour;

float sin256[256];
float cos256[256];

#define SBGL_SET_PIXEL(x,y,i) buffer[(y<<8)+(y<<6)+x]=i
#define VGA_GET_OFFSET(x,y) (y<<8)+(y<<6)+x
#define SBGL_GET_GFX_PIXEL(x,y,w) y*w+x;

void sbgl_write_buffer(){
	memcpy(vga,buffer,64000);
}

void sbgl_flood_fill_full(byte i){
	memset(buffer,i,64000);
}

struct sbgl_gfx sbgl_load_gfx(char *filename){
	struct sbgl_gfx gfx;
	FILE *file;
	word length;

	file=fopen(filename,"rb");
	fseek(file,0,SEEK_END); // get file size...
	length=ftell(file); // ...
	rewind(file); // ...
	gfx.data=(byte *)malloc(length); // give us a chunk of memory to load the data into
	if (gfx.data==NULL){
		printf("Not enough memory!\n");
		exit(1);
	}
	fread(gfx.data,1,length,file);
	gfx.width=gfx.data[0];
	gfx.height=gfx.data[1];
	gfx.transparent=gfx.data[2];
	gfx.opaque=0;
	gfx.mask=0;
	gfx.tint=0;
	gfx.erase=0;
	fclose(file);
	return gfx;
}

void sbgl_draw_gfx(sbgl_gfx gfx,int x,int y){
	word initial_offset=VGA_GET_OFFSET(x,y);
	word buffer_offset=initial_offset;
	word gfx_offset=2;
	byte gfx_index;
	byte gfx_index_modified;
	for(byte dy=1;dy<gfx.height+1;dy++){
		for(byte dx=0;dx<gfx.width;dx++){
			if(dx+x<320 && dx+x>=0 && dy+y<=200 && dy+y>=0){
				gfx_index=gfx.data[gfx_offset];
				gfx_index_modified=gfx_index+gfx.tint;
				if(gfx.mask!=0){
					gfx_index_modified+=buffer[buffer_offset];
				}
				if(gfx.erase!=0){
					gfx_index_modified=sbgl_background_colour;
				}
				if(gfx.opaque!=0){
					buffer[buffer_offset]=gfx_index_modified;
				}else{
					if(gfx_index!=gfx.transparent){
						buffer[buffer_offset]=gfx_index_modified;
					}
				}
			}
			gfx_offset++;
			buffer_offset++;
		}
		buffer_offset=(dy<<8)+(dy<<6)+initial_offset;
	}
}

void sbgl_draw_gfx_rotate(sbgl_gfx gfx,int x,int y,byte angle){
	word initial_offset=VGA_GET_OFFSET(x,y);
	word buffer_offset=initial_offset;
	word gfx_offset=2;
	byte gfx_index;
	byte gfx_index_modified;
	float rx;
	float ry;
	byte cx=gfx.width/2;
	byte cy=gfx.height/2;
	for(byte dy=1;dy<gfx.height+1;dy++){
		for(byte dx=0;dx<gfx.width;dx++){
			if(dx+x<320 && dx+x>=0 && dy+y<=200 && dy+y>=0){
				dy--;
				rx=((dx-cx)*cos256[angle])-((dy-cy)*sin256[angle])+cx;
				ry=((dx-cx)*sin256[angle])+((dy-cy)*cos256[angle])+cy;
				gfx_index=gfx.data[((byte)ry*gfx.width+(byte)rx)+2];
				dy++;
				if(rx<gfx.width && rx>0 && ry<gfx.height && ry>0){
					gfx_index_modified=gfx_index+gfx.tint;
					if(gfx.mask!=0){
						gfx_index_modified+=buffer[buffer_offset];
					}
					if(gfx.erase!=0){
						gfx_index_modified=sbgl_background_colour;
					}
					if(gfx.opaque!=0){
						buffer[buffer_offset]=gfx_index_modified;
					}else{
						if(gfx_index!=gfx.transparent){
							buffer[buffer_offset]=gfx_index_modified;
						}
					}
				}
			}
			gfx_offset++;
			buffer_offset++;
		}
		buffer_offset=(dy<<8)+(dy<<6)+initial_offset;
	}
}

void sbgl_draw_gfx_scale(sbgl_gfx gfx,int x,int y,float w,float h,byte centre){
	word initial_offset;
	if(centre==0){
		initial_offset=VGA_GET_OFFSET(x+gfx.width,y+gfx.height);
	}else{
		initial_offset=VGA_GET_OFFSET((x-(word)((w*gfx.width)/2))+gfx.width,(y-(word)((h*gfx.height)/2))+gfx.height);
	}
	word buffer_offset=initial_offset;
	word gfx_offset=2;
	byte gfx_index;
	byte gfx_index_modified;
	float rx;
	float ry;
	float sx=gfx.width/w;
	float sy=gfx.height/h;
	for(byte dy=1;dy<(gfx.height*h)+1;dy++){
		for(byte dx=0;dx<gfx.width*w;dx++){
			if(dx+x<320 && dx+x>=0 && dy+y<=200 && dy+y>=0){
				dy--;
				rx=dx*sx/gfx.width;
				ry=dy*sy/gfx.height;
				gfx_index=gfx.data[((byte)ry*gfx.width+(byte)rx)+2];
				dy++;
				gfx_index_modified=gfx_index+gfx.tint;
				if(gfx.mask!=0){
					gfx_index_modified+=buffer[buffer_offset];
				}
				if(gfx.erase!=0){
					gfx_index_modified=sbgl_background_colour;
				}
				if(gfx.opaque!=0){
					buffer[buffer_offset]=gfx_index_modified;
				}else{
					if(gfx_index!=gfx.transparent){
						buffer[buffer_offset]=gfx_index_modified;
					}
				}
			}
			gfx_offset++;
			buffer_offset++;
		}
		buffer_offset=(dy<<8)+(dy<<6)+initial_offset;
	}
}

void sbgl_wait_retrace(){
	while(inp(0x3DA) & 0x08);
	while(!(inp(0X3DA) & 0x08));
}

void sbgl_init(){
	buffer=(byte *)malloc(64000);
	if (buffer==NULL){
		printf("Not enough memory!\n");
		exit(1);
	}

	union REGS regs; // this union is specfically for registers
	regs.h.ah=0; // the "h" means that it's an 8-bit register, to use a 16-bit register, use "x"
	regs.h.al=0x13;
	int86(0x10,&regs,&regs);

	for(int a=0;a<256;a++){
		sin256[a]=sin(((2*3.1415)*a)/256);
		cos256[a]=cos(((2*3.1415)*a)/256);
	}
}