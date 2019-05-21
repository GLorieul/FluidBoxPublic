
#include "field.hpp"
#include "util/all.hpp"

namespace fb
{

double computeL1Error(const Field& A, const Field& B)
/*       1              
 * Err = —.∑|aᵢⱼ - bᵢⱼ| 
 *       N              
 *
 * The 1/N is there to have the result not depend on the resolution of the domain.
*/
{
    assume_shareTheSameDomain(__FILE__, __LINE__, __FUNCTION__, A, B);
    assume_shareTheSameAnchor(__FILE__, __LINE__, __FUNCTION__, A, B);
    const int nbCells = A.get_nbOfPhysicalCells();
    const Resolution& res = A.get_domain().get_resolution();
    double sum = 0.0;
    for(int i=0 ; i < res.x ; i++) {
        for(int j=0 ; j < res.y ; j++) {
            for(int k=0 ; k < res.z ; k++) 
                { sum += abs(A(i,j,k) - B(i,j,k)); }
        }
    }
    return sum/nbCells;
}

double computeL2Error(const Field& A, const Field& B)
/*       1                   
 * Err = ——.√[∑(aᵢⱼ - bᵢⱼ)²] 
 *       √N                  
 *
 * The 1/√N is there to have the result not depend on the resolution of the domain
*/
{
    assume_shareTheSameDomain(__FILE__, __LINE__, __FUNCTION__, A, B);
    assume_shareTheSameAnchor(__FILE__, __LINE__, __FUNCTION__, A, B);
    const int nbCells = A.get_nbOfPhysicalCells();
    const Resolution& res = A.get_domain().get_resolution();
    double sum = 0.0;
    for(int i=0 ; i < res.x ; i++) {
        for(int j=0 ; j < res.y ; j++) {
            for(int k=0 ; k < res.z ; k++) {
                const double diff = A(i,j,k) - B(i,j,k);
                sum += diff*diff;
            }
        }
    }
    return sqrt(sum/nbCells);
}

double computeMaxError(const Field& A, const Field& B)
/*                      
 * Err = max|aᵢⱼ - bᵢⱼ| 
 *                      
*/
{
    assume_shareTheSameDomain(__FILE__, __LINE__, __FUNCTION__, A, B);
    assume_shareTheSameAnchor(__FILE__, __LINE__, __FUNCTION__, A, B);
    const int nbCells = A.get_nbOfPhysicalCells();
    const Resolution& res = A.get_domain().get_resolution();
    double maxVal = 0.0;
    for(int i=0 ; i < res.x ; i++) {
        for(int j=0 ; j < res.y ; j++) {
            for(int k=0 ; k < res.z ; k++) {
                const double diff = A(i,j,k) - B(i,j,k);
                maxVal = max(abs(diff), 0.0);
            }
        }
    }
    return maxVal;
}

}

