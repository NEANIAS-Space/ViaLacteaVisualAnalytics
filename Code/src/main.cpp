
#include "vtkInteractorStyleDrawPolygon.h"
#include "vtkNew.h"
#include "vtkHardwareSelector.h"
#include "vtkSelection.h"
#include "vtkCollection.h"
#include "vtkExtractSelection.h"
#include "vtkUnstructuredGrid.h"
#include "vtkGeometryFilter.h"

#include "vtkObjectFactory.h"
#include "vtkInteractorStyleRubberBandPick.h"
#include "vtkAreaPicker.h"
#include "vtkAbstractPicker.h"
#include "vtkExtractGeometry.h"
#include "vtkInteractorStyleRubberBand2D.h"
#include "vtkVertexGlyphFilter.h"
#include "vtkInteractorStyleImage.h"
#include "vtkCellPicker.h"
#include "vtkCubeAxesActor2D.h"
#include "vtkTransform.h"
#include "vtkImageShiftScale.h"
#include "vtkCornerAnnotation.h"
#include "vtkfitstoolwidget_new.h"
#include "vtkfitstoolwidgetobject.h"
#include <vtkAutoInit.h>
#include "vtkPolyLine.h"
#include "vtkContourFilter.h"
#include "vtklegendscaleactor.h"
#include "vtkAxisActor2D.h"
#include "vtkCubeAxesActor.h"
#include "vtkextracthistogram.h"
#include "vtkDoubleArray.h"
#include "vtkTable.h"
#include <vtkGlyph2D.h>
#include "vtkRegularPolygonSource.h"

#include "vtkImageBlend.h"
#include "vtkDataSetMapper.h"
#include "vtkProperty2D.h"

#include "vtkImageSliceMapper.h"
#include "vtkImageProperty.h"
#include "vtkImageStack.h"
#include "vtkProp3D.h"
#include "vtkImageChangeInformation.h"
#include "vtkImageResize.h"
#include "vtkImageSliceCollection.h"
#include "vtkGenericOpenGLRenderWindow.h"


#include "vtkAssembly.h"

#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include <vtkSmartPointer.h>
#include <vtkCallbackCommand.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMath.h>
#include <vtkRenderWindow.h>
#include <vtkTimerLog.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkOutlineFilter.h>
#include <vtkMarchingCubes.h>
#include <vtkProperty.h>
#include <vtkFrustumSource.h>
#include <vtkPlanes.h>
#include <vtkPlane.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include "extendedglyph3d.h"

void CallbackFunction(vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* vtkNotUsed(clientData), void* vtkNotUsed(callData) )
{
  vtkRenderer* renderer = static_cast<vtkRenderer*>(caller);

  double timeInSeconds = renderer->GetLastRenderTimeInSeconds();
  double fps = 1.0/timeInSeconds;
  std::cout << "FPS: " << fps << std::endl;

  std::cout << "Callback" << std::endl;
}


int main(int argc, char *argv[])
{

    vtkSmartPointer<vtkFitsReader> fitsReader = vtkSmartPointer<vtkFitsReader>::New();
    fitsReader->is3D=true;
    fitsReader->SetFileName("LVHIS027.na.icln.fits");//LVHIS001.na.icln.fits");
    //fitsReader->Update();

    vtkStructuredPoints* myfits=fitsReader->GetOutput();

    /*
     *  //setto specie e transition
    imageObject->setSpecies(vis->getSpecies());
    imageObject->setSurvey(vis->getSurvey().replace("%20"," "));
    imageObject->setTransition(vis->getTransition());

    */

    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();

   int vis_style=2;

    switch(vis_style)
    {
    case 0:
    {
        //lut
        double* range = myfits->GetScalarRange();

        vtkSmartPointer<vtkImageShiftScale> resultScale = vtkSmartPointer<vtkImageShiftScale>::New();
        resultScale->SetOutputScalarTypeToUnsignedChar();
        resultScale->SetInputData( myfits );

        resultScale->Update();

        vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
        lut->SetScaleToLog10();
        float min=fitsReader->GetMin();
        if ( min <= 0 )
            min=1;
        lut->SetTableRange( min, fitsReader->GetMax() );

        vtkSmartPointer<vtkImageMapToColors> colors =  vtkSmartPointer<vtkImageMapToColors>::New();
        colors->SetInputData(myfits);
        colors->SetLookupTable(lut);
        colors->Update();

        vtkSmartPointer<vtkImageSliceMapper> imageSliceMapperBase = vtkSmartPointer<vtkImageSliceMapper>::New();

        imageSliceMapperBase->SetInputData(colors->GetOutput());


        vtkSmartPointer<vtkImageSlice> imageSliceBase = vtkSmartPointer<vtkImageSlice>::New();
        imageSliceBase->SetMapper(imageSliceMapperBase);
        imageSliceBase->GetProperty()->SetInterpolationTypeToNearest();

        imageSliceBase->GetProperty()->SetLayerNumber(0);


        // Stack
        vtkSmartPointer<vtkImageStack> imageStack = vtkSmartPointer<vtkImageStack>::New();
        imageStack->AddImage(imageSliceBase);


      /* TODO: clean from qt dependencies
         vtkSmartPointer<vtkLegendScaleActor> legendScaleActorImage =  vtkSmartPointer<vtkLegendScaleActor>::New();
        legendScaleActorImage->LegendVisibilityOff();
        legendScaleActorImage->setFitsFile(fitsReader);
    */

        //renderer->AddActor(legendScaleActorImage);
        renderer->AddViewProp(imageStack);
        renderer->ResetCamera();
        break;
    }

    case 1:
    {
         fitsReader->CalculateRMS();

         float max=fitsReader->GetMax();
         float min=fitsReader->GetMin();
         int naxis3=fitsReader->GetNaxes(2);

         // outline
         vtkOutlineFilter *outlineF = vtkOutlineFilter::New();
         outlineF->SetInputData(myfits);



         vtkPolyDataMapper *outlineM = vtkPolyDataMapper::New();
         outlineM->SetInputConnection(outlineF->GetOutputPort());
         outlineM->ScalarVisibilityOff();

         vtkActor *outlineA = vtkActor::New();
         outlineA->SetMapper(outlineM);

         // isosurface
         vtkMarchingCubes*shellE = vtkMarchingCubes::New();
         shellE->SetInputData(myfits);
         shellE->ComputeNormalsOn();

         shellE->SetValue(0, 3*fitsReader->GetRMS());

         vtkPolyDataMapper *shellM = vtkPolyDataMapper::New();
         shellM->SetInputConnection(shellE->GetOutputPort());
         shellM->ScalarVisibilityOff();

         vtkActor *shellA = vtkActor::New();
         shellA->SetMapper(shellM);
         shellA->GetProperty()->SetColor(1.0, 0.5, 1.0);

         vtkPlanes *sliceE= vtkPlanes::New();
         sliceE->SetBounds(myfits->GetBounds()[0], myfits->GetBounds()[1], myfits->GetBounds()[2], myfits->GetBounds()[3], 0, 1);

         vtkSmartPointer<vtkFrustumSource> frustumSource = vtkSmartPointer<vtkFrustumSource>::New();
         frustumSource->ShowLinesOff();
         frustumSource->SetPlanes(sliceE);
         frustumSource->Update();

         vtkPolyData* frustum = frustumSource->GetOutput();

         vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

         mapper->SetInputData(frustum);

         vtkSmartPointer<vtkActor> actor =
           vtkSmartPointer<vtkActor>::New();
         actor->SetMapper(mapper);

         renderer->AddActor(actor);
         renderer->AddActor(outlineA);
         renderer->AddActor(shellA);

        break;
    }

    case 2:
    {
         fitsReader->CalculateRMS();

         //lut
         double* range = myfits->GetScalarRange();

        /* vtkSmartPointer<vtkImageShiftScale> resultScale = vtkSmartPointer<vtkImageShiftScale>::New();
         resultScale->SetOutputScalarTypeToUnsignedChar();
         resultScale->SetInputData( myfits );

         resultScale->Update();

         vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
         lut->SetScaleToLinear();
         float min=fitsReader->GetMin();
         //if ( min <= 0 )
         //    min=1;
         lut->SetTableRange( min, fitsReader->GetMax() );

         vtkSmartPointer<vtkImageMapToColors> colors =  vtkSmartPointer<vtkImageMapToColors>::New();
         colors->SetInputData(myfits);
         colors->SetLookupTable(lut);
         colors->Update();*/

float min=fitsReader->GetMin();
float max=fitsReader->GetMax();
float err=fitsReader->GetRMS();

float cont=2*err;
         vtkPiecewiseFunction *opacityTransferFunction=vtkPiecewiseFunction::New();
          opacityTransferFunction->AddPoint(min-err,0.0);
 opacityTransferFunction->AddPoint(cont,0.0);
  opacityTransferFunction->AddPoint(cont+err,0.1);
  //opacityTransferFunction->AddPoint(4*err,0.1);
  opacityTransferFunction->AddPoint(max,0.3);
opacityTransferFunction->AddPoint(max+err,0.0);       //should it be (1,0.2) or (255, 0.2)? Shouldnt it be like table->SetValueRange(0,1)?

      //Color the image from black to white, does anybody has some good color functions for me??
  vtkColorTransferFunction *colorTransferFunction = vtkColorTransferFunction::New();
 colorTransferFunction->AddRGBPoint(min,0.0,0.0,0.0);
 colorTransferFunction->AddRGBPoint(cont+err,1.0,0.0,0.0);
 colorTransferFunction->AddRGBPoint((cont+err+max)/2,1.0,1.0,0.0);
 colorTransferFunction->AddRGBPoint(max,0.0,1.0,0.0);
 colorTransferFunction->AddRGBPoint(max+err,0.0,0.0,0.0);


 vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New();
       volumeProperty->SetColor(colorTransferFunction);
       volumeProperty->SetScalarOpacity(opacityTransferFunction);
       volumeProperty->SetInterpolationTypeToLinear();



        vtkGPUVolumeRayCastMapper *volumeMapper=vtkGPUVolumeRayCastMapper::New();
        volumeMapper->SetInputData(myfits);//colors->GetOutputPort());

          vtkVolume *volume=vtkVolume::New();
         volume->SetMapper(volumeMapper);
          volume->SetProperty(volumeProperty);

          renderer->AddVolume(volume);


          /*

         // outline
         vtkOutlineFilter *outlineF = vtkOutlineFilter::New();
         outlineF->SetInputData(myfits);



         vtkPolyDataMapper *outlineM = vtkPolyDataMapper::New();
         outlineM->SetInputConnection(outlineF->GetOutputPort());
         outlineM->ScalarVisibilityOff();

         vtkActor *outlineA = vtkActor::New();
         outlineA->SetMapper(outlineM);

         // isosurface
         vtkMarchingCubes*shellE = vtkMarchingCubes::New();
         shellE->SetInputData(myfits);
         shellE->ComputeNormalsOn();

         shellE->SetValue(0, 3*fitsReader->GetRMS());

         vtkPolyDataMapper *shellM = vtkPolyDataMapper::New();
         shellM->SetInputConnection(shellE->GetOutputPort());
         shellM->ScalarVisibilityOff();

         vtkActor *shellA = vtkActor::New();
         shellA->SetMapper(shellM);
         shellA->GetProperty()->SetColor(1.0, 0.5, 1.0);

         vtkPlanes *sliceE= vtkPlanes::New();
         sliceE->SetBounds(myfits->GetBounds()[0], myfits->GetBounds()[1], myfits->GetBounds()[2], myfits->GetBounds()[3], 0, 1);

         vtkSmartPointer<vtkFrustumSource> frustumSource = vtkSmartPointer<vtkFrustumSource>::New();
         frustumSource->ShowLinesOff();
         frustumSource->SetPlanes(sliceE);
         frustumSource->Update();

         vtkPolyData* frustum = frustumSource->GetOutput();

         vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

         mapper->SetInputData(frustum);

         vtkSmartPointer<vtkActor> actor =
           vtkSmartPointer<vtkActor>::New();
         actor->SetMapper(mapper);

         renderer->AddActor(actor);
         renderer->AddActor(outlineA);
         renderer->AddActor(shellA);
*/
        break;
    }

    }



      vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
      renderWindow->AddRenderer(renderer);

      vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
      renderWindowInteractor->SetRenderWindow(renderWindow);

      vtkSmartPointer<vtkCallbackCommand> callback =
        vtkSmartPointer<vtkCallbackCommand>::New();

      callback->SetCallback(CallbackFunction);
      renderer->AddObserver(vtkCommand::EndEvent, callback);

      renderer->SetBackground(.3, .6, .3); // Background color green

      // Render and interact
      renderWindow->Render();
      renderWindowInteractor->Start();
}
