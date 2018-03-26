project(
	'libsurvive', 'c',
	default_options : 'c_std=c99',
	version : '0.0.1',
)
library_version = '0'

sources = [
	'src/epnp/epnp.c',
	'src/ootx_decoder.c',
	'src/poser.c',
	'src/poser_charlesslow.c',
	'src/poser_daveortho.c',
	'src/poser_dummy.c',
	'src/poser_epnp.c',
	'src/poser_octavioradii.c',
	'src/poser_sba.c',
	'src/poser_turveytori.c',
	'src/survive.c',
	'src/survive_cal.c',
	'src/survive_charlesbiguator.c',
	'src/survive_config.c',
	'src/survive_default_devices.c',
	'src/survive_disambiguator.c',
	'src/survive_driverman.c',
	'src/survive_playback.c',
	'src/survive_process.c',
	'src/survive_reproject.c',
	'src/survive_sensor_activations.c',
	'src/survive_turveybiguator.c',
	'src/survive_usb.c',
	'src/survive_vive.c',
	'redist/crc32.c',
	'redist/dclhelpers.c',
	'redist/glutil.c',
	'redist/hid-linux.c',
	#'redist/hid-osx.c',
	#'redist/hid-windows.c',
	'redist/jsmn.c',
	'redist/json_helpers.c',
	'redist/linmath.c',
	'redist/lintest.c',
	'redist/minimal_opencv.c',
	#'redist/mymath.c',
	'redist/puff.c',
	#'redist/symbol_enumerator.c',
	'redist/sba/sba_chkjac.c',
	'redist/sba/sba_crsm.c',
	'redist/sba/sba_lapack.c',
	'redist/sba/sba_levmar.c',
	'redist/sba/sba_levmar_wrap.c'
]

cnfsources = [
	'redist/CNFG3D.c',
	'redist/CNFGFunctions.c',
	'redist/CNFGXDriver.c'
	#'redist/CNFGEGLDriver.c',
	#'redist/CNFGWinDriver.c',
]

c_args = ['-DUSE_DOUBLE', '-DFLT=double']
includes = include_directories(['./include/libsurvive', './redist'])

math_dep = meson.get_compiler('c').find_library('m', required : false) #-lm
thread_dep = dependency('threads') #pthread
cblas_dep = dependency('cblas')

gl_dep = dependency('GL')
gui_dep = dependency('x11')
#gui_dep = dependency('egl') # enable CNFGEGLDriver.c instead of CNFGXDriver.c in cnfsources to use this

survive_deps = [
	math_dep,
	thread_dep,
	dependency('lapack'),
	dependency('lapacke'),
	cblas_dep,
	dependency('libudev'),
	dependency('zlib'),
	dependency('libusb-1.0')
]

survive_lib = library('survive', sources, include_directories : includes, c_args : c_args, dependencies : survive_deps, install : true, version : library_version)

executable('test_survive', './test.c', include_directories : includes, c_args : c_args, dependencies: thread_dep, link_with: [survive_lib], install : true)
executable('simple_pose_test', ['./simple_pose_test.c'] + cnfsources, include_directories : includes, c_args : c_args, dependencies : [math_dep, thread_dep, gl_dep, gui_dep], link_with: [survive_lib], install : true)
executable('data_recorder', './data_recorder.c', include_directories : includes, c_args : c_args, dependencies: thread_dep, link_with: [survive_lib], install : true)
executable('calibrate', ['./calibrate.c' ] + cnfsources, include_directories : includes, c_args : c_args, dependencies : [math_dep, thread_dep, gl_dep, gui_dep] , link_with: [survive_lib], install : true)
executable('calibrate_client', ['./calibrate_client.c' ] + cnfsources, include_directories : includes, c_args : c_args, dependencies : [math_dep, thread_dep, gl_dep, gui_dep] , link_with: [survive_lib], install : true)

#internal tests
executable('test_dcl', './redist/test_dcl.c', include_directories : includes, c_args : c_args, dependencies : [thread_dep, cblas_dep], link_with: [survive_lib], install : true)
executable('test_minimal_cv', './src/epnp/test_minimal_cv.c', include_directories : includes, c_args : c_args, link_with: [survive_lib], install : true)
executable('test_epnp', './src/epnp/test_epnp.c', include_directories : includes, c_args : c_args, dependencies : [math_dep, thread_dep], link_with: [survive_lib], install : true)
