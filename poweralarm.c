#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <objbase.h>
#include <mmsystem.h>
#include <string.h>
#include <stdlib.h>

#define CC __stdcall

static const GUID kACDC  = {0x5d3e9a59,0xe9d5,0x4b00,{0xa6,0xbd,0xff,0x34,0xff,0x51,0x65,0x48}};
static const GUID kDevEnum={0xBCDE0395,0xE52F,0x467C,{0x8E,0x3D,0xC4,0x57,0x92,0x91,0x69,0x2E}};
static const GUID kIEnum = {0xA95664D2,0x9614,0x4F35,{0xA7,0x46,0xDE,0x8D,0xB6,0x36,0x17,0xE6}};
static const GUID kVol   = {0x5CDF2C82,0x841E,0x4546,{0x97,0x22,0x0C,0xF7,0x40,0x78,0x22,0x9A}};

typedef struct IEnum IEnum; typedef struct IDev IDev; typedef struct IVol IVol;

typedef struct { int(CC*q)(IEnum*,const void*,void**);unsigned(CC*a)(IEnum*);unsigned(CC*r)(IEnum*);
  int(CC*eep)(IEnum*,unsigned,unsigned,void**);int(CC*def)(IEnum*,unsigned,unsigned,IDev**);
  int(CC*get)(IEnum*,const wchar_t*,IDev**);int(CC*reg)(IEnum*,void*);int(CC*unreg)(IEnum*,void*);} VtEnum;
typedef struct { int(CC*q)(IDev*,const void*,void**);unsigned(CC*a)(IDev*);unsigned(CC*r)(IDev*);
  int(CC*act)(IDev*,const void*,unsigned,void*,void**);int(CC*store)(IDev*,unsigned,void**);
  int(CC*id)(IDev*,wchar_t**);int(CC*state)(IDev*,unsigned*);} VtDev;
typedef struct { int(CC*q)(IVol*,const void*,void**);unsigned(CC*a)(IVol*);unsigned(CC*r)(IVol*);
  int(CC*reg)(IVol*,void*);int(CC*unreg)(IVol*,void*);int(CC*ch)(IVol*,unsigned*);
  int(CC*sl)(IVol*,float,const void*);int(CC*sls)(IVol*,float,const void*);
  int(CC*gl)(IVol*,float*);int(CC*gls)(IVol*,float*);
  int(CC*scl)(IVol*,unsigned,float,const void*);int(CC*scls)(IVol*,unsigned,float,const void*);
  int(CC*gcl)(IVol*,unsigned,float*);int(CC*gcls)(IVol*,unsigned,float*);
  int(CC*sm)(IVol*,int,const void*);int(CC*gm)(IVol*,int*);
  int(CC*si)(IVol*,unsigned*,unsigned*);int(CC*su)(IVol*,const void*);
  int(CC*sd)(IVol*,const void*);int(CC*hs)(IVol*,unsigned*);int(CC*gr)(IVol*,float*,float*,float*);} VtVol;

struct IEnum { VtEnum *v; };
struct IDev  { VtDev  *v; };
struct IVol  { VtVol  *v; };

#define MOD_CRL 0x0002
#define MOD_SH  0x0004
#define MOD_NR  0x4000

static const char *kWavB64 =
"UklGRoQYAABXQVZFZm10IBAAAAABAAEAQB8AAEAfAAABAAgAZGF0YWAYAACAhoySdHqAhYySc3mAhYuRc3l/hIuRcnl/hIqRcnh+"
"hIqQcXh+g4mQcXd9g4mPcXd9gomPcHZ9goiOcHZ8gYiOb3V8gYeNb3V7gYeNbnV7gIaNbnR6gIaMknR6gIWMknN6gIWLknN5f4WL"
"kXJ5f4SKkXJ4foSKkHJ4foOKkHF3foOJj3F3fYKJj3B2fYKIjnB2fIKIjm92fIGHjm91e4GHjW51e4CGjW50e4CGjG50eoCGjJJz"
"eoCFi5JzeX+Fi5FzeX+Ei5FyeH+EipByeH6DipBxd36DiY9xd32DiY9wd32CiI9wdnyCiI5vdnyBh45vdXyBh41vdXuAh41udHuA"
"hoxudHqAhoySdHqAhYySc3mAhYuRc3l/hIuRcnh/hIqQcnh+hIqQcXh+g4mQcXd9g4mPcXd9gomPcHZ9goiOcHZ8gYiOb3V8gYeN"
"b3V7gYeNbnV7gIaNbnR6gIaMknR6gIWMknN6gIWLknN5f4WLkXJ5f4SKkXJ4foSKkHJ4foOKkHF3foOJj3F3fYKJj3B2fYKIjnB2"
"fIKIjm92fIGHjm91e4GHjW51e4CGjW50e4CGjG50eoCGjJJzeoCFi5JzeX+Fi5FzeX+Ei5FyeH+EipByeH6DipBxd36DiY9xd32D"
"iY9wd32CiI9wdnyCiI5vdnyBh45vdXyBh41vdXuAh41udHuAhoxudHqAhoySdHqAhYySc3mAhYuRc3l/hIuRcnh/hIqQcnh+hIqQ"
"cXh+g4mQcXd9g4mPcHd9goiPcHZ9goiOcHZ8gYiOb3V8gYeNb3V7gIeNbnV7gIaNbnR6gIaMknR6gIWMknN6gIWLknN5f4WLkXJ5"
"f4SKkXJ4foSKkHJ4foOKkHF3foOJj3F3fYKJj3B2fYKIjnB2fIKIjm92fIGHjm91e4GHjW51e4CGjW50e4CGjG50eoCGjJJzeoCF"
"i5JzeX+Fi5FzeX+Ei5FyeH+EipByeH6DipBxd36DiY9xd32DiY9wd32CiI9wdnyCiI5vdnyBh45vdXyBh41vdXuAh41udHuAhoxu"
"dHqAhoySdHqAhYySc3mAhYuRc3l/hIuRcnh/hIqQcnh+hIqQcXh+g4mQcXd9g4mPcHd9goiPcHZ9goiOcHZ8gYiOb3V8gYeNb3V7"
"gIeNbnV7gIaNbnR6gIaMknR6gIWMknN5gIWLkXN5f4WLkXJ5f4SKkXJ4foSKkHF4foOJkHF3foOJj3F3fYKJj3B2fYKIjnB2fIKI"
"jm92fIGHjm91e4GHjW51e4CGjW50e4CGjG50eoCGjJJzeoCFi5JzeX+Fi5FzeX+Ei5FyeH+EipByeH6DipBxd36DiY9xd32DiY9w"
"d32CiI9wdnyCiI5vdnyBh45vdXyBh41vdXuAh41udHuAhoxudHqAhoySdHqAhYySc3mAhYuRc3l/hIuRcnh/hIqQcnh+hIqQcXh+"
"g4mQcXd9g4mPcHd9goiPcHZ9goiOcHZ8gYiOb3V8gYeNb3V7gIeNbnV7gIaNbnR6gIaMknR6gIWMknN5gIWLkXN5f4WLkXJ5f4SK"
"kXJ4foSKkHF4foOJkHF3foOJj3F3fYKJj3B2fYKIjnB2fIGIjm92fIGHjm91e4GHjW51e4CGjW50eoCGjG50eoCGjJJzeoCFi5Jz"
"eX+Fi5FzeX+Ei5FyeH+EipByeH6DipBxd36DiY9xd32DiY9wd32CiI9wdnyCiI5vdnyBh45vdXyBh41vdXuAh41udHuAhoxudHqA"
"hoySdHqAhYySc3mAhYuRc3l/hIuRcnh/hIqQcnh+hIqQcXh+g4mQcXd9g4mPcHd9goiPcHZ9goiOcHZ8gYiOb3V8gYeNb3V7gIeN"
"bnV7gIaNbnR6gIaMknR6gIWMknN5gIWLkXN5f4WLkXJ5f4SKkXJ4foSKkHF4foOJkHF3foOJj3F3fYKJj3B2fYKIjnB2fIGIjm92"
"fIGHjm91e4GHjW51e4CGjW50eoCGjG50eoCGjJJzeoCFi5JzeX+Fi5FyeX+EipFyeH+EipByeH6DipBxd36DiY9xd32CiY9wd32C"
"iI9wdnyCiI5vdnyBh45vdXyBh41vdXuAh41udHuAhoxudHqAhoySdHqAhYySc3mAhYuRc3l/hIuRcnh/hIqQcnh+hIqQcXh+g4mQ"
"cXd9g4mPcHd9goiPcHZ9goiOcHZ8gYiOb3V8gYeNb3V7gIeNbnV7gIaNbnR6gIaMknR6gIWMknN5gIWLkXN5f4WLkXJ5f4SKkXJ4"
"foSKkHF4foOJkHF3foOJj3F3fYKJj3B2fYKIjnB2fIGIjm92fIGHjm91e4GHjW51e4CGjW50eoCGjG50eoCGjJJzeoCFi5JzeX+F"
"i5FyeX+EipFyeH+EipByeH6DipBxd36DiY9xd32CiY9wd32CiI9wdnyCiI5vdnyBh45vdXuBh41vdXuAh41udHuAhoxudHqAhoyS"
"c3qAhYuSc3mAhYuRc3l/hIuRcnh/hIqQcnh+hIqQcXh+g4mQcXd9g4mPcHd9goiPcHZ9goiOcHZ8gYiOb3V8gYeNb3V7gIeNbnV7"
"gIaNbnR6gIaMknR6gIWMknN5gIWLkXN5f4WLkXJ5f4SKkXJ4foSKkHF4foOJkHF3foOJj3F3fYKJj3B2fYKIjnB2fIGIjm92fIGH"
"jm91e4CGinN5fYCDhXp9f4CAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIaMknR6"
"gIWMknN5gIWLkXN5f4SLkXJ5f4SKkXJ4foSKkHF4foOJkHF3fYOJj3F3fYKJj3B2fYKIjnB2fIGIjm91fIGHjW91e4GHjW51e4CG"
"jW50eoCGjJJ0eoCFjJJzeoCFi5JzeX+Fi5FyeX+EipFyeH6EipByeH6DipBxd36DiY9xd32CiY9wdn2CiI5wdnyCiI5vdnyBh45v"
"dXuBh41udXuAho1udHuAhoxudHqAhoySc3qAhYuSc3l/hYuRc3l/hIuRcnh/hIqQcnh+g4qQcXd+g4mPcXd9g4mPcHd9goiPcHZ8"
"goiOb3Z8gYeOb3V8gYeNb3V7gIeNbnR7gIaMbnR6gIaMknR6gIWMknN5gIWLkXN5f4SLkXJ4f4SKkHJ4foSKkHF4foOJkHF3fYOJ"
"j3F3fYKJj3B2fYKIjnB2fIGIjm91fIGHjW91e4GHjW51e4CGjW50eoCGjJJ0eoCFjJJzeoCFi5JzeX+Fi5FyeX+EipFyeH6EipBy"
"eH6DipBxd36DiY9xd32CiY9wdn2CiI5wdnyCiI5vdnyBh45vdXuBh41udXuAho1udHuAhoxudHqAhoySc3qAhYuSc3l/hYuRc3l/"
"hIuRcnh/hIqQcnh+g4qQcXd+g4mPcXd9g4mPcHd9goiPcHZ8goiOb3Z8gYeOb3V8gYeNb3V7gIeNbnR7gIaMbnR6gIaMknR6gIWM"
"knN5gIWLkXN5f4SLkXJ4f4SKkHJ4foSKkHF4foOJkHF3fYOJj3B3fYKIj3B2fYKIjnB2fIGIjm91fIGHjW91e4CHjW51e4CGjW50"
"eoCGjJJ0eoCFjJJzeoCFi5JzeX+Fi5FyeX+EipFyeH6EipByeH6DipBxd36DiY9xd32CiY9wdn2CiI5wdnyCiI5vdnyBh45vdXuB"
"h41udXuAho1udHuAhoxudHqAhoySc3qAhYuSc3l/hYuRc3l/hIuRcnh/hIqQcnh+g4qQcXd+g4mPcXd9g4mPcHd9goiPcHZ8goiO"
"b3Z8gYeOb3V8gYeNb3V7gIeNbnR7gIaMbnR6gIaMknR6gIWMknN5gIWLkXN5f4SLkXJ4f4SKkHJ4foSKkHF4foOJkHF3fYOJj3B3"
"fYKIj3B2fYKIjnB2fIGIjm91fIGHjW91e4CHjW51e4CGjW50eoCGjJJ0eoCFjJJzeYCFi5FzeX+Fi5FyeX+EipFyeH6EipBxeH6D"
"iZBxd36DiY9xd32CiY9wdn2CiI5wdnyCiI5vdnyBh45vdXuBh41udXuAho1udHuAhoxudHqAhoySc3qAhYuSc3l/hYuRc3l/hIuR"
"cnh/hIqQcnh+g4qQcXd+g4mPcXd9g4mPcHd9goiPcHZ8goiOb3Z8gYeOb3V8gYeNb3V7gIeNbnR7gIaMbnR6gIaMknR6gIWMknN5"
"gIWLkXN5f4SLkXJ4f4SKkHJ4foSKkHF4foOJkHF3fYOJj3B3fYKIj3B2fYKIjnB2fIGIjm91fIGHjW91e4CHjW51e4CGjW50eoCG"
"jJJ0eoCFjJJzeYCFi5FzeX+Fi5FyeX+EipFyeH6EipBxeH6DiZBxd36DiY9xd32CiY9wdn2CiI5wdnyBiI5vdnyBh45vdXuBh41u"
"dXuAho1udHqAhoxudHqAhoySc3qAhYuSc3l/hYuRc3l/hIuRcnh/hIqQcnh+g4qQcXd+g4mPcXd9g4mPcHd9goiPcHZ8goiOb3Z8"
"gYeOb3V8gYeNb3V7gIeNbnR7gIaMbnR6gIaMknR6gIWMknN5gIWLkXN5f4SLkXJ4f4SKkHJ4foSKkHF4foOJkHF3fYOJj3B3fYKI"
"j3B2fYKIjnB2fIGIjm91fIGHjW91e4CHjW51e4CGjW50eoCGjJJ0eoCFjJJzeYCFi5FzeX+Fi5FyeX+EipFyeH6EipBxeH6DiZBx"
"d36DiY9xd32CiY9wdn2CiI5wdnyBiI5vdnyBh45vdXuBh41udXuAho1udHqAhoxudHqAhoySc3qAhYuSc3l/hYuRcnl/hIqRcnh/"
"hIqQcnh+g4qQcXd+g4mPcXd9gomPcHd9goiPcHZ8goiOb3Z8gYeOb3V8gYeNb3V7gIeNbnR7gIaMbnR6gIaMknR6gIWMknN5gIWL"
"kXN5f4SLkXJ4f4SKkHJ4foSKkHF4foOJkHF3fYOJj3B3fYKIj3B2fYKIjnB2fIGIjm91fIGHjW91e4CHjW51e4CGjW50eoCGjJJ0"
"eoCFjJJzeYCFi5FzeX+Fi5FyeX+EipFyeH6EipBxeH6DiZBxd36DiY9xd32CiY9wdn2CiI5wdnyBiI5vdnyBh45vdXuBh41udXuA"
"ho1udHqAhoxudHqAhoySc3qAhYuSc3l/hYuRcnl/hIqRcnh/hIqQcnh+g4qQcXd+g4mPcXd9gomPcHd9goiPcHZ8goiOb3Z8gYeO"
"b3V7gYeNb3V7gIeNbnR7gIaMbnR6gIaMknN6gIWLknN5gIWLkXN5f4SLkXJ4f4SKkHJ4foSKkHF4foOJkHF3fYOJj3B3fYKIj3B2"
"fYKIjnB2fIGIjm91fIGHjW91e4CHjW51e4CGjW50eoCGjJJ0eoCFjJJzeYCFi5FzeX+Fi5FyeX+EipFyeH6EipBxeH6DiZBxd36D"
"iY9xd32CiY9wdn2CiI5wdnyBiI5vdnyBh45vdXuAhopzeX2Ag4V6fX+AgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICA"
"gICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgIA=";

static HWND g_w; static int g_batt, g_on=1, g_playing, g_saved, g_svMute;
static float g_svVol; static DWORD g_svWave;
static IVol *g_vol; static unsigned char *g_wav; static int g_wavLen;

static int B64V(char c){ return c>='A'&&c<='Z'?c-'A':c>='a'&&c<='z'?c-'a'+26:c>='0'&&c<='9'?c-'0'+52:c=='+'?62:c=='/'?63:-1; }
static unsigned char *B64(const char*s,int*out){ int n=(int)strlen(s),acc=0,bits=0,i,len=0,cap=0,v;
  unsigned char*p=0; for(i=0;i<n;i++){ v=B64V(s[i]); if(v<0){ if(s[i]=='=')break; continue; }
  acc=(acc<<6)|v; bits+=6; if(bits>=8){ bits-=8; if(len>=cap){ cap=cap?cap*2:256; p=(unsigned char*)realloc(p,cap); }
  p[len++]=(unsigned char)((acc>>bits)&0xFF); acc&=(1<<bits)-1; } } *out=len; return p; }

static void save(void){ int m; float v;
  if(g_saved||!g_vol)return;
  if(g_vol->v->gm(g_vol,&m)==0 && g_vol->v->gls(g_vol,&v)==0){ g_svMute=m; g_svVol=v; g_saved=1; } }
static void restore(void){ if(g_saved&&g_vol){ g_vol->v->sm(g_vol,g_svMute,0); g_vol->v->sls(g_vol,g_svVol,0); g_saved=0; } }

static void start(void){ if(g_playing)return; save();
  if(g_vol){ g_vol->v->sm(g_vol,0,0); g_vol->v->sls(g_vol,1.0f,0); }      /* unmute + 100% */
  waveOutGetVolume(0,&g_svWave); waveOutSetVolume(0,0xFFFF);
  if(g_wav) PlaySound((LPCSTR)g_wav,0,SND_MEMORY|SND_ASYNC|SND_LOOP|SND_NODEFAULT);
  else MessageBeep(0x10);
  g_playing=1; }
static void stop(void){ if(!g_playing)return; PlaySound(0,0,0);
  waveOutSetVolume(0,g_svWave); restore(); g_playing=0; }

static void onac(void){ g_batt=0; KillTimer(g_w,1); stop();
  if(!g_on){ g_on=1; MessageBeep(0x40); } }                              /* auto re-arm */
static void check(void){ SYSTEM_POWER_STATUS s;
  if(!GetSystemPowerStatus(&s))return;
  if(s.ACLineStatus==0 && !g_batt){ g_batt=1; if(g_on){ SetTimer(g_w,1,2000,0); start(); } }
  else if(s.ACLineStatus==1 && g_batt) onac(); }
static void toggle(void){ if(g_on){ g_on=0; KillTimer(g_w,1); stop(); MessageBeep(0x30); }
  else { g_on=1; MessageBeep(0x40); check(); if(g_batt)start(); } }

static LRESULT CC Wnd(HWND h,UINT m,WPARAM wp,LPARAM lp){
  switch(m){
  case WM_HOTKEY: if(wp==1)toggle(); else if(wp==2)PostMessage(h,WM_CLOSE,0,0); return 0;
  case WM_POWERBROADCAST:
    if(wp==PBT_APMPOWERSTATUSCHANGE)check();
    else if(wp==PBT_POWERSETTINGCHANGE){ POWERBROADCAST_SETTING*p=(POWERBROADCAST_SETTING*)lp;
      if(p && memcmp(&p->PowerSetting,&kACDC,16)==0)check(); }
    return 1;
  case WM_TIMER: if(wp==1 && g_batt){ SYSTEM_POWER_STATUS s;
    if(!GetSystemPowerStatus(&s) || s.ACLineStatus!=0) onac();
    else if(g_on && !g_playing) start(); } return 0;
  case WM_CLOSE: stop(); if(g_vol)g_vol->v->r(g_vol); free(g_wav);
    UnregisterHotKey(h,1); UnregisterHotKey(h,2); PostQuitMessage(0); return 0;
  }
  return DefWindowProc(h,m,wp,lp); }

static void initvol(void){ IEnum*e=0; IDev*d=0;
  if(CoInitialize(0)!=0 && CoInitialize(0)!=1)return;
  if(CoCreateInstance(&kDevEnum,0,0x17,&kIEnum,(void**)&e)!=0)return;
  if(e->v->def(e,0,1,&d)==0){ d->v->act(d,&kVol,0x17,0,(void**)&g_vol); d->v->r(d); }
  e->v->r(e); }

int WINAPI WinMain(HINSTANCE h,HINSTANCE p,LPSTR c,int n){
  WNDCLASS wc; MSG m;
  (void)p;(void)c;(void)n;
  ZeroMemory(&wc,sizeof wc); wc.lpfnWndProc=Wnd; wc.hInstance=h; wc.lpszClassName="PAW";
  RegisterClass(&wc);
  g_w=CreateWindow("PAW",0,0,0,0,0,0,0,0,h,0);
  RegisterHotKey(g_w,1,MOD_CRL|MOD_SH|MOD_ALT|MOD_NR,0x2D);               /* Ctrl+Alt+Shift+Insert = mute/unmute */
  RegisterHotKey(g_w,2,MOD_CRL|MOD_SH|MOD_ALT|MOD_NR,0x7B);               /* Ctrl+Alt+Shift+F12  = exit        */
  g_wav=B64(kWavB64,&g_wavLen);
  initvol();
  MessageBeep(0x40);
  check();
  while(GetMessage(&m,0,0,0)){ TranslateMessage(&m); DispatchMessage(&m); }
  return 0; }
