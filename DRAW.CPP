#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sbgl.h"

#define AMOUNT 6

typedef struct{
	sbgl_gfx gfx;
	float x_counter;
	float y_counter;
} Thing;

word *timer=(word*)0x0000046c;

int main(){
	srand(*timer);

	sbgl_init();

	sbgl_gfx thing_gfx;
	thing_gfx=sbgl_load_gfx("bloke.gfx");
	Thing things[AMOUNT];
	for(int a=0;a<AMOUNT;a++){
		things[a].gfx=thing_gfx;
		things[a].x_counter=rand();
		things[a].y_counter=rand();
	}

	word centre_point_x=160-(thing_gfx.width/2);
	word centre_point_y=100-(thing_gfx.height/2);

	while(1){
		sbgl_flood_fill_full(3);

		for(int a=0;a<AMOUNT;a++){
			sbgl_draw_gfx(things[a].gfx);
			things[a].gfx.x=centre_point_x+sin(things[a].x_counter)*108;
			things[a].gfx.y=centre_point_y+sin(things[a].y_counter)*60;
			things[a].x_counter+=0.04;
			things[a].y_counter+=0.07;
		}

		sbgl_wait_retrace();
		sbgl_write_buffer();
	}

	return 0;
}