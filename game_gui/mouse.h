#include <SDL2/SDL.h>
#include <ctime>
#include "../sdlgui/sdlgui.h"
using namespace std;
typedef class mouse_struct : public GUI<mouse_struct,sdl_widget>
{
	public:
		mouse_struct();
		mouse_struct(const char*,int,int,int,int,Uint32);
		int init();
		int init(const char*,int,int,int,int,Uint32);
		int sysevent(SDL_Event*);

		//----------------------
		//下次出洞时差,当时差为0时老鼠不出洞
		int sleep(int);
		//----------------------
		//被打中次数
		int hit_count();
		//----------------------
		//出洞次数
		int go_count();
	protected:
		int step(int);
	protected:
		SDL_TimerID _mouse_go_id,_mouse_go_timer_id;
		sdl_clip _mouse_action;
		int _is_hit_mouse;
		int _hited_mouse;
		int _is_go;
		float _sleep;
		int _step;
		int _frame;
		int _move_f;
		int _hit_count;
		int _go_count;
	protected:
		static const int sp;
}*mouse_struct_ptr;
const int mouse_struct::sp = int(1000/8);
mouse_struct::mouse_struct()
:
GUI<mouse_struct,sdl_widget>()
{
	init();
}
mouse_struct::mouse_struct(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
:
GUI<mouse_struct,sdl_widget>()
{
	init(ptitle,px,py,pw,ph,pflag);
}
int mouse_struct::init()
{
	if(sdl_widget::init())return -1;
	return 0;
}
int mouse_struct::init(const char* ptitle,int px,int py,int pw,int ph,Uint32 pflag)
{
	Uint32 a = 0x00ff00-10;
	_mouse_go_timer_id = 0;
	_is_hit_mouse = 0;
	 _hited_mouse = 0;
	_is_go = 0;
	_sleep = 2000;
	srand((unsigned)this+(unsigned)time(NULL));
	_step = ((rand()%12+1000+(unsigned)this)%(rand()+1000))%(rand()+1000)%(rand()+1000);
	_frame = 0;
	_move_f = 1;
	_hit_count = 0;
	_go_count = 0;
	if(sdl_widget::init(ptitle,px,py,pw,ph,pflag))return -1;
	_mouse_action.img_load("res/mouse_action.jpg");
	_mouse_action.clip(73,76);
	_mouse_action(0,0)->blit_surface(NULL,this,NULL);
	//color_key(SDL_TRUE,0x00ff00+1);	
	add_timer(sp);
	//step(_step);
	return 0;
}
int mouse_struct::step(int ps)
{
	if(_mouse_go_timer_id)SDL_RemoveTimer(_mouse_go_timer_id);
	if(ps)
	{
		_step = ps;
		_mouse_go_timer_id = add_timer(_step);
		return 0;
	}
	_hit_count = 0;
	_go_count = 0;
	return 0;
}
int mouse_struct::sleep(int s)
{
	_step = s;
	_sleep = s;
	return step(s);
	return 0;
}
int mouse_struct::hit_count()
{
	return _hit_count;
}
int mouse_struct::go_count()
{
	return _go_count;
}
int mouse_struct::sysevent(SDL_Event* e)
{
	int s;
	switch(e->type)
	{
		case SDL_MOUSEBUTTONDOWN:
			if(_is_go)
			{
				_is_hit_mouse = 1;
				_hited_mouse = 1;
			}
		break;
		case SDL_MOUSEBUTTONUP:
			_is_hit_mouse = 0;
			_hited_mouse = 1;
		break;
		case SDL_USEREVENT:
			switch(e->user.code)
			{
				case sdlgui_event_timer:
					if((int)(e->user.data2)==_step)
					{
						srand(((rand()%12+(unsigned)time(NULL)+(unsigned)this)%(rand()+1000))%(rand()+1000)%((unsigned)this+1000));
						s = 1001+rand()%_step;
						srand(s*(unsigned)clock()*1000);
						s = 1000+(rand()%(int(_sleep*1000)-1000));
						step(s);
						_is_go = 1;
					}
					switch((int)(e->user.data2))
					{
						case sp:
							if(_is_go)
							{
								_frame+=_move_f;
								if(_frame<=0)
								{
									_move_f = 1;
									_is_go=0;
									_frame = 0;
									if(_hited_mouse)
									{
										_hit_count++;
									}
									_go_count++;
									_hited_mouse = 0;
								}
								else
								if(_frame>=8)	
								{
									_frame = 8;
									_move_f = -1;
								}
								_mouse_action(_frame,(_is_hit_mouse)?1:0)->blit_surface(NULL,this,NULL);
							}
						break;
					}
				break;
			}
		break;
	}
	return sdl_widget::sysevent(e);
}
