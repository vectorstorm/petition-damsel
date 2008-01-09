/*
 *  VS_Color.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_COLOR_H
#define VS_COLOR_H

class vsColor
{
public:
	float r, g, b, a;

	static vsColor White;
	static vsColor Blue;
	static vsColor Green;
	static vsColor Yellow;
	static vsColor LightBlue;
	static vsColor Red;
	static vsColor Black;	// totall invisible, with vector graphics
	
	vsColor(float red=0.f, float green=0.f, float blue=0.f, float alpha=1.f) { r=red; g=green; b=blue; a=alpha; }
	
	vsColor  operator+( const vsColor &o ) const {return vsColor(r+o.r,g+o.g,b+o.b,a+o.a);}
	vsColor  operator*( float scalar ) const {return vsColor(r*scalar,g*scalar,b*scalar, a*scalar);}
	vsColor  operator*=( float scalar ) {r*=scalar; g*=scalar; b*=scalar; a*=scalar; return *this;}
	
	bool	operator==( const vsColor &o ) const { return (r==o.r && b==o.b && g==o.g && a==o.a); }
	bool	operator!=( const vsColor &o ) const { return !(*this==o); }

	void Set(float red=0.f, float green=0.f, float blue=0.f, float alpha=1.f) { r=red; g=green; b=blue; a=alpha; };
};

vsColor  operator*( float scalar, const vsColor &color );
vsColor vsInterpolate( float alpha, const vsColor &a, const vsColor &b );

#endif // VS_COLOR_H
