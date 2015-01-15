#include "CatmullRom_splines.h"
#include "matrix.h"
//#include <GL/glu.h>
#include <GL/glut.h>



//Calculate specific point in the spline using four control points and interpolation parameter
point CR_point(point p0, point p1, point p2, point p3, double u){
	matrix uMat(1,4);
	matrix base(4,4);
	matrix control(4,3);
	matrix prod;
	point CR_point;
	
	double u2 = u*u;
	double u3 = u2*u;

	double u_data[4] = { 1.0, u, u2,u3};
	double base_data[] = {0.0, 2.0, 0.0, 0.0,
					   -1.0, 0.0, 1.0, 0.0,
 					    2.0, -5.0, 4.0, -1.0,
           			   -1.0, 3.0, -3.0, 1.0};
	double cont_data[] = {p0.x, p0.y, p0.z,
                    	p1.x, p1.y, p1.z, 
						p2.x, p2.y, p2.z,
						p3.x, p3.y, p3.z};
	
	uMat.assign(u_data);
	base.assign(base_data);
	control.assign(cont_data);
	
	prod = base.multiply(control);
	prod = uMat.multiply(prod);
	
	CR_point.x = prod.mat[0][0];
	CR_point.y = prod.mat[0][1];
	CR_point.z = prod.mat[0][2];
	
	return CR_point;
}

//Calculate specific point in the spline using four control points and interpolation parameter
vector3f CR_tangent(point p0, point p1, point p2, point p3, double u){
	matrix uMat(1,4);
	matrix base(4,4);
	matrix control(4,3);
	matrix prod;
	vector3f CR_tangent;

	double u2 = u*u;

	double u_data[4] = { 0.0, 1, 2*u,3*u2};
	double base_data[] = {0.0, 2.0, 0.0, 0.0,
		-1.0, 0.0, 1.0, 0.0,
		2.0, -5.0, 4.0, -1.0,
		-1.0, 3.0, -3.0, 1.0};
	double cont_data[] = {p0.x, p0.y, p0.z,
		p1.x, p1.y, p1.z, 
		p2.x, p2.y, p2.z,
		p3.x, p3.y, p3.z};

	uMat.assign(u_data);
	base.assign(base_data);
	control.assign(cont_data);

	prod = base.multiply(control);
	prod = uMat.multiply(prod);

	CR_tangent.x = prod.mat[0][0];
	CR_tangent.y = prod.mat[0][1];
	CR_tangent.z = prod.mat[0][2];

	return CR_tangent;
}

//create catmull-rom spline combining sequential 4 control points.
orient createCRspline(spline s){
	double u; 
	point p;
	orient r;
	int cnt = 0;

	r.length = (s.numControlPoints - 3) * numLerpPoints ;
	//glBegin(GL_LINE_STRIP);
		for(int i = 0; i < s.numControlPoints-3; i++){//point i, i+1, i+2, i+3 as control points for this segment
			for(int j = 0; j < numLerpPoints; j++){//100 points in one spline line
				u = j * (1.0 / numLerpPoints); // Interpolation parameter
				p = CR_point(s.points[i], s.points[i+1], s.points[i+2], s.points[i+3], u);
				r.tangent[cnt] = CR_tangent(s.points[i], s.points[i+1], s.points[i+2], s.points[i+3], u);
				r.tangent[cnt].normalize();
				//Calculate the tangent,normal,beside vector for each point using frenet frame
				if(i == 0 && j == 0){// the very first point
					point p1 = s.points[1];
					point p0 = s.points[0];
					
					vector3f vec0(p0.x, p0.y, p0.z);
					vector3f vec1(p1.x, p1.y, p1.z);
					vector3f vec2(0.0, 1.0, 0.0);
					
					r.vertice[0].x = p.x;   r.vertice[0].y = p.y;  r.vertice[0].z = p.z;

					//r.tangent[0] = vec1-vec0;
					
					//r.tangent[0].normalize();
					
					r.normal[0] = r.tangent[0].cross(vec2);
					r.normal[0].normalize();
					
					r.beside[0] = r.tangent[0].cross(r.normal[0]);
					r.beside[0].normalize();
					
				}else{
				
					point pi = s.points[i];
					point pj = s.points[i+1];
					
					vector3f vec0(pi.x, pi.y, pi.z);
					vector3f vec1(pj.x, pj.y, pj.z);
									
					r.vertice[cnt].x = p.x; r.vertice[cnt].y = p.y; r.vertice[cnt].z = p.z;
					
					//r.tangent[cnt] = vec1-vec0;
					//r.tangent[cnt].normalize();
					
					r.normal[cnt] = r.beside[cnt-1].cross(r.tangent[cnt]);
					r.normal[cnt].normalize();
					
					r.beside[cnt] = r.tangent[cnt].cross(r.normal[cnt]);
					r.beside[cnt].normalize();
					
				}
				//glNormal3f(r.normal[cnt].x, r.normal[cnt].y, r.normal[cnt].z);

				//glVertex3f(p.x,p.y,p.z);
				cnt++;
			}
		}
	//glEnd();

	return r;
}

