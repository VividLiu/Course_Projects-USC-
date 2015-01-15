
#ifndef _CatmullRom_splines_H
#define _CatmullRom_splines_H

#include "vector3f.h"

//The maximum number of interpolated points in whole spline
#define MAX 3000
//number of points to be interpolated into one spline segment
#define numLerpPoints 50

// represents one control point along the spline 
typedef struct{
	double x;
	double y;
	double z;
} point;

// spline struct which contains how many control points, and an array of control points 
typedef struct{
	int numControlPoints;
	point *points;
} spline;

//
typedef struct{
	int length;
	vector3f vertice[MAX];
	vector3f tangent[MAX];
	vector3f normal[MAX];
	vector3f beside[MAX];
}orient;

//function declare
point CR_point(point p0, point p1, point p2, point p3, double u);
orient createCRspline(spline s);

#endif;