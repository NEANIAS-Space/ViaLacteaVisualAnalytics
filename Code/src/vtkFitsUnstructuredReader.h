// .NAME vtkFitsUnstructuredReader - read structured points from FITS file.
// .SECTION Description
// vtkFitsUnstructuredReader is a source object that reads FITS data files
// .SECTION Caveats
// Uses CFITSIO v2.0 (http://heasarc.gsfc.nasa.gov/docs/software/fitsio)

#ifndef __vtkFitsUnstructuredReader_h
#define __vtkFitsUnstructuredReader_h


#include "vtkAlgorithm.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPoints.h"
#include "vtkFloatArray.h"
#include "vtkPolyDataAlgorithm.h"
#include "vtkPolyData.h"


extern "C" {
#include "fitsio.h"
}


//class VTK_EXPORT vtkFitsUnstructuredReader : public vtkStructuredPointsSource
class VTK_EXPORT vtkFitsUnstructuredReader : public vtkPolyDataAlgorithm
{
public:
    //    vtkFitsUnstructuredReader();
    //  static vtkFitsUnstructuredReader *New() {return new vtkFitsUnstructuredReader;}

    static vtkFitsUnstructuredReader *New();
    vtkFitsUnstructuredReader();
    ~vtkFitsUnstructuredReader();

    void SetFileName(std::string name);
    std::string GetFileName(){return filename;}

    vtkTypeMacro(vtkFitsUnstructuredReader,vtkAlgorithm);
    void PrintSelf(ostream& os, vtkIndent indent);
    void PrintHeader(ostream& os, vtkIndent indent);
    void PrintTrailer(std::ostream& os , vtkIndent indent);
    double getInitSlice(){return initSlice;}
    
    bool is3D;
    void CalculateRMS();
    void ReadPoints( vtkPolyData* output);
    double GetSigma(){return sigma;}
    double GetRMS(){return rms;}
    double GetMedia(){return media;}
    long GetEntries(){return npix;}
    float* GetRangeSlice(int i);

    float GetMin(){return datamin;}
    float GetMax(){return datamax;}
    
    float GetCrval(int i){
        if(i<3)
            return crval[i];
        else return -1;
    }
    float GetCpix(int i){
        if(i<3) return cpix[i];
        else return -1;}
    float GetCdelt(int i){if(i<3) return cdelt[i];
        else return -1;}

    int GetNaxes(int i);
    
    vtkFloatArray *fitsScalars;

    // Description:
    // Get the output data object for a port on this algorithm.
    vtkPolyData* GetOutput();
    vtkPolyData* GetOutput(int);
    virtual void SetOutput(vtkDataObject* d);
    // Description:
    // see vtkAlgorithm for details
   /* virtual int ProcessRequest(vtkInformation*,
                               vtkInformationVector**,
                               vtkInformationVector*);*/

    std::string getSpecies() {return species;};
    std::string getTransition() {return transition;};
    std::string getSurvey() {return survey;};

    void setSpecies(std::string s) {species=s;};
    void setTransition(std::string s) {transition=s;};
    void setSurvey(std::string s) {survey=s;};

protected:

    std::string survey;
    std::string species;
    std::string transition;

    float datamin;
    float datamax;
    float epoch;   // Part of FITS Header file
    int *dimensions;   // [x,y,z]
    int naxis;
    int points;
    double crval[3];
    double cpix[3];
    double cdelt[3];
    double sigma=0;
    double media=0;
    double rms=0;
    long npix=0;
    long naxes[3];

    float **minmaxslice;


    // Description:
    // This is called by the superclass.
    // This is the method you should override.
   int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;



private:
    //  vtkFitsUnstructuredReader( const vtkFitsUnstructuredReader& ); // Not implemented.
    //  void operator = ( const vtkFitsUnstructuredReader& );  // Not implemented.
    std::string filename;
    char title[80];
    char xStr[80];
    char yStr[80];
    char zStr[80];
//    char cunit3[1][200];

    void ReadHeader();
    void printerror(int status); // from fitsio distribution
    double initSlice;
};



#endif
