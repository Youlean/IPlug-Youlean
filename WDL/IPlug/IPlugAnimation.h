#ifndef _IPLUGANIMATION_
#define _IPLUGANIMATION_
/*
Youlean - IPlugAnimation - Adding easy animations to IPlug

Copyright (C) 2016 and later, Youlean

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. This notice may not be removed or altered from any source distribution.

*/

#include <vector>
#include <cmath>
#include <string>
#include "IPlugBase.h"
#include "IControl.h"

#ifndef M_PI
#define M_PI       3.14159265358979323846264338328      // Pi 
#endif

#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923132169164      // Pi/2 
#endif

#define SMOOTHSTEP(x) ((x) * (x) * (3 - 2 * (x)))

/*
Use this site to create custom bezier animations: https://matthewlein.com/ceaser/
See easing curves here: http://easings.net/
*/

using namespace std;

typedef enum animationType
{
	LinearEase,

	QuadraticEaseIn,
	QuadraticEaseOut,
	QuadraticEaseInOut,

	CubicEaseIn,
	CubicEaseOut,
	CubicEaseInOut,

	QuarticEaseIn,
	QuarticEaseOut,
	QuarticEaseInOut,

	QuinticEaseIn,
	QuinticEaseOut,
	QuinticEaseInOut,

	SineEaseIn,
	SineEaseOut,
	SineEaseInOut,

	CircularEaseIn,
	CircularEaseOut,
	CircularEaseInOut,

	ExponentialEaseIn,
	ExponentialEaseOut,
	ExponentialEaseInOut,

	ElasticEaseIn,
	ElasticEaseOut,
	ElasticEaseInOut,

	BackEaseIn,
	BackEaseOut,
	BackEaseInOut,

	BounceEaseIn,
	BounceEaseOut,
	BounceEaseInOut,

	BezierEase,
	BezierEaseIn,
	BezierEaseOut,
	BezierEaseInOut,
	BezierSwiftMove,
	BezierSwifterMove,
	BezierHeavyMove,
	BezierSwiftIn,
	BezierSwiftOut,
	BezierCustom
};

class IPlugEase
{
public:
	inline double LinearEase(double p)
	{
		return p;
	}

	double QuadraticEaseIn(double p);

	double QuadraticEaseOut(double p);

	double QuadraticEaseInOut(double p);

	double CubicEaseIn(double p);

	double CubicEaseOut(double p);

	double CubicEaseInOut(double p);

	double QuarticEaseIn(double p);

	double QuarticEaseOut(double p);

	double QuarticEaseInOut(double p);

	double QuinticEaseIn(double p);

	double QuinticEaseOut(double p);

	double QuinticEaseInOut(double p);

	double SineEaseIn(double p);

	double SineEaseOut(double p);

	double SineEaseInOut(double p);

	double CircularEaseIn(double p);

	double CircularEaseOut(double p);

	double CircularEaseInOut(double p);

	double ExponentialEaseIn(double p);

	double ExponentialEaseOut(double p);

	double ExponentialEaseInOut(double p);

	double ElasticEaseIn(double p);

	double ElasticEaseOut(double p);

	double ElasticEaseInOut(double p);

	double BackEaseIn(double p);

	double BackEaseOut(double p);

	double BackEaseInOut(double p);

	double BounceEaseIn(double p);

	double BounceEaseOut(double p);

	double BounceEaseInOut(double p);

};

class IPlugCubicBezierEase
{
public:
	inline double CubicBezierEase(double m, double p1, double p2, double p3, double p4)
	{
		// Defining the bezier doubles in the polynomial form
		double Cx = 3 * p1;
		double Bx = 3 * (p3 - p1) - Cx;
		double Ax = 1 - Cx - Bx;

		double Cy = 3 * p2;
		double By = 3 * (p4 - p2) - Cy;
		double Ay = 1 - Cy - By;

		//FindXFor(m, Cx, Bx, Ax);

		return BezierY(FindXFor(m, Cx, Bx, Ax), Cy, By, Ay);
	}

private:
	inline double BezierX(double t, double Cx, double Bx, double Ax) 
	{ 
		return t * (Cx + t * (Bx + t * Ax));
	}

	inline double BezierY(double t, double Cy, double By, double Ay) 
	{
		return t * (Cy + t * (By + t * Ay));
	}

	inline double BezierXDer(double t, double Cx, double Bx, double Ax) 
	{
		return Cx + t * (2 * Bx + 3 * Ax * t); 
	}

	inline double FindXFor(double t, double Cx, double Bx, double Ax)
	{
		double x = t, i = 0, z;

		// Making 10 iterations max
		while (i < 10)
		{
			z = BezierX(x, Cx, Bx, Ax) - t;

			// If already got close enough
			if (abs(z) < 0.001) break;

			x = x - z / BezierXDer(x, Cx, Bx, Ax);
			i++;
		}

		return x;
	}
};

class IPlugAnimationCounter
{
public:
	double Get()
	{
		return out;
	}

	void Count()
	{
		if (counterClass && !counterClass->Finished())
		{
			out = start;
			return;
		}

		if (end > count)
		{
			out = count;

			if (delayCounter == 0)
				count += step;
			else 
				delayCounter--;

			return;
		}

		out = end;
		return;
	}

	void Start()
	{
		count = start;
		delayCounter = delay;
	}

	void Step(double Step)
	{
		step = Step;
	}

	void Delay(int Delay)
	{
		delay = Delay;
	}

	void WaitCounterToFinish(IPlugAnimationCounter *CounterClass)
	{
		counterClass = CounterClass;
	}

	bool Finished()
	{
		if (end <= count) return true;

		return false;
	}

private:
	double start = 0.0, end = 1.0, count = 0.0;

	double out = 0.0;
	double step = 0.0;

	int delay = 0, delayCounter = 0;

	IPlugAnimationCounter *counterClass = nullptr;
};

class IPlugAnimationBase : public IPlugEase, IPlugCubicBezierEase
{
public:
	void SetPath(double Start, double End)
	{
		start = Start;
		end = End;
	}

	void SetAnimationType(animationType Type, double BezierX1 = 0, double BezierY1 = 0, double BezierX2 = 0, double BezierY2 = 0)
	{
		bezier_x1 = BezierX1;
		bezier_y1 = BezierY1;
		bezier_x2 = BezierX2;
		bezier_y2 = BezierY2;

		animation_type = Type;
	}

	void SetDurationInSeconds(double Duration)
	{
		duration = Duration;
		counter.Step(1.0 / (duration * 60.0));
	}

	void SetDurationInFrames(int Duration)
	{
		duration = Duration;
		counter.Step(1.0 / duration);
	}

	void SetDelayInSeconds(double Delay)
	{
		counter.Delay(int(Delay * 60.0));
	}

	void SetDelayInFrames(int Delay)
	{
		counter.Delay(Delay);
	}
	
	void WaitAnimationToFinish(IPlugAnimationBase *Animation)
	{
		counter.WaitCounterToFinish(Animation->GetCounter());
	}
	
	void UsingSmoothStep(bool State)
	{
		smooth_step = State;
	}

	void Start()
	{
		counter.Start();
	}

	double Get()
	{
		return output;
	}

	bool Finished()
	{
		return counter.Finished();
	}

	void Count()
	{
		counter.Count();

		double position = TransferFunction(counter.Get());

		output = LinearInterpolation(start, end, position);
	}

	IPlugAnimationCounter* GetCounter()
	{
		return &counter;
	}

private:
	double output = 0.0;
	double bezier_x1 = 0.0, bezier_y1 = 0.0, bezier_x2 = 1.0, bezier_y2 = 1.0;
	double start = 0.0, end = 0.0, duration = 0.0;
	bool smooth_step = false;
	animationType animation_type;
	IPlugAnimationCounter counter;

	inline double LinearInterpolation(double y1, double y2, double mu)
	{
		return(y1*(1 - mu) + y2 * mu);
	}

	inline double IPlugAnimationBase::TransferFunction(double pos)
	{
		if (smooth_step) pos = SMOOTHSTEP(pos);

		switch (animation_type)
		{
		case animationType::LinearEase: return pos;

		case animationType::QuadraticEaseIn: return QuadraticEaseIn(pos);
		case animationType::QuadraticEaseOut: return QuadraticEaseOut(pos);
		case animationType::QuadraticEaseInOut: return QuadraticEaseInOut(pos);

		case animationType::CubicEaseIn: return CubicEaseIn(pos);
		case animationType::CubicEaseOut: return CubicEaseOut(pos);
		case animationType::CubicEaseInOut: return CubicEaseInOut(pos);

		case animationType::QuarticEaseIn: return QuarticEaseIn(pos);
		case animationType::QuarticEaseOut: return QuarticEaseOut(pos);
		case animationType::QuarticEaseInOut: return QuarticEaseInOut(pos);

		case animationType::QuinticEaseIn: return QuinticEaseIn(pos);
		case animationType::QuinticEaseOut: return QuinticEaseOut(pos);
		case animationType::QuinticEaseInOut: return QuinticEaseInOut(pos);

		case animationType::SineEaseIn: return SineEaseIn(pos);
		case animationType::SineEaseOut: return SineEaseOut(pos);
		case animationType::SineEaseInOut: return SineEaseInOut(pos);

		case animationType::CircularEaseIn: return CircularEaseIn(pos);
		case animationType::CircularEaseOut: return CircularEaseOut(pos);
		case animationType::CircularEaseInOut: return CircularEaseInOut(pos);

		case animationType::ExponentialEaseIn: return ExponentialEaseIn(pos);
		case animationType::ExponentialEaseOut: return ExponentialEaseOut(pos);
		case animationType::ExponentialEaseInOut: return ExponentialEaseInOut(pos);

		case animationType::ElasticEaseIn: return ElasticEaseIn(pos);
		case animationType::ElasticEaseOut: return ElasticEaseOut(pos);
		case animationType::ElasticEaseInOut: return ElasticEaseInOut(pos);

		case animationType::BackEaseIn: return BackEaseIn(pos);
		case animationType::BackEaseOut: return BackEaseOut(pos);
		case animationType::BackEaseInOut: return BackEaseInOut(pos);

		case animationType::BounceEaseIn: return BounceEaseIn(pos);
		case animationType::BounceEaseOut: return BounceEaseOut(pos);
		case animationType::BounceEaseInOut: return BounceEaseInOut(pos);

		case animationType::BezierEase: return CubicBezierEase(pos, 0.25, 0.1, 0.25, 1.0);
		case animationType::BezierEaseIn: return CubicBezierEase(pos, 0.42, 0, 1.0, 1.0);
		case animationType::BezierEaseOut: return CubicBezierEase(pos, 0, 0, 0.58, 1.0);
		case animationType::BezierEaseInOut: return CubicBezierEase(pos, 0.42, 0, 0.58, 1.0);
		case animationType::BezierSwiftMove: return CubicBezierEase(pos, 0.4, 0, 0.2, 1.0);
		case animationType::BezierSwifterMove: return CubicBezierEase(pos, 0.4, 0, 0, 1.0);
		case animationType::BezierHeavyMove: return CubicBezierEase(pos, 0.7, 0, 0.6, 1.0);
		case animationType::BezierSwiftIn: return CubicBezierEase(pos, 0, 0, 0.2, 1.0);
		case animationType::BezierSwiftOut: return CubicBezierEase(pos, 0.4, 0, 1.0, 1.0);
		case animationType::BezierCustom: return CubicBezierEase(pos, bezier_x1, bezier_y1, bezier_x2, bezier_y2);

		default: return pos;
		}
		return 0.0;
	}
};

class IPlugAnimation
{
public:
	IPlugAnimation(IControl *Control)
	{
		control = Control;
	}


private:
	IControl *control;
};

#endif