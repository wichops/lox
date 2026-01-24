project('lox',
  'c',
  version: '0.1.0',
  default_options: [
    'warning_level=3',
    'werror=true',
    'c_std=c17',
    'optimization=2'
  ]
)

inc = include_directories('include')

src = files(
  'src/main.c',
  'src/scanner.c',
  'src/error.c'
)

executable('lox',
  src,
  include_directories: inc,
  install: true
)
