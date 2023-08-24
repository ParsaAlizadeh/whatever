# vim, ed, shell, whatever

## Build

For reasonable, logical and intuitive use cases:

```console
$ make
```

Teacher assistants may edit [settings.h](src/settings.h) and set `IS_IDIOT` to 1.

## Run

Legacy mode (phase 1):

```console
$ LEGACY=1 ./main [FILE]
```

Normal mode (phase 2):

```console
$ ./main [FILE]
```
