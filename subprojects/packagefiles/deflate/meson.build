project('deflate', 'c', default_options: ['buildtype=release', 'warning_level=3'])


ctoolbox = dependency('', fallback : ['ctoolbox', 'lib'])

projectsources = [
  'main.c'
]

projectsources = []
externalobjs   = []

base = 'src'
projectsources += [
  base / 'inflator.c',
  base / 'deflator.c',
  base / 'zstrm.c'
]

l = static_library('deflate', sources: projectsources, objects: externalobjs, dependencies: [ctoolbox])
i = include_directories('.')
lib = declare_dependency(link_with: l, include_directories: i)
