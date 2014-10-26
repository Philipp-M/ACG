#include "SmallPT.hpp"
#include "GScene.hpp"
#include "GSphere.hpp"
#include "GTriangle.hpp"
#include <cstdlib>
#include <cstdio>

int main(int argc, char *argv[])
{
	int w = 1024, h = 768; // image size
	int samps = argc == 2 ? atoi(argv[1]) / 4 : 1; // # samples (default of 1
	Ray cam(Vec(50, 52, 295.6), Vec(0, -0.042612, -1).norm()); // camera pos, dir
	Vec cx = Vec(w * .5135 / h); // x direction increment (uses implicit 0 for y, z)
	Vec cy = (cx % cam.direction).norm() * .5135; // y direction increment (note cross product)
	Vec r; // used for colors of samples
	Vec *c = new Vec[w * h]; // The image

	/********** create the scene *************/
	GScene scene;
//	scene.addItem(new GSphere(1e5, Vec(1e5 + 1, 40.8, 81.6), Vec(), Vec(.75, .25, .25), DIFF));
//	scene.addItem(new GSphere(1e5, Vec(-1e5 + 99, 40.8, 81.6), Vec(), Vec(.25, .25, .75), DIFF));
//	scene.addItem(new GSphere(1e5, Vec(50, 40.8, 1e5), Vec(), Vec(.75, .75, .75), DIFF));
//	scene.addItem(new GSphere(1e5, Vec(50, 40.8, -1e5 + 170), Vec(), Vec(), DIFF));
//	scene.addItem(new GSphere(1e5, Vec(50, 1e5, 81.6), Vec(), Vec(.75, .75, .75), DIFF));
//	scene.addItem(new GSphere(1e5, Vec(50, -1e5 + 81.6, 81.6), Vec(), Vec(.75, .75, .75), DIFF));
//	scene.addItem(new GSphere(16.5, Vec(27, 16.5, 47), Vec(), Vec(1, 1, 1) * .999, SPEC));
//	scene.addItem(new GSphere(16.5, Vec(73, 16.5, 78), Vec(), Vec(1, 1, 1) * .999, REFR));
//	scene.addItem(new GSphere(1.5, Vec(50, 81.6 - 16.5, 81.6), Vec(4, 4, 4) * 100, Vec(), DIFF));

	scene.addItem(new GSphere(1e5, Vec(1e5 + 1, 40.8, 81.6), Vec(), Vec(.75, .25, .25), DIFF));
	scene.addItem(new GSphere(1e5, Vec(-1e5 + 99, 40.8, 81.6), Vec(), Vec(.25, .25, .75), DIFF));
	scene.addItem(new GSphere(1e5, Vec(50, 40.8, 1e5), Vec(), Vec(.75, .75, .75), DIFF));
	scene.addItem(new GSphere(1e5, Vec(50, 40.8, -1e5 + 170), Vec(), Vec(), DIFF));
	scene.addItem(new GSphere(1e5, Vec(50, 1e5, 81.6), Vec(), Vec(.75, .75, .75), DIFF));
	scene.addItem(new GSphere(1e5, Vec(50, -1e5 + 81.6, 81.6), Vec(), Vec(.75, .75, .75), DIFF));
	scene.addItem(new GSphere(16.5, Vec(27, 16.5, 47), Vec(), Vec(1, 1, 1) * .999, SPEC));
	scene.addItem(new GTriangle(Vec(17, 34, 20), Vec(27+20, 60+20, 20-10), Vec(27-20, 60-5, 20+30), Vec(), Vec(0.5, 1, 0) * .999, DIFF));
	scene.addItem(new GSphere(16.5, Vec(73, 16.5, 78), Vec(), Vec(1, 0, 0) * .999, REFR));
	scene.addItem(new GSphere(600, Vec(50, 681.6 - .27, 81.6), Vec(12, 12, 12), Vec(), DIFF));

#pragma omp parallel for schedule(dynamic, 1) private(r) // OpenMP

	// LOOP OVER ALL IMAGE PIXELS
	for (int y = 0; y < h; y++)
	{ // Loop over image rows
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samps * 4, 100. * y / (h - 1)); // print progress
		for (unsigned short x = 0, Xi[3] =
		{ 0, 0, y * y * y }; x < w; x++)   // Loop columns

			// FOR EACH PIXEL DO 2x2 SUBSAMPLES, AND samps SAMPLES PER SUBSAMPLE
			for (int sy = 0, i = (h - y - 1) * w + x; sy < 2; sy++)     // 2x2 subpixel rows
				for (int sx = 0; sx < 2; sx++, r = Vec())
				{        // 2x2 subpixel cols
					for (int s = 0; s < samps; s++)
					{
						// I BELIEVE THIS PRODUCES A TENT FILTER
						double r1 = 2 * erand48(Xi), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
						double r2 = 2 * erand48(Xi), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
						Vec d = cx * (((sx + .5 + dx) / 2 + x) / w - .5) + cy * (((sy + .5 + dy) / 2 + y) / h - .5) + cam.direction;
						r = r + SmallPT::radiance(Ray(cam.origin + d * 140, d.norm()), &scene, 0, Xi) * (1. / samps);
					} // Camera rays are pushed ^^^^^ forward to start in interior
					c[i] = c[i] + Vec(SmallPT::clamp(r.x), SmallPT::clamp(r.y), SmallPT::clamp(r.z)) * .25;
				}
	}

	// WRITE OUT THE FILE TO A PPM
	FILE *f = fopen("image.ppm", "w");         // Write image to PPM file.
	fprintf(f, "P3\n%d %d\n%d\n", w, h, 255);
	for (int i = 0; i < w * h; i++)
	{
		fprintf(f, "%d %d %d ", SmallPT::toInt(c[i].x), SmallPT::toInt(c[i].y), SmallPT::toInt(c[i].z));
	}
}
