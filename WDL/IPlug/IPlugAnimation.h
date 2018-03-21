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

enum animationEase
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

enum executeDirection
{
	startToEndPath,
	endToStartPath,
	bothPaths
};

class IPlugEase
{
public:
	inline double LinearEase(double p)
	{
		return p;
	}

	inline double QuadraticEaseIn(double p)
	{
		return p * p;
	}

	inline double QuadraticEaseOut(double p)
	{
		return -(p * (p - 2));
	}

	inline double QuadraticEaseInOut(double p)
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

	inline double CubicEaseIn(double p)
	{
		return p * p * p;
	}

	inline double CubicEaseOut(double p)
	{
		double f = (p - 1);
		return f * f * f + 1;
	}

	inline double CubicEaseInOut(double p)
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

	inline double QuarticEaseIn(double p)
	{
		return p * p * p * p;
	}

	inline double QuarticEaseOut(double p)
	{
		double f = (p - 1);
		return f * f * f * (1 - p) + 1;
	}

	inline double QuarticEaseInOut(double p)
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

	inline double QuinticEaseIn(double p)
	{
		return p * p * p * p * p;
	}

	inline double QuinticEaseOut(double p)
	{
		double f = (p - 1);
		return f * f * f * f * f + 1;
	}

	inline double QuinticEaseInOut(double p)
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

	inline double SineEaseIn(double p)
	{
		return sin((p - 1) * M_PI_2) + 1;
	}

	inline double SineEaseOut(double p)
	{
		return sin(p * M_PI_2);
	}

	inline double SineEaseInOut(double p)
	{
		return 0.5 * (1 - cos(p * M_PI));
	}

	inline double CircularEaseIn(double p)
	{
		return 1 - sqrt(1 - (p * p));
	}

	inline double CircularEaseOut(double p)
	{
		return sqrt((2 - p) * p);
	}

	inline double CircularEaseInOut(double p)
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

	inline double ExponentialEaseIn(double p)
	{
		return (p == 0.0) ? p : pow(2, 10 * (p - 1));
	}

	inline double ExponentialEaseOut(double p)
	{
		return (p == 1.0) ? p : 1 - pow(2, -10 * p);
	}

	inline double ExponentialEaseInOut(double p)
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

	inline double ElasticEaseIn(double p)
	{
		return sin(13 * M_PI_2 * p) * pow(2, 10 * (p - 1));
	}

	inline double ElasticEaseOut(double p)
	{
		return sin(-13 * M_PI_2 * (p + 1)) * pow(2, -10 * p) + 1;
	}

	inline double ElasticEaseInOut(double p)
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

	inline double BackEaseIn(double p)
	{
		return p * p * p - p * sin(p * M_PI);
	}

	inline double BackEaseOut(double p)
	{
		double f = (1 - p);
		return 1 - (f * f * f - f * sin(f * M_PI));
	}

	inline double BackEaseInOut(double p)
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

	inline double BounceEaseIn(double p)
	{
		return 1 - BounceEaseOut(1 - p);
	}

	inline double BounceEaseOut(double p)
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

	inline double BounceEaseInOut(double p)
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
		if (notStarted)
		{
			out = start;
			return;
		}

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
		notStarted = false;
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
		if (end <= count || notStarted) return true;

		return false;
	}

private:
	double start = 0.0, end = 1.0, count = 0.0;

	double out = 0.0;
	double step = 0.0;

	int delay = 0, delayCounter = 0;

	bool notStarted = true;

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

	void SetAnimation(animationEase Type, double BezierX1 = 0, double BezierY1 = 0, double BezierX2 = 0, double BezierY2 = 0)
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
	
	void WaitAnimationToFinish(IPlugAnimationCounter *Counter)
	{
		counter.WaitCounterToFinish(Counter);
	}
	
	void UseSmoothStep(bool State)
	{
		smooth_step = State;
	}

	void Exectute()
	{
		counter.Start();
	}

	double Get()
	{
		return output;
	}

	double GetPosition()
	{
		return position;
	}

	bool Finished()
	{
		return counter.Finished();
	}

	void Count()
	{
		counter.Count();

		position = counter.Get();

		output = LinearInterpolation(start, end, TransferFunction(position));
	}

	IPlugAnimationCounter* GetCounter()
	{
		return &counter;
	}

private:
	double output = 0.0;
	double position = 0.0;
	double bezier_x1 = 0.0, bezier_y1 = 0.0, bezier_x2 = 1.0, bezier_y2 = 1.0;
	double start = 0.0, end = 1.0, duration = 1.0;
	bool smooth_step = false;
	animationEase animation_type = animationEase::LinearEase;
	IPlugAnimationCounter counter;

	inline double LinearInterpolation(double y1, double y2, double mu)
	{
		return(y1*(1 - mu) + y2 * mu);
	}

	double TransferFunction(double pos)
	{
		if (smooth_step) pos = SMOOTHSTEP(pos);

		switch (animation_type)
		{
		case animationEase::LinearEase: return pos;

		case animationEase::QuadraticEaseIn: return QuadraticEaseIn(pos);
		case animationEase::QuadraticEaseOut: return QuadraticEaseOut(pos);
		case animationEase::QuadraticEaseInOut: return QuadraticEaseInOut(pos);

		case animationEase::CubicEaseIn: return CubicEaseIn(pos);
		case animationEase::CubicEaseOut: return CubicEaseOut(pos);
		case animationEase::CubicEaseInOut: return CubicEaseInOut(pos);

		case animationEase::QuarticEaseIn: return QuarticEaseIn(pos);
		case animationEase::QuarticEaseOut: return QuarticEaseOut(pos);
		case animationEase::QuarticEaseInOut: return QuarticEaseInOut(pos);

		case animationEase::QuinticEaseIn: return QuinticEaseIn(pos);
		case animationEase::QuinticEaseOut: return QuinticEaseOut(pos);
		case animationEase::QuinticEaseInOut: return QuinticEaseInOut(pos);

		case animationEase::SineEaseIn: return SineEaseIn(pos);
		case animationEase::SineEaseOut: return SineEaseOut(pos);
		case animationEase::SineEaseInOut: return SineEaseInOut(pos);

		case animationEase::CircularEaseIn: return CircularEaseIn(pos);
		case animationEase::CircularEaseOut: return CircularEaseOut(pos);
		case animationEase::CircularEaseInOut: return CircularEaseInOut(pos);

		case animationEase::ExponentialEaseIn: return ExponentialEaseIn(pos);
		case animationEase::ExponentialEaseOut: return ExponentialEaseOut(pos);
		case animationEase::ExponentialEaseInOut: return ExponentialEaseInOut(pos);

		case animationEase::ElasticEaseIn: return ElasticEaseIn(pos);
		case animationEase::ElasticEaseOut: return ElasticEaseOut(pos);
		case animationEase::ElasticEaseInOut: return ElasticEaseInOut(pos);

		case animationEase::BackEaseIn: return BackEaseIn(pos);
		case animationEase::BackEaseOut: return BackEaseOut(pos);
		case animationEase::BackEaseInOut: return BackEaseInOut(pos);

		case animationEase::BounceEaseIn: return BounceEaseIn(pos);
		case animationEase::BounceEaseOut: return BounceEaseOut(pos);
		case animationEase::BounceEaseInOut: return BounceEaseInOut(pos);

		case animationEase::BezierEase: return CubicBezierEase(pos, 0.25, 0.1, 0.25, 1.0);
		case animationEase::BezierEaseIn: return CubicBezierEase(pos, 0.42, 0, 1.0, 1.0);
		case animationEase::BezierEaseOut: return CubicBezierEase(pos, 0, 0, 0.58, 1.0);
		case animationEase::BezierEaseInOut: return CubicBezierEase(pos, 0.42, 0, 0.58, 1.0);
		case animationEase::BezierSwiftMove: return CubicBezierEase(pos, 0.4, 0, 0.2, 1.0);
		case animationEase::BezierSwifterMove: return CubicBezierEase(pos, 0.4, 0, 0, 1.0);
		case animationEase::BezierHeavyMove: return CubicBezierEase(pos, 0.7, 0, 0.6, 1.0);
		case animationEase::BezierSwiftIn: return CubicBezierEase(pos, 0, 0, 0.2, 1.0);
		case animationEase::BezierSwiftOut: return CubicBezierEase(pos, 0.4, 0, 1.0, 1.0);
		case animationEase::BezierCustom: return CubicBezierEase(pos, bezier_x1, bezier_y1, bezier_x2, bezier_y2);

		default: return pos;
		}
		return 0.0;
	}
};

class IPlugAnimation
{
public:
	void Init(IControl *Control, bool StartFromEnd = false)
	{
		if (initialized == false)
		{
			control = Control;
			Control->GetGUI()->AddAnimation(this);

			if (StartFromEnd) nextExecuteFromStartToEnd = false;

			initialized = true;
		}
	}

	
	void ExecuteAutoFromStartToEnd()
	{
		if (settingsChangedSinceLastExecute)
		{
			animationStartToEnd.SetPath(start, end);

			if (durationInSeconds)
				animationStartToEnd.SetDurationInSeconds(durationStartToEnd);
			else
				animationStartToEnd.SetDurationInFrames(int(durationStartToEnd));

			if (delayInSeconds)
				animationStartToEnd.SetDelayInSeconds(delayStartToEnd);
			else
				animationStartToEnd.SetDelayInFrames(int(delayStartToEnd));

			animationStartToEnd.Exectute();

			nextExecuteFromStartToEnd = false;
			settingsChangedSinceLastExecute = false;
		}
	}

	void ExecuteAutoFromEndToStart()
	{
		if (settingsChangedSinceLastExecute)
		{
			animationEndToStart.SetPath(start, end);

			if (durationInSeconds)
				animationEndToStart.SetDurationInSeconds(durationEndToStart);
			else
				animationEndToStart.SetDurationInFrames(int(durationEndToStart));

			if (delayInSeconds)
				animationEndToStart.SetDelayInSeconds(delayEndToStart);
			else
				animationEndToStart.SetDelayInFrames(int(delayEndToStart));

			animationEndToStart.Exectute();

			nextExecuteFromStartToEnd = true;
			settingsChangedSinceLastExecute = false;
		}
	}

	void Exectute()
	{
		SetPathDirrection();

		if (nextExecuteFromStartToEnd)
		{
			if (animationEndToStart.Finished())
			{
				animationStartToEnd.SetPath(start, end);

				if (durationInSeconds)
					animationStartToEnd.SetDurationInSeconds(durationStartToEnd);
				else
					animationStartToEnd.SetDurationInFrames(int(durationStartToEnd));

				if (delayInSeconds)
					animationStartToEnd.SetDelayInSeconds(delayStartToEnd);
				else
					animationStartToEnd.SetDelayInFrames(int(delayStartToEnd));
			}
			else
			{
				animationStartToEnd.SetPath(out, end);

				double pos = animationEndToStart.GetPosition();

				if (durationInSeconds)
					animationStartToEnd.SetDurationInSeconds(durationStartToEnd * pos);
				else 
					animationStartToEnd.SetDurationInFrames(int(durationStartToEnd * pos));

				if (delayInSeconds)
					animationStartToEnd.SetDelayInSeconds(delayStartToEnd);
				else
					animationStartToEnd.SetDelayInFrames(int(delayStartToEnd));
			}

			animationStartToEnd.Exectute();

			nextExecuteFromStartToEnd = false;
		}
		else
		{
			if (animationStartToEnd.Finished())
			{
				animationEndToStart.SetPath(end, start);

				if (durationInSeconds)
					animationEndToStart.SetDurationInSeconds(durationEndToStart);
				else
					animationEndToStart.SetDurationInFrames(int(durationEndToStart));

				if (delayInSeconds)
					animationEndToStart.SetDelayInSeconds(delayEndToStart);
				else
					animationEndToStart.SetDelayInFrames(int(delayEndToStart));

			}
			else
			{
				animationEndToStart.SetPath(out, start);

				double pos = animationStartToEnd.GetPosition();

				if (durationInSeconds)
					animationEndToStart.SetDurationInSeconds(durationEndToStart * pos);
				else
					animationEndToStart.SetDurationInFrames(int(durationEndToStart * pos));

				if (delayInSeconds)
					animationEndToStart.SetDelayInSeconds(delayEndToStart);
				else
					animationEndToStart.SetDelayInFrames(int(delayEndToStart));
			}
			animationEndToStart.Exectute();

			nextExecuteFromStartToEnd = true;
		}
	}


	double Get()
	{
		return out;
	}


	void SetPath(double Start, double End, executeDirection ManualExecuteDirection = executeDirection::startToEndPath)
	{
		if (start != Start || end != End || pathDirection != ManualExecuteDirection)
			settingsChangedSinceLastExecute = true;

		start = Start;
		end = End;	
		pathDirection = ManualExecuteDirection;
	}


	void SetAnimation(animationEase Type, double BezierX1 = 0, double BezierY1 = 0, double BezierX2 = 0, double BezierY2 = 0)
	{
		SetAnimationStartToEnd(Type, BezierX1, BezierY1, BezierX2, BezierY2);
		SetAnimationEndToStart(Type, BezierX1, BezierY1, BezierX2, BezierY2);
	}

	void SetAnimationStartToEnd(animationEase Type, double BezierX1 = 0, double BezierY1 = 0, double BezierX2 = 0, double BezierY2 = 0)
	{
		if (animationTypeStartToEnd != Type)
			settingsChangedSinceLastExecute = true;

		animationTypeStartToEnd = Type;
		animationStartToEnd.SetAnimation(Type, BezierX1, BezierY1, BezierX2, BezierY2);
	}

	void SetAnimationEndToStart(animationEase Type, double BezierX1 = 0, double BezierY1 = 0, double BezierX2 = 0, double BezierY2 = 0)
	{
		if (animationTypeEndToStart != Type)
			settingsChangedSinceLastExecute = true;

		animationTypeEndToStart = Type;
		animationEndToStart.SetAnimation(Type, BezierX1, BezierY1, BezierX2, BezierY2);
	}


	void SetDurationInSeconds(double Duration)
	{
		SetDurationInSecondsStartToEnd(Duration);
		SetDurationInSecondsEndToStart(Duration);
	}

	void SetDurationInSecondsStartToEnd(double Duration)
	{
		if (durationInSeconds != true || durationStartToEnd != Duration)
			settingsChangedSinceLastExecute = true;

		durationInSeconds = true;
		durationStartToEnd = Duration;
	}

	void SetDurationInSecondsEndToStart(double Duration)
	{
		if (durationInSeconds != true || durationEndToStart != Duration)
			settingsChangedSinceLastExecute = true;

		durationInSeconds = true;
		durationEndToStart = Duration;
	}


	void SetDurationInFrames(int Duration)
	{
		SetDurationInFramesStartToEnd(Duration);
		SetDurationInFramesEndToStart(Duration);
	}

	void SetDurationInFramesStartToEnd(int Duration)
	{
		if (durationInSeconds != false || durationStartToEnd != (double)Duration)
			settingsChangedSinceLastExecute = true;

		durationInSeconds = false;
		durationStartToEnd = Duration;
	}

	void SetDurationInFramesEndToStart(int Duration)
	{
		durationInSeconds = false;
		durationEndToStart = Duration;
	}


	void SetDelayInSeconds(double Delay)
	{
		SetDelayInSecondsStartToEnd(Delay);
		SetDelayInSecondsEndToStart(Delay);
	}

	void SetDelayInSecondsStartToEnd(double Delay)
	{
		if (delayInSeconds != true || delayStartToEnd != Delay)
			settingsChangedSinceLastExecute = true;

		delayInSeconds = true;
		delayStartToEnd = Delay;
	}

	void SetDelayInSecondsEndToStart(double Delay)
	{
		if (delayInSeconds != true || delayEndToStart != Delay)
			settingsChangedSinceLastExecute = true;

		delayInSeconds = true;
		delayEndToStart = Delay;
	}


	void SetDelayInFrames(int Delay)
	{
		SetDelayInFramesStartToEnd(Delay);
		SetDelayInFramesEndToStart(Delay);
	}

	void SetDelayInFramesStartToEnd(int Delay)
	{
		if (delayInSeconds != false || delayStartToEnd != Delay)
			settingsChangedSinceLastExecute = true;

		delayInSeconds = false;
		delayStartToEnd = Delay;
	}

	void SetDelayInFramesEndToStart(int Delay)
	{
		if (delayInSeconds != false || delayEndToStart != Delay)
			settingsChangedSinceLastExecute = true;

		delayInSeconds = false;
		delayEndToStart = Delay;
	}

	// TODO: Add at what percentage completion to start
	void WaitAnimationToFinish(IPlugAnimation *Animation)
	{
		animationStartToEnd.WaitAnimationToFinish(Animation->GetStartToEndCounter());
		animationEndToStart.WaitAnimationToFinish(Animation->GetEndToStartCounter());
	}


	// System -------------------------------------------------------------------------------------------------------------------------------------------------------------

	void Count()
	{
		if (!nextExecuteFromStartToEnd)
		{
			animationStartToEnd.Count();
			out = animationStartToEnd.Get();
		}
		else
		{
			animationEndToStart.Count();
			out = animationEndToStart.Get();
		}
	}

	bool ShouldRedraw()
	{
		if (nextExecuteFromStartToEnd)
		{
			if (!control->IsHidden() && !animationStartToEnd.Finished()) return true;
		}
		else
		{
			if (!control->IsHidden() && !animationEndToStart.Finished()) return true;
		}

		return false;
	}

	IRECT* GetRedrawArea()
	{
		return control->GetDrawRECT();
	}

	IPlugAnimationCounter* GetStartToEndCounter()
	{
		return animationStartToEnd.GetCounter();
	}

	IPlugAnimationCounter* GetEndToStartCounter()
	{
		return animationEndToStart.GetCounter();
	}

private:
	void SetPathDirrection()
	{
		if (pathDirection == executeDirection::startToEndPath) nextExecuteFromStartToEnd = true;
		if (pathDirection == executeDirection::endToStartPath) nextExecuteFromStartToEnd = false;
	}
	
	bool initialized = false;
	bool useAutoExecute = false;
	bool settingsChangedSinceLastExecute = false;
	bool nextExecuteFromStartToEnd = true;

	double start = 0.0, end = 1.0, out = 0.0;
	double durationStartToEnd = 0.0, durationEndToStart = 0.0;
	double delayStartToEnd = 0.0, delayEndToStart = 0.0;

	animationEase animationTypeStartToEnd = animationEase::LinearEase;
	animationEase animationTypeEndToStart = animationEase::LinearEase;

	executeDirection pathDirection = executeDirection::startToEndPath;

	bool durationInSeconds = true;
	bool delayInSeconds = true;

	IRECT prevRect;

	IControl *control = nullptr;
	IPlugAnimationBase animationStartToEnd, animationEndToStart;
};

#endif
