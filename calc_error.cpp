#include "calc_error.h"
circledata calc_error(int rows,int cols,Mat carvan){
	circledata outp;
	Scalar s;
	double b, g, r;
	Mat tempcar=carvan.clone();
	RNG rng(getTickCount());
	outp.rad = rng.uniform(1, rows);
	outp.x = rng.uniform(0, cols);
	outp.y = rng.uniform(0, rows);
	b = rng.uniform(0, 255);
	g = rng.uniform(0, 255);
	r = rng.uniform(0, 255);
	outp.b = b / 65535;
	outp.g = g / 65535;
	outp.r = r / 65535;
	circle(tempcar, Point(outp.x, outp.y), outp.rad, 
		Scalar(outp.b, outp.g, outp.r), -1);//Ëæ»ú»­Ô²
	absdiff(tempcar, carvan, outp.diff);//¼ÆËãdiff
	outp.diff = outp.diff.mul(outp.diff);
	s = sum(outp.diff);
	outp.err = (s.val[0] + s.val[1] + s.val[2]) /
		(carvan.total() * carvan.channels());//¼ÆËãmse
	return outp;
}