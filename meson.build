project('please', 'cpp',
    version: '1.0',
    default_options: [
        'cpp_std=c++17',
        'buildtype=release',
    ],
)

src_dir = include_directories('src')

please_exe = executable(
    'please',
    include_directories: src_dir,
    sources:
    [
        'src/main.cpp',
        'src/solver.cpp'
    ],
)

bench_exe = executable(
    'bench',
    sources:
    [
        'bench/bench.cpp',
        'src/solver.cpp'
    ],
    include_directories: src_dir,
)

tests_exe = executable(
    'tests',
    sources:
    [
        'tests/tests.cpp',
        'src/solver.cpp',
        'src/oracle.cpp'
    ],
    include_directories: src_dir,
)

python = find_program('python3')

run_cli = files('tests/run_cli.py')
cases = [
  'example1',
  'example2',
  'n1-single',
  'n1-all-ones',
  'n3',
  'n4',
  'n6-product',
  'n6-single',
  'odd-product',
  'even-one-even',
  'n60-oracle-limit',
  'n60-split',
  'power-of-two',
  'all-bits-set',
  'mod-minus-1',
  'mod',
  'mod-plus-1',
  'mod-times-two',
  'big-1e18',
  'big-1e18-minus-1',
  'big-three-1e18',
  'max-k-ones',
  'max-k-1e18',
  'max-k-random',
]
foreach name : cases
  test(
    'case-' + name,
    python,
    args: [
      run_cli,
      please_exe,
      files('tests/data/' + name + '.in'),
      files('tests/data/' + name + '.out'),
    ],
  )
endforeach

test('unit', tests_exe, args: ['unit'])
test('stress', tests_exe, args: ['stress'], timeout: 120)

run_target(
    'experiments',
    command: [
        python,
        files('bench/experiments.py'),
        bench_exe,
        meson.current_build_dir() / 'experiments',
    ],
)