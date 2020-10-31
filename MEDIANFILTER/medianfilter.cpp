#include <opencv2\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <memory.h>

using namespace std;
using namespace cv;

typedef int element;

//   2D MEDIAN FILTER implementation
//     image  - input image
//     result - output image
//     N      - width of the image
//     M      - height of the image
void _medianfilter(const element* image, element* result, int N, int M)
{
   //   Move window through all elements of the image
   for (int m = 1; m < M - 1; ++m)
      for (int n = 1; n < N - 1; ++n)
      {
         //   Pick up window elements
         int k = 0;
         element window[9];
         for (int j = m - 1; j < m + 2; ++j)
            for (int i = n - 1; i < n + 2; ++i)
               window[k++] = image[j * N + i];
         //   Order elements (only half of them)
         for (int j = 0; j < 5; ++j)
         {
            //   Find position of minimum element
            int min = j;
            for (int l = j + 1; l < 9; ++l)
            if (window[l] < window[min])
               min = l;
            //   Put found minimum element in its place
            const element temp = window[j];
            window[j] = window[min];
            window[min] = temp;
         }
         //   Get result - the middle element
         result[(m - 1) * (N - 2) + n - 1] = window[4];
      }
}

//   2D MEDIAN FILTER wrapper
//     image  - input image
//     result - output image
//     N      - width of the image
//     M      - height of the image
void medianfilter(element* image, element* result, int N, int M)
{
   //   Check arguments
   if (!image || N < 1 || M < 1)
      return;
   //   Allocate memory for signal extension
   element* extension = new element[(N + 2) * (M + 2)];
   //   Check memory allocation
   if (!extension)
      return;
   //   Create image extension
   for (int i = 0; i < M; ++i)
   {
      memcpy(extension + (N + 2) * (i + 1) + 1, image + N * i, N * sizeof(element));
      extension[(N + 2) * (i + 1)] = image[N * i];
      extension[(N + 2) * (i + 2) - 1] = image[N * (i + 1) - 1];
   }
   //   Fill first line of image extension
   memcpy(extension, extension + N + 2, (N + 2) * sizeof(element));
   //   Fill last line of image extension
   memcpy(extension + (N + 2) * (M + 1), extension + (N + 2) * M, (N + 2) * sizeof(element));
   //   Call median filter implementation
   _medianfilter(extension, result ? result : image, N + 2, M + 2);
   //   Free memory
   delete[] extension;
}

int main(int argc, char** argv)
{
	Mat ImgSrc = imread("scratch_noises.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	if (!ImgSrc.data)          // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat ImgDst = ImgSrc.clone();
	int imgSize = ImgSrc.cols * ImgSrc.rows;	// Get the Image size

	unsigned char *pSrcData = (unsigned char*)(ImgSrc.data);
	unsigned char *pDstData = (unsigned char*)(ImgDst.data);

	// Call median filter 5 times
	for (int i = 0; i < 5; ++i)
	{
		medianfilter(pSrcData, pDstData, ImgSrc.cols, ImgSrc.rows);
	}
	imwrite("processed_image.bmp", ImgDst);
	namedWindow("Original Image");
	imshow("Original Image", ImgSrc);
	namedWindow("Processed Image");
	imshow("Processed Image", ImgDst);
	cvWaitKey();
	return 0;
}