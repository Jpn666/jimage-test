project('image', 'c', default_options: ['buildtype=release', 'warning_level=3'])


deps = []
deps += [dependency('', fallback : ['ctoolbox', 'lib'])]
deps += [dependency('', fallback : [ 'deflate', 'lib'])]


target = target_machine.cpu_family()

externalasm = false
if target == 'x86_64'
  if target_machine.system() == 'windows'
    externalasm = true
  else
    if target_machine.system() != 'haiku'
      externalasm = true
    endif
  endif
endif

externalobjs = []
if externalasm
  add_project_arguments('-DJPGR_CFG_EXTERNALASM', language: 'c')
  add_project_arguments('-DPNGR_CFG_EXTERNALASM', language: 'c')
  base = 'src/asm'
  if target_machine.system() == 'windows'
    externalobjs += [base / 'windows/jpgreaderx64.o']
    externalobjs += [base / 'windows/pngreaderx64.o']
  else
    if target_machine.system() != 'haiku'
      externalobjs += [base / 'unix/jpgreaderx64.o']
      externalobjs += [base / 'unix/pngreaderx64.o']
    endif
  endif
endif


includes = include_directories([
  '.'
])

projectsources = files([
  'src/pngreader.c',
  'src/jpgreader.c',
])


largs = []
if externalasm
  cc = meson.get_compiler('c')
  if cc.get_linker_id() == 'ld.bfd'
    largs += ['-z', 'noexecstack']
  endif
endif

l = static_library('image', sources: projectsources, objects: externalobjs, dependencies: deps)
i = include_directories('.')
lib = declare_dependency(link_with: l, include_directories: i)
