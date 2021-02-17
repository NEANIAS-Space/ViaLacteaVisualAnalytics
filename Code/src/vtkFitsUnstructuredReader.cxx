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
#include <algorithm>
#include <curl/curl.h>


template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    assert( !(hi < lo) );
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

//vtkCxxRevisionMacro(vtkFitsUnstructuredReader, "$Revision: 1.1 $");
vtkStandardNewMacro(vtkFitsUnstructuredReader);

size_t  vtkFitsUnstructuredReader::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
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


    for (int i=0; i<3; i++)
    {
        crval[i]=0;
        cpix[i]=0;
        cdelt[i]=0;
        naxes[i]= 10;
    }
    

    this->SetNumberOfInputPorts(0);
    m_fileToDownload="temp.fits";


}

void vtkFitsUnstructuredReader::GenerateVLKBUrl(std::string point,std::string radius)
{
    std::vector<std::string> strings;
        std::istringstream fpoint(point);
        std::string s;
        float p[2];
        getline(fpoint, s, ',');
        p[0]=::atof(s.c_str()); //l_lineEdit
        getline(fpoint, s, ',');
        p[1]=::atof(s.c_str()); //b_lineEdit

           // ui->glatLineEdit->setText(QString::number( pieces[1].toDouble(), 'f', 4 ));
           // ui->glonLineEdit->setText(QString::number( pieces[0].toDouble(), 'f', 4 ));

        std::istringstream fradius(radius);
        float r[2];
        getline(fradius, s, ',');
        r[0]=::atof(s.c_str()); //dlLineEdit
        getline(fradius, s, ',');
        r[1]=::atof(s.c_str()); //dbLineEdit
        bool isRadius=false;
        r[0]=clamp<float>(r[0],0.0,4.0);
        r[1]=clamp<float>(r[1],0.0,4.0);


        //settings.setValue("vlkburl","http://ia2-vialactea.oats.inaf.it/libjnifitsdb-1.0.2p/");

        std::string vlkbUrl="http://ia2-vialactea.oats.inaf.it/libjnifitsdb-1.0.2p/"; //I guess it should be different, with some fille added to it
        std::string urlString=vlkbUrl+"/vlkb_search?l="+std::to_string(p[0])+"&b="+std::to_string(p[1]);//+"&species="+species;
            if(isRadius)
            {
                //urlString+="&r="+ui->r_lineEdit->text();
            }
            else
                urlString+="&dl="+std::to_string(r[0])+"&db="+std::to_string(r[1]);

            urlString+="&vl=-500000&vu=500000";

        /*
         * Download from url
         *  vlkbUrl= settings.value("vlkburl", "").toString();
         *  QString urlString=vlkbUrl+"/vlkb_search?l="+ui->l_lineEdit->text()+"&b="+ui->b_lineEdit->text();//+"&species="+species;
    if(isRadius)
    {
        urlString+="&r="+ui->r_lineEdit->text();
    }
    else
        urlString+="&dl="+ui->dlLineEdit->text()+"&db="+ui->dbLineEdit->text();

    urlString+="&vl=-500000&vu=500000";

    QUrl url2 (urlString);

    nam->get(QNetworkRequest(url2));
         */


            //As far just download from url and save as fits just an xml file
            //TODO preprocess xml instead

        DownloadFromUrl(urlString);
        std::cout<<"URL "<<urlString<<std::endl;

       // after void VialacteaInitialQuery::finishedSlot(QNetworkReply* reply)
       //         performs downloading from url and xml preprocessing

        //OTHERS

        //Query example
        /*
         * vq= new VialacteaInitialQuery(ui->fileNameLineEdit->text());
         * vq= new VialacteaInitialQuery();

    vq->setL(ui->glonLineEdit->text()); //i->l_lineEdit->setText(l);
    vq->setB(ui->glatLineEdit->text()); //ui->b_lineEdit->setText(b.replace(" ",""));
    if (ui->radiumLineEdit->text()!="")
        vq->setR(ui->radiumLineEdit->text());
        //isRadius=true;
        // ui->r_lineEdit->setText(r);
    else
    {
        vq->setDeltaRect(ui->dlLineEdit->text(),ui->dbLineEdit->text());
        //isRadius=false;
      //ui->dlLineEdit->setText(dl);
    //ui->dbLineEdit->setText(db);

    }

    QList < QPair<QString, QString> > selectedSurvey;

    QList<QCheckBox *> allButtons = ui->surveySelectorGroupBox->findChildren<QCheckBox *>();
    for(int i = 0; i < allButtons.size(); ++i)
    {
        qDebug()<<"i: "<<i<<" "<<allButtons.at(i);

        if(allButtons.at(i)->isChecked())
        {


            selectedSurvey.append(mapSurvey.value(i));
        }
    }

    //connettere la banda selezionata
    vq->setSpecies("Continuum");
    vq->setSurveyname(selectedSurvey.at(0).first);
    vq->setTransition(selectedSurvey.at(0).second);
    vq->setSelectedSurveyMap(selectedSurvey);
    vq->on_queryPushButton_clicked();
         */

        /*
         * Access settings
         * QSettings settings(m_sSettingsFile, QSettings::NativeFormat);

    if (settings.value("vlkbtype", "public").toString()=="public")
    {
        qDebug()<<"public access to vlkb";
        settings.setValue("vlkburl","http://ia2-vialactea.oats.inaf.it/libjnifitsdb-1.0.2p/");
        settings.setValue("vlkbtableurl","http://ia2-vialactea.oats.inaf.it/vlkb/catalogues/tap");



    }
    else if (settings.value("vlkbtype", "public").toString()=="private")
    {
        qDebug()<<"private access to vlkb";


        QString user= settings.value("vlkbuser", "").toString();
        QString pass = settings.value("vlkbpass", "").toString();


       // settings.setValue("vlkburl","http://"+user+":"+pass+"@ia2-vialactea.oats.inaf.it:8080/libjnifitsdb-0.23.2/");
      //  settings.setValue("vlkburl","http://"+user+":"+pass+"@ia2-vialactea.oats.inaf.it:8080/libjnifitsdb-0.23.16/");
        settings.setValue("vlkburl","http://"+user+":"+pass+"@ia2-vialactea.oats.inaf.it:8080/libjnifitsdb-1.0.2/");
        settings.setValue("vlkbtableurl","http://ia2-vialactea.oats.inaf.it:8080/vlkb");


    }

    if (settings.value("online",true) == true)
    {
        tilePath = settings.value("onlinetilepath", "http://visivo.oact.inaf.it/vialacteatiles/openlayers.html").toString();
        ui->webView->load(QUrl(tilePath));

    }
    else
    {
       tilePath = settings.value("tilepath", "").toString();
       ui->webView->load(QUrl::fromLocalFile(tilePath));

    }
         */





}
void vtkFitsUnstructuredReader::DownloadFile(std::string url,std::string outName)
{
CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(outName.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        /* always cleanup */
        curl_easy_cleanup(curl);
        fclose(fp);
    }
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

    //Calculating contour value to further through away the points
                 std::vector<VALS> vals;
                  CalculateRMS(vals);

                int num2=0; //number of particles





 vtkPoints* newPoints;
 vtkCellArray* newVerts;

 int npixels=vals.size();

 newPoints = vtkPoints::New();

 // Set the desired precision for the points in the output.

   newPoints->SetDataType(VTK_FLOAT);


 newPoints->Allocate(npixels);
 newVerts = vtkCellArray::New();
 newVerts->AllocateEstimate(1, npixels);

 newVerts->InsertNextCell(npixels);

 vtkFloatArray *scalars = vtkFloatArray::New();
 scalars->Allocate(npixels);


  std::cout<<"number of points expected "<<npixels<<std::endl;


  float point[3];
  float p[3];
  float bmin[3]={0,0,0};
  float norm=fmax(naxes[0],naxes[1]);
  norm=fmax(norm,naxes[1])/100;

  float bmax[3]={naxes[0]/norm,naxes[1]/norm,naxes[2]/norm};
  float delta[3]={(bmax[0]-bmin[0])/naxes[0],(bmax[1]-bmin[1])/naxes[1],(bmax[2]-bmin[2])/naxes[2]};

  float normilise=naxes[0]/naxes[2];
      int i,j,k;
      i=0;
      j=0;
      k=0;


      //For every pixel
      for (std::vector<VALS>::iterator it = vals.begin() ; it != vals.end(); ++it)

      {


          float I= it->val;

          if((I>3*rms))//&&(I<=(7*rms))) //check if still should skip
           {
              float approx=float(it->ii)/float(naxes[1]*naxes[0]);
              k=int(floor(approx));
              float rest=approx-k;

              if (rest>0)
              {
                  int ii2=it->ii-naxes[1]*naxes[0]*k;
                  approx=float(ii2)/float(naxes[0]);
                  j=int(floor(approx));
                  if (approx-float(j)>0){
                      i=ii2-naxes[0]*j;

                  } else i=0;
              }
              else {
                  j=0;
                  i=0;
              }

              p[0]=bmin[0]+delta[0]*i;
              p[1]=bmin[1]+delta[1]*j;
              p[2]=bmin[2]+delta[2]*k;

                   newVerts->InsertCellPoint(newPoints->InsertNextPoint(p[0],p[1],p[2]));

                 scalars->InsertNextValue(I);


             num2++;
          }





     }



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
void vtkFitsUnstructuredReader::CalculateRMS(std::vector<VALS>& vals) {
    
    ReadHeader();

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

    if ( fits_read_keys_lng(fptr, "NAXIS", 1, 3, naxes, &nfound, &status) )
        printerror( status );

    npixels  = naxes[0] * naxes[1] * naxes[2];
    n=npixels;

    fpixel   = 1;
    nullval  = 0;
    datamin  = 1.0E30;
    datamax  = -1.0E30;

    int bad=0;



    //For every pixel

    int num2=0;

    while (npixels > 0) {




        nbuffer = npixels;
        if (npixels > buffsize)
            nbuffer = buffsize;

        if ( fits_read_img(fptr, TFLOAT, fpixel, nbuffer, &nullval,
                           buffer, &anynull, &status) )
            printerror( status );



        for (ii = 0; ii < nbuffer; ii++)  {

            if (!std::isnan(buffer[ii])&&(buffer[ii]>=0.0f))
            {


                if ( buffer[ii] < datamin )
                    datamin = buffer[ii];
                if ( buffer[ii] > datamax   )
                    datamax = buffer[ii];

                meansquare+=buffer[ii]*buffer[ii];
                vals.push_back(VALS(num2,buffer[ii]));


            }
            else{
                bad++;
            }
            num2++;
        }
        npixels -= nbuffer;
        fpixel  += nbuffer;
    }

    n=n-bad;
    printf("pixesl=%d\n",n);
    double means=meansquare/n;
    rms=sqrt(means);


    if ( fits_close_file(fptr, &status) )
        printerror( status );



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
