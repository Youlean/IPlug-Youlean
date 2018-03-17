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

inline double IPlugEase::QuadraticEaseIn(double p)
{
	return p * p;
}

inline double IPlugEase::QuadraticEaseOut(double p)
{
	return -(p * (p - 2));
}

inline double IPlugEase::QuadraticEaseInOut(double p)
{
	if (p < 0.5)
	{
		return 2 * p * p;
	}
	else
	{
		return (-2 * p * p) + (4 * p) - 1;
	}
}

inline double IPlugEase::CubicEaseIn(double p)
{
	return p * p * p;
}

inline double IPlugEase::CubicEaseOut(double p)
{
	double f = (p - 1);
	return f * f * f + 1;
}

inline double IPlugEase::CubicEaseInOut(double p)
{
	if (p < 0.5)
	{
		return 4 * p * p * p;
	}
	else
	{
		double f = ((2 * p) - 2);
		return 0.5 * f * f * f + 1;
	}
}

inline double IPlugEase::QuarticEaseIn(double p)
{
	return p * p * p * p;
}

inline double IPlugEase::QuarticEaseOut(double p)
{
	double f = (p - 1);
	return f * f * f * (1 - p) + 1;
}

inline double IPlugEase::QuarticEaseInOut(double p)
{
	if (p < 0.5)
	{
		return 8 * p * p * p * p;
	}
	else
	{
		double f = (p - 1);
		return -8 * f * f * f * f + 1;
	}
}

inline double IPlugEase::QuinticEaseIn(double p)
{
	return p * p * p * p * p;
}

inline double IPlugEase::QuinticEaseOut(double p)
{
	double f = (p - 1);
	return f * f * f * f * f + 1;
}

inline double IPlugEase::QuinticEaseInOut(double p)
{
	if (p < 0.5)
	{
		return 16 * p * p * p * p * p;
	}
	else
	{
		double f = ((2 * p) - 2);
		return  0.5 * f * f * f * f * f + 1;
	}
}

inline double IPlugEase::SineEaseIn(double p)
{
	return sin((p - 1) * M_PI_2) + 1;
}

inline double IPlugEase::SineEaseOut(double p)
{
	return sin(p * M_PI_2);
}

inline double IPlugEase::SineEaseInOut(double p)
{
	return 0.5 * (1 - cos(p * M_PI));
}

inline double IPlugEase::CircularEaseIn(double p)
{
	return 1 - sqrt(1 - (p * p));
}

inline double IPlugEase::CircularEaseOut(double p)
{
	return sqrt((2 - p) * p);
}

inline double IPlugEase::CircularEaseInOut(double p)
{
	if (p < 0.5)
	{
		return 0.5 * (1 - sqrt(1 - 4 * (p * p)));
	}
	else
	{
		return 0.5 * (sqrt(-((2 * p) - 3) * ((2 * p) - 1)) + 1);
	}
}

inline double IPlugEase::ExponentialEaseIn(double p)
{
	return (p == 0.0) ? p : pow(2, 10 * (p - 1));
}

inline double IPlugEase::ExponentialEaseOut(double p)
{
	return (p == 1.0) ? p : 1 - pow(2, -10 * p);
}

inline double IPlugEase::ExponentialEaseInOut(double p)
{
	if (p == 0.0 || p == 1.0) return p;

	if (p < 0.5)
	{
		return 0.5 * pow(2, (20 * p) - 10);
	}
	else
	{
		return -0.5 * pow(2, (-20 * p) + 10) + 1;
	}
}

inline double IPlugEase::ElasticEaseIn(double p)
{
	return sin(13 * M_PI_2 * p) * pow(2, 10 * (p - 1));
}

inline double IPlugEase::ElasticEaseOut(double p)
{
	return sin(-13 * M_PI_2 * (p + 1)) * pow(2, -10 * p) + 1;
}

inline double IPlugEase::ElasticEaseInOut(double p)
{
	if (p < 0.5)
	{
		return 0.5 * sin(13 * M_PI_2 * (2 * p)) * pow(2, 10 * ((2 * p) - 1));
	}
	else
	{
		return 0.5 * (sin(-13 * M_PI_2 * ((2 * p - 1) + 1)) * pow(2, -10 * (2 * p - 1)) + 2);
	}
}

inline double IPlugEase::BackEaseIn(double p)
{
	return p * p * p - p * sin(p * M_PI);
}

inline double IPlugEase::BackEaseOut(double p)
{
	double f = (1 - p);
	return 1 - (f * f * f - f * sin(f * M_PI));
}

inline double IPlugEase::BackEaseInOut(double p)
{
	if (p < 0.5)
	{
		double f = 2 * p;
		return 0.5 * (f * f * f - f * sin(f * M_PI));
	}
	else
	{
		double f = (1 - (2 * p - 1));
		return 0.5 * (1 - (f * f * f - f * sin(f * M_PI))) + 0.5;
	}
}

inline double IPlugEase::BounceEaseIn(double p)
{
	return 1 - BounceEaseOut(1 - p);
}

inline double IPlugEase::BounceEaseOut(double p)
{
	if (p < 4 / 11.0)
	{
		return (121 * p * p) / 16.0;
	}
	else if (p < 8 / 11.0)
	{
		return (363 / 40.0 * p * p) - (99 / 10.0 * p) + 17 / 5.0;
	}
	else if (p < 9 / 10.0)
	{
		return (4356 / 361.0 * p * p) - (35442 / 1805.0 * p) + 16061 / 1805.0;
	}
	else
	{
		return (54 / 5.0 * p * p) - (513 / 25.0 * p) + 268 / 25.0;
	}
}

inline double IPlugEase::BounceEaseInOut(double p)
{
	if (p < 0.5)
	{
		return 0.5 * BounceEaseIn(p * 2);
	}
	else
	{
		return 0.5 * BounceEaseOut(p * 2 - 1) + 0.5;
	}
}
