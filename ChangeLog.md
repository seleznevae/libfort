## v0.2.0

### API

- Added support for UTF-8 strings (`ft_u8write`, `ft_u8printf` ...).

## v0.1.6

### Internal

- Removed blank lines from `PLAIN_STYLE` and `SIMPLE_STYLE`.

### Bug fixes

- Changed specific style reset tags to universal reset style tag.
- Fix incorrect border style for the last line in the table.

### Tests

- Added more tests for border styles.

## v0.1.5

### Tests

- Add tests for 'mk_wcswidth' function.

## v0.1.4

### Internal

- Removed redundant build options from cmake files.
- Added build for arm platform with drone.ci.

## v0.1.3

### Internal

- Fixed error with incorrect types when determine class of 'wchar_t' symbol for platforms with unsigned 'wchar_t'.

## v0.1.2

### Internal

- Removed '-Werror' flag from the build process.

## v0.1.1

### Internal

- Add library version and soversion to the built library.

## v0.1.0

Initial release
