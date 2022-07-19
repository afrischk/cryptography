project('craxor', 'c', default_options: ['c_std=c11'])

compiler_args = [
 '-Wall',
 '-Wextra',
 '-Wpedantic',
 '-Wformat=2',
 '-Wno-unused-parameter',
 '-Wshadow',
 '-Wwrite-strings',
 '-Wstrict-prototypes',
 '-Wold-style-definition',
 '-Wredundant-decls',
 '-Wnested-externs',
 '-Wmissing-include-dirs'
]

# math lib
cc = meson.get_compiler('c')
m_dep = cc.find_library('m', required : false)

incdir = include_directories('test')

src = [
  'main/craxor.c',
  'test/set-1-test.c',
  'xor.c',
  'base64.c',
  'algorithms.c',
  'score.c',
  'io.c'
]


executable('craxor', sources: src, include_directories: incdir, dependencies: m_dep, c_args: compiler_args)
