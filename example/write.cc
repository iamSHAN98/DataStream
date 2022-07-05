/*
		Stores N (input) pairs of random numbers uniformly
		distributed in the range [0, 1].
*/

#include <iostream>
#include <random>
#include <cmath>
#include "DataStream.h"

struct Point{
	double x, y;
};

int main(int argc, char **argv){

	// DataStream
	DataStream::File Obj;
	Obj.Open("Data.h5", DataStream::Write);

	// Data containers
	Point R;

	// MetaData
	DataStream::MetaData Info(DataStream::Compound, {1}, "Point");
	Info.AddMember(&Point::x, "x", DataStream::Double);
	Info.AddMember(&Point::y, "y", DataStream::Double);

	// Datasets
	Obj.Add("Group/Data", &R, Info);

	Obj.SetCompression(DataStream::Compress::ZStd, 10);

	// Initialize
	int N = (argc > 1) ? std::stoi(argv[1]) : 1;
	Obj.Configure(N);

	// Data generation
	// Random number generator
	std::random_device seed;
  std::mt19937 gen(seed());
	std::uniform_real_distribution<double> dist(0., 1.);

	// Attributes
	char Gen[] = "Mersenne-Twister";
	Obj.SetAttribute("Group", "Generator", Gen, DataStream::String);

	// Store data
	for(int n = 0; n < N; n++){
		R.x = dist(gen);
		R.y = dist(gen);
		Obj.Write();
	}

	Obj.Close();
	return 0;
}
