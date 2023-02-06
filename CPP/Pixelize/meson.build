
src = ['src/main.cpp', 'src/pixelize.cpp']

common_includes = ['../common/']

common_sources = [
  '../common/frequencytransformer.cpp',
  '../common/speakermapper.cpp',
  '../common/speaker.cpp',
  '../common/linearinterpolator.cpp',
  '../common/featuremachine.cpp'
]

executable('pixelize',
  sources: src+common_sources,
  include_directories: common_includes,
  dependencies: [lib_m, dep_jack, dep_fftw, dep_yaml],
  install: true
)
