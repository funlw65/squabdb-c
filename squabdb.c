/*
 * SquabDB is a small Pigeon Database application,
 * intended as a demonstration for typhoon database engine and
 * for raylib 4.0 + raygui 3.0
 *
 * Copyright 2022 by Vasile Guta-Ciucur and Lorena Guta-Ciucur
 *
 * Application released under BSD license.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <raylib.h>
#define  RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <string.h>
#include "environ.h"
#include <unistd.h>
#include <sys/stat.h>
#include "squabdb.h"
#include "typhoon.h"
//#include "leaks.h"

#define clTeal        CLITERAL(Color){97, 190, 221, 255}
#define clGray        CLITERAL(Color){206, 206, 206, 255}
#define clBlueGray    CLITERAL(Color){231, 231, 239, 255}
#define clGreen       CLITERAL(Color){180, 192, 69, 255}
#define clLCD         CLITERAL(Color){50, 50, 50, 255}
#define clDarkBlue    CLITERAL(Color){45, 43, 56, 255}
#define clLightBlue   CLITERAL(Color){51, 102, 152, 255}
#define clAlmostBlack CLITERAL(Color){33, 33, 33, 255}
#define clCreme       CLITERAL(Color){223, 196, 125, 255}
#define clPhotoFrame  CLITERAL(Color){45, 57, 95, 255}
#define clBrick       CLITERAL(Color){206, 72, 0, 255}
#define clGrass       CLITERAL(Color){98, 187, 45, 255}

Color clBackground;
Color clPanel;
Color clLabels;
Color clLetter;
Color clFrames;
Color clPhoto;

const int screenWidth  = 1366;
const int screenHeight = 706;
const int pSplash_posX = 332;
const int pSplash_posY = 136;
const int pMain_posX   = 213;
const int pMain_posY   = 40;

Image bufferIcon;
Texture2D bufferTex, bufferTex2, iNoImage;
Texture2D iPigeon_Welcome, iPigeon_Bye;
Texture2D iSlime1, iSlime2;
Texture2D iAbout, iGrass1a, iGrass2b, iFirefly1, iFirefly2, iLadybug;
Texture2D iFindLarge, iAddLarge, iModLarge;
Texture2D sbFirst,sbPrev,sbNext,sbLast,sbFind,sbAdd,sbMod,sbDel,sbInfo,sbExit;
Texture2D sbFirst_gray,sbPrev_gray,sbNext_gray,sbLast_gray,sbFind_gray,sbAdd_gray,sbMod_gray,sbDel_gray;

Vector2 mousePosition, commonPos, commonPos2;

Rectangle rec_cbTheme, rec_photoFrame, rec_common;
Rectangle rec_sbFirst;
Rectangle rec_sbPrev;
Rectangle rec_sbNext;
Rectangle rec_sbLast;
Rectangle rec_sbFind;
Rectangle rec_sbAdd;
Rectangle rec_sbMod;
Rectangle rec_sbDel;
Rectangle rec_sbInfo;
Rectangle rec_sbExit;

// combobox_index
int  cbTheme_idx;
bool cbTheme_edit;

// buttons action
bool bSplashClose_click;
bool bQuitYes_click, bQuitNo_click;
bool bErrorNextKey_click;
bool bFindKey_click;
bool bErrorFindKey_click;
bool bResultDelYes_click;
bool bResultDelNo_click;
bool bModSave_click;
bool bModCancel_click;
bool bAddSave_click;
bool bAddCancel_click;
bool bErrorAddKey_click;
// speedbuttons status
bool sbFirst_active;
bool sbPrev_active;
bool sbNext_active;
bool sbLast_active;
bool sbFind_active;
bool sbAdd_active;
bool sbMod_active;
bool sbDel_active;
// find field
bool iSearchEdit;
bool iBreedEdit;
bool iCategoryEdit;
bool iOriginEdit;
bool iDesc1Edit;
bool iDesc2Edit;
bool iImageidEdit;
//
bool bDelYes_click, bDelNo_click;
//
bool bResultClose_click, bResultDel_click, bResultMod_click;

int stp;

// database elements
struct breeds breeds;
DB_ADDR cr_rec;
bool squabdb_empty, cycle_common;

char image_path[71] = "";
char project_subfolder[10] = "pictures/";
//
char breed[60] = "";
char cat[11] = "";
char orig[60] = "";
char imgid[60] = "";
char desc[160] = "";
char desc1[80] = "";
char desc2[80] = "";
//
char eBreed[60] = "";
char eCat[11] = "";
char eOrig[60] = "";
char eImgid[60] = "";
char eDesc[160] = "";
char eDesc1[80] = "";
char eDesc2[80] = "";
//
char breed_buff[60] = "";
char cat_buff[11] = "";
char orig_buff[60] = "";
char imgid_buff[60] = "";
char desc_buff[160] = "";
char desc1_buff[80] = "";
char desc2_buff[80] = "";
//
char eBreed_buff[60] = "";
char eCat_buff[11] = "";
char eOrig_buff[60] = "";
char eImgid_buff[60] = "";
char eDesc_buff[160] = "";
char eDesc1_buff[80] = "";
char eDesc2_buff[80] = "";

char eText[60] = "";

char err_msg[50] = "";

void clear_mod_fields(void)
{
  eBreed_buff[0] = '\0';
  eCat_buff[0] = '\0';
  eOrig_buff[0] = '\0';
  eImgid_buff[0] = '\0';
  eDesc_buff[0] = '\0';
  eDesc1_buff[0] = '\0';
  eDesc2_buff[0] = '\0';
}


void clear_add_fields(void)
{
  eBreed[0] = '\0';
  eCat[0] = '\0';
  eOrig[0] = '\0';
  eImgid[0] = '\0';
  eDesc[0] = '\0';
  eDesc1[0] = '\0';
  eDesc2[0] = '\0';
}

void clear_buffer_fields(void)
{
  breed_buff[0] = '\0';
  cat_buff[0] = '\0';
  orig_buff[0] = '\0';
  imgid_buff[0] = '\0';
  desc_buff[0] = '\0';
  desc1_buff[0] = '\0';
  desc2_buff[0] = '\0';
  //
}

void clear_fields(void)
{
  //
  breed[0] = '\0';
  cat[0] = '\0';
  orig[0] = '\0';
  imgid[0] = '\0';
  desc[0] = '\0';
  desc1[0] = '\0';
  desc2[0] = '\0';
}

char *ltrim(char *str, const char *seps)
{
  size_t totrim;
  if (seps == NULL) {
    seps = "\t\n\v\f\r ";
  }
  totrim = strspn(str, seps);
  if (totrim > 0) {
    size_t len = strlen(str);
    if (totrim == len) {
      str[0] = '\0';
    } else {
      memmove(str, str + totrim, len + 1 - totrim);
    }
  }
  return str;
}

char *rtrim(char *str, const char *seps)
{
  int i;
  if (seps == NULL) {
    seps = "\t\n\v\f\r ";
  }
  i = strlen(str) - 1;
  while (i >= 0 && strchr(seps, str[i]) != NULL) {
    str[i] = '\0';
    i--;
  }
  return str;
}

char *trim(char *str, const char *seps)
{
  return ltrim(rtrim(str, seps), seps);
}

uint8_t i, ln, add_err, mod_err;

bool image_exists(char *im){
  char tmp[71]="";
  strcpy(tmp, project_subfolder);
  strcat(tmp, trim(im,NULL));
  if( access( tmp, F_OK ) == 0 ) return true;
  else return false;
}

void load_imageid(char *im, Texture *tex)
{
  //
  char tmp[71]="";
  UnloadTexture(*tex);
  if (im[0] != '\0' ) {
    if(image_exists(im)){
      strcpy(tmp, project_subfolder);
      strcat(tmp, trim(im,NULL));
      strcpy(image_path, tmp);
      bufferIcon = LoadImage(image_path);
      *tex = LoadTextureFromImage(bufferIcon);
      UnloadImage(bufferIcon);
    }
    else {
      bufferIcon = LoadImage("pictures/noimage.png");
      *tex = LoadTextureFromImage(bufferIcon);
      UnloadImage(bufferIcon);
    }
  } else {
    bufferIcon = LoadImage("pictures/noimage.png");
    *tex = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
  }
}

void prepare_fields()
{
  d_crget(&cr_rec);
  ln = strlen(desc);
  strcpy(eCat, cat);
  strcpy(eOrig, orig);
  strcpy(eImgid, imgid);
  if (ln <= 80) {
    strcpy(eDesc1, desc);
    eDesc2[0] = '\0';
  } else {
    strncpy(eDesc1, desc, 80);
    for(i=80; i<ln; i++)
      eDesc2[i-80] = desc[i];
    eDesc2[ln-80] = '\0';
  }
}

int main(void)
{
  //atexit(report_mem_leak);
  //--------------------------------------------------------------------
  // Database stuff
  //--------------------------------------------------------------------
  clear_fields();
  clear_buffer_fields();
  clear_mod_fields();
  clear_add_fields();

  mkdir("data", 0777);
  d_dbfpath("data");
  if( d_open("squabdb", "x") == S_OKAY ){
    d_keyfrst(NAME);
    if (db_status == S_NOTFOUND) {
      squabdb_empty = true;
      printf("Database empty!\r\n");
    } else if(db_status == S_OKAY) {
      squabdb_empty = false;
      d_recread(&breeds);
      if(db_status == S_OKAY){
        strcpy(breed, breeds.name);
        strcpy(cat, breeds.category);
        strcpy(orig, breeds.origin);
        strcpy(imgid, breeds.imageid);
        strcpy(desc, breeds.description);
        //
        sbFirst_active = false;
        sbPrev_active  = false;
        sbNext_active  = true;
        sbLast_active  = true;
        sbFind_active  = true;
        sbAdd_active   = true;
        sbMod_active   = true;
        sbDel_active   = true;
      }
      else printf("Record reading error! \r\n");
    }
    //----------------------------------------------------------------
    // GRAPHIC STUFF
    //----------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "SquabDB - Pigeon Database");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
    GuiSetState(GUI_STATE_NORMAL);
    SetTargetFPS(16);
    SetExitKey(0);
    bool exitWindow = false;

    GuiLoadStyle("style/squabdb_dark.rgs");
    clBackground = clDarkBlue;
    clPanel      = clAlmostBlack;
    clLabels     = LIGHTGRAY;
    clLetter     = WHITE;
    clFrames     = LIGHTGRAY;
    clPhoto      = DARKGRAY;

    Font fontTitle   = LoadFontEx("style/Accanthis.otf", 56, 0, 0);
    Font fontLetter  = LoadFontEx("style/Accanthis.otf", 24, 0, 0);
    Font fontLabel   = LoadFontEx("style/FreeSans.ttf", 20, 0, 0);
    Font fontSButton = LoadFontEx("style/Mecha.ttf", 14, 0, 0);
    Font fontDesc    = LoadFontEx("style/Mecha.ttf", 16, 0, 0);

    bufferIcon = LoadImage("pictures/noimage.png");
    iNoImage = LoadTextureFromImage(bufferIcon);
    bufferTex = LoadTextureFromImage(bufferIcon);
    bufferTex2 = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    load_imageid(imgid, &bufferTex);


    // Splash
    bufferIcon = LoadImage("img/Pigeon.png");
    iPigeon_Welcome = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    // Exit
    bufferIcon = LoadImage("img/Flying_Pigeon.png");
    iPigeon_Bye = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    bufferIcon = LoadImage("img/slime1.png");
    iSlime1 = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    bufferIcon = LoadImage("img/slime2.png");
    iSlime2 = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    // About
    bufferIcon = LoadImage("img/about.png");
    iAbout = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    bufferIcon = LoadImage("img/grass1a.png");
    iGrass1a = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    bufferIcon = LoadImage("img/grass2b.png");
    iGrass2b = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    bufferIcon = LoadImage("img/firefly1.png");
    iFirefly1 = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    bufferIcon = LoadImage("img/firefly2.png");
    iFirefly2 = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    bufferIcon = LoadImage("img/ladybug.png");
    iLadybug = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/128/iLfind.png");
    iFindLarge = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    bufferIcon = LoadImage("img/general/128/iLmod.png");// downgraded to 64x64, aestetic reasons
    iModLarge = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    bufferIcon = LoadImage("img/general/iLadd.png");// downgraded to 64x64
    iAddLarge = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    // load the icons of emulated speedbuttons
    bufferIcon = LoadImage("img/general/first.png");
    sbFirst = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/prev.png");
    sbPrev = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/next.png");
    sbNext = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/last.png");
    sbLast = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/find.png");
    sbFind = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/add.png");
    sbAdd = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/mod.png");
    sbMod = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/del.png");
    sbDel = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/info.png");
    sbInfo = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/quit.png");
    sbExit = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    // _gray
    bufferIcon = LoadImage("img/general/first_gray.png");
    sbFirst_gray = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/prev_gray.png");
    sbPrev_gray = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/next_gray.png");
    sbNext_gray = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/last_gray.png");
    sbLast_gray = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/find_gray.png");
    sbFind_gray = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/add_gray.png");
    sbAdd_gray = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/mod_gray.png");
    sbMod_gray = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);
    //
    bufferIcon = LoadImage("img/general/del_gray.png");
    sbDel_gray = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);

    //bSplashClose_click  = false;
    bQuitYes_click      = false;
    bQuitNo_click       = false;
    bErrorNextKey_click = false;

    // combobox rectangle
    rec_cbTheme.x = pSplash_posX + 150;
    rec_cbTheme.y = pSplash_posY + 398;
    rec_cbTheme.width  = 130;
    rec_cbTheme.height = 32;
    // combobox index
    cbTheme_idx = 0;
    cbTheme_edit = false;

    rec_sbFirst.x = 252 - 48;
    rec_sbFirst.y = 72;
    rec_sbFirst.width = 32;
    rec_sbFirst.height = 32;

    rec_sbPrev.x = 252;
    rec_sbPrev.y = 72;
    rec_sbPrev.width = 32;
    rec_sbPrev.height = 32;

    rec_sbNext.x = 300;
    rec_sbNext.y = 72;
    rec_sbNext.width = 32;
    rec_sbNext.height = 32;

    rec_sbLast.x = 348;
    rec_sbLast.y = 72;
    rec_sbLast.width = 32;
    rec_sbLast.height = 32;

    rec_sbFind.x = 348+48;
    rec_sbFind.y = 72;
    rec_sbFind.width = 32;
    rec_sbFind.height = 32;

    rec_sbAdd.x = 396+48;
    rec_sbAdd.y = 72;
    rec_sbAdd.width = 32;
    rec_sbAdd.height = 32;

    rec_sbMod.x = 444+48;
    rec_sbMod.y = 72;
    rec_sbMod.width = 32;
    rec_sbMod.height = 32;

    rec_sbDel.x = 492+48;
    rec_sbDel.y = 72;
    rec_sbDel.width = 32;
    rec_sbDel.height = 32;

    rec_sbInfo.x = 540+48;
    rec_sbInfo.y = 72;
    rec_sbInfo.width = 32;
    rec_sbInfo.height = 32;

    rec_sbExit.x = 588+48;
    rec_sbExit.y = 72;
    rec_sbExit.width = 32;
    rec_sbExit.height = 32;

    //
    rec_photoFrame.x = 677+48;
    rec_photoFrame.y = 72;
    rec_photoFrame.width = 400;
    rec_photoFrame.height = 400;


    int8_t originated = 0;
    int8_t currentScreen = -1;
    while(!exitWindow) {
      mousePosition = GetMousePosition();
      BeginDrawing();
      /* ==================================================================================
       * SPLASH SCREEN
       * ==================================================================================
       */
      if (currentScreen == -1) {
        ClearBackground(clBackground);
        DrawRectangle(pSplash_posX,pSplash_posY,696,664-204,clPanel);
        DrawTexture(iPigeon_Welcome, 200,100, WHITE);
        commonPos.x = (float)pSplash_posX + 280;
        commonPos.y = (float)pSplash_posY + 50;
        DrawTextEx(fontTitle, "Welcome!", commonPos, 56, 2, clLabels);
        commonPos.x = (float)pSplash_posX + 180;
        commonPos.y = (float)pSplash_posY + 150;
        DrawTextEx(fontLetter, "SquabDB is a little Pigeon database, intended as", commonPos, 24, 2, clLabels);
        commonPos.x = (float)pSplash_posX + 180;
        commonPos.y = (float)pSplash_posY + 174;
        DrawTextEx(fontLetter, "demonstrator for typhoon database engine, raylib", commonPos, 24, 2, clLabels);
        commonPos.x = (float)pSplash_posX + 180;
        commonPos.y = (float)pSplash_posY + 198;
        DrawTextEx(fontLetter, "and raygui graphical libraries. ", commonPos, 24, 2, clLabels);
        commonPos.x = (float)pSplash_posX + 180;
        commonPos.y = (float)pSplash_posY + 222;
        DrawTextEx(fontLetter, "Pigeon cartoons are licensed to: ", commonPos, 24, 2, clLabels);
        commonPos.x = (float)pSplash_posX + 180;
        commonPos.y = (float)pSplash_posY + 246;
        DrawTextEx(fontLetter, "   - David StClair (Wildchief)", commonPos, 24, 2, clLabels);
        commonPos.x = (float)pSplash_posX + 180;
        commonPos.y = (float)pSplash_posY + 270;
        DrawTextEx(fontLetter, "   - Dmitry Moiseenko", commonPos, 24, 2, clLabels);
        //
        commonPos.x = (float)pSplash_posX + 180;
        commonPos.y = (float)pSplash_posY + 314;
        DrawTextEx(fontLetter, "Copyright 2022 by Vasile Guta-Ciucur.", commonPos, 24, 2, clLabels);
        commonPos.x = (float)pSplash_posX + 180;
        commonPos.y = (float)pSplash_posY + 338;
        DrawTextEx(fontLetter, "Application is BSD licensed.", commonPos, 24, 2, clLabels);
        //
        commonPos.x = (float)pSplash_posX + 80;
        commonPos.y = (float)pSplash_posY + 406;
        DrawTextEx(fontLabel, "Theme:", commonPos, 20, 1, clLabels);

        if (GuiDropdownBox(rec_cbTheme,"DARK;LIGHT", &cbTheme_idx, cbTheme_edit)) cbTheme_edit =  !cbTheme_edit;
        bSplashClose_click = GuiButton((Rectangle) {
          pSplash_posX + 530, pSplash_posY + 398, 88, 32
        }, "Close");
        if ((bSplashClose_click) || (IsKeyPressed(KEY_ENTER))) {
          bSplashClose_click = false;
          //exitWindow = true;
          currentScreen = 0;
          if (cbTheme_idx == 1) {
            GuiLoadStyle("style/squabdb_light.rgs");
            clBackground = clLightBlue;
            clPanel      = clBlueGray;
            clLabels     = DARKGRAY;
            clLetter     = BLACK;
            clFrames     = GRAY;
            clPhoto      = clPhotoFrame;
          }
        }
      }
      /* ==================================================================================
       * MAIN SCREEN
       * ==================================================================================
       */
      if (currentScreen == 0) {
        //
        originated = 0;
        if (squabdb_empty) {
          sbFirst_active = false;
          sbPrev_active = false;
          sbNext_active = false;
          sbLast_active = false;
          sbFind_active = false;
          sbAdd_active = true;
          sbMod_active = false;
          sbDel_active = false;
        }
        //exitWindow := WindowShouldClose;
        ClearBackground(clBackground);
        DrawTexture(iGrass1a, pMain_posX-82-48, pMain_posY + 425, WHITE);
        DrawRectangle(pMain_posX-48,pMain_posY,896+96,600,clPanel);
        DrawTexture(iGrass2b, pMain_posX+802+48, pMain_posY + 423, WHITE);
        //
        DrawRectangleRoundedLines(rec_sbFirst, 0.2, 4, 2, DARKGRAY);
        DrawRectangleRoundedLines(rec_sbPrev, 0.2, 4, 2, DARKGRAY);
        DrawRectangleRoundedLines(rec_sbNext, 0.2, 4, 2, DARKGRAY);
        DrawRectangleRoundedLines(rec_sbLast, 0.2, 4, 2, DARKGRAY);
        DrawRectangleRoundedLines(rec_sbFind, 0.2, 4, 2, DARKGRAY);
        DrawRectangleRoundedLines(rec_sbAdd, 0.2, 4, 2, DARKGRAY);
        DrawRectangleRoundedLines(rec_sbMod, 0.2, 4, 2, DARKGRAY);
        DrawRectangleRoundedLines(rec_sbDel, 0.2, 4, 2, DARKGRAY);
        DrawRectangleRoundedLines(rec_sbInfo, 0.2, 4, 2, DARKGRAY);
        DrawRectangleRoundedLines(rec_sbExit, 0.2, 4, 2, DARKGRAY);
        //
        // highlight the active speedbuttons under the mouse.
        if (CheckCollisionPointRec(mousePosition, rec_sbFirst))
          if (sbFirst_active)
            DrawRectangleRoundedLines(rec_sbFirst, 0.2, 4, 2, ORANGE);
        if (CheckCollisionPointRec(mousePosition, rec_sbPrev))
          if (sbPrev_active)
            DrawRectangleRoundedLines(rec_sbPrev, 0.2, 4, 2, ORANGE);
        if (CheckCollisionPointRec(mousePosition, rec_sbNext))
          if (sbNext_active)
            DrawRectangleRoundedLines(rec_sbNext, 0.2, 4, 2, ORANGE);
        if (CheckCollisionPointRec(mousePosition, rec_sbLast))
          if (sbLast_active)
            DrawRectangleRoundedLines(rec_sbLast, 0.2, 4, 2, ORANGE);
        if (CheckCollisionPointRec(mousePosition, rec_sbFind))
          if (sbFind_active)
            DrawRectangleRoundedLines(rec_sbFind, 0.2, 4, 2, ORANGE);
        if (CheckCollisionPointRec(mousePosition, rec_sbAdd))
          if (sbAdd_active)
            DrawRectangleRoundedLines(rec_sbAdd, 0.2, 4, 2, ORANGE);
        if (CheckCollisionPointRec(mousePosition, rec_sbMod))
          if (sbMod_active)
            DrawRectangleRoundedLines(rec_sbMod, 0.2, 4, 2, ORANGE);
        if (CheckCollisionPointRec(mousePosition, rec_sbDel))
          if (sbDel_active)
            DrawRectangleRoundedLines(rec_sbDel, 0.2, 4, 2, ORANGE);
        if (CheckCollisionPointRec(mousePosition, rec_sbInfo))
          DrawRectangleRoundedLines(rec_sbInfo, 0.2, 4, 2, ORANGE);
        if (CheckCollisionPointRec(mousePosition, rec_sbExit))
          DrawRectangleRoundedLines(rec_sbExit, 0.2, 4, 2, ORANGE);

        if (sbFirst_active)
          DrawTexture(sbFirst,rec_sbFirst.x,72,WHITE);
        else
          DrawTexture(sbFirst_gray,rec_sbFirst.x,72,WHITE);
        if (sbPrev_active)
          DrawTexture(sbPrev,rec_sbPrev.x,72,WHITE);
        else
          DrawTexture(sbPrev_gray,rec_sbPrev.x,72,WHITE);
        if (sbNext_active)
          DrawTexture(sbNext,rec_sbNext.x,72,WHITE);
        else
          DrawTexture(sbNext_gray,rec_sbNext.x,72,WHITE);
        if (sbLast_active)
          DrawTexture(sbLast,rec_sbLast.x,72,WHITE);
        else
          DrawTexture(sbLast_gray,rec_sbLast.x,72,WHITE);
        if (sbFind_active)
          DrawTexture(sbFind,rec_sbFind.x,72,WHITE);
        else
          DrawTexture(sbFind_gray,rec_sbFind.x,72,WHITE);
        if (sbAdd_active)
          DrawTexture(sbAdd,rec_sbAdd.x,72,WHITE);
        else
          DrawTexture(sbAdd_gray,rec_sbAdd.x,72,WHITE);
        if (sbMod_active)
          DrawTexture(sbMod,rec_sbMod.x,72,WHITE);
        else
          DrawTexture(sbMod_gray,rec_sbMod.x,72,WHITE);
        if (sbDel_active)
          DrawTexture(sbDel,rec_sbDel.x,72,WHITE);
        else
          DrawTexture(sbDel_gray,rec_sbDel.x,72,WHITE);

        DrawTexture(sbInfo, rec_sbInfo.x, 72, WHITE);
        DrawTexture(sbExit, rec_sbExit.x, 72, WHITE);
        //
        commonPos.x = rec_sbFirst.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "First", commonPos, 14, 1, clLabels);
        commonPos.x = rec_sbPrev.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "Prev", commonPos, 14, 1, clLabels);
        commonPos.x = rec_sbNext.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "Next", commonPos, 14, 1, clLabels);
        commonPos.x = rec_sbLast.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "Last", commonPos, 14, 1, clLabels);
        commonPos.x = rec_sbFind.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "Find", commonPos, 14, 1, clLabels);
        commonPos.x = rec_sbAdd.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "Add", commonPos, 14, 1, clLabels);
        commonPos.x = rec_sbMod.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "Mod", commonPos, 14, 1, clLabels);
        commonPos.x = rec_sbDel.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "Del", commonPos, 14, 1, clLabels);
        commonPos.x = rec_sbInfo.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "About", commonPos, 14, 1, clLabels);
        commonPos.x = rec_sbExit.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "Quit", commonPos, 14, 1, clLabels);

        // labels and fields
        commonPos.x = 252-48;
        commonPos.y = 181;
        DrawTextEx(fontLabel, "BREED NAME:", commonPos, 20, 1, clLabels);
        commonPos.x = 270-48;
        commonPos.y = 215;
        if (breed[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, breed, commonPos, 24, 1, clLabels);
        commonPos.x = 252-48;
        commonPos.y = 261;
        DrawTextEx(fontLabel, "CATEGORY:", commonPos, 20, 1, clLabels);
        commonPos.x = 270-48;
        commonPos.y = 295;
        if (cat[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, cat, commonPos, 24, 1, clLabels);
        commonPos.x = 252-48;
        commonPos.y = 341;
        DrawTextEx(fontLabel, "ORIGIN:", commonPos, 20, 1, clLabels);
        commonPos.x = 270-48;
        commonPos.y = 375;
        if (orig[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, orig, commonPos, 24, 1, clLabels);
        commonPos.x = 252-48;
        commonPos.y = 506;
        DrawTextEx(fontLabel, "DESCRIPTION:", commonPos, 20, 1, clLabels);
        commonPos.x = 270-48;
        commonPos.y = 544;
        DrawTextEx(fontDesc, desc, commonPos, 16, 1, clLabels);
        //
        // draw lines and photo frame
        commonPos.x = pMain_posX-48;
        commonPos.y = pMain_posY+110;
        commonPos2.x = commonPos.x + 896.0+96;
        commonPos2.y= commonPos.y;
        DrawLineEx(commonPos, commonPos2, 4.0, clFrames);
        DrawRectangleRounded(rec_photoFrame,0.1,4,clPhoto);
        commonPos.x = rec_photoFrame.x + 176;
        commonPos.y = rec_photoFrame.y + 410;
        DrawTextEx(fontSButton, "372 x 372", commonPos, 16, 2, clLetter);
        if (imgid[0] == '\0')
          DrawTexture(iNoImage,691+48,86,WHITE);
        else
          DrawTexture(bufferTex,691+48,86,WHITE);

        // MOUSE EVENT!!!
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { 
          if (CheckCollisionPointRec(mousePosition, rec_sbFirst))
            if (sbFirst_active)
              currentScreen = 1;
          if (CheckCollisionPointRec(mousePosition, rec_sbPrev))
            if (sbPrev_active)
              currentScreen = 2;
          if (CheckCollisionPointRec(mousePosition, rec_sbNext))
            if (sbNext_active)
              currentScreen = 3;
          if (CheckCollisionPointRec(mousePosition, rec_sbLast))
            if (sbLast_active)
              currentScreen = 4;
          if (CheckCollisionPointRec(mousePosition, rec_sbFind))
            if (sbFind_active)
              currentScreen = 5;
          if (CheckCollisionPointRec(mousePosition, rec_sbAdd))
            if (sbAdd_active) {
              currentScreen = 6; // first, goto (a little detour) last record 
              clear_add_fields();
              clear_mod_fields();
            }
          if (CheckCollisionPointRec(mousePosition, rec_sbMod))
            if (sbMod_active) {
              currentScreen = 7;
              prepare_fields();
            }
          if (CheckCollisionPointRec(mousePosition, rec_sbDel))
            if (sbDel_active)
              currentScreen = 8;
          if (CheckCollisionPointRec(mousePosition, rec_sbInfo))
            currentScreen = 10;
          if (CheckCollisionPointRec(mousePosition, rec_sbExit))
            currentScreen = 11;
        }
        // KEY EVENT
        if (IsKeyPressed(KEY_P))
          if (sbFirst_active)
            currentScreen = 1;
        if (IsKeyPressed(KEY_LEFT))
          if (sbPrev_active)
            currentScreen = 2;
        if (IsKeyPressed(KEY_RIGHT))
          if (sbNext_active)
            currentScreen = 3;
        if (IsKeyPressed(KEY_L))
          if (sbLast_active)
            currentScreen = 4;
        if (IsKeyPressed(KEY_F))
          if (sbFind_active)
            currentScreen = 5;
        if (IsKeyPressed(KEY_A))
          if (sbAdd_active) {
            currentScreen = 6;
            clear_add_fields();
            clear_mod_fields();
          }
        if (IsKeyPressed(KEY_M))
          if (sbMod_active) {
            currentScreen = 7;
            prepare_fields();
          }
        if (IsKeyPressed(KEY_D))
          if (sbDel_active)
            currentScreen = 8;
        if (IsKeyPressed(KEY_I))
          currentScreen = 10;
        if (IsKeyPressed(KEY_X))
          currentScreen = 11;
      }
      /* ==================================================================================
       * FIRST RECORD "SCREEN" - local to MAIN SCREEN
       * ==================================================================================
       */
      if (currentScreen == 1) {
        //
        clear_fields();
        //keydb = gdbm_firstkey(pigeondb);
        d_keyfrst(NAME);
        if (db_status == S_NOTFOUND){
          squabdb_empty = true;
        } else if (db_status == S_OKAY){
          squabdb_empty = false;
          sbFirst_active = false;
          sbPrev_active = false;
          if (!sbNext_active){
            sbLast_active = true;
            sbNext_active = true;
          }
          d_recread(&breeds);
          if(db_status == S_OKAY){
            strcpy(breed, breeds.name);
            strcpy(cat, breeds.category);
            strcpy(orig, breeds.origin);
            strcpy(desc, breeds.description);
            strcpy(imgid, breeds.imageid);
          }
        }
        DrawRectangle(691+48,86,372,372,clBackground);
        load_imageid(imgid, &bufferTex);
        currentScreen = 0;
      }
      /* ==================================================================================
       * PREV RECORD "SCREEN" - local to MAIN SCREEN
       * ==================================================================================
       */
      if (currentScreen == 2) {
        //
        clear_fields();
        d_keyprev(NAME);
        if (db_status == S_NOTFOUND) {
          d_keyfrst(NAME);
          if (db_status == S_NOTFOUND)
            squabdb_empty = true;
          else if (db_status == S_OKAY) {
            squabdb_empty = false;
            sbFirst_active = false;
            sbPrev_active = false;
            if (!sbNext_active){
              sbNext_active = true;
              sbLast_active = true;
            }
            d_recread(&breeds);
            if(db_status == S_OKAY){
              strcpy(breed, breeds.name);
              strcpy(cat, breeds.category);
              strcpy(orig, breeds.origin);
              strcpy(desc, breeds.description);
              strcpy(imgid, breeds.imageid);
            }            
          }  
        } 
        else if (db_status == S_OKAY){
          squabdb_empty = false;
          sbFirst_active = true;
          sbPrev_active = true;
          if (!sbNext_active){
            sbNext_active = true;
            sbLast_active = true;
          }
          d_recread(&breeds);
          if(db_status == S_OKAY){
            strcpy(breed, breeds.name);
            strcpy(cat, breeds.category);
            strcpy(orig, breeds.origin);
            strcpy(desc, breeds.description);
            strcpy(imgid, breeds.imageid);
          }            
        }
        DrawRectangle(691+48,86,372,372,clBackground);
        load_imageid(imgid, &bufferTex);
        currentScreen = 0;
      }
      /* ==================================================================================
       * NEXT RECORD "SCREEN" - local to MAIN SCREEN
       * ==================================================================================
       */
      if (currentScreen == 3) {
        //
        clear_fields();
        d_keynext(NAME);
        if (db_status == S_NOTFOUND) {
          d_keylast(NAME);
          if (db_status == S_NOTFOUND)
            squabdb_empty = true;
          else if (db_status == S_OKAY) {
            squabdb_empty = false;
            sbLast_active = false;
            sbNext_active = false;
            if (!sbFirst_active){
              sbPrev_active = true;
              sbFirst_active = true;
            }
            d_recread(&breeds);
            if(db_status == S_OKAY){
              strcpy(breed, breeds.name);
              strcpy(cat, breeds.category);
              strcpy(orig, breeds.origin);
              strcpy(desc, breeds.description);
              strcpy(imgid, breeds.imageid);
            }            
          }  
        } 
        else if (db_status == S_OKAY){
          squabdb_empty = false;
          sbFirst_active = true;
          sbPrev_active = true;
          if (!sbFirst_active){
            sbPrev_active = true;
            sbFirst_active = true;
          }
          d_recread(&breeds);
          if(db_status == S_OKAY){
            strcpy(breed, breeds.name);
            strcpy(cat, breeds.category);
            strcpy(orig, breeds.origin);
            strcpy(desc, breeds.description);
            strcpy(imgid, breeds.imageid);
          }            
        }
        DrawRectangle(691+48,86,372,372,clBackground);
        load_imageid(imgid, &bufferTex);
        currentScreen = 0;
      }
      /* ==================================================================================
       * LAST RECORD "SCREEN" - local to MAIN SCREEN
       * ==================================================================================
       */
      if (currentScreen == 4) {
        //
        clear_fields();
        //keydb = gdbm_firstkey(pigeondb);
        d_keylast(NAME);
        if (db_status == S_NOTFOUND)
          squabdb_empty = true;
        if (db_status == S_OKAY){
          squabdb_empty = false;
          sbLast_active = false;
          sbNext_active = false;
          if (!sbFirst_active){
            sbFirst_active = true;
            sbPrev_active = true;
          }
          d_recread(&breeds);
          if(db_status == S_OKAY){
            strcpy(breed, breeds.name);
            strcpy(cat, breeds.category);
            strcpy(orig, breeds.origin);
            strcpy(desc, breeds.description);
            strcpy(imgid, breeds.imageid);
          }
        }
        DrawRectangle(691+48,86,372,372,clBackground);
        load_imageid(imgid, &bufferTex);
        currentScreen = 0;
      }
      /* ==================================================================================
       * FIND RECORD "SCREEN" - local to MAIN SCREEN
       * ==================================================================================
       */
      if (currentScreen == 5) {
        //
        //clear_buffer_fields();
        cycle_common = true;
        rec_common.x = 277;
        rec_common.y = 132;
        rec_common.width = 820;
        rec_common.height = 100;
        DrawRectangleRounded(rec_common,0.4,4,DARKGRAY);
        commonPos.x = 293;
        commonPos.y = 173;
        DrawTextEx(fontLabel, "BREED:", commonPos, 20, 1, LIGHTGRAY);
        rec_common.x = 375;
        rec_common.y = 166;
        rec_common.width = 600;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eText,60,iSearchEdit))iSearchEdit =  !iSearchEdit;
        bFindKey_click = GuiButton((Rectangle) {
          990, 166, 88, 32
        }, "Find");
        // EVENTS
        if (bFindKey_click) {
          clear_buffer_fields();
          strcpy(breed_buff, trim(eText, NULL));
          d_keyfind(NAME, breed_buff);
          if (db_status == S_OKAY) {
            d_recread(&breeds);
            if(db_status == S_OKAY){
              strcpy(breed_buff, breeds.name);
              strcpy(cat_buff, breeds.category);
              strcpy(orig_buff, breeds.origin);
              strcpy(desc_buff, breeds.description);
              strcpy(imgid_buff, breeds.imageid);
              load_imageid(imgid_buff,&bufferTex2);
            }
            currentScreen = 14;
          } else currentScreen = 13;
        }
      }
      /* ==================================================================================
       * ADD RECORDS SCREEN
       * ==================================================================================
       */
      if (currentScreen == 6) {
        //
        originated = 6;
        ClearBackground(clBackground);
        DrawRectangle(pMain_posX,pMain_posY,896,640,clPanel); // SECONDARY PANEL
        commonPos.x = 252;
        commonPos.y = 181;
        DrawTextEx(fontLabel, "BREED NAME:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 215;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eBreed,60, iBreedEdit)) iBreedEdit = ! iBreedEdit;
        commonPos.x = 252;
        commonPos.y = 261;
        DrawTextEx(fontLabel, "CATEGORY ('High flyer','Roller','Tumbler'):", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 295;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eCat,60,iCategoryEdit)) iCategoryEdit =  !iCategoryEdit;
        commonPos.x = 252;
        commonPos.y = 341;
        DrawTextEx(fontLabel, "ORIGIN (Town and Country):", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 375;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eOrig,60,iOriginEdit)) iOriginEdit =  !iOriginEdit;
        commonPos.x = 252;
        commonPos.y = 421;
        DrawTextEx(fontLabel, "IMAGE ID (image file name in 'pictures' subfolder):", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 455;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eImgid,60,iImageidEdit)) iImageidEdit = !iImageidEdit;
        commonPos.x = 252;
        commonPos.y = 506;
        DrawTextEx(fontLabel, "DESCRIPTION (100 characters maxim):", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 544;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc1,80, iDesc1Edit)) iDesc1Edit = ! iDesc1Edit;
        rec_common.x = 270;
        rec_common.y = 580;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc2,80, iDesc2Edit)) iDesc2Edit = ! iDesc2Edit;
        //
        commonPos.x = pMain_posX;
        commonPos.y = pMain_posY+110;
        commonPos2.x = commonPos.x + 896.0;
        commonPos2.y = commonPos.y;
        DrawLineEx(commonPos, commonPos2, 4.0, clFrames);
        DrawTexture(iAddLarge,192,65,WHITE);
        DrawTextEx(fontTitle, "Add a new record.", (Vector2) {
          277,83
        }, 56, 2, clLabels);
        bAddSave_click   = GuiButton((Rectangle) {
          461, 630, 88, 32
        }, "Save");
        bAddCancel_click = GuiButton((Rectangle) {
          791, 630, 88, 32
        }, "Cancel");
        if (bAddSave_click) {
          // transfer the fields in the buffer and update the record
          strcpy(eBreed_buff, trim(eBreed,NULL));
          strcpy(eCat_buff, trim(eCat,NULL));
          strcpy(eOrig_buff, trim(eOrig,NULL));
          strcpy(eImgid_buff, trim(eImgid,NULL));
          strcpy(eDesc_buff, ltrim(eDesc1,NULL));
          strcat(eDesc_buff, rtrim(eDesc2,NULL));
          //
          add_err = 0;
          if (eBreed_buff[0] == '\0') {
            add_err = 1;
            strcpy(err_msg, "The key (breed) can't be empty!");
          }
          if (eCat_buff[0] == '\0') {
            add_err = 1;
            strcpy(err_msg, "The category can't be empty!");
          }
          if (eDesc_buff[0] == '\0') {
            add_err = 1;
            strcpy(err_msg, "The description can't be empty!");
          }
          if (eImgid_buff[0] == '\0')
            strcpy(eImgid_buff, "noimage.png");
          // display error:
          if(add_err > 0) currentScreen = 17;
          if (add_err == 0) {
            // 
            // ADD NEW RECORD!
            //
            strcpy(breeds.name, eBreed_buff);
            strcpy(breeds.category, eCat_buff);
            strcpy(breeds.origin, eOrig_buff);
            strcpy(breeds.description, eDesc_buff);
            strcpy(breeds.imageid, eImgid_buff);
            if (d_keyfind(NAME,eBreed_buff) == S_NOTFOUND){
              if (d_fillnew(BREEDS, &breeds) == S_OKAY){
                d_recread(&breeds);
                if(db_status == S_OKAY){
                  strcpy(breed, breeds.name);
                  strcpy(cat, breeds.category);
                  strcpy(orig, breeds.origin);
                  strcpy(desc, breeds.description);
                  strcpy(imgid, breeds.imageid);
                }
                //
                if (squabdb_empty){
                  squabdb_empty = false;
                  sbFirst_active = false;
                  sbNext_active  = true;
                  sbFind_active  = true;
                  sbAdd_active   = true;
                  sbMod_active   = true;
                  sbDel_active   = true;
                }
                clear_add_fields();
                clear_buffer_fields();
                DrawRectangle(691+48,86,372,372,clBackground);
                load_imageid(imgid, &bufferTex);
                currentScreen = 1; // jump to the first key in the database
              } else printf("Error adding a new record!\r\n");
            }else {
              currentScreen = 17;
              strcpy(err_msg, "Sorry, this breed is already here!"); 
            }
          }
        }
        if (bAddCancel_click) {
          // no transfer...
          currentScreen = 0;
        }
      }
      /* ==================================================================================
       * MODIFY RECORDS SCREEN
       * ==================================================================================
       */
      if (currentScreen == 7) {
        //
        originated = 7;
        ClearBackground(clBackground);
        DrawRectangle(pMain_posX,pMain_posY,896,640,clPanel); // SECONDARY PANEL
        commonPos.x = 252;
        commonPos.y = 181;
        DrawTextEx(fontLabel, "BREED NAME:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 215;
        DrawTextEx(fontLetter, breed,commonPos, 24,1,clLabels);
        commonPos.x = 252;
        commonPos.y = 261;
        DrawTextEx(fontLabel, "CATEGORY:", commonPos, 20, 1, clLabels);
        if (GuiTextBox((Rectangle) {
          270,295,760,32
        },eCat,60,iCategoryEdit)) iCategoryEdit =  !iCategoryEdit;
        commonPos.x = 252;
        commonPos.y = 341;
        DrawTextEx(fontLabel, "ORIGIN:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 375;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eOrig,60,iOriginEdit)) iOriginEdit =  !iOriginEdit;
        commonPos.x = 252;
        commonPos.y = 421;
        DrawTextEx(fontLabel, "IMAGE ID:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 455;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eImgid,60,iImageidEdit)) iImageidEdit = !iImageidEdit;
        commonPos.x = 252;
        commonPos.y = 506;
        DrawTextEx(fontLabel, "DESCRIPTION:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 544;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc1,80, iDesc1Edit)) iDesc1Edit = ! iDesc1Edit;
        rec_common.x = 270;
        rec_common.y = 580;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc2,80, iDesc2Edit)) iDesc2Edit = ! iDesc2Edit;
        //
        commonPos.x = pMain_posX;
        commonPos.y = pMain_posY+110;
        commonPos2.x = commonPos.x + 896.0;
        commonPos2.y = commonPos.y;
        DrawLineEx(commonPos, commonPos2, 4.0, clFrames);
        DrawTexture(iModLarge,201,65,WHITE);
        DrawTextEx(fontTitle, "Modify the record (navigation).", (Vector2) {
          277,83
        }, 56, 2, clLabels);
        bModSave_click = GuiButton((Rectangle) {
          461, 630, 88, 32
        }, "Save");
        bModCancel_click = GuiButton((Rectangle) {
          791, 630, 88, 32
        }, "Cancel");
        if (bModSave_click) {
          mod_err = 0;
          // transfer the fields in the buffer and update the record if no err
          strcpy(cat, trim(eCat,NULL));
          strcpy(orig, trim(eOrig,NULL));
          strcpy(imgid, trim(eImgid,NULL));
          strcpy(desc, ltrim(eDesc1, NULL));
          strcat(desc, rtrim(eDesc2, NULL));
          if (cat[0] == '\0'){
            mod_err = 1;
            strcpy(err_msg, "Category can't be empty!");
          }
          if (orig[0] == '\0'){
            mod_err = 1;
            strcpy(err_msg, "Origin can't be empty!");
          }
          if (desc[0] == '\0'){
            mod_err = 1;
            strcpy(err_msg, "Description can't be empty!");
          }
          if (imgid[0] == '\0'){
            strcpy(imgid, "noimage.png");
          }
          if(mod_err > 0) currentScreen = 17;
          if(mod_err == 0){
            currentScreen = 0;
            //
            // UPDATE THE RECORD!
            //
            strcpy(breeds.name, breed);
            strcpy(breeds.category, cat);
            strcpy(breeds.origin, orig);
            strcpy(breeds.description, desc);
            strcpy(breeds.imageid, imgid);
            if(d_recwrite(&breeds) == S_OKAY ){
              printf("Record updated!\r\n");
              d_recread(&breeds);
              if(db_status == S_OKAY){
                strcpy(breed, breeds.name);
                strcpy(cat, breeds.category);
                strcpy(orig, breeds.origin);
                strcpy(desc, breeds.description);
                strcpy(imgid, breeds.imageid);
              }               
            }
            else 
              printf("Error updating the record!\r\n");
          }
        }
        if (bModCancel_click) {
          // no transfer...
          currentScreen = 0;
          d_recread(&breeds);
          if(db_status == S_OKAY){
            strcpy(breed, breeds.name);
            strcpy(cat, breeds.category);
            strcpy(orig, breeds.origin);
            strcpy(desc, breeds.description);
            strcpy(imgid, breeds.imageid);
          } 
        }
      }
      /* ==================================================================================
       * DELETE RECORDS "SCREEN" - local to MAIN SCREEN
       * ==================================================================================
       */
      if (currentScreen == 8) {
        //
        rec_common.x = 326;
        rec_common.y = 508;
        rec_common.width = 660;
        rec_common.height = 100;
        DrawRectangleRounded(rec_common, 0.2,4,clBrick);
        rec_common.x = 441;
        rec_common.y = 589;
        rec_common.width = 100;
        rec_common.height = 40;
        DrawRectangleRounded(rec_common, 0.6,4,clBrick);
        rec_common.x = 771;
        rec_common.y = 589;
        rec_common.width = 100;
        rec_common.height = 40;
        DrawRectangleRounded(rec_common, 0.6,4,clBrick);
        commonPos.x = 501;
        commonPos.y = 542;
        DrawTextEx(fontLabel, "Do you really want to delete this record?", commonPos, 20, 1, WHITE);
        bDelYes_click = GuiButton((Rectangle) {
          461, 590, 60, 28
        }, "Yes");
        bDelNo_click = GuiButton((Rectangle) {
          791, 590, 60, 28
        }, "No");
        if ((bDelNo_click) || (IsKeyPressed(KEY_N)))
          currentScreen = 0;
        if ((bDelYes_click) || (IsKeyPressed(KEY_Y))) {
          currentScreen = 1; // go to reading the first key...
          if(d_delete() == S_OKAY) printf("Record deleted!\r\n"); 
          else  printf("Error deleting the record!\r\n");
        }
      }
      /* ==================================================================================
       * ABOUT SCREEN
       * ==================================================================================
       */
      if (currentScreen == 10) {
        //
        stp = 60;
        ClearBackground(clBackground);
        DrawTexture(iGrass1a, pSplash_posX-82, pSplash_posY + 285, WHITE);
        DrawTexture(iAbout, 890, 11, WHITE);
        DrawRectangle(pSplash_posX,pSplash_posY,896-200,664-204,clPanel);
        if (cbTheme_idx == 0) { // dark theme
          DrawTexture(iFirefly2, 236, 494, WHITE);
          DrawTexture(iFirefly1, 298, 478, WHITE);
        } else // light theme
          DrawTexture(iLadybug, 291, 516, WHITE);
        commonPos.x = pSplash_posX + 20;
        commonPos.y = pSplash_posY + 10;
        DrawTextEx(fontTitle, "Actually, I wanna give thanks:", commonPos, 56, 2, clLabels);
        commonPos.x = pSplash_posX + 20;
        commonPos.y = pSplash_posY + stp + 75;
        DrawTextEx(fontLetter, "- to Lorena, my daughter for graphics advising;", commonPos, 24, 2, clLabels);
        commonPos.x = pSplash_posX + 20;
        commonPos.y = pSplash_posY + stp + 103;
        DrawTextEx(fontLetter, "- to FreePascal team and forums;", commonPos, 24, 2, clLabels);
        commonPos.x = pSplash_posX + 20;
        commonPos.y = pSplash_posY + stp + 131;
        DrawTextEx(fontLetter, "- to Gunko Vadim for raylib pascal translation;", commonPos, 24, 2, clLabels);
        commonPos.x = pSplash_posX + 20;
        commonPos.y = pSplash_posY + stp + 159;
        DrawTextEx(fontLetter, "- to Thomas B. Pedersen, original developer of typhoon;", commonPos, 24, 2, clLabels);
        commonPos.x = pSplash_posX + 20;
        commonPos.y = pSplash_posY + stp + 187;
        DrawTextEx(fontLetter, "- to Kaz Kylheku for update on typhoon library;", commonPos, 24, 2, clLabels);
        commonPos.x = pSplash_posX + 20;
        commonPos.y = pSplash_posY + stp + 215;
        DrawTextEx(fontLetter, "- to Ramon Santamaria and raylib development team.", commonPos, 24, 2, clLabels);
        bQuitYes_click = GuiButton((Rectangle) {
          pSplash_posX + 315, pSplash_posY + 398, 88, 32
        }, "OK");
        if ((bQuitYes_click) || IsKeyPressed(KEY_ENTER))
          currentScreen = 0;
      }
      /* ==================================================================================
       * EXIT SCREEN
       * ==================================================================================
       */
      if (currentScreen == 11) {
        //
        ClearBackground(clBackground);
        DrawRectangle(pSplash_posX,pSplash_posY,896-200,664-204,clPanel);
        DrawTexture(iPigeon_Bye, 200,100, WHITE);
        commonPos.x = (float)pSplash_posX + 320;
        commonPos.y = (float)pSplash_posY + 130;
        DrawTextEx(fontTitle, "Quitting?", commonPos, 56, 2, clLabels);
        commonPos.x = (float)pSplash_posX + 280;
        commonPos.y = (float)pSplash_posY + 230;
        DrawTextEx(fontLetter, "... it seems you wanna leave...", commonPos, 24, 2, clLabels);
        DrawTexture(iSlime1, pSplash_posX + 105,pSplash_posY + 420, WHITE);
        DrawTexture(iSlime2, pSplash_posX + 136,pSplash_posY + 355, WHITE);
        bQuitYes_click = GuiButton((Rectangle) {
          pSplash_posX + 100, pSplash_posY + 398, 88, 32
        }, "Yes");
        bQuitNo_click = GuiButton((Rectangle) {
          pSplash_posX + 530, pSplash_posY + 398, 88, 32
        }, "No");
        if (bQuitYes_click)
          exitWindow = true;
        if (bQuitNo_click)
          currentScreen = 0;
        if (IsKeyPressed(KEY_Y))
          exitWindow = true;
        if (IsKeyPressed(KEY_N))
          currentScreen = 0;
      }
      /* ==================================================================================
       * ERROR NEXTKEY "SCREEN" - local to MAIN
       * ==================================================================================
       */
      if (currentScreen == 12) {
        //
        rec_common.x = 326;
        rec_common.y = 508;
        rec_common.width = 660;
        rec_common.height = 100;
        DrawRectangleRounded(rec_common, 0.2,4,clBrick);
        rec_common.x = 611;
        rec_common.y = 589;
        rec_common.width = 100;
        rec_common.height = 40;
        DrawRectangleRounded(rec_common, 0.6,4,clBrick);
        commonPos.x = 546;
        commonPos.y = 542;
        DrawTextEx(fontLabel, "Already at the last record!", commonPos, 20, 1, WHITE);
        bErrorNextKey_click = GuiButton((Rectangle) {
          631, 590, 60, 28
        }, "Ok");
        if (bErrorNextKey_click) {
          currentScreen = 0;
          bErrorNextKey_click = false;
        }
        if (IsKeyPressed(KEY_ENTER)) {
          currentScreen = 0;
          bErrorNextKey_click = false;
        }

      }
      /* ==================================================================================
       * ERROR FIND KEY  "SCREEN" - local to MAIN
       * ==================================================================================
       */
      if (currentScreen == 13) {
        //
        rec_common.x = 326;
        rec_common.y = 508;
        rec_common.width = 660;
        rec_common.height = 100;
        DrawRectangleRounded(rec_common, 0.2,4,clBrick);
        rec_common.x = 611;
        rec_common.y = 589;
        rec_common.width = 100;
        rec_common.height = 40;
        DrawRectangleRounded(rec_common, 0.6,4,clBrick);
        commonPos.x = 540;
        commonPos.y = 542;
        DrawTextEx(fontLabel, "Sorry, there is no such key!", commonPos, 20, 1, WHITE);
        bErrorFindKey_click = GuiButton((Rectangle) {
          631, 590, 60, 28
        }, "Ok");
        if (bErrorFindKey_click) {
          currentScreen = originated;
          bErrorFindKey_click = false;
        }
        if (IsKeyPressed(KEY_ENTER)) {
          currentScreen = originated;
          bErrorFindKey_click = false;
        }
      }
      /* ==================================================================================
       * FIND RESULT SCREEN
       * ==================================================================================
       */
      if (currentScreen == 14) {
        //
        originated = 14;
        ClearBackground(clBackground);
        DrawRectangle(pMain_posX,pMain_posY,896,600,clPanel);
        commonPos.x = 252;
        commonPos.y = 181;
        DrawTextEx(fontLabel, "BREED NAME:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 215;
        if (breed_buff[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, breed_buff, commonPos, 24, 1, clLabels);
        commonPos.x = 252;
        commonPos.y = 261;
        DrawTextEx(fontLabel, "CATEGORY:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 295;
        if (cat_buff[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, cat_buff, commonPos, 24, 1, clLabels);
        commonPos.x = 252;
        commonPos.y = 341;
        DrawTextEx(fontLabel, "ORIGIN:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 375;
        if (orig_buff[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, orig_buff, commonPos, 24, 1, clLabels);
        commonPos.x = 252;
        commonPos.y = 506;
        DrawTextEx(fontLabel, "DESCRIPTION:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 544;
        DrawTextEx(fontDesc, desc_buff, commonPos, 16, 1, clLabels);
        //
        // draw lines and photo frame
        commonPos.x = pMain_posX;
        commonPos.y = pMain_posY+110;
        commonPos2.x = commonPos.x + 896.0;
        commonPos2.y= commonPos.y;
        DrawLineEx(commonPos, commonPos2, 4.0, clFrames);
        DrawTexture(iFindLarge,189,13,WHITE);
        DrawTextEx(fontTitle, "I Found It!", (Vector2) {
          337,83
        }, 56, 2, clLabels);
        rec_common.x = 677;
        rec_common.y = 72;
        rec_common.width = 400;
        rec_common.height = 400;
        DrawRectangleRounded(rec_common,0.1,4,clPhoto);
        if (imgid_buff[0] == '\0')
          DrawTexture(iNoImage,691,86,WHITE);
        else
          DrawTexture(bufferTex2,691,86,WHITE);

        bResultMod_click   = GuiButton((Rectangle) {
          762, 576, 88, 32
        }, "Modify");
        bResultDel_click   = GuiButton((Rectangle) {
          862, 576, 88, 32
        }, "Delete");
        bResultClose_click = GuiButton((Rectangle) {
          962, 576, 88, 32
        }, "Close");

        if ((bResultClose_click) || (IsKeyPressed(KEY_C))) {
          currentScreen = 1;
          bResultClose_click = false;
        }
        if ((bResultDel_click) || (IsKeyPressed(KEY_D))) {
          currentScreen = 15;
          bResultDel_click = false;
        }
        if ((bResultMod_click) || (IsKeyPressed(KEY_M))) {
          ln = strlen(desc_buff);
          currentScreen = 16;
          bResultMod_click = false;
          strcpy(eCat_buff, cat_buff);
          strcpy(eOrig_buff, orig_buff);
          strcpy(eImgid_buff, imgid_buff);
          if (ln <=80) {
            strcpy(eDesc1_buff, desc_buff);
            eDesc2_buff[0] = '\0';
          } else {
            strncpy(eDesc1_buff, desc_buff, 80);
            for(i=80; i<ln; i++)
              eDesc2_buff[i-80] = desc_buff[i];
            eDesc2_buff[ln-80] = '\0';
          }
        }
      }
      /* ==================================================================================
       * FIND RESULT DELETE SCREEN
       * ==================================================================================
       */
      if (currentScreen == 15) {
        rec_common.x = 326;
        rec_common.y = 508;
        rec_common.width = 660;
        rec_common.height = 100;
        DrawRectangleRounded(rec_common, 0.2,4,clBrick);
        rec_common.x = 441;
        rec_common.y = 589;
        rec_common.width = 100;
        rec_common.height = 40;
        DrawRectangleRounded(rec_common, 0.6,4,clBrick);
        rec_common.x = 771;
        rec_common.y = 589;
        rec_common.width = 100;
        rec_common.height = 40;
        DrawRectangleRounded(rec_common, 0.6,4,clBrick);
        commonPos.x = 501;
        commonPos.y = 542;
        DrawTextEx(fontLabel, "Do you really want to delete this record?", commonPos, 20, 1, WHITE);
        bResultDelYes_click = GuiButton((Rectangle) {
          461, 590, 60, 28
        }, "Yes");
        bResultDelNo_click = GuiButton((Rectangle) {
          791, 590, 60, 28
        }, "No");
        if ((bResultDelNo_click) || (IsKeyPressed(KEY_N)))
          currentScreen = 14;
        if ((bResultDelYes_click)  || (IsKeyPressed(KEY_Y))) {
          currentScreen = 1;
          if(d_delete() != S_OKAY)
            printf("Record can't be deleted!\r\n");
        }
      }
      /* ==================================================================================
       * FIND RESULT MODIFY SCREEN
       * ==================================================================================
       */
      if (currentScreen == 16) {
        originated = 16;
        ClearBackground(clBackground);
        DrawRectangle(pMain_posX,pMain_posY,896,640,clPanel); // SECONDARY PANEL
        commonPos.x = 252;
        commonPos.y = 181;
        DrawTextEx(fontLabel, "BREED NAME:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 215;
        DrawTextEx(fontLetter, breed_buff,commonPos, 24,1,clLabels);
        commonPos.x = 252;
        commonPos.y = 261;
        DrawTextEx(fontLabel, "CATEGORY:", commonPos, 20, 1, clLabels);
        if (GuiTextBox((Rectangle) {
          270,295,760,32
        },eCat_buff,60,iCategoryEdit)) iCategoryEdit =  !iCategoryEdit;
        commonPos.x = 252;
        commonPos.y = 341;
        DrawTextEx(fontLabel, "ORIGIN:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 375;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eOrig_buff,60,iOriginEdit)) iOriginEdit =  !iOriginEdit;
        commonPos.x = 252;
        commonPos.y = 421;
        DrawTextEx(fontLabel, "IMAGE ID:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 455;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eImgid_buff,60,iImageidEdit)) iImageidEdit = !iImageidEdit;
        commonPos.x = 252;
        commonPos.y = 506;
        DrawTextEx(fontLabel, "DESCRIPTION:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 544;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc1_buff,80, iDesc1Edit)) iDesc1Edit = ! iDesc1Edit;
        rec_common.x = 270;
        rec_common.y = 580;
        rec_common.width = 760;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc2_buff,80, iDesc2Edit)) iDesc2Edit = ! iDesc2Edit;
        //
        commonPos.x = pMain_posX;
        commonPos.y = pMain_posY+110;
        commonPos2.x = commonPos.x + 896.0;
        commonPos2.y = commonPos.y;
        DrawLineEx(commonPos, commonPos2, 4.0, clFrames);
        DrawTexture(iModLarge,201,65,WHITE);
        DrawTextEx(fontTitle, "Modify the record (search result).", (Vector2) {
          277,83
        }, 56, 2, clLabels);
        bModSave_click = GuiButton((Rectangle) {
          461, 630, 88, 32
        }, "Save");
        bModCancel_click = GuiButton((Rectangle) {
          791, 630, 88, 32
        }, "Cancel");
        if (bModSave_click) {
          mod_err = 0;
          // transfer the fields in the buffer and update the record if no err
          strcpy(cat_buff, trim(eCat_buff,NULL));
          strcpy(orig_buff, trim(eOrig_buff,NULL));
          strcpy(imgid_buff, trim(eImgid_buff,NULL));
          strcpy(desc_buff, ltrim(eDesc1_buff, NULL));
          strcat(desc_buff, rtrim(eDesc2_buff, NULL));
          if (cat_buff[0] =='\0'){
            mod_err = 1;
            strcpy(err_msg, "Category can't be empty!");
          }
          if (orig_buff[0] =='\0'){
            mod_err = 1;
            strcpy(err_msg, "Origin can't be empty!");
          }
          if (desc_buff[0] =='\0'){
            mod_err = 1;
            strcpy(err_msg, "Description can't be empty!");
          }
          if (imgid_buff[0] == '\0')
            strcpy(imgid_buff, "noimage.png");
          if (mod_err > 0) currentScreen = 17;
          if (mod_err == 0){
            currentScreen = 14;
            strcpy(breeds.name, breed_buff);
            strcpy(breeds.category, cat_buff);
            strcpy(breeds.origin, orig_buff);
            strcpy(breeds.description, desc_buff);
            strcpy(breeds.imageid, imgid_buff);
            if(d_recwrite(&breeds) == S_OKAY ){
              printf("Record updated!\r\n");
            }
            load_imageid(imgid_buff, &bufferTex2);
          }
        }
        if (bModCancel_click) {
          // no transfer...
          currentScreen = 14;
        }
      }
      /* ==================================================================================
       * ERRORS FROM ADD AND MOD SCREENS
       * ==================================================================================
       */
      if (currentScreen == 17) {
        //
        rec_common.x = 326;
        rec_common.y = 508;
        rec_common.width = 660;
        rec_common.height = 100;
        DrawRectangleRounded(rec_common, 0.2,4,clBrick);
        rec_common.x = 611;
        rec_common.y = 589;
        rec_common.width = 100;
        rec_common.height = 40;
        DrawRectangleRounded(rec_common, 0.6,4,clBrick);
        commonPos.x = 440;
        commonPos.y = 542;
        DrawTextEx(fontLabel, err_msg, commonPos, 20, 1, WHITE);
        bErrorAddKey_click = GuiButton((Rectangle) {
          631, 590, 60, 28
        }, "Ok");
        if ((bErrorAddKey_click) || (IsKeyPressed(KEY_ENTER))) {
          currentScreen = originated;
          bErrorAddKey_click = false;
        }
      }
      EndDrawing();
    }
    d_close();
    UnloadFont(fontTitle);
    UnloadFont(fontLetter);
    UnloadFont(fontLabel);
    UnloadFont(fontSButton);
    UnloadFont(fontDesc);
    //
    UnloadTexture(iPigeon_Welcome);
    //
    UnloadTexture(iPigeon_Bye);
    UnloadTexture(iSlime1);
    UnloadTexture(iSlime2);
    //
    UnloadTexture(iAbout);
    UnloadTexture(iGrass1a);
    UnloadTexture(iGrass2b);
    UnloadTexture(iFirefly1);
    UnloadTexture(iFirefly2);
    UnloadTexture(iLadybug);
    // search + navigation
    UnloadTexture(iNoImage);
    UnloadTexture(bufferTex);
    UnloadTexture(bufferTex2);
    // find,add,modify forms
    UnloadTexture(iFindLarge);
    UnloadTexture(iAddLarge);
    UnloadTexture(iModLarge);
    //
    UnloadTexture(sbDel_gray);
    UnloadTexture(sbMod_gray);
    UnloadTexture(sbAdd_gray);
    UnloadTexture(sbFind_gray);
    UnloadTexture(sbLast_gray);
    UnloadTexture(sbNext_gray);
    UnloadTexture(sbPrev_gray);
    UnloadTexture(sbFirst_gray);
    //
    UnloadTexture(sbExit);
    UnloadTexture(sbInfo);
    UnloadTexture(sbDel);
    UnloadTexture(sbMod);
    UnloadTexture(sbAdd);
    UnloadTexture(sbFind);
    UnloadTexture(sbLast);
    UnloadTexture(sbNext);
    UnloadTexture(sbPrev);
    UnloadTexture(sbFirst);
    //
  } else printf("Error when creating/opening database!\r\n");
  return 0;
}


