/*
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <inttypes.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <sys/mman.h>
#include <syscall.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "graphics.h"
#include <time.h>

#define arrNum 260
#define N 10
#define Stack_Size 1000
#define bool int
#define true 1
#define false 0
#define LEFT 1
#define RIGHT 2
#define BOTTOM 4
#define TOP 8

extern void *tlsf_create_with_pool(void *mem, size_t bytes);
extern void *g_heap;
const double pi = 3.14;
int ProPos = 0; //生产者缓冲区坐标
int temp[N][arrNum];
int array[N][arrNum];
unsigned char position[8];
int xstart = 26;
int ystart = 20;
int x1 = 26;
int y1 = 20;
int x2 = 26;
int y2 = 20;
int x3 = 26;
int y3 = 20;
int x4 = 26;
int y4 = 21;
int r = 10;
int a = 10;
int b = 5;
int XL = 20;
int XR = 32;
int YB = 15;
int YT = 25;
int m_time = 2000; //睡眠时间

typedef struct
{
	int x;
	int y;
} Seed;
typedef struct stack
{
	Seed Point[Stack_Size];
	int top;
} SeqStack;
SeqStack s;

void DDALine(int x0, int y0, int x1, int y1, COLORREF cr);
void gragh();
void drawPixel(int x, int y, COLORREF cr);
void BresenhamLine(int x0, int y0, int x1, int y1, COLORREF cr);
void MidpointLine(int x0, int y0, int x1, int y1, COLORREF cr);
void CirclePoints(int x1, int y1, int x, int y, COLORREF cr);
void MidpointCircle(int x1, int y1, int r, COLORREF cr);
void CirclePoints2(int x1, int y1, int x, int y, COLORREF cr);
void BresenhamCircle(int x1, int y1, int r, COLORREF cr);
COLORREF getpixel(int x, int y);

void __main()
{
	size_t heap_size = 32 * 1024 * 1024;
	void *heap_base = mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	g_heap = tlsf_create_with_pool(heap_base, heap_size);
}
void init()
{
	s.top = -1;
}
bool Empty()
{
	if (s.top == -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool full()
{
	if (s.top == Stack_Size - 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Push(Seed element)
{
	if (!full())
	{
		s.top++;
		s.Point[s.top] = element;
	}
	else
	{
		printf("栈满\n");
	}
}
Seed Pop()
{
	if (!Empty())
	{
		Seed k = s.Point[s.top];
		s.top--;
		return k;
	}
	else
	{
		printf("栈空\n");
	}
}
COLORREF getpixel(int x, int y)
{
	uint8_t bpp = g_graphic_dev.BitsPerPixel;
	uint32_t addr = ((575 - 15 * y) * g_graphic_dev.BytesPerScanLine) + ((x * 15 + 25) * bpp) / 8;
	uint8_t *p;
	p = g_graphic_dev.FrameBuffer + addr;
	return ((COLORREF)((uint8_t)(getRValue(*((uint32_t *)p))) |
					   ((uint8_t)(getGValue(*((uint32_t *)p))) << 8) |
					   ((uint8_t)(getBValue(*((uint32_t *)p))) << 16)));
}
int msleep(const uint32_t msec)
{
	struct timespec ts = {0, 1000000 * msec};
	return nanosleep(&ts, NULL);
}
int abs(int x)
{
	if (x < 0)
		x = 0 - x;
	return x;
}
void clear()
{
	int i;
	for (i = 0; i < 600; i++)
		line(0, i, 800, i, RGB(0, 0, 0));
	gragh();
}
void resetbuf(int *arry, int n, int x)
{
	int i;
	for (i = 0; i < arrNum; i++)
		line(x, i * 2, x + 79, i * 2, RGB(0, 0, 0));
}
void swap(int *a, int *b)
{
	int temp;

	temp = *a, *a = *b, *b = temp;
}
void tsk_foo_line3(void *pv)
{
	int key;
	while (1)
	{
		key = getchar();
		switch (key)
		{
		case 0x0231: //1!
		{
			int i = 0;
			Seed Point[100];
			int key1;
			clear();
			while (Point[0].x != Point[i - 1].x || Point[0].y != Point[i - 1].y || (i == 0) || (i == 1))
			{
				key1 = getchar();
				switch (key1)
				{
				case 0x4800: //up
				{
					y2++;
					drawPixel(x2 * 15 + 25, 575 - 15 * y2, RGB(255, 0, 255));
					Point[i].x = x2;
					Point[i].y = y2;
					i++;
				}
				break;
				case 0x5000: //down
				{
					y2--;
					drawPixel(x2 * 15 + 25, 575 - 15 * y2, RGB(255, 0, 255));
					Point[i].x = x2;
					Point[i].y = y2;
					i++;
				}
				break;
				case 0x4d00: //right
				{
					x2++;
					drawPixel(x2 * 15 + 25, 575 - 15 * y2, RGB(255, 0, 255));
					Point[i].x = x2;
					Point[i].y = y2;
					i++;
				}
				break;
				case 0x4b00: //left
				{
					x2--;
					drawPixel(x2 * 15 + 25, 575 - 15 * y2, RGB(255, 0, 255));
					Point[i].x = x2;
					Point[i].y = y2;
					i++;
				}
				break;
				}
			}
			ScanLineFill4(27, 20, RGB(255, 255, 255), RGB(255, 255, 0));
		}
		break;
		case 0x0332: //2#
		{
			int key3;
			int flagpos1 = 0;
			while (1)
			{
				key3 = getchar();
				if (key3 == 0x011b)
					break;
				switch (key3)
				{
				case 0x4800: //up
				{
					if (flagpos1 == 0)
						++y1;
					if (flagpos1 == 1)
						++r;
				}
				break;
				case 0x5000: //down
				{
					if (flagpos1 == 0)
						--y1;
					if (flagpos1 == 1)
						--r;
				}
				break;
				case 0x4d00: //right
				{
					if (flagpos1 == 0)
						++x1;
				}
				break;
				case 0x4b00: //left
				{
					if (flagpos1 == 0)
						--x1;
				}
				break;
				case 0x0F09: //TAB
				{
					flagpos1 = (flagpos1 + 1) % 2;
				}
				break;
				}
				clear();
				BresenhamCircle(x1, y1, abs(r), RGB(255, 255, 0));
				ScanLineFill4(x1, y1, RGB(255, 255, 255), RGB(0, 255, 255));
			}
		}
		break;
		case 0x0433: //3@
		{
			int key2;
			int flagpos = 0;
			while (1)
			{
				key2 = getchar();
				if (key2 == 0x011b)
					break;
				switch (key2)
				{
				case 0x4800: //up
				{
					if (flagpos == 0)
						++y3;
					if (flagpos == 1)
						++y4;
				}
				break;
				case 0x5000: //down
				{
					if (flagpos == 0)
						--y3;
					if (flagpos == 1)
						--y4;
				}
				break;
				case 0x4d00: //right
				{
					if (flagpos == 0)
						++x3;
					if (flagpos == 1)
						++x4;
				}
				break;
				case 0x4b00: //left
				{
					if (flagpos == 0)
						--x3;
					if (flagpos == 1)
						--x4;
				}
				break;
				case 0x0F09: //TAB
				{
					flagpos = (flagpos + 1) % 2;
				}
				break;
				}
				clear();
				DDALine(XL, YB, XR, YB, RGB(0, 255, 255));
				DDALine(XR, YT, XR, YB, RGB(0, 255, 255));
				DDALine(XR, YT, XL, YT, RGB(0, 255, 255));
				DDALine(XL, YB, XL, YT, RGB(0, 255, 255));
				CS_LineClip(x3, y3, x4, y4);
			}
		}
		break;
		}
	}
	task_exit(0);
}
void drawPixel(int x, int y, COLORREF cr)
{
	int j;
	for (j = 0; j < 360; j++)
	{
		double sj = 2 * pi * j / 360;
		line(x, y, x + 7 * cos(sj), y - 7 * sin(sj), cr);
	}
}
void gragh()
{
	line(414, 20, 414, 575, RGB(255, 255, 255));
	line(415, 20, 415, 575, RGB(255, 255, 255));
	line(416, 20, 416, 575, RGB(255, 255, 255));
	//纵轴
	line(25, 274, 785, 274, RGB(255, 255, 255));
	line(25, 275, 785, 275, RGB(255, 255, 255));
	line(25, 276, 785, 276, RGB(255, 255, 255));
	//横轴
	line(415, 19, 421, 25, RGB(255, 255, 255));
	line(415, 20, 420, 25, RGB(255, 255, 255));
	line(415, 19, 409, 25, RGB(255, 255, 255));
	line(415, 20, 410, 25, RGB(255, 255, 255));
	//箭头之一
	line(786, 275, 780, 270, RGB(255, 255, 255));
	line(785, 275, 780, 269, RGB(255, 255, 255));
	line(786, 275, 780, 280, RGB(255, 255, 255));
	line(785, 275, 780, 281, RGB(255, 255, 255));
	//箭头之二
	int i;
	for (i = 0; i < 50; i++)
		line(40 + i * 15, 50, 40 + i * 15, 575, RGB(255, 255, 255));
	for (i = 0; i < 35; i++)
		line(25, 50 + i * 15, 775, 50 + i * 15, RGB(255, 255, 255));
}
void CirclePoints(int x1, int y1, int x, int y, COLORREF cr) //Midpoint画圆算法计算的是第一象限上半部分的所有点，所以需要找8个对称点
{
	drawPixel((x + x1) * 15 + 25, 575 - 15 * (y + y1), cr);
	drawPixel((-x + x1) * 15 + 25, 575 - 15 * (y + y1), cr);
	drawPixel((x + x1) * 15 + 25, 575 - 15 * (-y + y1), cr);
	drawPixel((-x + x1) * 15 + 25, 575 - 15 * (-y + y1), cr);
	drawPixel((y + x1) * 15 + 25, 575 - 15 * (x + y1), cr);
	drawPixel((y + x1) * 15 + 25, 575 - 15 * (-x + y1), cr);
	drawPixel((-y + x1) * 15 + 25, 575 - 15 * (x + y1), cr);
	drawPixel((-y + x1) * 15 + 25, 575 - 15 * (-x + y1), cr);
}
void CirclePoints2(int x1, int y1, int x, int y, COLORREF cr) //Bresenham画圆算法与Midpoint画椭圆算法计算的是第一象限的所有点，所以需要找四个对称点
{
	drawPixel((x + x1) * 15 + 25, 575 - 15 * (y + y1), cr);
	drawPixel((-x + x1) * 15 + 25, 575 - 15 * (y + y1), cr);
	drawPixel((x + x1) * 15 + 25, 575 - 15 * (-y + y1), cr);
	drawPixel((-x + x1) * 15 + 25, 575 - 15 * (-y + y1), cr);
}
void DDALine(int x0, int y0, int x1, int y1, COLORREF cr)
{

	float increx, increy, x, y;
	int steps, i;

	if (abs(x1 - x0) > abs(y1 - y0))
		steps = abs(x1 - x0);
	else
		steps = abs(y1 - y0);

	increx = (float)(x1 - x0) / steps;
	increy = (float)(y1 - y0) / steps;

	x = x0;
	y = y0;

	for (i = 0; i <= steps; i++)
	{
		drawPixel(((int)(x + 0.5)) * 15 + 25, 575 - 15 * ((int)(y + 0.5)), cr);
		//只有在画点的时候才要进行四舍五入，为了确保每一个点的精确性，y的值不能是整数值
		x += increx;
		y += increy;
	}
}
void BresenhamLine(int x0, int y0, int x1, int y1, COLORREF cr)
{

	int x, y, dx, dy, e, i;

	if (abs(x0 - x1) > abs(y0 - y1))
	{

		if (x1 < x0)
		{
			swap(&x1, &x0);
			swap(&y1, &y0);
		}
		dx = x1 - x0;
		dy = y1 - y0;
		x = x0;
		y = y0;
		if (dy * dx >= 0)
		{
			e = -dx;
			for (i = 0; i <= abs(dx); i++)
			{
				drawPixel(x * 15 + 25, 575 - 15 * y, cr);
				x = x + 1;
				e = e + 2 * dy;
				if (e >= 0)
				{
					y = y + 1;
					e = e - 2 * dx;
				}
			}
		}
		else
		{
			e = dx;
			for (i = 0; i <= abs(dx); i++)
			{
				drawPixel(x * 15 + 25, 575 - 15 * y, cr);
				x = x + 1;
				e = e + 2 * dy;
				if (e <= 0)
				{
					y = y - 1;
					e = e + 2 * dx;
				}
			}
		}
	}

	else
	{
		if (y1 < y0)
		{
			swap(&x1, &x0);
			swap(&y1, &y0);
		}
		dx = x1 - x0;
		dy = y1 - y0;
		x = x0;
		y = y0;
		if (dy * dx >= 0)
		{
			e = -dy;
			for (i = 0; i <= abs(dy); i++)
			{
				drawPixel(x * 15 + 25, 575 - 15 * y, cr);
				y = y + 1;
				e = e + 2 * dx;
				if (e >= 0)
				{
					x = x + 1;
					e = e - 2 * dy;
				}
			}
		}
		else
		{
			e = dy;
			for (i = 0; i <= abs(dy); i++)
			{
				drawPixel(x * 15 + 25, 575 - 15 * y, cr);
				y = y + 1;
				e = e + 2 * dx;
				if (e <= 0)
				{
					x = x - 1;
					e = e + 2 * dy;
				}
			}
		}
	}
}
void MidpointLine(int x0, int y0, int x1, int y1, COLORREF cr)
{

	int a, b, d, x, y;

	if (x1 < x0)
	{
		swap(&x1, &x0);
		swap(&y1, &y0);
	}

	a = y0 - y1;
	b = x1 - x0;

	x = x0, y = y0;

	if (abs(x0 - x1) >= abs(y0 - y1))
	{
		if ((x0 - x1) * (y0 - y1) >= 0)
		{

			d = 2 * a + b;

			drawPixel(x * 15 + 25, 575 - 15 * y, cr);
			while (x < x1)
			{
				if (d <= 0)
				{
					x++;
					y++;
					d += 2 * (a + b);
				}
				else
				{
					x++;
					d += 2 * a;
				}
				drawPixel(x * 15 + 25, 575 - 15 * y, cr);
				;
			}
		}
		else
		{
			d = 2 * a - b;
			drawPixel(x * 15 + 25, 575 - 15 * y, cr);
			while (x < x1)
			{
				if (d > 0)
				{
					x++;
					y--;
					d += 2 * (a - b);
				}
				else
				{
					x++;
					d += 2 * a;
				}
				drawPixel(x * 15 + 25, 575 - 15 * y, cr);
				;
			}
		}
	}
	else if (abs(x0 - x1) < abs(y0 - y1))
	{
		if ((x0 - x1) * (y0 - y1) >= 0)
		{
			d = a + 2 * b;
			drawPixel(x * 15 + 25, 575 - 15 * y, cr);
			;
			while (y < y1)
			{
				if (d > 0)
				{
					y++;
					x++;
					d += 2 * (a + b);
				}
				else
				{
					y++;
					d += 2 * b;
				}
				drawPixel(x * 15 + 25, 575 - 15 * y, cr);
				;
			}
		}
		else
		{
			d = a - 2 * b;
			drawPixel(x * 15 + 25, 575 - 15 * y, cr);
			while (y > y1)
			{
				if (d <= 0)
				{
					x++;
					y--;
					d += 2 * (a - b);
				}
				else
				{

					y--;
					d += -2 * b;
				}
				drawPixel(x * 15 + 25, 575 - 15 * y, cr);
				;
			}
		}
	}
}
void MidpointCircle(int x1, int y1, int r, COLORREF cr)
{
	int x, y;
	float d;
	x = 0;
	y = r;
	d = 1.25 - r;
	CirclePoints(x1, y1, x, y, cr);
	while (x <= y)
	{
		if (d < 0)
			d += 2 * x + 3;
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		;
		CirclePoints(x1, y1, x, y, cr);
	}
}
void BresenhamCircle(int x1, int y1, int r, COLORREF cr)
{
	int x, y, delta, delta1, delta2, direction;
	x = 0;
	y = r;
	delta = 2 * (1 - r); //△d的初始值
	int Limit = 0;
	while (y >= Limit)
	{
		CirclePoints2(x1, y1, x, y, cr);
		if (delta < 0)
		{
			delta1 = 2 * (delta + y) - 1;

			if (delta1 <= 0)
				direction = 1; //取H点
			else
				direction = 2; //取D点
		}
		else if (delta > 0)
		{
			delta2 = 2 * (delta - x) - 1;

			if (delta2 < 0)
				direction = 2; // 取D点
			else
				direction = 3; //取V点
		}
		else
			direction = 2;
		switch (direction)
		{
		case 1:
			delta += 2 * x + 3;
			x++;
			break;
		case 2:
			delta += 2 * (x - y) + 6; //x,y是先加的1，所以delta中的式子只要加2就可以了
			x++;
			y--;
			break;
		case 3:
			delta += (-2 * y + 3);
			y--;
			break;
		} /* switch */
	}	 /*while*/
}
void MidpointEllipse(int a, int b, int x1, int y1, COLORREF cr)
{
	int x, y;
	double d1, d2;
	x = 0;
	y = b;
	d1 = b * b + a * a * (-b + 0.25);
	CirclePoints2(x1, y1, x, y, cr);
	while (b * b * (x + 1) < a * a * (y - 0.5)) //代入中点防止下一个点是(x+1,y)
	{
		if (d1 < 0)
		{
			d1 += b * b * (2 * x + 3);
			x++;
		}
		else
		{
			d1 += (b * b * (2 * x + 3) + a * a * (-2 * y + 2));
			x++;
			y--;
		}
		CirclePoints2(x1, y1, x, y, cr);

	} //上半部分绘制完毕

	d2 = (b * (x + 0.5)) * (b * (x + 0.5)) + (a * (y - 1)) * (a * (y - 1)) - (a * b) * (a * b);
	while (y > 0)
	{
		if (d2 < 0)
		{
			d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
			x++;
			y--;
		}
		else
		{
			d2 += a * a * (-2 * y + 3);
			y--;
		}
		CirclePoints2(x1, y1, x, y, cr);
	} //下半部分绘制完毕
}
void ScanLineFill4(int x, int y, COLORREF oldcolor, COLORREF newcolor)
{
	int xl, xr;
	bool SpanNeedFill;
	Seed pt; //种子点
	pt.x = x;
	pt.y = y;
	init();
	Push(pt); // 种子点(x, y)入栈
	while (!Empty())
	{
		pt = Pop(); //取种子点
		y = pt.y;
		x = pt.x;
		while (getpixel(x, y) == oldcolor) // 从种子点开始向右填充
		{
			drawPixel(x * 15 + 25, 575 - 15 * y, newcolor);
			x++;
		}
		xr = x - 1;
		x = pt.x - 1;
		while (getpixel(x, y) == oldcolor) // 从种子点开始向左填充
		{
			drawPixel(x * 15 + 25, 575 - 15 * y, newcolor);
			x--;
		}
		xl = x + 1;
		x = xl;
		y = y + 1; // 处理上面一条扫描线
		while (x <= xr)
		{
			SpanNeedFill = false;
			while (getpixel(x, y) == oldcolor)
			{
				SpanNeedFill = true;
				x++;
			}				  // 待填充区段搜索完毕
			if (SpanNeedFill) // 将右端点作为种子点入栈
			{
				pt.x = x - 1;
				pt.y = y;
				Push(pt);
				SpanNeedFill = false;
			} //继续向右检查以防遗漏
			while ((getpixel(x, y) != oldcolor) && (x <= xr))
				x++;
		} //上一条扫描线上检查完毕
		x = xl;
		y = y - 2; // 处理下面一条扫描线
		while (x <= xr)
		{
			SpanNeedFill = false;
			while (getpixel(x, y) == oldcolor)
			{
				SpanNeedFill = true;
				x++;
			}
			if (SpanNeedFill)
			{
				pt.x = x - 1;
				pt.y = y;
				Push(pt);
				SpanNeedFill = false;
			}
			while ((getpixel(x, y) != oldcolor) && (x <= xr))
				x++;
		}
	}
}
int encode(int x, int y)
{
	int c = 0;
	if (x <= XL)
		c |= LEFT;
	if (x >= XR)
		c |= RIGHT;
	if (y <= YB)
		c |= BOTTOM;
	if (y >= YT)
		c |= TOP;
	return c;
}

void CS_LineClip(int x1, int y1, int x2, int y2)
{
	int code1, code2, code;
	code1 = encode(x1, y1);
	code2 = encode(x2, y2);
	int x, y;
	while (code1 != 0 || code2 != 0)
	{
		if ((code1 & code2) != 0)
			return;
		if (code1 != 0)
			code = code1;
		else
			code = code2;
		if ((LEFT & code) != 0)
		{
			x = XL + 1;
			y = (int)(y1 + (y2 - y1) * (XL - x1) / (x2 - x1));
		}
		else if ((RIGHT & code) != 0)
		{
			x = XR - 1;
			y = (int)(y1 + (y2 - y1) * (XR - x1) / (x2 - x1));
		}
		else if ((BOTTOM & code) != 0)
		{
			y = YB + 1;
			x = (int)(x1 + (x2 - x1) * (YB - y1) / (y2 - y1));
		}
		else if ((TOP & code) != 0)
		{
			y = YT - 1;
			x = (int)(x1 + (x2 - x1) * (YT - y1) / (y2 - y1));
		}
		if (code == code1)
		{
			x1 = x;
			y1 = y;
			code1 = encode(x, y);
		}
		else
		{
			x2 = x;
			y2 = y;
			code2 = encode(x, y);
		}
	}
	DDALine(x1, y1, x2, y2, RGB(255, 255, 0));
}

void main(void *pv)
{
	printf("task #%d: I'm the first user task(pv=0x%08x)!\r\n",
		   task_getid(), pv);
	int mode = 0x143;
	init_graphic(mode);
	gragh();
	int *pcode_exit = NULL;
	//申请用户栈
	unsigned char *stack_foo3 = (unsigned char *)malloc(1024 * 1024);
	int tid_foo3;
	tid_foo3 = task_create(stack_foo3 + 1024 * 1024, tsk_foo_line3, (void *)0);
	setpriority(tid_foo3, 0);
	task_wait(tid_foo3, pcode_exit);
	free(stack_foo3);
	getchar();

	while (1)
		;
	task_exit(0);
}