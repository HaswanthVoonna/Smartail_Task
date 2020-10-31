
# MEDIAN FILTER

## A 2D median filter is implemented from scratch using C++

### Median filter is windowed filter of nonlinear class, which easily removes destructive noise while preserving edges. The basic idea behind filter is for any element of the signal (image) look at its neighborhood and pick up the element most similar to others. Median filter in its properties resembles mean filter, or average filter, but much better in treating “salt and pepper” noise and edge preserving.

## Median Filter Algorithm

1. Place a window over element
2. Pick up elements
3. Order elements
4. Take the middle element

## Code Explanation

### The first step is we will develop 2D median filter with window of 3x3 size. We have a 2D image of dimensions M X N as input. The first step is window placing — we do that by changing index of the leading element:

``` c++
//   Move window through all elements of the image  
for (int m = 1; m < M - 1; ++m)
    for (int n = 1; n < N - 1; ++n)
```

### we are starting with the second element and finishing with the last but one. The problem is we cannot start with the first element, because in this case the left part of the filter window is empty. This is problem is resolved and will be discussed below.

### The second step is picking up elements around

```c++
//   Pick up window elements
for (int j = m - 1; j < m + 2; ++j)
            for (int i = n - 1; i < n + 2; ++i)
               window[k++] = image[j * N + i];
```

### The third step is putting window elements in order. But we will use a code optimization trick here. Everything we need is middle element. So, it is enough to order just a half of elements.

```c++
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
```

### The final step — take the middle

```c++
//   Get result - the middle element
    result[(m - 1) * (N - 2) + n - 1] = window[4];
```

### At last, let us write down the entire algorithm as function:

```c++
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
```
### For all window filters there is some problem. That is edge treating. If you place window over first (last) element, the left (right) part of the window will be empty. To fill the gap, signal should be extended. For median filter there is good idea to extend signal or image symmetrically

### So, before passing image to our median filter function the image should be extended. To do that we are to add lines at the top and at the bottom of the image and add columns to the left and to the right. Let us write down the wrapper, which makes all preparations.

### Here is our wrapper function, which does that job.

```c++
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
```

### As you can see, our code takes into account some practical issues. First of all we check our input parameters — image should not be NULL, and image width and length should be positive

```c++
//   Check arguments
   if (!image || N < 1 || M < 1)
      return;
```

### Now let us allocate memory for image extension.

```c++
//   Allocate memory for signal extension
   element* extension = new element[(N + 2) * (M + 2)];
```

### And check memory allocation.

```c++
//   Check memory allocation
   if (!extension)
      return;
```

### Now we are building extension.

```c++
for (int i = 0; i < M; ++i)
   {
      memcpy(extension + (N + 2) * (i + 1) + 1, image + N * i, N * sizeof(element));
      extension[(N + 2) * (i + 1)] = image[N * i];
      extension[(N + 2) * (i + 2) - 1] = image[N * (i + 1) - 1];
   }
```
### Filling first and last line of image extension

```c++
//   Fill first line of image extension
   memcpy(extension, extension + N + 2, (N + 2) * sizeof(element));
   //   Fill last line of image extension
   memcpy(extension + (N + 2) * (M + 1), extension + (N + 2) * M, (N + 2) * sizeof(element));
```

### Finally, everything is ready — filtering!

```c++
//   Call median filter implementation
   _medianfilter(extension, result ? result : image, N + 2, M + 2);
```

### And to complete the job — free memory.

```c++
//   Free memory
   delete[] extension;
```

### And Finally to test the algorithm openCV is used to take the input image in grayscale format and write the image to a file




