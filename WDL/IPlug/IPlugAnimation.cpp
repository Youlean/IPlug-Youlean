#include "IPlugAnimation.h"
#include "IGraphics.h"


//void IPlugAnimation::DEBUG_DrawAnimationCurve(IGraphics * pGraphics, animationFlag flag, int size, int x, int y)
//{
//	// Draw background
//	IRECT background = IRECT(x, y, x + size, y + size);
//	pGraphics->FillIRect(&COLOR_BLACK, &background);
//
//	// Draw lines
//	float lineL = float(x + size / 4);
//	float lineT = float(y + size / 4);
//	float lineR = float(x + size - size / 4);
//	float lineB = float(y + size - size / 4);
//
//	// X line
//	pGraphics->DrawLine(&COLOR_WHITE, lineL, lineB, lineR, lineB);
//	// Y line
//	pGraphics->DrawLine(&COLOR_WHITE, lineL, lineB, lineL, lineT);
//
//	// Draw text
//	WDL_String text;
//	IRECT rect;
//
//	// Draw what animation is selected
//	if (flag == _LinearInterpolation) text.Set("LinearInterpolation");
//
//	if (flag == _QuadraticEaseIn) text.Set("QuadraticEaseIn");
//	if (flag == _QuadraticEaseOut) text.Set("QuadraticEaseOut");
//	if (flag == _QuadraticEaseInOut) text.Set("QuadraticEaseInOut");
//
//	if (flag == _CubicEaseIn) text.Set("CubicEaseIn");
//	if (flag == _CubicEaseOut) text.Set("CubicEaseOut");
//	if (flag == _CubicEaseInOut) text.Set("CubicEaseInOut");
//
//	if (flag == _QuarticEaseIn) text.Set("QuarticEaseIn");
//	if (flag == _QuarticEaseOut) text.Set("QuarticEaseOut");
//	if (flag == _QuarticEaseInOut) text.Set("QuarticEaseInOut");
//
//	if (flag == _QuinticEaseIn) text.Set("QuinticEaseIn");
//	if (flag == _QuinticEaseOut) text.Set("QuinticEaseOut");
//	if (flag == _QuinticEaseInOut) text.Set("QuinticEaseInOut");
//
//	if (flag == _SineEaseIn) text.Set("SineEaseIn");
//	if (flag == _SineEaseOut) text.Set("SineEaseOut");
//	if (flag == _SineEaseInOut) text.Set("SineEaseInOut");
//
//	if (flag == _CircularEaseIn) text.Set("CircularEaseIn");
//	if (flag == _CircularEaseOut) text.Set("CircularEaseOut");
//	if (flag == _CircularEaseInOut) text.Set("CircularEaseInOut");
//
//	if (flag == _ExponentialEaseIn) text.Set("ExponentialEaseIn");
//	if (flag == _ExponentialEaseOut) text.Set("ExponentialEaseOut");
//	if (flag == _ExponentialEaseInOut) text.Set("ExponentialEaseInOut");
//
//	if (flag == _ElasticEaseIn) text.Set("ElasticEaseIn");
//	if (flag == _ElasticEaseOut) text.Set("ElasticEaseOut");
//	if (flag == _ElasticEaseInOut) text.Set("ElasticEaseInOut");
//
//	if (flag == _BackEaseIn) text.Set("BackEaseIn");
//	if (flag == _BackEaseOut) text.Set("BackEaseOut");
//	if (flag == _BackEaseInOut) text.Set("BackEaseInOut");
//
//	if (flag == _BounceEaseIn) text.Set("BounceEaseIn");
//	if (flag == _BounceEaseOut) text.Set("BounceEaseOut");
//	if (flag == _BounceEaseInOut) text.Set("BounceEaseInOut");
//
//	if (flag == _BezierEase) text.Set("BezierEase");
//	if (flag == _BezierEaseIn) text.Set("BezierEaseIn");
//	if (flag == _BezierEaseOut) text.Set("ezierEaseOut");
//	if (flag == _BezierEaseInOut) text.Set("BezierEaseInOut");
//	if (flag == _BezierSwiftMove) text.Set("BezierSwiftMove");
//	if (flag == _BezierSwifterMove) text.Set("BezierSwifterMove");
//	if (flag == _BezierHeavyMove) text.Set("BezierHeavyMove");
//	if (flag == _BezierSwiftIn) text.Set("BezierSwiftIn");
//	if (flag == _BezierSwiftOut) text.Set("BezierSwiftOut");
//	if (flag == _BezierCustom) text.Set("BezierCustom");
//
//	int textSize = size / 20;
//	IText txtIText(textSize, &COLOR_WHITE);
//	txtIText.mAlign = IText::kAlignNear;
//
//	rect = IRECT(x, y, x + size, y + size);
//	pGraphics->DrawIText(&txtIText, text.Get(), &rect);
//
//	// Draw numbers
//	// XY0
//	text.Set("XY0");
//	rect = IRECT((int)lineL - textSize, (int)lineB, (int)lineR, (int)lineB);
//	pGraphics->DrawIText(&txtIText, text.Get(), &rect);
//
//	// X1
//	text.Set("X1");
//	rect = IRECT((int)lineR, (int)lineB, (int)lineR + textSize, (int)lineB);
//	pGraphics->DrawIText(&txtIText, text.Get(), &rect);
//
//	// Y1
//	text.Set("Y1");
//	rect = IRECT((int)lineL - textSize, (int)lineT, (int)lineR, (int)lineB);
//	pGraphics->DrawIText(&txtIText, text.Get(), &rect);
//
//	// Draw curve
//	for (int i = (int)lineL; i < (int)lineR; i++)
//	{
//		double pos = (double)(i - (int)lineL) / (double)((int)lineR - (int)lineL);
//
//		int pixelY = int(TransferFunction(lineB, lineT, pos, flag));
//
//		if (i >= 0 && i <= pGraphics->Width() && pixelY >= 0 && pixelY <= pGraphics->Height())
//			pGraphics->ForcePixel(&COLOR_RED, i, pixelY);
//	}
//
//	pGraphics->SetAllControlsDirty();
//}


