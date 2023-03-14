#ifndef __LCD_H
#define __LCD_H

extern void LCD_Init();
extern void LCD_DeInit();
extern void LCD_DrawPoint(int x,int y,int color);
extern void LCD_DrawRect(int x,int y,int w,int h,int color);
extern void LCD_DrawCircle(int x,int y,int r,int color);
extern void LCD_DrawEllipse(int x,int y,int a,int b,int color);
extern void LCD_DrawWord(int x0,int y0,int w,int h,char *data,int color);
extern void LCD_DrawTriangle(int x1,int y1,int x2,int y2,int x3,int y3,int color);
extern void LCD_DrawBmp(const char *bmpname, int x,int y,int flag);
extern void LCD_DrawNum(int num,int x,int y,int color);
extern void LCD_name();
extern void LCD_yanwu();
extern void LCD_guangzhao();
extern void LCD_wendu();
extern void LCD_haiba();
extern void LCD_qiya();
extern void LCD_shidu();
void LCD_Clear(int color);
// void LCD_DrawWord(int x0, int y0, int w, int h, const char *data, int color);

#endif