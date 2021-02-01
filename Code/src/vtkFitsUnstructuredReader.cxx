#include "vtkFitsUnstructuredreader.h"

#include "vtkCommand.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkFloatArray.h"
#include <cmath>
#include "vtkPointData.h"
#include <iostream>     // std::cout
#include <sstream>


//vtkCxxRevisionMacro(vtkFitsUnstructuredReader, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkFitsUnstructuredReader);

//----------------------------------------------------------------------------
vtkFitsUnstructuredReader::vtkFitsUnstructuredReader()
{
    this->filename[0]='\0';
    this->xStr[0]='\0';
    this->yStr[0]='\0';
    this->zStr[0]='\0';
    this->title[0]='\0';
    this->SetNumberOfInputPorts( 0 );
    this->SetNumberOfOutputPorts( 1 );
    //this->is3D=is3D;

    for (int i=0; i<3; i++)
    {
        crval[i]=0;
        cpix[i]=0;
        cdelt[i]=0;
        naxes[i]= 10;
    }
    
    this->is3D=false;
    this->SetNumberOfInputPorts(0);


}

//----------------------------------------------------------------------------
vtkFitsUnstructuredReader::~vtkFitsUnstructuredReader()
{
}

void vtkFitsUnstructuredReader::SetFileName(std::string name) {


    if (name.empty()) {
        vtkErrorMacro(<<"Null Datafile!");
        return;
    }


    filename= name;
    this->Modified();


}
//----------------------------------------------------------------------------
void vtkFitsUnstructuredReader::PrintSelf(ostream& os, vtkIndent indent)
{
    // this->Superclass::PrintSelf(os, indent);
}

void vtkFitsUnstructuredReader::PrintHeader(ostream& os, vtkIndent indent)
{
    // this->Superclass::PrintHeader(os, indent);

}

void vtkFitsUnstructuredReader::PrintTrailer(std::ostream& os , vtkIndent indent)
{
    // this->Superclass::PrintTrailer(os, indent);
}

//----------------------------------------------------------------------------
vtkPolyData* vtkFitsUnstructuredReader::GetOutput()
{
    return this->GetOutput(0);
}

//----------------------------------------------------------------------------
vtkPolyData* vtkFitsUnstructuredReader::GetOutput(int port)
{
    return vtkPolyData::SafeDownCast(this->GetOutputDataObject(port));
}

//----------------------------------------------------------------------------
void vtkFitsUnstructuredReader::SetOutput(vtkDataObject* d)
{
    this->GetExecutive()->SetOutputData(0, d);
}




//----------------------------------------------------------------------------
//------------------------------------------------------------------------------
int vtkFitsUnstructuredReader::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** vtkNotUsed(inputVector), vtkInformationVector* outputVector)
{
  // get the info object
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  // get the output
  vtkPolyData* output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  ReadPoints(output);


int num2=output->GetNumberOfPoints();
 std::cout<<"number of points inserted double check"<<num2<<std::endl;


  return 1;
}

void vtkFitsUnstructuredReader::ReadHeader() {



    fitsfile *fptr;       /* pointer to the FITS file, defined in fitsio.h */

    int status, nkeys, keypos, hdutype, ii, jj;
    char card[FLEN_CARD];   /* standard string lengths defined in fitsioc.h */
    
    
    char crval1[80];
    char crval2[80];
    char crval3[80];
    char crpix1[80];
    char crpix2[80];
    char crpix3[80];
    char cdelt1[80];
    char cdelt2[80];
    char cdelt3[80];
    char naxis1[80];
    char naxis2[80];
    char naxis3[80];
    
    
    crval1[0] ='\0';
    crval2[0] ='\0';
    crval3[0] ='\0';
    crpix1[0] ='\0';
    crpix2[0] ='\0';
    crpix3[0] ='\0';
    cdelt1[0] ='\0';
    cdelt2[0] ='\0';
    cdelt3[0] ='\0';
    
    std::string val1, val2, val3, pix1,pix2, pix3, delt1, delt2, delt3, nax1, nax2, nax3;

    status = 0;


    char *fn=new char[filename.length() + 1];;
    strcpy(fn, filename.c_str());

    if ( fits_open_file(&fptr, fn, READONLY, &status) )
        printerror( status );
    delete []fn;

    /* attempt to move to next HDU, until we get an EOF error */
    for (ii = 1; !(fits_movabs_hdu(fptr, ii, &hdutype, &status) ); ii++)
    {

        /* get no. of keywords */
        if (fits_get_hdrpos(fptr, &nkeys, &keypos, &status) )
            printerror( status );

        for (jj = 1; jj <= nkeys; jj++)  {

            if ( fits_read_record(fptr, jj, card, &status) )
                printerror( status );

            if (!strncmp(card, "CTYPE", 5)) {
                // cerr << card << endl;
                char *first = strchr(card, '\'');
                char *last = strrchr(card, '\'');

                *last = '\0';
                if (card[5] == '1')
                    strcpy(xStr, first+1);
                if (card[5] == '2')
                    strcpy(yStr, first+1);
                if (card[5] == '3')
                    strcpy(zStr, first+1);

                        }

            if (!strncmp(card, "OBJECT", 6)) {
                cerr << card << endl;
                char *first = strchr(card, '\'');
                char *last = strrchr(card, '\'');
                *last = '\0';
                strcpy(title, first+1);
            }

            if (!strncmp(card, "CRVAL", 5)) {
                char *first = strchr(card, '=');
                char *last = strrchr(card, '=');
                *last = '\0';

                // char *last = strrchr(card, '/');
                //*last = '\0';

                if (card[5] == '1')
                {
                    strcpy(crval1, first+1);
                    char *pch = strtok (crval1," ,");
                    strcpy(crval1, pch);
                    
                }
                
                if (card[5] == '2')
                {
                    strcpy(crval2, first+1);
                    char *pch = strtok (crval2," ,");
                    strcpy(crval2, pch);

                }
                
                if (card[5] == '3')
                {
                    strcpy(crval3, first+1);
                    char *pch = strtok (crval3," ,");
                    strcpy(crval3, pch);

                }
            }

            if (!strncmp(card, "CRPIX", 5)) {
                char *first = strchr(card, '=');
                char *last = strrchr(card, '=');
                *last = '\0';
                
                
                if (card[5] == '1')
                {
                    strcpy(crpix1, first+1);

                    char *pch = strtok (crpix1," ,");
                    strcpy(crpix1, pch);
                }
                
                if (card[5] == '2')
                {
                    strcpy(crpix2, first+1);

                    char *pch = strtok (crpix2," ,");
                    strcpy(crpix2, pch);
                }
                if (card[5] == '3')
                {
                    strcpy(crpix3, first+1);

                    char *pch = strtok (crpix3," ,");
                    strcpy(crpix3, pch);
                }
            }

            if (!strncmp(card, "CDELT", 5)) {
                char *first = strchr(card, '=');
                char *last = strrchr(card, '=');
                *last = '\0';
                
                if (card[5] == '1')
                {
                    strcpy(cdelt1, first+1);
                    char *pch = strtok (cdelt1," ,");
                    strcpy(cdelt1, pch);
                    
                }
                
                if (card[5] == '2')
                {
                    strcpy(cdelt2, first+1);
                    char *pch = strtok (cdelt2," ,");
                    strcpy(cdelt2, pch);
                }
                
                if (card[5] == '3')
                {
                    strcpy(cdelt3, first+1);
                    char *pch = strtok (cdelt3," ,");
                    strcpy(cdelt3, pch);
                }
            }
            
            

        }
    }


    val1=crval1;
    val2=crval2;
    val3=crval3;
    pix1=crpix1;
    pix2=crpix2;
    pix3=crpix3;
    delt1=cdelt1;
    delt2=cdelt2;
    delt3=cdelt3;


    
    crval[0]=::atof(val1.c_str());//val1.toDouble(); //problema
    crval[1]=::atof(val2.c_str());//val2.toDouble();
    crval[2]=::atof(val3.c_str());//val3.toDouble();
    cpix[0]=::atof(pix1.c_str());//pix1.toDouble();
    cpix[1]=::atof(pix2.c_str());//pix2.toDouble();
    cpix[2]=::atof(pix3.c_str());//pix3.toDouble();
    cdelt[0]=::atof(delt1.c_str());//delt1.toDouble();
    cdelt[1]=::atof(delt1.c_str());//delt2.toDouble();
    cdelt[2]=::atof(delt1.c_str());//delt3.toDouble();

    initSlice=crval[2]-(cdelt[2]*(cpix[2]-1));
    
    

    

    
}

// Note: from cookbook.c in fitsio distribution.
void vtkFitsUnstructuredReader::printerror(int status) {

    cerr << "vtkFitsUnstructuredReader ERROR.";
    if (status) {
        fits_report_error(stderr, status); /* print error report */
        exit( status );    /* terminate the program, returning error status */
    }
    return;
}


// Note: This function adapted from readimage() from cookbook.c in
// fitsio distribution.
void vtkFitsUnstructuredReader::ReadPoints( vtkPolyData *output){

 int num2=0;

 fitsfile *fptr;
 int status = 0, nfound = 0, anynull = 0;
 long fpixel, nbuffer, npixels, ii, n=0;
 double meansquare=0;
 const int buffsize = 1000;


 float nullval, buffer[buffsize];
 char *fn=new char[filename.length() + 1];
 strcpy(fn, filename.c_str());

 if ( fits_open_file(&fptr, fn, READONLY, &status) )
     printerror( status );

 delete []fn;
 vtkFloatArray *scalars = vtkFloatArray::New();
 if ( fits_read_keys_lng(fptr, "NAXIS", 1, 3, naxes, &nfound, &status) )
     printerror( status );

 npixels  = naxes[0] * naxes[1] * naxes[2];
 n=npixels;

 vtkPoints* newPoints;
 vtkCellArray* newVerts;

 newPoints = vtkPoints::New();

 // Set the desired precision for the points in the output.

   newPoints->SetDataType(VTK_FLOAT);


 newPoints->Allocate(npixels);
 newVerts = vtkCellArray::New();
 newVerts->AllocateEstimate(1, npixels);

 newVerts->InsertNextCell(npixels);

 fpixel   = 1;
 nullval  = 0;
 datamin  = 1.0E30;
 datamax  = -1.0E30;
 //output->SetDimensions(naxes[0], naxes[1], naxes[2]);
 //output->SetOrigin(0.0, 0.0, 0.0);

 scalars->Allocate(npixels);
 std::cout<<"dims "<< naxes[0]<<" "<< naxes[1]<<" "<< naxes[2]<<" "<<std::endl;

  std::cout<<"number of points expected "<<npixels<<std::endl;

  float fmax=std::max(naxes[0],naxes[1]);
  fmax=std::max(fmax,naxes[2]);

  float norm=fmax/100;

  float bmin[3]={0,0,0};
  float bmax[3]={naxes[0]/norm,naxes[1]/norm,naxes[2]/norm};
  float delta[3]={(bmax[0]-bmin[0])/naxes[0],(bmax[1]-bmin[1])/naxes[1],(bmax[2]-bmin[2])/naxes[2]};

      int i_index,j_index,k_index;
      i_index=0;
      j_index=0;
      k_index=0;
 //For every pixel
 while (npixels > 0) {


     nbuffer = npixels;
     if (npixels > buffsize)
         nbuffer = buffsize;

     if ( fits_read_img(fptr, TFLOAT, fpixel, nbuffer, &nullval,
                        buffer, &anynull, &status) )
         printerror( status );




 // std::cout<<"number  "<<rms<<std::endl;
     for (ii = 0; ii < nbuffer; ii++)  {
             if (!std::isnan(buffer[ii]))
         { //fill not empty array

                 float I= buffer[ii];
                 if(I-3*rms>=0) //check if still should skip
                 {

                 float val=I;
                 float r=val+0.01-3*rms;
                 int type=0;
                 bool active=true;
                 float p[3];
                 p[0]=bmin[0]+delta[0]*i_index;
                 p[1]=bmin[1]+delta[1]*j_index;
                 p[2]=bmin[2]+delta[2]*k_index;
                /* Flipping
                 *    if(i_index<naxes[0]/2)
                 {
                     p[0]=bmin[0]-delta[0]*(i_index);
                  }
                   else {
                       p[0]=bmax[0]-delta[0]*(i_index);
                   }
                 */

                   newVerts->InsertCellPoint(newPoints->InsertNextPoint(p[0],p[1],p[2]));

                 scalars->InsertNextValue(buffer[ii]);


             num2++;

                 //skip=true;
             }



         }

         //-----
         if(i_index<naxes[0]) i_index++;
         else {

                 if(j_index<naxes[1])
                 {
                     j_index++;
                     i_index=0;
                 }
                 else
                 {
                     if(k_index<naxes[2]) {
                         k_index++;
                         i_index=0;
                         j_index=0;
                        // std::cout<<"increase z="<<k_index<<std::endl;
                     } else
                     {
                         std::cout<<"left="<<npixels-nbuffer<<std::endl;
                     }
                  }

     }
         //---------




     }

     npixels -= nbuffer;
     fpixel  += nbuffer;
 }



 if ( fits_close_file(fptr, &status) )
     printerror( status );


 newPoints->Resize(num2);
 //newVerts->ResizeExact(num2,num2) ;
 output->SetPoints(newPoints);
 newPoints->Delete();

 output->SetVerts(newVerts);
 newVerts->Delete();
 output->GetPointData()->SetScalars(scalars);

 std::cout<<"number of points inserted "<<num2<<std::endl;
 //-----

}
void vtkFitsUnstructuredReader::CalculateRMS() {
    

    ReadHeader();
    
    //vtkPoints *output = (vtkPoints *) this->GetOutput();
    fitsfile *fptr;
    int status = 0, nfound = 0, anynull = 0;
    long fpixel, nbuffer, npixels, ii, n=0;
    double meansquare=0;
    const int buffsize = 1000;
    
    
    float nullval, buffer[buffsize];
    char *fn=new char[filename.length() + 1];
    strcpy(fn, filename.c_str());
    
    if ( fits_open_file(&fptr, fn, READONLY, &status) )
        printerror( status );
    
    delete []fn;
   // vtkFloatArray *scalars = vtkFloatArray::New();
    if ( fits_read_keys_lng(fptr, "NAXIS", 1, 3, naxes, &nfound, &status) )
        printerror( status );
    
    npixels  = naxes[0] * naxes[1] * naxes[2];
    n=npixels;
    
    fpixel   = 1;
    nullval  = 0;
    datamin  = 1.0E30;
    datamax  = -1.0E30;


    int bad=0;
    int slice;
    int num=0;



    minmaxslice=new float*[naxes[2]];
    for(int i=0;i< naxes[2];i++)
    {

        minmaxslice[i] = new float[2];

        minmaxslice[i][0]= 1.0E30;
        minmaxslice[i][1]= -1.0E30;

    }

    //For every pixel
    while (npixels > 0) {




        nbuffer = npixels;
        if (npixels > buffsize)
            nbuffer = buffsize;
        
        if ( fits_read_img(fptr, TFLOAT, fpixel, nbuffer, &nullval,
                           buffer, &anynull, &status) )
            printerror( status );


        for (ii = 0; ii < nbuffer; ii++)  {
            // slice= (num/(naxes[0]*naxes[1]))%(naxes[0]*naxes[1]);
            slice= (num/ (naxes[0]*naxes[1]) );
            num++;

            // qDebug()<<"npixel: "<<num <<" è sulla slice "<< slice <<" x: "<<naxes[0]<<" y: "<<naxes[1]<<" z: "<<naxes[2];

            if (std::isnan(buffer[ii]))
                buffer[ii] = -1000000.0;
            //scalars->InsertNextValue(buffer[ii]);

            if ( buffer[ii]!=-1000000.0)
            {
                if ( buffer[ii] < datamin )
                    datamin = buffer[ii];
                if ( buffer[ii] > datamax   )
                    datamax = buffer[ii];

                //qDebug()<<"poreeeee "<<slice;
                if ( buffer[ii] < minmaxslice[slice][0] )
                    minmaxslice[slice][0] = buffer[ii];
                if ( buffer[ii] > minmaxslice[slice][1]   )
                    minmaxslice[slice][1] = buffer[ii];

                //meansquare+=buffer[ii]*buffer[ii];
                //  media+=buffer[ii];
                meansquare+=buffer[ii]*buffer[ii];

            }
            else
                bad++;
        }
        
        npixels -= nbuffer;
        fpixel  += nbuffer;
    }

    n=n-bad;
    double means=meansquare/n;
    rms=sqrt(means);
    // sigma=qSqrt(sigma/n);

    if ( fits_close_file(fptr, &status) )
        printerror( status );
    
   // output->GetPointData()->SetScalars(scalars);

    return;
}
int vtkFitsUnstructuredReader::GetNaxes(int i)
{

    return naxes[i];

}
float* vtkFitsUnstructuredReader::GetRangeSlice(int i)
{

    return minmaxslice[i];

}
