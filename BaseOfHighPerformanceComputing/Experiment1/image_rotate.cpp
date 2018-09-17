#include <boost/progress.hpp>
#include <iostream>

using namespace std;
typedef int pixel;
#define RIDX(x, y, dim) ((x)*(dim)+(y))
#define RIDX2(x, y, dim) ((y)*(dim)+(x))
#define RIDX3(x, y, dim) ((x)*(dim)+(y+(y+1)%4))
#define DIM 7680


void rotate_original(int dim, pixel *src, pixel *dst) {
    int i, j;
    boost::progress_timer elapsed_time;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}

void rotate_partition(int dim, pixel *src, pixel *dst) {
    int i, j, ii, jj;
    boost::progress_timer elapsed_time;
    for (ii = 0; ii < dim; ii += 4)
        for (jj = 0; jj < dim; jj += 4)
            for (i = ii; i < ii + 4; i++)
                for (j = jj; j < jj + 4; j++)
                    dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
}

void rotate_partition_loop_unroll(int dim, pixel *src, pixel *dst) {
    int i,j,ii,jj;
    boost::progress_timer elapsed_time;
    for(ii=0; ii < dim; ii+=32)
        for(jj=0; jj < dim; jj+=32)
            for(i=ii; i < ii+32; i+=4)
                for(j=jj; j < jj+32; j+=4) {
                    dst[RIDX(dim - 1 - j, i, dim)] = src[RIDX(i, j, dim)];
                    dst[RIDX(dim - 1 - j, i + 1, dim)] = src[RIDX(i + 1, j, dim)];
                    dst[RIDX(dim - 1 - j, i + 2, dim)] = src[RIDX(i + 2, j, dim)];
                    dst[RIDX(dim - 1 - j, i + 3, dim)] = src[RIDX(i + 3, j, dim)];
                    dst[RIDX(dim - 1 - j - 1, i, dim)] = src[RIDX(i, j + 1, dim)];
                    dst[RIDX(dim - 1 - j - 1, i + 1, dim)] = src[RIDX(i + 1, j + 1, dim)];
                    dst[RIDX(dim - 1 - j - 1, i + 2, dim)] = src[RIDX(i + 2, j + 1, dim)];
                    dst[RIDX(dim - 1 - j - 1, i + 3, dim)] = src[RIDX(i + 3, j + 1, dim)];
                    dst[RIDX(dim - 1 - j - 2, i, dim)] = src[RIDX(i, j + 2, dim)];
                    dst[RIDX(dim - 1 - j - 2, i + 1, dim)] = src[RIDX(i + 1, j + 2, dim)];
                    dst[RIDX(dim - 1 - j - 2, i + 2, dim)] = src[RIDX(i + 2, j + 2, dim)];
                    dst[RIDX(dim - 1 - j - 2, i + 3, dim)] = src[RIDX(i + 3, j + 2, dim)];
                    dst[RIDX(dim - 1 - j - 3, i, dim)] = src[RIDX(i, j + 3, dim)];
                    dst[RIDX(dim - 1 - j - 3, i + 1, dim)] = src[RIDX(i + 1, j + 3, dim)];
                    dst[RIDX(dim - 1 - j - 3, i + 2, dim)] = src[RIDX(i + 2, j + 3, dim)];
                    dst[RIDX(dim - 1 - j - 3, i + 3, dim)] = src[RIDX(i + 3, j + 3, dim)];
                }
}

void rotate_partition_2(int dim, pixel *src, pixel *dst) {
    int i, j, ii, jj;
    boost::progress_timer elapsed_time;
    for (ii = 0; ii < dim; ii += 4)
        for (jj = 0; jj < dim; jj += 4)
            for (i = ii; i < ii + 4; i++)
                for (j = jj; j < jj + 4; j++)
                    dst[RIDX3(dim - 1 - j, i, dim)] = src[RIDX3(i, j, dim)];
}

#define COPY(d,s) *(d) = *(s)
void rotate_partition_loop_unroll_dst_address_continuation(int dim, pixel *src, pixel *dst) {
    int i, j;
    boost::progress_timer elapsed_time;
    for (i = 0; i < dim; i+=32)
        for (j = dim-1; j >= 0; j-=1) {
            pixel *dptr = dst+RIDX(dim-1-j,i,dim);
            pixel *sptr = src+RIDX(i,j,dim);
            COPY(dptr, sptr); sptr += dim;
            COPY(dptr+1, sptr); sptr += dim;
            COPY(dptr+2, sptr); sptr += dim;
            COPY(dptr+3, sptr); sptr += dim;
            COPY(dptr+4, sptr); sptr += dim;
            COPY(dptr+5, sptr); sptr += dim;
            COPY(dptr+6, sptr); sptr += dim;
            COPY(dptr+7, sptr); sptr += dim;
            COPY(dptr+8, sptr); sptr += dim;
            COPY(dptr+9, sptr); sptr += dim;
            COPY(dptr+10, sptr); sptr += dim;
            COPY(dptr+11, sptr); sptr += dim;
            COPY(dptr+12, sptr); sptr += dim;
            COPY(dptr+13, sptr); sptr += dim;
            COPY(dptr+14, sptr); sptr += dim;
            COPY(dptr+15, sptr); sptr += dim;
            COPY(dptr+16, sptr); sptr += dim;
            COPY(dptr+17, sptr); sptr += dim;
            COPY(dptr+18, sptr); sptr += dim;
            COPY(dptr+19, sptr); sptr += dim;
            COPY(dptr+20, sptr); sptr += dim;
            COPY(dptr+21, sptr); sptr += dim;
            COPY(dptr+22, sptr); sptr += dim;
            COPY(dptr+23, sptr); sptr += dim;
            COPY(dptr+24, sptr); sptr += dim;
            COPY(dptr+25, sptr); sptr += dim;
            COPY(dptr+26, sptr); sptr += dim;
            COPY(dptr+27, sptr); sptr += dim;
            COPY(dptr+28, sptr); sptr += dim;
            COPY(dptr+29, sptr); sptr += dim;
            COPY(dptr+30, sptr); sptr += dim;
            COPY(dptr+31, sptr); sptr += dim;
        }
}

void rotate_partition_2_base(){
    int i, j, ii, jj;
    boost::progress_timer elapsed_time;
    for (ii = 0; ii < DIM; ii += 4)
        for (jj = 0; jj < DIM; jj += 4)
            for (i = ii; i < ii + 4; i++)
                for (j = jj; j < jj + 4; j++){
                    RIDX3(DIM - 1 - j, i, DIM);
                    RIDX3(i, j, DIM);
                }
}

int main(int argc, char *argv[]) {
    pixel *src, *dst;
    src = new(int[DIM * DIM]);
    dst = new(int[DIM * DIM]);
    string options = "1";
    if(argc>1){
        options.replace(0, 1, argv[1]);
    }

    if (string::npos!=options.find('1')){
        cout << "rotate_original:                                       ";
        rotate_original(DIM, src, dst);
    }
    if (string::npos!=options.find('2')){
        cout << "rotate_partition:                                      ";
        rotate_partition(DIM, src, dst);
    }
    if (string::npos!=options.find('3')){
        cout << "rotate_partition_loop_unroll:                          ";
        rotate_partition_loop_unroll(DIM, src, dst);
    }
    if (string::npos!=options.find('4')){
        cout << "rotate_partition_2:                                    ";
        rotate_partition_2(DIM, src, dst);
    }
    if (string::npos!=options.find('5')){
        cout << "rotate_partition_loop_unroll_dst_address_continuation: ";
        rotate_partition_loop_unroll_dst_address_continuation(DIM, src, dst);
    }
    if (string::npos!=options.find('6')){
        cout << "rotate_partition_2_base:                               ";
        rotate_partition_2_base();
    }

    delete[](src);
    delete[](dst);
}

