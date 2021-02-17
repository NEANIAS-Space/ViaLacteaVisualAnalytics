
#ifndef VTKCOMMONPYTHON_EXPORT_H
#define VTKCOMMONPYTHON_EXPORT_H

#ifdef VTKCOMMONPYTHON_STATIC_DEFINE
#  define VTKCOMMONPYTHON_EXPORT
#  define VTKCOMMONPYTHON_NO_EXPORT
#else
#  ifndef VTKCOMMONPYTHON_EXPORT
#    ifdef CommonPython_EXPORTS
        /* We are building this library */
#      define VTKCOMMONPYTHON_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VTKCOMMONPYTHON_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VTKCOMMONPYTHON_NO_EXPORT
#    define VTKCOMMONPYTHON_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VTKCOMMONPYTHON_DEPRECATED
#  define VTKCOMMONPYTHON_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef VTKCOMMONPYTHON_DEPRECATED_EXPORT
#  define VTKCOMMONPYTHON_DEPRECATED_EXPORT VTKCOMMONPYTHON_EXPORT VTKCOMMONPYTHON_DEPRECATED
#endif

#ifndef VTKCOMMONPYTHON_DEPRECATED_NO_EXPORT
#  define VTKCOMMONPYTHON_DEPRECATED_NO_EXPORT VTKCOMMONPYTHON_NO_EXPORT VTKCOMMONPYTHON_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKCOMMONPYTHON_NO_DEPRECATED
#    define VTKCOMMONPYTHON_NO_DEPRECATED
#  endif
#endif

#endif /* VTKCOMMONPYTHON_EXPORT_H */
