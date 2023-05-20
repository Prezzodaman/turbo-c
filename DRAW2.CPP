#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sbgl.h"

#define AMOUNT 5

word *timer=(word*)0x0000046c;

typedef struct{
	sbgl_gfx gfx;
	int x;
	int y;
	int x_vel;
	int y_vel;
	float scale_x_index;
	float scale_y_index;
	byte angle;
} Thing;

int main(){
	srand(*timer);

	sbgl_init();

	sbgl_gfx thing_gfx;
	thing_gfx=sbgl_load_gfx("bloke.gfx");
	Thing things[AMOUNT];
	for(int a=0;a<AMOUNT;a++){
		things[a].gfx=thing_gfx;
		things[a].x=16+a*32;
		things[a].y=16+a*32;
		things[a].x_vel=1;
		things[a].y_vel=1;
		things[a].angle=0;
		things[a].gfx.tint=a;
		things[a].gfx.mask=1;
		things[a].scale_x_index=rand();
		things[a].scale_y_index=rand();
	}

	float scale_x;
	float scale_y;
	float scale_x_index=0;
	float scale_y_index=0;

	while(1){
		sbgl_flood_fill_full(3);

		for(int a=0;a<AMOUNT;a++){
			scale_x=(sin(things[a].scale_x_index)/2)+1.5;
			scale_y=(sin(things[a].scale_y_index)/2)+1.5;
			sbgl_draw_gfx_scale(things[a].gfx,things[a].x,things[a].y,scale_x,scale_y,1);
			things[a].scale_x_index+=0.08;
			things[a].scale_y_index+=0.05;
		}

		sbgl_wait_retrace();
		sbgl_write_buffer();
	}

	return 0;
}