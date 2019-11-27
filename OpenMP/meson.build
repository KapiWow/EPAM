project('example', 'cpp')

cpp = meson.get_compiler('cpp')

srcs = [
    'TaskOne',
    'TaskTwo',
    'TaskThree',
    'TaskFour',
]

lib = [
]

cpp_arg = [
    '-O3',
    '-march=native',
    '-g',
    '-masm=intel',
    '-lpthread',
    '-ffast-math',
    '-fopenmp',
]

linker_arg = [
    '-fopenmp',
]

foreach src : srcs
    src_name= src + '.cc'
    executable(
        src,
        sources: src_name,
        dependencies : lib,
        cpp_args : cpp_arg,
        link_args : linker_arg,
    )
endforeach

