
src = ['src/main.cpp', 'src/peakshuffle.cpp']
 

executable('peakshuffle',
  sources: src+common_sources,
  include_directories: common_includes,
  dependencies: [lib_m, dep_jack, dep_fftw, dep_yaml],
  install: true
)
