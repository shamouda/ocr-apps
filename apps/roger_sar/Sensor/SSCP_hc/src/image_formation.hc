#include "common.h"

void FormImage(struct DigSpotVars *dig_spot, struct ImageParams *image_params, struct Inputs *in, struct complexData **curImage, struct RadarParams *radar_params)
{
	int n, i, j;
	float xc, yc;
	struct complexData **Xsubimg;
	fftwf_plan plan_forward;
	fftwf_complex *input, *fft_result;

	for(n=0; n<image_params->Iy; n++) {
		memset(&curImage[n][0], 0, image_params->Ix*sizeof(struct complexData));
	}
	
	if(image_params->TF > 1)
	{
		// FFTW initialization
#ifdef RAG_DEBUG
fprintf(stderr,"FormImage FFTW initialization TF = %d\n",image_params->TF);fflush(stderr);
#endif
		input = (fftwf_complex*)fftwf_malloc(image_params->S1 * sizeof(fftwf_complex));
		fft_result = (fftwf_complex*)fftwf_malloc(image_params->S1 * sizeof(fftwf_complex));
		plan_forward = fftwf_plan_dft_1d(image_params->S1, input, fft_result, FFTW_FORWARD, FFTW_ESTIMATE);

		for(i=0; i<image_params->P1; i++)
		{
			// Copy data from one pulse to FFT input array (first and second halves swapped)
			for(j=(int)ceilf(image_params->S1/2), n=0; j<image_params->S1; j++, n++) {
				input[n][0] = in->X[i][j].real;
				input[n][1] = in->X[i][j].imag;
			}
			for(j=0; j<(int)ceilf(image_params->S1/2); j++, n++) {
				input[n][0] = in->X[i][j].real;
				input[n][1] = in->X[i][j].imag;
			}

			// Perform FFT
			fftwf_execute(plan_forward);

			// Copy FFT results to X2
			memcpy(&dig_spot->X2[i][0], fft_result, image_params->S1*sizeof(struct complexData));
		}

		for(i=0; i<image_params->TF; i++)
		{
			for(j=0; j<image_params->TF; j++)
			{
				// Subimage center
				xc = image_params->xr[(image_params->Sx-1)/2 + i*image_params->Sx];
				yc = image_params->yr[(image_params->Sy-1)/2 + j*image_params->Sy];

				// Perform Digital Spotlighting
				Xsubimg = DigSpot(xc, yc, dig_spot, image_params, radar_params, in);

				// Adjust platform locations
				for(n=0; n<image_params->P2; n++) {
					dig_spot->Pt2[n][0] = in->Pt[n*image_params->TF][0] + xc;
					dig_spot->Pt2[n][1] = in->Pt[n*image_params->TF][1] + yc;
					dig_spot->Pt2[n][2] = in->Pt[n*image_params->TF][2];
				}

				// Perform backprojection over subimage
finish { 		BackProj(Xsubimg, dig_spot->Pt2, i*image_params->Sx, (i+1)*image_params->Sx-1, j*image_params->Sy, (j+1)*image_params->Sy-1, image_params, curImage, radar_params); }
			}
		}
		
		fftwf_free(input);
		fftwf_free(fft_result);
		fftwf_destroy_plan(plan_forward);
	}
	else
	{
		// Perform backprojection over full image
finish {	BackProj(in->X, in->Pt, 0, image_params->Ix-1, 0, image_params->Iy-1, image_params, curImage, radar_params); }
	}
#ifdef RAG_DEBUG
fprintf(stderr,"Form Image returns\n");fflush(stderr);
#endif
}