
#ifndef VTKWEBCORE_EXPORT_H
#define VTKWEBCORE_EXPORT_H

#ifdef VTKWEBCORE_STATIC_DEFINE
#  define VTKWEBCORE_EXPORT
#  define VTKWEBCORE_NO_EXPORT
#else
#  ifndef VTKWEBCORE_EXPORT
#    ifdef WebCore_EXPORTS
        /* We are building this library */
#      define VTKWEBCORE_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define VTKWEBCORE_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef VTKWEBCORE_NO_EXPORT
#    define VTKWEBCORE_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef VTKWEBCORE_DEPRECATED
#  define VTKWEBCORE_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef VTKWEBCORE_DEPRECATED_EXPORT
#  define VTKWEBCORE_DEPRECATED_EXPORT VTKWEBCORE_EXPORT VTKWEBCORE_DEPRECATED
#endif

#ifndef VTKWEBCORE_DEPRECATED_NO_EXPORT
#  define VTKWEBCORE_DEPRECATED_NO_EXPORT VTKWEBCORE_NO_EXPORT VTKWEBCORE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VTKWEBCORE_NO_DEPRECATED
#    define VTKWEBCORE_NO_DEPRECATED
#  endif
#endif

#endif /* VTKWEBCORE_EXPORT_H */
