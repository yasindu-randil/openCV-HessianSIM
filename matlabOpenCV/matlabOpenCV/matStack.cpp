#include "matStack.h"


matStack::matStack( std::string filename, std::string printfile):
	printFile(printfile)
{
	std::cout << "Inside matStack Constructor" << std::endl;

	// Read the mulit framed imaged into a  vector of cv::Mat
	imreadmulti(filename, imgS, cv::IMREAD_UNCHANGED);
	initialize();
	diffOperatorFFT();
	printTest();

}

/*
// Initialize the variables
// Basic configurations of the imgS and frac cv::Mats
*/

void matStack::initialize()
{
	int tempVal = 0;
	double testMaxval = 0;

	width = imgS[0].cols;
	height = imgS[0].rows;
	frames = imgS.size();

	zbei = 1;
	siranu = 150;
	lamda = 0.5;

	divide.convertTo(divide, CV_32F);

	// Prevents re-allocation of the vector as push_back is used.
	frac.reserve(frames);
	
	// Convert to 4 byte floating values 
	// Get the max value
	for (int i = 0; i < frames; i++) {
		imgS[i].convertTo(imgS[i], CV_32F);

		minMaxIdx(imgS[i], 0, &testMaxval);
		if (tempVal < testMaxval)
		{
			tempVal = testMaxval;
		}


	}

	for (int i = 0; i < frames; i++)
	{
		imgS[i] /= tempVal;

		frac.push_back(imgS[i] * (siranu / lamda));

	}


	//printImageMat(printFile, frac.at(179));


	std::cout << " Max Value from class = " << tempVal << std::endl;
}

/*
// Calculate the DFT of difference operator
*/
void matStack::diffOperatorFFT()
{
	float data[3] = { 1, -2, 1 };
	float data2[3][1] = { {1}, {-2}, { 1} };
	float dataZtiduzz[3] = { 0 };
	float data3[2][2][2][2] = { {{1, -1}, {1, -1}}, {{-1, 1}, {-1, 1}} };
	float data4[2][2] = { {1 ,-1},{-1 ,1} };
	float data5[2][2][2] = { { {1}, {-1} }, { {-1}, {1} } };

	cv::Mat fftData = cv::Mat(1, 3, CV_32F, data);
	cv::Mat fftData2 = cv::Mat(3, 1, CV_32F, data2);
	cv::Mat fftData4 = cv::Mat(2, 2, CV_32F, data4);
	cv::Mat ztiduzz = cv::Mat(1, 3, CV_32F, dataZtiduzz);
	cv::Mat ztiduxz = cv::Mat(2, 2, CV_32F, data3);
	cv::Mat ztiduyz = cv::Mat(2, 2, CV_32F, data5);

	cv::Mat fftOutput, fftConj, fftConjPrev;

	fftConjugate(fftData, fftOutput, fftConj);
	// deep copy is used, because shallow copy will modify the shared memory
	fftConjPrev = fftConj.clone();


	fftConjugate(fftData2, fftOutput, fftConj);
	cv::add(fftConj, fftConjPrev, fftConjPrev); 


	fftConjugate(ztiduzz, fftOutput, fftConj);
	fftConj.mul(fftConj * (zbei * zbei));
	cv::add(fftConj, fftConjPrev, fftConjPrev);


	fftConjugate(fftData4, fftOutput, fftConj);
	fftConj *= 2;
	cv::add(fftConj, fftConjPrev, fftConjPrev);


	fftConjugate(ztiduxz, fftOutput, fftConj);
	fftConj = fftConj * (zbei) * 2;
	cv::add(fftConj, fftConjPrev, fftConjPrev);


	fftConjugate(ztiduyz, fftOutput, fftConj);
	fftConj = fftConj * (2 * zbei);
	cv::add(fftConj, fftConjPrev, fftConjPrev);

	fftConjPrev +=  (siranu / lamda);

	divide = converToRealNumbers(fftConjPrev);




}

// Get multiplication of two cv::Mat. In this case the conjugate of second 
// Mat will be used. 

void matStack::fftConjugate(cv::Mat fftData, cv::Mat& fftOutput, cv::Mat& fftConj)
{
	fft2(fftData, fftOutput, height, width);
	cv::mulSpectrums(fftOutput, fftOutput, fftConj, 0, true);
}

void matStack::printImageVectorMat( std::string printfile, int index)
{
	std::ofstream myfile;
	myfile.open(printfile, std::ios::out | std::ios::binary);


	myfile << imgS[index] << std::endl;



	myfile.close();
}

void matStack::printImageMat(std::string printfile, const cv::Mat img)
{
	std::ofstream myfile;
	myfile.open(printfile, std::ios::out | std::ios::binary);


	myfile << img << std::endl;

	myfile.close();
}

void matStack::printImageMat(std::string printfile, std::vector<cv::Mat> img)
{
	std::ofstream myfile;
	myfile.open(printfile, std::ios::out | std::ios::binary);

	for (int i = 0; i < frames; i++)
	{
		myfile << img.at(i) << std::endl;
	}

	myfile.close();
}

std::vector<cv::Mat> matStack::getVectorFrac()
{
	return frac;
}

std::vector<cv::Mat> matStack::getVectorImg()
{
	return imgS;
}
