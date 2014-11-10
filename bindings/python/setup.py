#!/usr/bin/env python
# Copyright (c) 2014 Yandex LLC. All rights reserved.

from distutils.core import setup, Extension

__name__ = 'python-handystats'
__copyright__ = 'Copyright (c) 2014 Yandex LLC. All rights reserved.'
__license__ = 'LGPLv3+'
__version__ = '2.0.0'
__maintainer__ = 'Danil Osherov'
__maintainer_email__ = 'shindo@yandex-team.ru'
__url__ = 'https://github.com/shindo/handystats'
__description__ = 'Python bindings for handystats library'
__ext_modules__ = [
  Extension(module,
    sources=[source],
    language="c++",
    libraries=[
      "handystats"
    ],
    extra_compile_args=[
      "-std=c++0x",
      "-Wreorder",
      "-Wreturn-type",
      "-Wunused-variable",
      "-pedantic",
    ],
  )
  for module, source in [
    ["handystats.statistics", "handystats/statistics.cpp"],
    ["handystats.chrono", "handystats/chrono.cpp"],
  ]
]

setup(
  name = __name__,
  license = __license__,
  version = __version__,
  maintainer = __maintainer__,
  maintainer_email = __maintainer_email__,
  url = __url__,
  description = __description__,
  ext_modules = __ext_modules__,
  packages=[
      "handystats",
  ],
  classifiers=[
    'Development Status :: 4 - Beta',
    'Intended Audience :: Developers',
    'Programming Language :: Python',
    'Programming Language :: Python :: 2.6',
    'Programming Language :: Python :: 2.7',
    'Programming Language :: Python :: Implementation :: CPython',
    'Operating System :: OS Independent',
    'Topic :: Software Development :: Libraries :: Python Modules',
    'License :: OSI Approved :: GNU Lesser General Public License v3 or later (LGPLv3+)',
  ],
  platforms=[
    'Independent'
  ],
)
