/* ********************************
 * 作者：weiby3
 * 文件说明：二维高斯函数，OpenMP版
 * ********************************
 */
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <string>

using namespace std;

const char *DEBUG_ENV = "DEBUG_TIME";

/* ************************************
 * 函数：new_float_array
 * 函数描述：新建二维单精度浮点数组
 * 参数描述：
 * x：数组一维大小
 * y：数组二维大小
 * 返回值：指向二维单精度浮点数组的指针
 * ************************************
 */
float **new_float_array(int x, int y) {
  auto pf = new float[x * y];
  auto ppf = new float *[x];
  for (int i = 0; i < x; i++) {
    ppf[i] = pf + y * i;
  }
  return ppf;
}

/* *********************************
 * 函数：delete_float_array
 * 函数描述：删除二维单精度浮点数组
 * 参数描述：
 * ppf：指向二维单精度浮点数组的指针
 * *********************************
 */
void delete_float_array(float **ppf) {
  delete[] ppf[0];
  delete[] ppf;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Wrong input!\n"
           "Usage: %s s, s is an integer, others are undefined behavior.\n",
           argv[0]);
    return 1;
  }

  const char *penv = getenv(DEBUG_ENV);

  auto s = atoi(argv[1]);
  auto s6p1 = 6 * s + 1;
  auto ppf = new_float_array(s6p1, s6p1);
  auto rss2pi = 1.0f / (float(s) * sqrtf(float(2 * acos(-1.0))));
  auto ss = float(2 * s * s);
  /*auto xyarr = new float[s6p1];
  for (int i = 0; i < s6p1; i++) {
    xyarr[i] = float(i - 3 * s);
  }*/

  auto start_time = chrono::high_resolution_clock::now();
#pragma omp parallel for
  for (int i = 0; i < s6p1; i++) {
    // auto x = xyarr[i];
    auto x = float(i - 3 * s);
    for (int j = 0; j < s6p1; j++) {
      // auto y = xyarr[j];
      auto y = float(j - 3 * s);
      ppf[i][j] = rss2pi * exp(-(float((x * x) + (y * y)) / ss));
    }
  }
  auto end_time = chrono::high_resolution_clock::now();

  if (penv != nullptr) {
    /*cout << chrono::duration_cast<chrono::microseconds>(end_time - start_time)
                .count()
         << endl;*/
    cout << (end_time - start_time).count() << endl;
  } else {
    for (int i = 0; i < s6p1; i++) {
      for (int j = 0; j < s6p1; j++) {
        printf("%5.4f ", ppf[i][j]);
      }
    }
  }

  // delete[] xyarr;
  delete_float_array(ppf);
  return 0;
}