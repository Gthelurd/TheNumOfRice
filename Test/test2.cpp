
#include "PainterEngine.h"
px_texture srcTexture;
px_texture dstTexture;

px_void MarkRegion(px_texture* ptexture, px_int x, px_int y,px_color detectcolor, px_color markcolor,px_int *counter)
{
	
	if (x < 0 || y < 0 || x >= ptexture->width || y >= ptexture->height)
	{
		return (px_void)0;
	}
	if (PX_SURFACECOLOR(ptexture,x,y)._argb.ucolor== detectcolor._argb.ucolor)
	{
		PX_SURFACECOLOR(ptexture,x,y)=markcolor;
		(*counter)++;
		MarkRegion(ptexture, x + 1, y, detectcolor, markcolor,counter);
		MarkRegion(ptexture, x - 1, y, detectcolor, markcolor,counter);
		MarkRegion(ptexture, x, y + 1, detectcolor, markcolor,counter);
		MarkRegion(ptexture, x, y - 1, detectcolor, markcolor,counter);
	}
}

px_int main()
{
	px_int x, y;
	px_int riceCount=0;
	px_char content[32];
	PainterEngine_Initialize(800, 800);
	PX_LoadTextureFromFile(mp_static, &srcTexture, "assets/sample.png");
	PX_TextureCopy(mp_static, &srcTexture, &dstTexture);
	PX_ImageFilter_dBinarization(&dstTexture,160,3);
	
	for ( y= 0; y < dstTexture.height; y++)
	{
		for ( x = 0; x < dstTexture.width; x++)
		{
			px_int counter=0;
			MarkRegion(&dstTexture, x, y, PX_COLOR(255, 255, 255, 255), PX_COLOR(255, 0, 0, 255),&counter);
			if (counter>100)
			{
				riceCount++;
				PX_sprintf1(content,sizeof(content), "rice %1", PX_STRINGFORMAT_INT(riceCount));
				PX_FontDrawText(&srcTexture, x, y,PX_ALIGN_LEFTBOTTOM,content,PX_COLOR_RED);
			}
		}

	}
	
	//sum rice
	PX_sprintf1(content, sizeof(content), "Rice Count:%1", PX_STRINGFORMAT_INT(riceCount));
	PainterEngine_DrawTexture(&srcTexture, 400, 400, PX_ALIGN_CENTER);
	PainterEngine_DrawText(0, 0, content, PX_ALIGN_LEFTTOP, PX_COLOR_RED);
	
	
}