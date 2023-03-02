#ifndef _LCD_H_
#define _LCD_H_

extern struct fb_var_screeninfo vinfo;
extern struct fb_fix_screeninfo finfo;
extern char *fbp;
extern long int screensize;
extern int fbfd;

#define WHITE 0xffffff
/* LCD */
int LCD_init(void);
void LCD_exit(void);
void LCD_Clear(int color);
int LCD_Draw_point(int x, int y, int color);
void LCD_Draw_line(int x0, int y0, int x1, int y1, int color);
int LCD_Draw_rect(int x1, int y1, int x2, int y2, int color);
void LCD_Draw_circle(int xc, int yc, int r, int color);
void LCD_Draw_ellipse(int xc, int yc, int a, int b, int color);
void LCD_Draw_arc(int xc, int yc, int r, int start_angle, int end_angle, int color);
void LCD_Draw_polygon(int x[], int y[], int n, int color);
void LCD_Fill_circle(int xc, int yc, int r, int color);
void LCD_Fill_ellipse(int xc, int yc, int a, int b, int color);

void LCD_Draw_Font(const unsigned char font[], int x, int y, int color);
// void LCD_Draw_Num(int x, int y, int num, int color);
// void LCD_Draw_Number(int x, int y, int number, int color);
#define LCD_Clear_screen() LCD_Clear(WHITE)

#endif // _LCD_H_
