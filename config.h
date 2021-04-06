/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int snap = 20; /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
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

/*static const char *fonts[]     = { "cure:lang=ru:size=8" };
static const char dmenufont[]  = "cure:lang=ru:size=8";*/
static const char *fonts[]     = {
    "Terminus:lang=ru:size=8",
    "FontAwesome:size=8",
};
/*static const char *fonts[]     = { "Terminus:lang=ru:size=8" };*/
static const char dmenufont[]  = "Terminus:lang=ru:size=8";
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
/*    [SchemeWarn]  =  { col_yellow, col_gray1, col_yellow },
    [SchemeUrgent]=  { col_red   , col_gray1, col_red }   ,*/
};

/* tagging */
static const char *tags[] = { "all", "main", "www", "cli", "im", "docs", "media", "rss", "mail", "etc"};

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask       isfloating   monitor */
    /*  common  */
    { "stalonetray",     NULL,       NULL,       1 << 1,        1,           -1 },
    { "URxvt",           "urxvt",    "vim",      0,             1,           -1 },
    { "Nm-connection-editor",NULL,   NULL,       0,             1,           -1 },
    { "Viewnior",        NULL,       NULL,       0,             1,           -1 },
    { "feh",             NULL,       NULL,       0,             1,           -1 },
    /*  var software  */
    { "Spacefm",         NULL,       NULL,       1 << 1,        1,           -1 },
    { "DBeaver",         NULL,       NULL,       1 << 1,        0,           -1 },
    { "Postman",         NULL,       NULL,       1 << 1,        0,           -1 },
    /*  web browsers  */
    { "Brave-browser",   NULL,       NULL,       1 << 2,        0,           -1 },
    { "Chromium",        NULL,       NULL,       1 << 2,        0,           -1 },
    { "Firefox",         NULL,       NULL,       1 << 2,        0,           -1 },
    { "Iridium-browser", NULL,       NULL,       1 << 2,        0,           -1 },
    { "Pale moon",       NULL,       NULL,       1 << 2,        0,           -1 },
    { "Pale moon",       NULL, "Pale Moon Preferences",1 << 2,  1,           -1 },
    /*  messaging  */
    { "TelegramDesktop", NULL,       NULL,       1 << 4,       0,           -1 },
    /*  documents  */
    { "Evince",          NULL,       NULL,       1 << 5,       0,           -1 },
    { "Zathura",         NULL,       NULL,       1 << 5,       0,           -1 },
    /*  media  */
    { "smplayer",        NULL,       NULL,       1 << 6,       1,           -1 },
    { "Clementine",      NULL,       NULL,       1 << 6,       0,           -1 },
    { "mpv",             NULL,       NULL,       1 << 6,       1,           -1 },
};

static const char systray_app_class[] = "trayer";

/* layout(s) */
#include "tatami.c"
#include "grid.c"
static const float mfact     = 0.45; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
    { "HHH",      grid },
    { "|+|",      tatami },
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
static const char *termcmd[]     = { "urxvtc", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "urxvtc", "-title", scratchpadname, "-geometry", "120x34", NULL };
static const char *dmenu_bri[]    = { "addkeys", "dmenu_brightness", NULL };
static const char *cmusstop[]    = { "addkeys", "musstop", NULL };
static const char *cmusplay[]    = { "addkeys", "mustoggle", NULL };
static const char *cmusnext[]    = { "addkeys", "musnext", NULL };
static const char *cmusprev[]    = { "addkeys", "musprev", NULL };
static const char *brightup[]    = { "addkeys", "brightup", NULL };
static const char *brightdn[]    = { "addkeys", "brightdn", NULL };
static const char *volplus[]     = { "addkeys", "raise", NULL };
static const char *volminus[]    = { "addkeys", "lower", NULL };
static const char *voltoggle[]   = { "addkeys", "mute", NULL };
static const char *mictoggle[]   = { "addkeys", "mute_mic", NULL };
/*static const char *statusupdate[]= { "addkeys", "update", NULL };*/
static const char *sshot_full[]  = { "addkeys", "screenshot_full", NULL };
static const char *sshot_part[]  = { "addkeys", "screenshot_part", NULL };
static const char *rename_title[]= { "addkeys", "rename_title", NULL };
static const char *screensaver[] = { "addkeys", "lock_screen", NULL };
static const char *screenconf[]  = { "lxrandr", NULL };
static const char *rofimenu[]    = { "rofi", "-show", "combi", NULL };
static const char *editor[]      = { "urxvt", "-e", "vim", NULL };
static const char *startmenu[]   = { "addkeys", "rofi-bangs", NULL };
static const char *wallpaper[]   = { "addkeys", "fehbg", NULL };


#include "movestack.c"
#include <X11/XF86keysym.h>
static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_v,      spawn,          {.v = editor } },
    { MODKEY,                       XK_a,      spawn,          {.v = rename_title } },
    { MODKEY,                       XK_r,      spawn,          {.v = rofimenu } },
    { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
/*    { MODKEY,                       XK_x,      togglescratch,  {.v = scratchpadcmd } },*/
    { MODKEY,                       XK_b,      spawn,          {.v = dmenu_bri} },
    { MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
    { MODKEY|ShiftMask,             XK_w,      tabmode,        {-1} },
    { MODKEY,                       XK_Down,   focusstack,     {.i = +1 } },
    { MODKEY,                       XK_Up,     focusstack,     {.i = -1 } },
    { MODKEY,                       XK_minus,  incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_equal,  incnmaster,     {.i = -1 } },
    { MODKEY|ControlMask,           XK_comma,  cyclelayout,    {.i = -1 } },
    { MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
    { MODKEY,                       XK_Left,   setmfact,       {.f = -0.008} },
    { MODKEY,                       XK_Right,  setmfact,       {.f = +0.008} },
    { MODKEY|ShiftMask,             XK_Down,   movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,             XK_Up,     movestack,      {.i = -1 } },
    { MODKEY|ControlMask,           XK_Down,   setcfact,       {.f = +0.05} },
    { MODKEY|ControlMask,           XK_Up,     setcfact,       {.f = -0.05} },
    { MODKEY,                       XK_w,      zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY,                       XK_k,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
    /*{ MODKEY,                       XK_d,      setlayout,      {.v = &layouts[3]} },*/
    { MODKEY,                       XK_space,  setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
    { MODKEY,                       XK_f,      togglefloating, {0} },
/*    { MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },*/
/*    { MODKEY,                       XK_s,      togglesticky,   {0} },*/
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    { 0,                            0x1008ff15, spawn,   {.v = cmusstop } },
    { MODKEY|ShiftMask|ControlMask, XK_Up,      spawn,   {.v = cmusstop } },
    { 0,                            0x1008ff14, spawn,   {.v = cmusplay } },
    { MODKEY|ShiftMask|ControlMask, XK_Down,    spawn,   {.v = cmusplay } },
    { 0,                            0x1008ff17, spawn,   {.v = cmusnext } },
    { MODKEY|ShiftMask|ControlMask, XK_Right,   spawn,   {.v = cmusnext } },
    { 0,                            0x1008ff16, spawn,   {.v = cmusprev } },
    { MODKEY|ShiftMask|ControlMask, XK_Left,    spawn,   {.v = cmusprev } },
    { 0,                            0x1008ff13, spawn,   {.v = volplus } },
    { 0,                            0x1008ff11, spawn,   {.v = volminus } },
    { 0,                            0x1008ff12, spawn,   {.v = voltoggle } },
    { 0,                            0x1008ffb2, spawn,   {.v = mictoggle } },
/*    { 0,                            0xfe08,     spawn,   {.v = statusupdate } },*/
    { 0,                            0x1008ff2d, spawn,   {.v = screensaver } },
    { MODKEY,                       XK_l,       spawn,   {.v = screensaver } },
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

