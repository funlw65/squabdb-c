/*
 * SquabDB is a small Pigeon Database application,
 * intended as a demonstration for qdbm database engine and
 * for raylib 4.0 + raygui 3.0
 *
 * Copyright 2022 by Vasile Guta-Ciucur and Lorena Guta-Ciucur
 *
 * Application released under BSD license.
 *
 */

#include <stdint.h>
#include <hovel.h> /* gdbm compatible functions */
#include <raylib.h>
#define  RAYGUI_IMPLEMENTATION
#include <raygui.h>
#include <string.h>
#include <unistd.h>
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
Texture2D sbFirst,sbNext,sbFind,sbAdd,sbMod,sbDel,sbInfo,sbExit;
Texture2D sbFirst_gray,sbNext_gray,sbFind_gray,sbAdd_gray,sbMod_gray,sbDel_gray;

Vector2 mousePosition, commonPos, commonPos2;

Rectangle rec_cbTheme, rec_photoFrame, rec_common;
Rectangle rec_sbFirst;
Rectangle rec_sbNext;
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
bool sbNext_active;
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
GDBM_FILE  pigeondb; // qdbm file
datum      keydb, datadb, keydb2, datadb2, keydb_temp;
gdbm_error pigeondb_error;
bool       squabdb_empty, cycle_common;

char image_path[71] = "";
char project_subfolder[10] = "pictures/";
//
char breed[60] = "";
char category[11] = "";
char origin[60] = "";
char imageid[60] = "";
char description[100] = "";
char desc1[50] = "";
char desc2[50] = "";
char recordBuffer[294] = "";
char recordBuffer2[294] = "";
//
char eBreed[60] = "";
char eCategory[11] = "";
char eOrigin[60] = "";
char eImageid[60] = "";
char eDescription[100] = "";
char eDesc1[50] = "";
char eDesc2[50] = "";
//
char breed_buff[60] = "";
char category_buff[11] = "";
char origin_buff[60] = "";
char imageid_buff[60] = "";
char description_buff[100] = "";
char desc1_buff[50] = "";
char desc2_buff[50] = "";
//
char eBreed_buff[60] = "";
char eCategory_buff[11] = "";
char eOrigin_buff[60] = "";
char eImageid_buff[60] = "";
char eDescription_buff[100] = "";
char eDesc1_buff[50] = "";
char eDesc2_buff[50] = "";

char eText[60] = "";

char err_msg[50] = "";

void clear_mod_fields(void)
{
  eBreed_buff[0] = '\0';
  eCategory_buff[0] = '\0';
  eOrigin_buff[0] = '\0';
  eImageid_buff[0] = '\0';
  eDescription_buff[0] = '\0';
  eDesc1_buff[0] = '\0';
  eDesc2_buff[0] = '\0';
}


void clear_add_fields(void)
{
  eBreed[0] = '\0';
  eCategory[0] = '\0';
  eOrigin[0] = '\0';
  eImageid[0] = '\0';
  //description[0] = '\0';
  eDesc1[0] = '\0';
  eDesc2[0] = '\0';
}

void clear_buffer_fields(void)
{
  breed_buff[0] = '\0';
  category_buff[0] = '\0';
  origin_buff[0] = '\0';
  imageid_buff[0] = '\0';
  //description_buff[100] = '\0';
  desc1_buff[0] = '\0';
  desc2_buff[0] = '\0';
  //
}

void clear_fields(void)
{
  //
  breed[0] = '\0';
  category[0] = '\0';
  origin[0] = '\0';
  imageid[0] = '\0';
  //description[0] = '\0';
  desc1[0] = '\0';
  desc2[0] = '\0';
  recordBuffer[0] = '\0';
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

void split_words3sep(char *s1, char *s2, char *s3, char *s4, char *s5)
{
  uint16_t i,z,ln;
  uint8_t cnt;
  cnt=0;
  ln = strlen(s5);
  for(i=0; i<ln; i++)  // check separators
    if(s5[i]==';') cnt++;
  if(cnt==3) {
    for(i=0; i<ln; i++) {
      if(s5[i] != ';')
        s1[i] = s5[i];
      else {
        s1[i] = '\0';
        z = i+1;
        break;
      }
    }
    for(i=z; i<ln; i++) {
      if(s5[i] != ';')
        s2[i-z] = s5[i];
      else {
        s2[i-z] = '\0';
        z = i+1;
        break;
      }
    }
    for(i=z; i<ln; i++) {
      if(s5[i] != ';')
        s3[i-z] = s5[i];
      else {
        s3[i-z] = '\0';
        z = i+1;
        break;
      }
    }
    for(i=z; i<ln; i++) s4[i-z] = s5[i];
    s4[ln-z] = '\0';
  } else {
    //
    s1[0] = '\00';
    s2[0] = '\00';
    s3[0] = '\00';
    s4[0] = '\00';
    printf("Error! Malformed data - more or less than 3 ';' separators!\r\n");
  }
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
  ln = strlen(description);
  strcpy(eCategory, category);
  strcpy(eOrigin, origin);
  strcpy(eImageid, imageid);
  if (ln <= 50) {
    strcpy(eDesc1, description);
    eDesc2[0] = '\0';
  } else {
    strncpy(eDesc1, description, 50);
    for(i=50; i<ln; i++)
      eDesc2[i-50] = description[i];
    eDesc2[ln-50] = '\0';
  }
}

int main(void)
{
  //atexit(report_mem_leak);
  //--------------------------------------------------------------------
  // Database stuff
  //--------------------------------------------------------------------
  clear_fields();
  pigeondb=gdbm_open("squabdb.dat",1024,GDBM_WRCREAT | GDBM_SYNC,432,NULL);
  if (pigeondb != NULL) {
    keydb = gdbm_firstkey(pigeondb);
    keydb2 = keydb; // initialize also keydb2...
    if (keydb.dptr == NULL) {
      squabdb_empty = true;
      printf("Database empty!\r\n");
    } else {
      squabdb_empty = false;
      strcpy(breed, keydb.dptr);
      datadb = gdbm_fetch(pigeondb,keydb);
      datadb2 = datadb; // initialize also datadb2...
      strcpy(recordBuffer, datadb.dptr);
      split_words3sep(category,origin,description,imageid, recordBuffer);
      //
      sbFirst_active = false;
      sbNext_active  = true;
      sbFind_active  = true;
      sbAdd_active   = true;
      sbMod_active   = true;
      sbDel_active   = true;
    }
    //----------------------------------------------------------------
    // GRAPHIC STUFF
    //----------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "SquabDB - Pigeon Database");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
    GuiSetState(GUI_STATE_NORMAL);
    SetTargetFPS(60);
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
    load_imageid(imageid, &bufferTex);
    bufferIcon = LoadImage("pictures/noimage.png");
    iNoImage = LoadTextureFromImage(bufferIcon);
    UnloadImage(bufferIcon);

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
    bufferIcon = LoadImage("img/general/next.png");
    sbNext = LoadTextureFromImage(bufferIcon);
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
    bufferIcon = LoadImage("img/general/next_gray.png");
    sbNext_gray = LoadTextureFromImage(bufferIcon);
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

    rec_sbFirst.x = 252;
    rec_sbFirst.y = 72;
    rec_sbFirst.width = 32;
    rec_sbFirst.height = 32;

    rec_sbNext.x = 300;
    rec_sbNext.y = 72;
    rec_sbNext.width = 32;
    rec_sbNext.height = 32;

    rec_sbFind.x = 348;
    rec_sbFind.y = 72;
    rec_sbFind.width = 32;
    rec_sbFind.height = 32;

    rec_sbAdd.x = 396;
    rec_sbAdd.y = 72;
    rec_sbAdd.width = 32;
    rec_sbAdd.height = 32;

    rec_sbMod.x = 444;
    rec_sbMod.y = 72;
    rec_sbMod.width = 32;
    rec_sbMod.height = 32;

    rec_sbDel.x = 492;
    rec_sbDel.y = 72;
    rec_sbDel.width = 32;
    rec_sbDel.height = 32;

    rec_sbInfo.x = 540;
    rec_sbInfo.y = 72;
    rec_sbInfo.width = 32;
    rec_sbInfo.height = 32;

    rec_sbExit.x = 588;
    rec_sbExit.y = 72;
    rec_sbExit.width = 32;
    rec_sbExit.height = 32;

    //
    rec_photoFrame.x = 677;
    rec_photoFrame.y = 72;
    rec_photoFrame.width = 400;
    rec_photoFrame.height = 400;


    uint8_t originated = 2;
    uint8_t currentScreen = 1;
    while(!exitWindow) {
      mousePosition = GetMousePosition();
      BeginDrawing();
      /* ==================================================================================
       * SPLASH SCREEN
       * ==================================================================================
       */
      if (currentScreen == 1) {
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
        DrawTextEx(fontLetter, "demonstrator for qdbm database engine, raylib", commonPos, 24, 2, clLabels);
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
          currentScreen = 2;
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
      if (currentScreen == 2) {
        //
        originated = 2;
        if (squabdb_empty) {
          sbFirst_active = false;
          sbNext_active = false;
          sbFind_active = false;
          sbAdd_active = true;
          sbMod_active = false;
          sbDel_active = false;
        }
        //exitWindow := WindowShouldClose;
        ClearBackground(clBackground);
        DrawTexture(iGrass1a, pMain_posX-82, pMain_posY + 425, WHITE);
        DrawRectangle(pMain_posX,pMain_posY,896,600,clPanel);
        DrawTexture(iGrass2b, pMain_posX+802, pMain_posY + 423, WHITE);
        //
        DrawRectangleRoundedLines(rec_sbFirst, 0.2, 4, 2, DARKGRAY);
        DrawRectangleRoundedLines(rec_sbNext, 0.2, 4, 2, DARKGRAY);
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
        if (CheckCollisionPointRec(mousePosition, rec_sbNext))
          if (sbNext_active)
            DrawRectangleRoundedLines(rec_sbNext, 0.2, 4, 2, ORANGE);
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
        if (sbNext_active)
          DrawTexture(sbNext,rec_sbNext.x,72,WHITE);
        else
          DrawTexture(sbNext_gray,rec_sbNext.x,72,WHITE);
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
        commonPos.x = rec_sbNext.x;
        commonPos.y = 108;
        DrawTextEx(fontSButton, "Next", commonPos, 14, 1, clLabels);
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
        commonPos.x = 252;
        commonPos.y = 181;
        DrawTextEx(fontLabel, "BREED NAME:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 215;
        if (breed[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, breed, commonPos, 24, 1, clLabels);
        commonPos.x = 252;
        commonPos.y = 261;
        DrawTextEx(fontLabel, "CATEGORY:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 295;
        if (category[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, category, commonPos, 24, 1, clLabels);
        commonPos.x = 252;
        commonPos.y = 341;
        DrawTextEx(fontLabel, "ORIGIN:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 375;
        if (origin[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, origin, commonPos, 24, 1, clLabels);
        commonPos.x = 252;
        commonPos.y = 506;
        DrawTextEx(fontLabel, "DESCRIPTION:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 544;
        DrawTextEx(fontDesc, description, commonPos, 16, 1, clLabels);
        //
        // draw lines and photo frame
        commonPos.x = pMain_posX;
        commonPos.y = pMain_posY+110;
        commonPos2.x = commonPos.x + 896.0;
        commonPos2.y= commonPos.y;
        DrawLineEx(commonPos, commonPos2, 4.0, clFrames);
        DrawRectangleRounded(rec_photoFrame,0.1,4,clPhoto);
        commonPos.x = rec_photoFrame.x + 176;
        commonPos.y = rec_photoFrame.y + 410;
        DrawTextEx(fontSButton, "372 x 372", commonPos, 14, 2, clLetter);
        if (imageid[0] == '\0')
          DrawTexture(iNoImage,691,86,WHITE);
        else
          DrawTexture(bufferTex,691,86,WHITE);

        // MOUSE EVENT!!!
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
          if (CheckCollisionPointRec(mousePosition, rec_sbFirst))
            if (sbFirst_active)
              currentScreen = 3;
          if (CheckCollisionPointRec(mousePosition, rec_sbNext))
            if (sbNext_active)
              currentScreen = 4;
          if (CheckCollisionPointRec(mousePosition, rec_sbFind))
            if (sbFind_active)
              currentScreen = 5;
          if (CheckCollisionPointRec(mousePosition, rec_sbAdd))
            if (sbAdd_active) {
              currentScreen = 6;
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
        if (IsKeyPressed(KEY_LEFT))
          if (sbFirst_active)
            currentScreen = 3;
        if (IsKeyPressed(KEY_RIGHT))
          if (sbNext_active)
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
      if (currentScreen == 3) {
        //
        clear_fields();
        keydb = gdbm_firstkey(pigeondb);
        if (keydb.dptr == NULL) {
          squabdb_empty = true;
        } else {
          squabdb_empty = false;
          sbFirst_active = false;
          if (!sbNext_active)
            sbNext_active = true;
          strcpy(breed, keydb.dptr);
          datadb = gdbm_fetch(pigeondb,keydb);
          strcpy(recordBuffer, datadb.dptr);
          split_words3sep(category,origin,description,imageid, recordBuffer);
        }
        DrawRectangle(691,86,372,372,clBackground);
        load_imageid(imageid, &bufferTex);
        currentScreen = 2;
      }
      /* ==================================================================================
       * NEXT RECORD "SCREEN" - local to MAIN SCREEN
       * ==================================================================================
       */
      if (currentScreen == 4) {
        //
        cycle_common = true;
        strcpy(breed_buff, breed);
        keydb = gdbm_nextkey(pigeondb,keydb);
        if (keydb.dptr == NULL) {
          sbNext_active = false;
          originated = 2;
          cycle_common = false;
        } else {
          if (!sbFirst_active)
            sbFirst_active = true;
          datadb = gdbm_fetch(pigeondb,keydb);
          strcpy(recordBuffer, datadb.dptr);
          strcpy(breed,keydb.dptr);
          split_words3sep(category,origin,description,imageid, recordBuffer);
        }
        if (cycle_common) {
          DrawRectangle(691,86,372,372,clBackground);
          load_imageid(imageid, &bufferTex);
          currentScreen = 2;
        } else currentScreen = 12;
        //
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
          strcpy(breed_buff, eText);
          strcpy(keydb2.dptr, breed_buff);
          keydb2.dsize = strlen(breed_buff);
          datadb2 = gdbm_fetch(pigeondb,keydb2);
          if (datadb2.dptr != NULL) {
            strcpy(recordBuffer2, datadb2.dptr);
            split_words3sep(category_buff,origin_buff,description_buff,imageid_buff, recordBuffer2);
            load_imageid(imageid_buff, &bufferTex2);
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
        ClearBackground(clBackground);
        DrawRectangle(pMain_posX,pMain_posY,896,640,clPanel); // SECONDARY PANEL
        commonPos.x = 252;
        commonPos.y = 181;
        DrawTextEx(fontLabel, "BREED NAME:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 215;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eBreed,60, iBreedEdit)) iBreedEdit = ! iBreedEdit;
        commonPos.x = 252;
        commonPos.y = 261;
        DrawTextEx(fontLabel, "CATEGORY ('High flyer','Roller','Tumbler'):", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 295;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eCategory,60,iCategoryEdit)) iCategoryEdit =  !iCategoryEdit;
        commonPos.x = 252;
        commonPos.y = 341;
        DrawTextEx(fontLabel, "ORIGIN (Town and Country):", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 375;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eOrigin,60,iOriginEdit)) iOriginEdit =  !iOriginEdit;
        commonPos.x = 252;
        commonPos.y = 421;
        DrawTextEx(fontLabel, "IMAGE ID (image file name in 'pictures' subfolder):", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 455;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eImageid,60,iImageidEdit)) iImageidEdit = !iImageidEdit;
        commonPos.x = 252;
        commonPos.y = 506;
        DrawTextEx(fontLabel, "DESCRIPTION (100 characters maxim):", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 544;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc1,50, iDesc1Edit)) iDesc1Edit = ! iDesc1Edit;
        rec_common.x = 270;
        rec_common.y = 580;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc2,50, iDesc2Edit)) iDesc2Edit = ! iDesc2Edit;
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
          strcpy(eBreed,trim(eBreed,NULL));
          strcpy(eCategory,trim(eCategory,NULL));
          strcpy(eOrigin,trim(eOrigin,NULL));
          strcpy(eImageid,trim(eImageid,NULL));
          strcpy(eDesc1, ltrim(eDesc1, NULL));
          strcpy(eDesc2, rtrim(eDesc2, NULL));
          //
          /*
          strcpy(eBreed_buff, eBreed);
          strcpy(eCategory_buff, eCategory);
          strcpy(eOrigin_buff, eOrigin);
          strcpy(eImageid_buff, eImageid);*/
          strcpy(eDescription_buff, eDesc1);
          strcat(eDescription_buff, eDesc2); 
          //
          add_err = 0;
          if (eBreed[0] == '\0') {
            add_err = 1;
            strcpy(err_msg, "The key (breed) can't be empty!");
          }
          if (eCategory[0] == '\0') {
            add_err = 1;
            strcpy(err_msg, "The category can't be empty!");
          }
          if (eDesc1[0] == '\0') {
            add_err = 1;
            strcpy(err_msg, "The description can't be empty!");
          }
          if (eImageid[0] == '\0')
            strcpy(eImageid, "noimage.png");
          // display error:
          if(add_err > 0) currentScreen = 17;
          if (add_err == 0) {
            // reconstruct the recordBuffer
            strcpy(recordBuffer, eCategory);
            strcat(recordBuffer, ";");
            strcat(recordBuffer, eOrigin);
            strcat(recordBuffer, ";");
            strcat(recordBuffer, eDescription_buff);
            strcat(recordBuffer, ";");
            strcat(recordBuffer, eImageid);
            keydb = gdbm_firstkey(pigeondb); //this is needed if you
            strcpy(keydb.dptr, eBreed);       // navigate beyond last record
            keydb.dsize = strlen(eBreed);
            if(gdbm_exists(pigeondb,keydb) == false) {
              //datadb = gdbm_fetch(pigeondb,keydb);
              strcpy(datadb.dptr, recordBuffer);
              datadb.dsize = strlen(recordBuffer);
              if (gdbm_store(pigeondb,keydb,datadb,GDBM_INSERT) != GDBM_NO_ERROR)
                printf("Error inserting data\r\n");
              clear_add_fields();
              clear_buffer_fields();
              currentScreen = 3; //goto first record because the key order might change if adding a record
            } else {
              strcpy(err_msg, "ERROR! Key (breed) already in database!");
              currentScreen = 17;
            }
          }
        }
        if (bAddCancel_click) {
          // no transfer...
          currentScreen = 3;
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
          270,295,700,32
        },eCategory,60,iCategoryEdit)) iCategoryEdit =  !iCategoryEdit;
        commonPos.x = 252;
        commonPos.y = 341;
        DrawTextEx(fontLabel, "ORIGIN:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 375;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eOrigin,60,iOriginEdit)) iOriginEdit =  !iOriginEdit;
        commonPos.x = 252;
        commonPos.y = 421;
        DrawTextEx(fontLabel, "IMAGE ID:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 455;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eImageid,60,iImageidEdit)) iImageidEdit = !iImageidEdit;
        commonPos.x = 252;
        commonPos.y = 506;
        DrawTextEx(fontLabel, "DESCRIPTION:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 544;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc1,50, iDesc1Edit)) iDesc1Edit = ! iDesc1Edit;
        rec_common.x = 270;
        rec_common.y = 580;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc2,50, iDesc2Edit)) iDesc2Edit = ! iDesc2Edit;
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
          strcpy(category, trim(eCategory,NULL));
          strcpy(origin, trim(eOrigin,NULL));
          strcpy(imageid, trim(eImageid,NULL));
          strcpy(description, ltrim(eDesc1, NULL));
          strcat(description, rtrim(eDesc2, NULL));
          if (category[0] == '\0'){
            mod_err = 1;
            strcpy(err_msg, "Category can't be empty!");
          }
          if (origin[0] == '\0'){
            mod_err = 1;
            strcpy(err_msg, "Origin can't be empty!");
          }
          if (description[0] == '\0'){
            mod_err = 1;
            strcpy(err_msg, "Description can't be empty!");
          }
          if (imageid[0] == '\0'){
            strcpy(imageid, "noimage.png");
          }
          if(mod_err > 0) currentScreen = 17;
          if(mod_err == 0){
            currentScreen = 3;
            // reconstruct the recordBuffer2
            strcpy(recordBuffer, category);
            strcat(recordBuffer, ";");
            strcat(recordBuffer, origin);
            strcat(recordBuffer, ";");
            strcat(recordBuffer, description);
            strcat(recordBuffer, ";");
            strcat(recordBuffer, imageid);
            //
            keydb = gdbm_firstkey(pigeondb); //this is needed if you
            strcpy(keydb.dptr, breed);       // navigate beyond last record
            keydb.dsize = strlen(breed);
            datadb = gdbm_fetch(pigeondb,keydb);
            strcpy(datadb.dptr, recordBuffer);
            datadb.dsize = strlen(recordBuffer);
            if (gdbm_store(pigeondb,keydb,datadb,GDBM_REPLACE) != GDBM_NO_ERROR)
              printf("Error replacing data\r\n");
          }
        }
        if (bModCancel_click) {
          // no transfer...
          currentScreen = 3;
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
          currentScreen = 2;
        if ((bDelYes_click) || (IsKeyPressed(KEY_Y))) {
          currentScreen = 3; // go to reading the first key...
          keydb.dptr = breed;
          keydb.dsize = strlen(breed);
          if (gdbm_delete(pigeondb,keydb) != 0)  // it must return 0 on success
            printf("Error deleting the record!\r\n");
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
        DrawTextEx(fontLetter, "- to Michael Van Canneyt for gdbm pascal translation;", commonPos, 24, 2, clLabels);
        commonPos.x = pSplash_posX + 20;
        commonPos.y = pSplash_posY + stp + 187;
        DrawTextEx(fontLetter, "- to raylib development team;", commonPos, 24, 2, clLabels);
        commonPos.x = pSplash_posX + 20;
        commonPos.y = pSplash_posY + stp + 215;
        DrawTextEx(fontLetter, "- to Mikio Hirabayashi, developer of qdbm.", commonPos, 24, 2, clLabels);
        bQuitYes_click = GuiButton((Rectangle) {
          pSplash_posX + 315, pSplash_posY + 398, 88, 32
        }, "OK");
        if (bQuitYes_click)
          currentScreen = 2;
        //
        if (IsKeyPressed(KEY_ENTER))
          currentScreen = 2;
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
          currentScreen = 2;
        if (IsKeyPressed(KEY_Y))
          exitWindow = true;
        if (IsKeyPressed(KEY_N))
          currentScreen = 2;
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
          currentScreen = 2;
          bErrorNextKey_click = false;
        }
        if (IsKeyPressed(KEY_ENTER)) {
          currentScreen = 2;
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
        if (category_buff[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, category_buff, commonPos, 24, 1, clLabels);
        commonPos.x = 252;
        commonPos.y = 341;
        DrawTextEx(fontLabel, "ORIGIN:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 375;
        if (origin_buff[0] == '\0')
          DrawTextEx(fontLetter, "...", commonPos, 24, 1, clLabels);
        else
          DrawTextEx(fontLetter, origin_buff, commonPos, 24, 1, clLabels);
        commonPos.x = 252;
        commonPos.y = 506;
        DrawTextEx(fontLabel, "DESCRIPTION:", commonPos, 20, 1, clLabels);
        commonPos.x = 270;
        commonPos.y = 544;
        DrawTextEx(fontDesc, description_buff, commonPos, 16, 1, clLabels);
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
        DrawRectangleRounded(rec_photoFrame,0.1,4,clPhoto);
        if (imageid_buff[0] == '\0')
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
          currentScreen = 2;
          bResultClose_click = false;
        }
        if ((bResultDel_click) || (IsKeyPressed(KEY_D))) {
          currentScreen = 15;
          bResultDel_click = false;
        }
        if ((bResultMod_click) || (IsKeyPressed(KEY_M))) {
          ln = strlen(description_buff);
          currentScreen = 16;
          bResultMod_click = false;
          strcpy(eCategory_buff, category_buff);
          strcpy(eOrigin_buff, origin_buff);
          strcpy(eImageid_buff, imageid_buff);
          if (ln <= 50) {
            strcpy(eDesc1_buff, description_buff);
            eDesc2_buff[0] = '\0';
          } else {
            strncpy(eDesc1_buff, description_buff, 50);
            for(i=50; i<ln; i++)
              eDesc2_buff[i-50] = description_buff[i];
            eDesc2_buff[ln-50] = '\0';
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
          currentScreen = 3;
          strcpy(keydb.dptr, breed_buff);
          keydb.dsize = strlen(breed_buff);
          if (gdbm_delete(pigeondb,keydb) != 0)
            printf("Error deleting the found record!\r\n");
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
        270,295,700,32
      },eCategory_buff,60,iCategoryEdit)) iCategoryEdit =  !iCategoryEdit;
        commonPos.x = 252;
        commonPos.y = 341;
        DrawTextEx(fontLabel, "ORIGIN:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 375;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eOrigin_buff,60,iOriginEdit)) iOriginEdit =  !iOriginEdit;
        commonPos.x = 252;
        commonPos.y = 421;
        DrawTextEx(fontLabel, "IMAGE ID:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 455;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eImageid_buff,60,iImageidEdit)) iImageidEdit = !iImageidEdit;
        commonPos.x = 252;
        commonPos.y = 506;
        DrawTextEx(fontLabel, "DESCRIPTION:", commonPos, 20, 1, clLabels);
        rec_common.x = 270;
        rec_common.y = 544;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc1_buff,50, iDesc1Edit)) iDesc1Edit = ! iDesc1Edit;
        rec_common.x = 270;
        rec_common.y = 580;
        rec_common.width = 700;
        rec_common.height = 32;
        if (GuiTextBox(rec_common,eDesc2_buff,50, iDesc2Edit)) iDesc2Edit = ! iDesc2Edit;
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
          strcpy(category_buff, trim(eCategory_buff,NULL));
          strcpy(origin_buff, trim(eOrigin_buff,NULL));
          strcpy(imageid_buff, trim(eImageid_buff,NULL));
          strcpy(description_buff, ltrim(eDesc1_buff, NULL));
          strcat(description_buff, rtrim(eDesc2_buff, NULL));
          if (category_buff[0] =='\0'){
            mod_err = 1;
            strcpy(err_msg, "Category can't be empty!");
          }
          if (origin_buff[0] =='\0'){
            mod_err = 1;
            strcpy(err_msg, "Origin can't be empty!");
          }
          if (description_buff[0] =='\0'){
            mod_err = 1;
            strcpy(err_msg, "Description can't be empty!");
          }
          if (imageid_buff[0] == '\0')
            strcpy(imageid_buff, "noimage.png");
          if (mod_err > 0) currentScreen = 17;
          if (mod_err == 0){
            currentScreen = 14;
            // reconstruct the recordBuffer2
            strcpy(recordBuffer2, category_buff);
            strcat(recordBuffer2, ";");
            strcat(recordBuffer2, origin_buff);
            strcat(recordBuffer2, ";");
            strcat(recordBuffer2, description_buff);
            strcat(recordBuffer2, ";");
            strcat(recordBuffer2, imageid_buff);
            //
            keydb2 = gdbm_firstkey(pigeondb);
            strcpy(keydb2.dptr, breed_buff);
            keydb2.dsize = strlen(breed_buff);
            datadb2 = gdbm_fetch(pigeondb, keydb2);
            strcpy(datadb2.dptr, recordBuffer2);
            datadb2.dsize = strlen(recordBuffer2);
            if (gdbm_store(pigeondb,keydb2,datadb2,GDBM_REPLACE) != GDBM_NO_ERROR)
              printf("Error inserting data\r\n");
            else
              load_imageid(imageid_buff, &bufferTex2);
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
    UnloadTexture(sbNext_gray);
    UnloadTexture(sbFirst_gray);
    //
    UnloadTexture(sbExit);
    UnloadTexture(sbInfo);
    UnloadTexture(sbDel);
    UnloadTexture(sbMod);
    UnloadTexture(sbAdd);
    UnloadTexture(sbFind);
    UnloadTexture(sbNext);
    UnloadTexture(sbFirst);
    //
    gdbm_close(pigeondb);
  } else printf("Error when creating/opening database!\r\n");
  return 0;
}


