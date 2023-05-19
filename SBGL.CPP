#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <mem.h>
#include <conio.h>
#include <dos.h>
#include "sbgl.h"

byte *vga=(byte*)0xa0000000;
byte *buffer;

#define SBGL_SET_PIXEL(x,y,i) buffer[(y<<8)+(y<<6)+x]=i
#define VGA_GET_OFFSET(x,y) (y<<8)+(y<<6)+x

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
	fclose(file);
	return gfx;
}

void sbgl_draw_gfx(sbgl_gfx gfx){
	word initial_offset=VGA_GET_OFFSET(gfx.x,gfx.y);
	word buffer_offset=initial_offset;
	word gfx_offset=2;
	byte gfx_index;
	for(byte dy=1;dy<gfx.height+1;dy++){
		for(byte dx=0;dx<gfx.width;dx++){
			gfx_index=gfx.data[gfx_offset];
			if(gfx_index!=gfx.transparent){
				buffer[buffer_offset]=gfx_index;
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
}