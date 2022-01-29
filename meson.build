project('image', 'c', default_options: ['optimization=3'])


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
  target_machine.system() == 'windows'
    externalasm = true
  else
    if target_machine.system() != 'haiku'
      externalasm = true
    endif
  endif
endif


if externalasm
  add_project_arguments('-DJPGR_CFG_EXTERNALASM')
  add_project_arguments('-DPNGR_CFG_EXTERNALASM')
  target_machine.system() == 'windows'
    externalobjs = list([
      'image/src/asm/windows/pngreader.o',
      'image/src/asm/windows/jpgreader.o'
      ])
  else
    if target_machine.system() != 'haiku'
      externalobjs = list([
        'image/src/asm/unix/pngreader.o',
        'image/src/asm/unix/jpgreader.o'
        ])
    endif
  endif
else
    externalobjs = list([])
endif


includes = include_directories([
  'image'
  'image/deflate',
  'image/deflate/ctoolbox'])

common = files([
  'image/deflate/src/inflator.c',
	'image/deflate/ctoolbox/crypto/src/crc32.c',
  'image/jpgreader.c',
  'image/pngreader.c'
])

executable('jpg2tga', sources: [common + 'jpg2tga.c'], include_directories: includes, objects: externalobjs)
executable('png2tga', sources: [common + 'png2tga.c'], include_directories: includes, objects: externalobjs)