project('image', 'c', default_options: ['buildtype=release', 'warning_level=3'])


deps = []
deps += [dependency('', fallback : ['ctoolbox', 'lib'])]
deps += [dependency('', fallback : [   'image', 'lib'])]


jpgrsources = files([
  'jpg2tga.c'
])
pngrsources = files([
  'png2tga.c'
])

executable('jpg2tga', sources: jpgrsources, dependencies: deps)
executable('png2tga', sources: pngrsources, dependencies: deps)
