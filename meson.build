project('jimage-test', 'c', default_options: ['buildtype=release', 'warning_level=3'])


deps = []
deps += [dependency('', fallback : ['ctoolbox', 'lib'])]
deps += [dependency('', fallback : [  'jimage', 'lib'])]

cc = meson.get_compiler('c')
if cc.get_id() == 'msvc'
  add_project_arguments('-D__MSVC__', language: 'c')
endif


jpgrsources = files([
  'jpg2tga.c'
])
pngrsources = files([
  'png2tga.c'
])

executable('jpg2tga', sources: jpgrsources, dependencies: deps)
executable('png2tga', sources: pngrsources, dependencies: deps)
