#include "peek-mill-utils.hpp"


/**
 * Loads XML stereo calibration files.
 *
 * @retval true on success, false on falure.
 *
 */
bool LoadCalibration(OpenCvCalibration *calibration)
{
    printf("Attempting to load camera parameters:\n\tM1-single.xml\n\tD1-single.xml\n");
        
        
    Mat m1_mat, d1_mat;
    
    CvMat *M1p = (CvMat *)cvLoad("M1-single.xml",NULL,NULL,NULL);
    if (M1p == NULL)
    {
        fprintf(stderr, "Error: Failed to load M1-single.xml\n");
        exit(-1);
    } else {
        m1_mat = Mat(M1p, true);
    }
    
    CvMat *D1p = (CvMat *)cvLoad("D1-single.xml",NULL,NULL,NULL);
    if (D1p == NULL)
    {
        fprintf(stderr, "Error: Failed to load D1-single.xml\n");
        exit(-1);
    } else {
        d1_mat = Mat(D1p, true);
    }
    
    calibration->M1 = m1_mat;
    calibration->D1 = d1_mat;
    
    
    return true;
}
