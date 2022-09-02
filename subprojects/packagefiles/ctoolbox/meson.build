project('ctoolbox', 'c', default_options: ['buildtype=release', 'warning_level=3'])

conf = configuration_data()


# CPU configuration
if target_machine.endian() == 'little'
  conf.set('CTB_CFG_LITTLEENDIAN', true)
else
  if target_machine.endianess() == 'big'
    conf.set('CTB_CFG_BIGENDIAN', true)
  else
    abort()
  endif
endif

target = target_machine.cpu_family()
conf.set('CTB_CFG_ENV64', target.contains('64'))
if target.contains('x86')
  conf.set('CTB_CFG_FASTUNALIGNED', true)
else
  conf.set('CTB_CFG_STRICTALIGNMENT', true)
endif


isunix    = false
iswindows = false

# System platform
if target_machine.system() == 'windows'
  conf.set('CTB_CFG_PLATFORM_WINDOWS', true)
  iswindows = true
else
  if target_machine.system() == 'haiku'
    conf.set('CTB_CFG_PLATFORM_BEOS', true)
  else
    conf.set('CTB_CFG_PLATFORM_UNIX', true)
    isunix = true
  endif
endif

configure_file(output: 'ctbconfig.h', configuration: conf)


projectsources = []
externalobjs   = []

base = 'src'
projectsources += [
  base / 'ctoolbox.c'
]

base = 'crypto/src'
projectsources += [
  base / 'adler32.c',
  base / 'crc32.c'
]

if target == 'x86_64'
  if isunix
    externalobjs += [base / 'asm/unix/adler32x64.o']
    externalobjs += [base / 'asm/unix/crc32x64.o']

    add_project_arguments('-DADLER32_CFG_EXTERNALASM', language: 'c')
    add_project_arguments(  '-DCRC32_CFG_EXTERNALASM', language: 'c')
  endif
endif


largs = []
cc = meson.get_compiler('c')
if cc.get_linker_id() == 'ld.bfd'
  largs += ['-z', 'noexecstack']
endif

l = static_library('ctoolbox', sources: projectsources, objects: externalobjs)
i = include_directories('.')
lib = declare_dependency(link_with: l, include_directories: i, link_args: largs)
