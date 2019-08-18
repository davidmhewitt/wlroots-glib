api_version = '0.1'

wlroots_glib_sources = [
  'wlroots-backend.c',
  'wlroots-box.c',
  'wlroots-compositor.c',
  'wlroots-cursor.c',
  'wlroots-data-device-manager.c',
  'wlroots-events.c',
  'wlroots-input-device.c',
  'wlroots-matrix.c',
  'wlroots-output.c',
  'wlroots-output-layout.c',
  'wlroots-output-transform.c',
  'wlroots-renderer.c',
  'wlroots-seat.c',
  'wlroots-surface.c',
  'wlroots-surface-state.c',
  'wlroots-texture.c',
  'wlroots-wayland-display.c',
  'wlroots-xcursor-manager.c',
  'wlroots-xdg-shell.c',
  'wlroots-xdg-surface.c',
  'wlroots-xdg-toplevel.c',
]

wlroots_glib_headers = [
  'wlroots-glib.h',
  'wlroots-backend.h',
  'wlroots-box.h',
  'wlroots-compositor.h',
  'wlroots-cursor.h',
  'wlroots-data-device-manager.h',
  'wlroots-events.h',
  'wlroots-input-device.h',
  'wlroots-matrix.h',
  'wlroots-output.h',
  'wlroots-output-layout.h',
  'wlroots-output-transform.h',
  'wlroots-renderer.h',
  'wlroots-seat.h',
  'wlroots-surface.h',
  'wlroots-surface-state.h',
  'wlroots-texture.h',
  'wlroots-wayland-display.h',
  'wlroots-xcursor-manager.h',
  'wlroots-xdg-shell.h',
  'wlroots-xdg-surface.h',
  'wlroots-xdg-toplevel.h',
]

wlroots_glib_enums = gnome.mkenums(
  'wlroots-enum-types',
  sources: wlroots_glib_headers,
  h_template: 'wlroots-enum-types.h.in',
  c_template: 'wlroots-enum-types.c.in',
  install_header: true,
  install_dir: join_paths(get_option('includedir'), meson.project_name()),
)

# Keep a reference to the generated header, for internal dependencies
json_enum_types_h = wlroots_glib_enums.get(1)

version_split = meson.project_version().split('.')
MAJOR_VERSION = version_split[0]
MINOR_VERSION = version_split[1]
MICRO_VERSION = version_split[2]

version_conf = configuration_data()
version_conf.set('VERSION', meson.project_version())
version_conf.set('MAJOR_VERSION', MAJOR_VERSION)
version_conf.set('MINOR_VERSION', MINOR_VERSION)
version_conf.set('MICRO_VERSION', MICRO_VERSION)

configure_file(
  input: 'wlroots-glib-version.h.in',
  output: 'wlroots-glib-version.h',
  configuration: version_conf,
  install: true,
  install_dir: join_paths(get_option('includedir'), meson.project_name())
)

wlroots_glib_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('wayland-server'),
  dependency('wlroots'),
]

wlroots_glib_lib = shared_library(meson.project_name() + '-' + api_version,
  wlroots_glib_sources + wlroots_glib_enums,
  dependencies: [
  	wlroots_glib_deps,
  	wlr_protos,
  ],
  install: true,
)

install_headers(wlroots_glib_headers, subdir: meson.project_name())

gir_args = [
  '--quiet',
  '-I' + join_paths (meson.build_root (), 'protocols'),
]

wlroots_gir = gnome.generate_gir(
  wlroots_glib_lib,
  sources : [
    wlroots_glib_sources,
    wlroots_glib_headers
  ],
  nsversion : api_version,
  namespace : 'Wlroots',
  symbol_prefix : 'wlroots',
  identifier_prefix : 'Wlroots',
  export_packages : [meson.project_name()],
  extra_args : gir_args,
  includes: [ 'GObject-2.0' ],
  install : true,
  header: join_paths(meson.project_name(), 'wlroots-glib.h'),
)

gnome.generate_vapi(
  meson.project_name() + '-' + api_version,
  sources: wlroots_gir[0],
  packages: ['glib-2.0', 'posix'],
  install: true
)

pkg = import('pkgconfig')

pkg.generate(
  description: 'A shared library for ...',
    libraries: wlroots_glib_lib,
         name: meson.project_name(),
     filebase: meson.project_name() + '-' + api_version,
      version: meson.project_version(),
      subdirs: meson.project_name(),
     requires: 'glib-2.0',
  install_dir: join_paths(get_option('libdir'), 'pkgconfig')
)
