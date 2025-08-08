#include <a_samp>
#include <YSI_Visual\y_commands>
#include <sscanf2>

#define MAX_rndwstLOADINGBAR_INFO 50
enum rndwstLoadingBar_Player
{
    bool:rndwstLoadingBarPlayer_Loaded,
    PlayerText:rndwstLoadingBarPlayer_TD[3],
    
    bool:rndwstLoadingBarPlayer_Showed,
    rndwstLoadingBarPlayer_Function[32],
    rndwstLoadingBarPlayer_Info[MAX_rndwstLOADINGBAR_INFO],
    rndwstLoadingBarPlayer_Timer1,
    rndwstLoadingBarPlayer_Timer2,
    rndwstLoadingBarPlayer_Percent,
    rndwstLoadingBarPlayer_Count,
}
new rndwstLoadingBarPlayer[MAX_PLAYERS][rndwstLoadingBar_Player];

#define Loop(%0,%1) for(new %0 = 0; %0 < %1; %0++)
#define function%0(%1) forward %0(%1); public %0(%1)

#define LoadingBar:%0(%1) \
	forward ldbar_%0(%1); public ldbar_%0(%1)

#define ShowPlayerLoadingBar rndwst_ShowPlayerLoadingBar

public OnPlayerConnect(playerid)
{
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Loaded] = false;
    rndwstLoadingBar_Load(playerid);
    return CallLocalFunction("rndwstLDBar_OnPlayerConnect", "d", playerid);
}

public OnPlayerDisconnect(playerid, reason)
{
    rndwstLoadingBar_Destroy(playerid);
    return CallLocalFunction("rndwstLDBar__OnPlayerDisconnect", "dd", playerid, reason);
}


stock rndwst_ShowPlayerLoadingBar(playerid, const function[], const info[], time /* In Milisecond */)
{
    if(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Showed] == true) rndwstLoadingBar_Reset(playerid);
    
    format(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Function], 32, function); 
    format(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Info], MAX_rndwstLOADINGBAR_INFO, info);
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Timer1] = SetTimerEx("rndwstLoadingBar_Timer1", time / 100, true, "d", playerid);
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Timer2] = SetTimerEx("rndwstLoadingBar_Timer2", 500, true, "d", playerid);

    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Percent] = 1;
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Count] = 0;

    rndwstLoadingBar_Show(playerid, info, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Count], rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Percent]);
    return 1;
}

function rndwstLoadingBar_Timer1(playerid)
{
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Percent] += 1;

    if(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Percent] == 100)
    {
        rndwstLoadingBar_Reset(playerid);

        new
			string[40];
        format(string, sizeof(string), "ldbar_%s", rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Function]);
        CallLocalFunction(string, "d", playerid);

        printf("Callbacks %s %d", string, playerid);
    }
    else
    {
        rndwstLoadingBar_Show(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Info], rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Count], rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Percent]);
    }
    return 1;
}

function rndwstLoadingBar_Timer2(playerid) 
{
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Count] += 1;
    if(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Count] == 4) rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Count] = 0;
    rndwstLoadingBar_Show(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Info], rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Count], rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Percent]);
    return 1;
}

stock rndwstLoadingBar_Reset(playerid)
{
    rndwstLoadingBar_Hide(playerid);
    KillTimer(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Timer1]);
    KillTimer(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Timer2]);

    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Showed] = false;
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Percent] = 1;
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Count] = 0;
    return 1;
}

stock rndwstLoadingBar_Show(playerid, const info[], counting, percent)
{
    if(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Showed] == false)
    {
        PlayerTextDrawShow(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0]);
    }
    
    new info_fix[MAX_rndwstLOADINGBAR_INFO + 3];
    if(counting == 0) format(info_fix, sizeof(info_fix), "%s", info);
    else if(counting == 1) format(info_fix, sizeof(info_fix), "%s.", info);
    else if(counting == 2) format(info_fix, sizeof(info_fix), "%s..", info);
    else if(counting == 3) format(info_fix, sizeof(info_fix), "%s...", info);
    else if(counting == 3) format(info_fix, sizeof(info_fix), "%s", info);
    
    PlayerTextDrawSetString(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], info_fix);

    // Start: 268 End: 371.5
    new Float:value = (268) + (1.045 * percent);
    PlayerTextDrawTextSize(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], value, 18.000000);

    PlayerTextDrawShow(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1]);
    PlayerTextDrawShow(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2]);

    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Showed] = true;
    return 1;
}

stock rndwstLoadingBar_Hide(playerid)
{
    if(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Showed] == false) return 1;
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Showed] = false;

    PlayerTextDrawHide(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0]);
	PlayerTextDrawHide(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1]);
	PlayerTextDrawHide(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2]);
    return 1;
}

stock rndwstLoadingBar_Destroy(playerid) 
{
    if(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Loaded] == false) return 1;
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Loaded] = false;

    PlayerTextDrawDestroy(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0]);
	PlayerTextDrawDestroy(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1]);
	PlayerTextDrawDestroy(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2]);
    return 1;
}

stock rndwstLoadingBar_Load(playerid) 
{
    if(rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Loaded] == true) return 1;
    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_Loaded] = true;

    rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0] = CreatePlayerTextDraw(playerid, 320.000000, 268.000000, "_");
	PlayerTextDrawFont(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 1);
	PlayerTextDrawLetterSize(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 0.600000, 2.600003);
	PlayerTextDrawTextSize(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 298.500000, 108.000000);
	PlayerTextDrawSetOutline(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 1);
	PlayerTextDrawSetShadow(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 0);
	PlayerTextDrawAlignment(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 2);
	PlayerTextDrawColor(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 255);
	PlayerTextDrawBoxColor(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 135);
	PlayerTextDrawUseBox(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 1);
	PlayerTextDrawSetProportional(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][0], 0);

	rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1] = CreatePlayerTextDraw(playerid, 268.000000, 271.000000, "_");
	PlayerTextDrawFont(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], 1);
	PlayerTextDrawLetterSize(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 0.569, 1.899);
	PlayerTextDrawTextSize(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], 371.500000, 17.000000);
	PlayerTextDrawSetOutline(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], 1);
	PlayerTextDrawSetShadow(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], 0);
	PlayerTextDrawAlignment(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], 1);
	PlayerTextDrawColor(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], 255);
	PlayerTextDrawBoxColor(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], -111);
	PlayerTextDrawUseBox(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], 1);
	PlayerTextDrawSetProportional(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][1], 0);

	rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2] = CreatePlayerTextDraw(playerid, 320.000000, 254.000000, "Info");
	PlayerTextDrawFont(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 1);
	PlayerTextDrawLetterSize(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 0.287500, 1.150000);
	PlayerTextDrawTextSize(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 400.000000, 291.000000);
	PlayerTextDrawSetOutline(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 0);
	PlayerTextDrawSetShadow(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 1);
	PlayerTextDrawAlignment(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 2);
	PlayerTextDrawColor(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], -1);
	PlayerTextDrawBackgroundColor(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 255);
	PlayerTextDrawBoxColor(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 50);
	PlayerTextDrawUseBox(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 0);
	PlayerTextDrawSetProportional(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 1);
	PlayerTextDrawSetSelectable(playerid, rndwstLoadingBarPlayer[playerid][rndwstLoadingBarPlayer_TD][2], 0);
    return 1;
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerConnect rndwstLDBar_OnPlayerConnect
#define OnPlayerDisconnect rndwstLDBar_OnPlayerDisconnect
forward rndwstLDBar_OnPlayerConnect(playerid);
forward rndwstLDBar_OnPlayerDisconnect(playerid, reason);
