
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <random>
#include "SmallestEnclosingCircle.hpp"

using std::size_t;
using std::vector;
// Random number generation global variables
static std::default_random_engine randGen((std::random_device())());
static std::normal_distribution<double> normalDist;

static const double EPSILON = 1e-12;

static vector<Point> makeRandomPoints(size_t n) {
	vector<Point> result;
	if (std::uniform_real_distribution<double>()(randGen) < 0.2) {  // Discrete lattice (to have a chance of duplicated points)
		std::uniform_int_distribution<int> dist(0, 9);
		for (size_t i = 0; i < n; i++)
			result.push_back(Point{static_cast<double>(dist(randGen)), static_cast<double>(dist(randGen))});
	} else {  // Gaussian distribution
		for (size_t i = 0; i < n; i++)
			result.push_back(Point{normalDist(randGen), normalDist(randGen)});
	}
	return result;
}

int main() {
  const vector<Point> points = makeRandomPoints(100);
  Circle actual = makeSmallestEnclosingCircle(points);
  std::cout << "c = (" << actual.c.x << ", " << actual.c.y << ")\n";
  std::cout << "r = " << actual.r << "\n";
  return 0;
}
