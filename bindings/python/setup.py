from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize


include_dirs=['../../include', '.']

library_dirs=['.']

extra_compile_args=["-std=c++0x", "-Wreorder", "-Wreturn-type",
                "-Wunused-variable", "-pedantic",
                "-D_GLIBCXX_USE_NANOSLEEP",
                "-D_GLIBCXX_USE_CLOCK_MONOTONIC",
                "-D_GLIBCXX_USE_SCHED_YIEL",]

extra_link_args=["-L."]

ext_modules = [
        Extension(module, [source],
            include_dirs=include_dirs,
            library_dirs=library_dirs,
            libraries=["handystats"],
            language='c++',
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args,
        )
        for module, source in [
            ["handystats.statistics", "handystats/statistics.pyx"],
            ["handystats.chrono", "handystats/chrono.pyx"],
        ]
    ]

setup(
    name = 'python-handystats',
    cmdclass = {'build_ext': build_ext},
    ext_modules = cythonize(ext_modules),
)
