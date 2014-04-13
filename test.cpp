#include "sdlgui/sdlgui.h"
#include "game_gui/mouse.h"
#include <stdio.h>
#include <iostream>
using namespace std;
sdltext info_text;
sdlsurface bg_img;
sdl_widget_ptr info_plane;
sdl_button_ptr start_but;
mouse_struct_ptr mouse[9];
SDL_TimerID frame_step;
sdl_frame frame("hit mouse",40,40,600,306,SDL_WINDOW_SHOWN);
int frame_event(sdl_frame_ptr obj,SDL_Event*e)
{
	int i;
	static float ok_count;
	static float ok_s;
	static char buf[200];
	switch(e->type)
	{
		case SDL_USEREVENT:
			switch(e->user.code)
			{
				case sdlgui_event_timer:
					ok_count = 0;
					ok_s = 0;
					for(i=0;i<9;i++)
					{
						ok_count += mouse[i]->hit_count();
						ok_s += mouse[i]->go_count();
						mouse[i]->sleep(0);
					}
					memset(buf,0x00,sizeof(char)*200);
					sprintf(buf,"you kill %d mouses on %d %",ok_count,int(ok_count/ok_s*100));
					cout<<"you kill"<<ok_count<<" mouses on"<<int(ok_count/ok_s*100)<<"%"<<endl;
					//cout<<buf<<endl;
					SDL_RemoveTimer(frame_step);
					if(!info_plane)
					{
						info_plane = frame.add<sdl_widget>("",0,0,300,306,1);
						start_but = info_plane->add<sdl_button>("start",150,100,100,30,1);
					}
					bg_img.blit_surface(NULL,info_plane,NULL);
					memset(buf,0x00,sizeof(char)*200);
					//sprintf(buf,"你摆平了%d的地鼠,命中率%d%",ok_count,int(ok_count/ok_s));
					sprintf(buf,"you kill %d mouses on %d%",ok_count,int(ok_count/ok_s*100));
					info_text.render_utf8_solid(buf,0xff00ff);
					info_text.blit_surface(NULL,info_plane,NULL);
					info_plane->show();
				break;
			}
		break;
	}
	return 0;
}
int start_event(sdl_button_ptr obj,SDL_Event*e)
{
	int i;
	switch(e->type)
	{
		case SDL_MOUSEBUTTONUP:
			frame_step = frame.add_timer(60000);			
			if(info_plane)
			{
				info_plane->hide();
				for(i=0;i<9;i++)				
				{
					mouse[i]->sleep(18);
				}
			}
		break;
	}
	return 0;
}
int main(int argc,char** argv)
{
	
	int i=0;
	for(i=0;i<9;i++)
	{
		mouse[i] = frame.add<mouse_struct>("",300+i%3*100,i/3*100,73,76,1);
	}
	bg_img.img_load("res/bg.jpg");
	bg_img.blit_surface(NULL,&frame,NULL);
	info_text.font("res/font.ttf",30);
	info_text.render_utf8_solid("点开始来消灭地鼠吧！",0x0000ff);
	info_plane = frame.add<sdl_widget>("",0,0,600,306,1);
	bg_img.blit_surface(NULL,info_plane,NULL);
	info_text.blit_surface(NULL,info_plane,NULL);
	start_but = info_plane->add<sdl_button>("点我开始",150,100,100,30,1);
	start_but->event(start_event);
	frame.ime.hide();
	frame.event(frame_event);
	return frame.run();
}

