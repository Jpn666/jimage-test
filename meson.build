project('image', 'c', default_options: ['buildtype=release', 'warning_level=3'])


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

# System platform
if target_machine.system() == 'windows'
  conf.set('CTB_CFG_PLATFORM_WINDOWS', true)
else
  if target_machine.system() == 'haiku'
    conf.set('CTB_CFG_PLATFORM_BEOS', true)
  else
    conf.set('CTB_CFG_PLATFORM_UNIX', true)
  endif
endif


configure_file(output: 'ctbconfig.h', configuration: conf)


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

if externalasm
  add_project_arguments('-DJPGR_CFG_EXTERNALASM', language: 'c')
  add_project_arguments('-DPNGR_CFG_EXTERNALASM', language: 'c')
  if target_machine.system() == 'windows'
    jpgrexternalobjs = ['image/src/asm/windows/jpgreaderx64.o']
    pngrexternalobjs = ['image/src/asm/windows/pngreaderx64.o']
  else
    if target_machine.system() != 'haiku'
      jpgrexternalobjs = ['image/src/asm/unix/jpgreaderx64.o']
      pngrexternalobjs = ['image/src/asm/unix/pngreaderx64.o']
    endif
  endif
else
    jpgrexternalobjs = []
    pngrexternalobjs = []
endif


includes = include_directories([
  'image',
  'image/deflate',
  'image/deflate/ctoolbox'])

jpgrsources = files([
  'image/src/jpgreader.c',
  'jpg2tga.c'
])
pngrsources = files([
  'image/deflate/src/inflator.c',
  'image/deflate/ctoolbox/crypto/src/crc32.c',
  'image/src/pngreader.c',
  'png2tga.c'
])


largs = []
if externalasm
  cc = meson.get_compiler('c')
  if cc.get_linker_id() == 'ld.bfd'
    largs += ['-z', 'noexecstack']
  endif
endif

executable('jpg2tga', sources: jpgrsources, include_directories: includes, objects: jpgrexternalobjs, link_args: largs)
executable('png2tga', sources: pngrsources, include_directories: includes, objects: pngrexternalobjs, link_args: largs)
