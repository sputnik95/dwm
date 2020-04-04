/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 20; /* snap pixel */
static const int showbar = 1; /* 0 means no bar */
static const int topbar  = 1; /* 0 means bottom bar */
/* False means using the scroll wheel on a window will not change focus */
static const Bool focusonwheelscroll = False;

/*  Display modes of the tab bar: never shown, always shown, shown only in  */
/*  monocle mode in the presence of several windows.                        */
/*  Modes after showtab_nmodes are disabled.                                */
enum showtab_modes { showtab_never, showtab_auto, showtab_nmodes, showtab_always};
static const int showtab            = showtab_auto;        /* Default tab bar show mode */
static const int toptab             = True;               /* False means bottom tab bar */

/* static const char *fonts[]     = { "terminus:lang=ru:size=9" }; */
static const char *fonts[]     = { "cure:lang=ru:size=8" };
static const char dmenufont[]  = "cure:lang=ru:size=8";
/* static const char dmenufont[]  = "terminus:lang=ru:size=9"; */
static const char col_gray1[]  = "#111111";
static const char col_gray2[]  = "#222222";
static const char col_gray3[]  = "#8b8792";
static const char col_gray4[]  = "#cfcfcf";
static const char col_cyan[]   = "#005577";
static const char col_red[]    = "#b82525";
static const char col_yellow[] = "#b0a225";
static const char mel_pink[]   = "#8D5972";
static const char mel_cyan[]   = "#598D74";
static const char mel_blue[]   = "#59728D";
static const char *colors[][3] = {
                      /* fg           bg       border */
    [SchemeNorm]  =  { col_gray3 , col_gray1, col_gray1 } ,
    [SchemeSel]   =  { col_gray4 , col_gray1, mel_cyan }  ,
    [SchemeWarn]  =  { col_yellow, col_gray1, col_yellow },
    [SchemeUrgent]=  { col_red   , col_gray1, col_red }   ,
};

/* tagging */
static const char *tags[] = { "all", "main", "www", "cli", "im", "docs", "media", "rss", "mail", "etc"};

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     iscentered    isfloating   monitor */
    /*  common  */
    { "Viewnior",        NULL,       NULL,       0,             1,            1,           -1 },
    { "feh",             NULL,       NULL,       0,             1,            1,           -1 },
    /*  var software  */
    { "Spacefm",         NULL,       NULL,       1 << 1,        1,            1,           -1 },
    { "DBeaver",         NULL,       NULL,       1 << 1,        1,            0,           -1 },
    { "Postman",         NULL,       NULL,       1 << 1,        1,            0,           -1 },
    /*  web browsers  */
    { "Firefox",         NULL,       NULL,       1 << 2,        1,            0,           -1 },
    { "Chromium",        NULL,       NULL,       1 << 2,        1,            0,           -1 },
    { "Pale moon",       NULL,       NULL,       1 << 2,        1,            0,           -1 },
    { "Pale moon",       NULL, "Pale Moon Preferences",1 << 2,  1,            1,           -1 },
    { "Brave-browser",   NULL,       NULL,       1 << 2,        1,            0,           -1 },
    /*  messaging  */
    { "TelegramDesktop", NULL,       NULL,       1 << 4,        1,            0,           -1 },
    /*  documents  */
    { "Evince",          NULL,       NULL,       1 << 5,        1,            0,           -1 },
    { "Zathura",         NULL,       NULL,       1 << 5,        1,            0,           -1 },
    /*  media  */
    { "smplayer",        NULL,       NULL,       1 << 6,        1,            1,           -1 },
    { "Clementine",      NULL,       NULL,       1 << 6,        1,            0,           -1 },
    { "mpv",             NULL,       NULL,       1 << 6,        1,            1,           -1 },
};

static const char systray_app_class[] = "stalonetray";

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
#include "fibonacci.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
    { "HHH",      grid },
    { "[D]",      deck },
    { "(@)",      spiral },
    { "[\\]",     dwindle },
    { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2]          = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]    = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_gray3, "-sf", col_gray1, NULL };
static const char *termcmd[]     = { "urxvt", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "urxvt", "-title", scratchpadname, "-geometry", "120x34", NULL };
static const char *cmusstop[]    = { "dwmkeys", "musstop", NULL };
static const char *cmusplay[]    = { "dwmkeys", "mustoggle", NULL };
static const char *cmusnext[]    = { "dwmkeys", "musnext", NULL };
static const char *cmusprev[]    = { "dwmkeys", "musprev", NULL };
static const char *brightup[]    = { "dwmkeys", "brightup", NULL };
static const char *brightdn[]    = { "dwmkeys", "brightdn", NULL };
static const char *volplus[]     = { "dwmkeys", "raise", NULL };
static const char *volminus[]    = { "dwmkeys", "lower", NULL };
static const char *voltoggle[]   = { "dwmkeys", "mute", NULL };
static const char *statusupdate[]= { "dwmkeys", "update", NULL };
static const char *sshot_full[]  = { "dwmkeys", "screenshot_full", NULL };
static const char *sshot_part[]  = { "dwmkeys", "screenshot_part", NULL };
static const char *screensaver[] = { "i3lockr", "--blur=75", NULL };
static const char *screenconf[]  = { "lxrandr", NULL };
static const char *rofimenu[]    = { "rofi", "-show", "combi", NULL };
static const char *startmenu[]   = { "bash", "/home/aezakmi/Apps/scripts/rofi/rofi-bangs.sh", NULL };
static const char *wallpaper[]   = { "bash", "fehbg", "&", NULL };


#include "movestack.c"
#include <X11/XF86keysym.h>
static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_e,      spawn,          {.v = dmenucmd } },
    { MODKEY,                       XK_d,      spawn,          {.v = rofimenu } },
    { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_x,      togglescratch,  {.v = scratchpadcmd } },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY|ShiftMask,             XK_w,      tabmode,        {-1} },
    { MODKEY,                       XK_Down,   focusstack,     {.i = +1 } },
    { MODKEY,                       XK_Up,     focusstack,     {.i = -1 } },
    { MODKEY,                       XK_comma,  incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_period, incnmaster,     {.i = -1 } },
    { MODKEY|ShiftMask,             XK_comma,  cyclelayout,    {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, cyclelayout,    {.i = +1 } },
    { MODKEY,                       XK_Left,   setmfact,       {.f = -0.01} },
    { MODKEY,                       XK_Right,  setmfact,       {.f = +0.01} },
    { MODKEY|ControlMask,           XK_Down,   movestack,      {.i = +1 } },
    { MODKEY|ControlMask,           XK_Up,     movestack,      {.i = -1 } },
    { MODKEY|ShiftMask,             XK_Down,   setcfact,       {.f = +0.25} },
    { MODKEY|ShiftMask,             XK_Up,     setcfact,       {.f = -0.25} },
    { MODKEY,                       XK_w,      zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY,                       XK_k,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
    { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
    { MODKEY,                       XK_s,      togglesticky,   {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { 0,                            0x1008ff15, spawn,   {.v = cmusstop } },
    { 0,                            0x1008ff14, spawn,   {.v = cmusplay } },
    { 0,                            0x1008ff17, spawn,   {.v = cmusnext } },
    { 0,                            0x1008ff16, spawn,   {.v = cmusprev } },
    { 0,                            0x1008ff13, spawn,   {.v = volplus } },
    { 0,                            0x1008ff11, spawn,   {.v = volminus } },
    { 0,                            0x1008ff12, spawn,   {.v = voltoggle } },
    { 0,                            0xfe08, spawn,   {.v = statusupdate } },
    { 0,                            0x1008ff2d, spawn,   {.v = screensaver } },
    { 0,                            0x1008ff59, spawn,   {.v = screenconf } },
    { 0,                            0x1008ff41, spawn,   {.v = startmenu } },
    { 0,                            0x1008ff02, spawn,   {.v = brightup } },
    { 0,                            0x1008ff03, spawn,   {.v = brightdn } },
    { 0,                            0xff61,     spawn,   {.v = sshot_full } },
    { ShiftMask,                    XK_Print,   spawn,   {.v = sshot_part } },
    { MODKEY,                       XK_slash,   spawn,   {.v = wallpaper } },
    TAGKEYS(                        XK_1,                      1)
    TAGKEYS(                        XK_2,                      2)
    TAGKEYS(                        XK_3,                      3)
    TAGKEYS(                        XK_4,                      4)
    TAGKEYS(                        XK_5,                      5)
    TAGKEYS(                        XK_6,                      6)
    TAGKEYS(                        XK_7,                      7)
    TAGKEYS(                        XK_8,                      8)
    TAGKEYS(                        XK_9,                      9)
/*  { MODKEY|ShiftMask,             XK_q,      quit,           {0} }, */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
};

