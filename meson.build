project('cpp-boilerplate', 'cpp')
compiler = meson.get_compiler('cpp')
libGLU = compiler.find_library('GLU')
executable('rubik',
           'main.cpp',
           'support-3d.cpp',
           'transform-3d.cpp',
           'world-drawer-3d.cpp',
           dependencies: [
            dependency('glut'),
            libGLU,
            dependency('OpenGL')
           ]
)
