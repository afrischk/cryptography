project('craxor', 'c', default_options: ['c_std=c2x'])

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
unity_dep = dependency('unity', required: false)
if not unity_dep.found()
  unity_proj = subproject('unity')
  unity_dep = unity_proj.get_variable('unity_dep')
endif

incdir = include_directories('test')

src = [
  'main/craxor.c',
  'xor.c',
  'base64.c',
  'algorithms.c',
  'score.c',
  'io.c'
]

executable('craxor', sources: src, include_directories: incdir, dependencies: [m_dep,unity_dep], c_args: compiler_args)

# unit tests
unittest_src = [
  'test/set-1-test.c',
  'xor.c',
  'base64.c',
  'algorithms.c',
  'score.c',
  'io.c'
]
#incdir_unittests = include_directories('subprojects/unity/src')
unittests = executable('craxor-unittests', sources: unittest_src, dependencies: [unity_dep, m_dep])
test('Bla', unittests)
