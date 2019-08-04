project('wlroots-glib', 'c',        version: '0.1.0',
  meson_version: '>= 0.50.0',
)


config_h = configuration_data()
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
configure_file(
  output: 'wlroots-config.h',
  configuration: config_h,
)
add_project_arguments([
  '-I' + meson.build_root(),
  '-DWLR_USE_UNSTABLE',
], language: 'c')

gnome = import('gnome')

wayland_client = dependency('wayland-client')
wayland_protos = dependency('wayland-protocols', version: '>=1.17')

subdir('protocols')
subdir('src')

