r"""
    This module is a VTK Web server application.
    The following command line illustrates how to use it::

        $ vtkpython .../vtk_server.py

    Any VTK Web executable script comes with a set of standard arguments that
    can be overriden if need be::
        --host localhost
             Interface on which the HTTP server will listen.

        --port 8080
             Port number on which the HTTP server will listen.

        --content /path-to-web-content/
             Directory that you want to serve as static web content.
             By default, this variable is empty which means that we rely on another server
             to deliver the static content and the current process only focuses on the
             WebSocket connectivity of clients.

        --authKey wslink-secret
             Secret key that should be provided by the client to allow it to make any
             WebSocket communication. The client will assume if none is given that the
             server expects "wslink-secret" as the secret key.
"""

# import to process args
import sys
import os

# import vtk modules.
import vtk
from vtk.web import protocols
from vtk.web import wslink as vtk_wslink
from wslink import server

try:
    import argparse

except ImportError:
    # since  Python 2.6 and earlier don't have argparse, we simply provide
    # the source for the same as _argparse and we use it instead.
    from vtk.util import _argparse as argparse

import vtk.vtkVialactea
# =============================================================================
# Create custom ServerProtocol class to handle clients requests
# =============================================================================

class _WebCone(vtk_wslink.ServerProtocol):

    # Application configuration
    view    = None
    authKey = "wslink-secret"

    def initialize(self):
        global renderer, renderWindow, renderWindowInteractor, cone, mapper, actor

        # Bring used components
        self.registerVtkWebProtocol(protocols.vtkWebMouseHandler())
        self.registerVtkWebProtocol(protocols.vtkWebViewPort())
        self.registerVtkWebProtocol(protocols.vtkWebViewPortImageDelivery())
        self.registerVtkWebProtocol(protocols.vtkWebViewPortGeometryDelivery())

        # Update authentication key to use
        self.updateSecret(_WebCone.authKey)

        # Create default pipeline (Only once for all the session)
        if not _WebCone.view:
            # VTK specific code
            renderer = vtk.vtkRenderer()
            renderWindow = vtk.vtkRenderWindow()
            renderWindow.AddRenderer(renderer)

            renderWindowInteractor = vtk.vtkRenderWindowInteractor()
            renderWindowInteractor.SetRenderWindow(renderWindow)
            renderWindowInteractor.GetInteractorStyle().SetCurrentStyleToTrackballCamera()


            fitsReader = vtk.vtkVialactea.vtkFitsUnstructuredReader()
            fitsReader.is3D=True;
            fitsReader.SetFileName("LVHIS027.na.icln.fits");

            fitsReader.CalculateRMS();
            fitsReader.Update();

            max=fitsReader.GetMax();
            min=fitsReader.GetMin();

            polydata=fitsReader.GetOutput();



            popSplatter = vtk.vtkGaussianSplatter()
            popSplatter.SetInputData(polydata)
            popSplatter.SetSampleDimensions(100, 100, 100)
            popSplatter.SetRadius(0.01)
            popSplatter.ScalarWarpingOff()
            popSplatter.Update()


            shellE = vtk.vtkMarchingCubes();
            shellE.SetInputData(popSplatter.GetOutput())
            shellE.ComputeNormalsOff();
            shellE.SetValue(0, 0.01)#3*fitsReader.GetRMS())


            mapper = vtk.vtkPolyDataMapper()
            actor = vtk.vtkActor()

            mapper.SetInputConnection(shellE.GetOutputPort())
            actor.SetMapper(mapper)

            renderer.AddActor(actor)
            renderer.ResetCamera()
            renderWindow.Render()

            # VTK Web application specific
            _WebCone.view = renderWindow
            self.getApplication().GetObjectIdMap().SetActiveObject("VIEW", renderWindow)

# =============================================================================
# Main: Parse args and start server
# =============================================================================

if __name__ == "__main__":
    # Create argument parser
    parser = argparse.ArgumentParser(description="VTK/Web Cone web-application")

    # Add default arguments
    server.add_arguments(parser)

    # Extract arguments
    args = parser.parse_args()

    # Configure our current application
    _WebCone.authKey = args.authKey

    # Start server
    server.start_webserver(options=args, protocol=_WebCone)
