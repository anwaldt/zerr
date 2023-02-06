project('zerr', 'cpp', version: '10')

os = build_machine.system()
cc = meson.get_compiler('cpp')

lib_m    = cc.find_library('m')
dep_jack = dependency('jack')
dep_fftw = dependency('fftw3')
dep_yaml = dependency('yaml-cpp')

common_includes = ['common/']

common_sources = [
  'common/frequencytransformer.cpp',
  'common/speakermapper.cpp',
  'common/speaker.cpp',
  'common/linearinterpolator.cpp',
  'common/featuremachine.cpp'
]

subdir('CyclicShift')
subdir('PeakShuffle')
subdir('Pixelize')
subdir('ThreadWalk')
subdir('WeightShifter')
subdir('WaveQuant')
